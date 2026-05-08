#ifndef __CS_MULTISET_H__
#define __CS_MULTISET_H__

#include <cs/universal.h>
#include <cs/pair.h>
#include <cs/rbt.h>

typedef struct multiset {
    __rbt *t;                /*!< red black tree containing the multiset data */
    elem_attr_t* el_attr;    /*!< attributes of the multiset elements */
    elem_attr_t* count_attr; /*!< attributes of the count elements */
    int size;                /*!< number of unique elements in the multiset */
} multiset;

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline void __multiset_node_copy(void *dest, const void *src) {
    CS_RETURN_IF(dest == NULL || src == NULL);
    const pair* s = (const pair*)src;
    memcpy(dest, src, sizeof(pair) + s->first_attr->size + s->second_attr->size);
}

static inline int __multiset_node_comp(const void *a, const void *b) {
    const pair* pa = (const pair*)a;
    const pair* pb = (const pair*)b;
    
    // Direct access, no macros, no magic checks
    void *key_a = (void*)pa->data;
    void *key_b = (void*)pb->data;

    if (__builtin_expect(pa->first_attr->comp != NULL, 1)) {
        return pa->first_attr->comp(key_a, key_b);
    }
    return memcmp(key_a, key_b, pa->first_attr->size);
}

static inline void __multiset_node_print(FILE *stream, const void *node) {
    CS_RETURN_IF(node == NULL || stream == NULL);
    pair *p = (pair *)node;
    CS_RETURN_IF(pair_first(p) == NULL || pair_second(p) == NULL);
    int count = *((int *)(pair_second(p)));
    CS_RETURN_IF(p->first_attr->print == NULL);
    for (int i = 0; i < count; i++) {
        p->first_attr->print(stream, pair_first(p));
        if (i < count - 1) {
            fprintf(stream, ", ");
        }
    }
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

/*!
 * Initialize a multiset structure
 * @param[in] ms Pointer to the multiset structure to initialize
 * @param[in] attr Attributes of the multiset element datatype
 * @return CS_SUCCESS on success, or an error code on failure
 */
cs_codes multiset_init(multiset *ms, elem_attr_t attr);

/*! 
 * Insert an element into the multiset
 * @param[in,out] ms    Pointer to the multiset
 * @param[in]     elem  Pointer to the element to insert
 * @return CS_SUCCESS on success, or an error code on failure
 */
static inline cs_codes multiset_insert(multiset *ms, const void *elem) {
    CS_RETURN_IF(ms == NULL || elem == NULL, CS_NULL);
    
    size_t k_sz = ms->el_attr->size;
    size_t v_sz = sizeof(int);
    // 1. Create a "Search Node" on the stack
    char buffer[sizeof(__rbt_node) + sizeof(pair) + k_sz + v_sz];
    __rbt_node *stack_node = (__rbt_node *)buffer;
    pair *p_stack = (pair *)stack_node->data;

    // Initialize stack node for comparison
    stack_node->left = stack_node->right = stack_node->father = NULL;
    p_stack->header.magic = CS_PAIR_MAGIC;
    p_stack->first_attr = ms->el_attr;
    p_stack->has_first = 1;
    
    if (ms->el_attr->copy) ms->el_attr->copy(p_stack->data, elem);
    else memcpy(p_stack->data, elem, k_sz);

    // 2. THE SINGLE PASS: Try to find or find the insertion point
    // We modify your insert_internal to return NULL if it SHOULD HAVE inserted
    // but we want to handle the allocation ourselves.
    __rbt_node *existing = __rbt_node_find(ms->t, p_stack);

    if (existing) {
        pair *p_tree = (pair *)existing->data;
        (*(int *)(p_tree->data + k_sz))++;
        ms->size++;
        return CS_SUCCESS;
    }

    // 3. Not found: Allocate a REAL heap node now
    __rbt_node *real_node = __rbt_node_init(NULL, ms->t->attr);
    if (!real_node) return CS_MEM;

    // Setup the real node's pair
    pair *p_real = (pair *)real_node->data;
    p_real->header.magic = CS_PAIR_MAGIC;
    p_real->first_attr = ms->el_attr;
    p_real->second_attr = ms->count_attr;
    p_real->has_first = 1;
    p_real->has_second = 1;
    memcpy(p_real->data, p_stack->data, k_sz);
    *(int *)(p_real->data + k_sz) = 1;

    // 4. Link the REAL heap node
    __rbt_insert_internal(ms->t, real_node);
    ms->size++;
    
    return CS_SUCCESS;
}

/*! 
 * Delete an element from the multiset
 * @param[in,out] ms    Pointer to the multiset
 * @param[in]     elem  Pointer to the element to delete
 * @return CS_SUCCESS on success, or an error code on failure
 */
static inline cs_codes multiset_delete(multiset *ms, const void *elem) {
    CS_RETURN_IF(ms == NULL || elem == NULL, CS_NULL);
    int rc;
    int k_sz = ms->el_attr->size;
    int total_sz = sizeof(pair) + k_sz;
    char buffer[total_sz];
    pair *data = (pair *)buffer;

    data->header.magic = CS_PAIR_MAGIC;
    data->first_attr = ms->el_attr;
    data->has_first = 1;
    if (ms->el_attr->copy) {
        ms->el_attr->copy(data->data, elem);
    } else {
        memcpy(data->data, elem, ms->el_attr->size);
    }

    __rbt_node *node = __rbt_node_find(ms->t, data);
    if (node != NULL) {
        pair *p = (pair *)node->data;
        int *count = (int *)pair_second(p);
        if (*count > 1) {
            (*count)--;
            rc = CS_SUCCESS;
        } else {
            rc = __rbt_delete_internal(ms->t, node);
        }
    } else {
        rc = CS_ELEM;
    }

    if (rc == CS_SUCCESS) {
        ms->size--;
    }

    return rc;
}

/*! 
 * Count the occurrences of an element in the multiset
 * @param[in] ms    Pointer to the multiset
 * @param[in] elem  Pointer to the element to count
 * @return Number of occurrences of the element in the multiset
 */
int multiset_count(multiset *ms, const void *elem) {
    CS_RETURN_IF(ms == NULL || elem == NULL, 0);
    int k_sz = ms->el_attr->size;
    char buffer[sizeof(pair) + k_sz];
    pair *data = (pair *)buffer;

    data->header.magic = CS_PAIR_MAGIC;
    data->first_attr = ms->el_attr;
    data->has_first = 1;
    if (ms->el_attr->copy) {
        ms->el_attr->copy(data->data, elem);
    } else {
        memcpy(data->data, elem, ms->el_attr->size);
    }

    void *node = __rbt_find((ms->t), data);
    if (node != NULL) {
        pair *p = (pair *)node;
        return *((int *)pair_second(p));
    } else {
        return 0;
    }
}

/*! 
 * Check if the multiset is empty
 * @param[in] ms Pointer to the multiset
 * @return 1 if the multiset is empty, 0 otherwise
 */
static inline int multiset_empty(multiset *ms) { return ms->size == 0; }

/*! 
 * Get the number of unique elements in the multiset
 * @param[in] ms Pointer to the multiset
 * @return Number of unique elements in the multiset
 */
static inline int multiset_size(multiset *ms) { return ms->size; }

/*! 
 * Clear all entries from the multiset
 * @param[in,out] ms Pointer to the multiset
 */
void multiset_clear(multiset *ms);

/*! 
 * Swap the contents of two multisets
 * @param[in,out] ms1 Pointer to the first multiset
 * @param[in,out] ms2 Pointer to the second multiset
 */
void multiset_swap(multiset *ms1, multiset *ms2);

/*! 
 * Print the contents of the multiset
 * @param[in] stream Output stream
 * @param[in] v_ms  Pointer to the multiset
 */
void multiset_print(FILE *stream, void *v_ms);

/*! 
 * Free the multiset and its resources
 * @param[in] v_ms Pointer to the multiset
 */
void multiset_free(void *v_ms);

#endif