#ifndef __CS_DEQUE_H__
#define __CS_DEQUE_H__

#include <cs/universal.h>

// Internal values for growth direction in _deque_grow_internal
#define __DEQUE_GROW_INTERNAL_BACK 1
#define __DEQUE_GROW_INTERNAL_FRONT -1

// Default block size and initial block count for the deque
#define DEQUE_INIT_BLOCKS 64
#define DEQUE_BLOCK_SIZE 64

// Magic number for validating deque structures
#define CS_DEQUE_MAGIC 0xDEC6E123

/*!
 * @brief Attributes for the deque structure controllable by the user
 */
typedef struct deque_attr_t {
    int min_cap; /*!< Minimum capacity of the deque, used for optimization */
    int block_size; /*!< Number of elements per block, used for optimization */
} deque_attr_t;

typedef struct deque_block_t {
    void* data; /*!< Pointer to the block's data */
    int front;  /*!< Index of the front element in the block, points to the first element */
    int back;   /*!< Index of the back element in the block, points to the next insertion position */
} deque_block_t;

typedef struct deque {
    cs_header_t header;    /*!< Common header for all data structures */
    deque_block_t *blocks; /*!< Array of blocks in the deque */
    elem_attr_t attr;      /*!< Attributes of the deque */
    deque_attr_t dq_attr;  /*!< User-controllable attributes for optimization */
    int size;              /*!< Total number of elements in the deque */
    int block_cap;         /*!< Total number of blocks allocated */
    int front;             /*!< Index of the front element in the deque, points to the first block */
    int back;              /*!< Index of the back element in the deque, points to the last block */
} deque;

cs_codes _deque_grow_internal(deque *dq, int direction);

/*!
 * Initializes a deque with the specified attributes.
 * @param dq Pointer to the deque to initialize.
 * @param attr Attributes for the deque.
 * @param dq_attr User-controllable attributes for optimization.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
deque* deque_init(elem_attr_t attr, deque_attr_t dq_attr);

/*! 
 * Pushes an element to the back of the deque.
 * @param dq Pointer to the deque.
 * @param el Pointer to the element to push.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
static inline cs_codes deque_push_back(deque *dq, const void* el) {
    CS_RETURN_IF(dq == NULL || el == NULL, CS_NULL);
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC, CS_UNINITIALIZED);

    if (__builtin_expect(dq->blocks[dq->back].back >= dq->dq_attr.block_size, 0)) {
        cs_codes rc = _deque_grow_internal(dq, __DEQUE_GROW_INTERNAL_BACK);
        if (rc != CS_SUCCESS) {
            return rc;
        }
    }

    deque_block_t *block = &dq->blocks[dq->back];
    void *dest = block->data + (block->back * dq->attr.size);
    deepcopy copy_func = dq->attr.copy;

    if (copy_func) {
        copy_func(dest, el);
    } else {
        memcpy(dest, el, dq->attr.size);
    }
    block->back++;
    dq->size++;
    return CS_SUCCESS;
}

/*! 
 * Pushes an element to the front of the deque.
 * @param dq Pointer to the deque.
 * @param el Pointer to the element to push.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
static inline cs_codes deque_push_front(deque *dq, const void* el) {
    CS_RETURN_IF(dq == NULL || el == NULL, CS_NULL);
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC, CS_UNINITIALIZED);

    if (__builtin_expect(dq->blocks[dq->front].front <= 0, 0)) {
        cs_codes rc = _deque_grow_internal(dq, __DEQUE_GROW_INTERNAL_FRONT);
        if (rc != CS_SUCCESS) {
            return rc;
        }
    }

    deque_block_t *block = &dq->blocks[dq->front];
    block->front--;
    void *dest = (char*)block->data + (block->front * dq->attr.size);
    deepcopy copy_func = dq->attr.copy;
    
    if (copy_func) {
        copy_func(dest, el);
    } else {
        memcpy(dest, el, dq->attr.size);
    }
    
    dq->size++;
    return CS_SUCCESS;
}

/*! 
 * Inserts an element at the specified index in the deque.
 * @param dq Pointer to the deque.
 * @param index The index at which to insert the element.
 * @param el Pointer to the element to insert.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_insert_at(deque *dq, const void *el, int index);

/*! 
 * Pops an element from the back of the deque.
 * @param dq Pointer to the deque.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
static inline cs_codes deque_pop_back(deque *dq) {
    CS_RETURN_IF(dq == NULL, CS_NULL);
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC, CS_UNINITIALIZED);
    CS_RETURN_IF(dq->size == 0, CS_EMPTY);

    deque_block_t *block = &dq->blocks[dq->back];
    block->back--;
    freer free_func = dq->attr.fr;
    void *elem = (char *) block->data + (block->back * dq->attr.size);

    if (free_func) {
        free_func(elem);
    }
    dq->size--;

    if (block->back == 0) {
        dq->back--;
        free(dq->blocks[dq->back + 1].data);
        // Check if we deleted last available block and now deque is empty
        if (dq->back < dq->front) {
            dq->front = dq->block_cap / 2;
            dq->back = dq->block_cap / 2;
            dq->blocks[dq->front].data = malloc(dq->dq_attr.block_size * dq->attr.size);
            if (!dq->blocks[dq->front].data) {
                return CS_MEM;
            }
            dq->blocks[dq->front].front = dq->dq_attr.block_size / 2;
            dq->blocks[dq->front].back = dq->dq_attr.block_size / 2;
        }
    }
    return CS_SUCCESS;
}

/*! 
 * Pops an element from the front of the deque.
 * @param dq Pointer to the deque.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
static inline cs_codes deque_pop_front(deque *dq) {
    CS_RETURN_IF(dq == NULL, CS_NULL);
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC, CS_UNINITIALIZED);
    CS_RETURN_IF(dq->size == 0, CS_EMPTY);

    deque_block_t *block = &dq->blocks[dq->front];
    void *elem = block->data + (block->front * dq->attr.size);
    freer free_func = dq->attr.fr;

    if (free_func) {
        free_func(elem);
    }
    block->front++;
    dq->size--;

    if (block->front >= dq->dq_attr.block_size) {
        dq->front++;
        if (dq->front > dq->back) {
            dq->front = dq->back;
            dq->blocks[dq->front].front = dq->block_cap / 2;
            dq->blocks[dq->front].back = dq->block_cap / 2;
        } else {
            free(dq->blocks[dq->front - 1].data);
        }
    }
    return CS_SUCCESS;
}

/*! 
 * Erases the element at the specified index in the deque.
 * @param dq Pointer to the deque.
 * @param index The index of the element to erase.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_erase(deque *dq, int index);

/*! 
 * Pops an element from the back of the deque.
 * @param dq Pointer to the deque.
 * @return Pointer to the popped element, or NULL on failure.
 */
static inline void* deque_back(deque *dq) {
    CS_RETURN_IF(dq == NULL || dq->header.magic != CS_DEQUE_MAGIC || dq->size == 0, NULL);
    return dq->blocks[dq->back].data + ((dq->blocks[dq->back].back - 1) * dq->attr.size);
}

/*! 
 * Pops an element from the front of the deque.
 * @param dq Pointer to the deque.
 * @return Pointer to the popped element, or NULL on failure.
 */
static inline void* deque_front(deque *dq) {
    CS_RETURN_IF(dq == NULL || dq->header.magic != CS_DEQUE_MAGIC || dq->size == 0, NULL);
    return dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size);
}

/*! 
 * Retrieves the element at the specified index in the deque.
 * @param dq The deque.
 * @param index The index of the element to retrieve.
 * @return Pointer to the element at the specified index, or NULL if index is out of bounds.
 */
static inline void *deque_at(deque *dq, int index) {
    CS_RETURN_IF(dq == NULL || dq->header.magic != CS_DEQUE_MAGIC, NULL);
    int size = dq->size;
    CS_RETURN_IF(index < 0 || index >= size, NULL);

    if (index < dq->blocks[dq->front].back - dq->blocks[dq->front].front) {
        return dq->blocks[dq->front].data + ((dq->blocks[dq->front].front + index) * dq->attr.size);
    }
    index -= (dq->blocks[dq->front].back - dq->blocks[dq->front].front);
    int offset = index % dq->dq_attr.block_size;
    index = index / dq->dq_attr.block_size;

    return dq->blocks[dq->front + 1 + index].data + (offset * dq->attr.size);
}

/*! 
 * Checks if the deque is empty.
 * @param dq The deque.
 * @return 1 if the deque is empty, 0 otherwise.
 */
static inline int deque_empty(deque *dq) { return dq->size == 0; }

/*! 
 * Returns the number of elements in the deque.
 * @param dq The deque.
 * @return The number of elements in the deque.
 */
static inline int deque_size(deque *dq) { return dq->size; }

/*! 
 * Swaps the contents of two deques.
 * @param dq1 Pointer to the first deque.
 * @param dq2 Pointer to the second deque.
 */
void deque_swap(deque *dq1, deque *dq2);

/*! 
 * Clears all elements from the deque.
 * @param dq Pointer to the deque to clear.
 */
void deque_clear(deque *dq);

/*! 
 * Prints the contents of the deque.
 * @param v_dq Pointer to the deque to print.
 */
void deque_print(FILE *stream, const void*  v_dq);

/*! 
 * Frees the resources associated with the deque.
 * @param v_dq Pointer to the deque to free.
 */
void deque_free(void* v_dq);

#endif