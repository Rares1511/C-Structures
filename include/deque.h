#ifndef __CS_DEQUE_H__
#define __CS_DEQUE_H__

#include <cs/universal.h>

#define DEQUE_INIT_BLOCKS 64
#define DEQUE_BLOCK_SIZE 64

typedef univ_attr_t deque_attr_t;

typedef struct deque_block_t {
    void* data; /*!< Pointer to the block's data */
    int front;  /*!< Index of the front element in the block, points to the first element */
    int back;   /*!< Index of the back element in the block, points to the next insertion position */
} deque_block_t;

typedef struct deque {
    deque_block_t *blocks; /*!< Array of blocks in the deque */
    deque_attr_t attr;     /*!< Attributes of the deque */
    metadata_t *meta;      /*!< Metadata for safety checks */
    int block_size;        /*!< Number of elements per block */
    int block_cap;         /*!< Total number of blocks allocated */
    int front;             /*!< Index of the front element in the deque, points to the first block */
    int back;              /*!< Index of the back element in the deque, points to the last block */
} deque;

/*!
 * Initializes a deque with the specified attributes.
  * @param dq Pointer to the deque to initialize.
  * @return Pointer to the initialized deque, or NULL on failure.
 */
deque *deque_init(deque_attr_t attr);

/*! 
 * Pushes an element to the back of the deque.
 * @param dq Pointer to the deque.
 * @param el Pointer to the element to push.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_push_back(deque *dq, const void*  el);

/*! 
 * Pushes an element to the front of the deque.
 * @param dq Pointer to the deque.
 * @param el Pointer to the element to push.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_push_front(deque *dq, const void*  el);

/*! 
 * Pops an element from the back of the deque.
 * @param dq Pointer to the deque.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_pop_back(deque *dq);

/*! 
 * Pops an element from the front of the deque.
 * @param dq Pointer to the deque.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_pop_front(deque *dq);

/*! 
 * Pops an element from the back of the deque.
 * @param dq Pointer to the deque.
 * @return Pointer to the popped element, or NULL on failure.
 */
void* deque_back(deque dq);

/*! 
 * Pops an element from the front of the deque.
 * @param dq Pointer to the deque.
 * @return Pointer to the popped element, or NULL on failure.
 */
void* deque_front(deque dq);

/*! 
 * Retrieves the element at the specified index in the deque.
 * @param dq The deque.
 * @param index The index of the element to retrieve.
 * @return Pointer to the element at the specified index, or NULL if index is out of bounds.
 */
void* deque_at(deque dq, int index);

/*! 
 * Checks if the deque is empty.
 * @param dq The deque.
 * @return 1 if the deque is empty, 0 otherwise.
 */
static inline int deque_empty(deque dq) { return dq.meta->size == 0; }

/*! 
 * Returns the number of elements in the deque.
 * @param dq The deque.
 * @return The number of elements in the deque.
 */
static inline int deque_size(deque dq) { return dq.meta->size; }

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