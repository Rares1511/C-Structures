#ifndef __CS_MULTIMAP_H__
#define __CS_MULTIMAP_H__

#include <cs/universal.h>
#include <cs/rbt.h>
#include <cs/pair.h>
#include <cs/vector.h>

typedef struct multimap {
    __rbt *t;
    elem_attr_t* key_attr;
    elem_attr_t* value_attr;
    elem_attr_t* vec_attr;
    int size;
} multimap;

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

/*!
 * Compare function for multimap nodes (pairs).
 * @param[in] a  First pair to compare
 * @param[in] b  Second pair to compare
 * @return    Negative if a < b, zero if a == b, positive if a > b
 */
static inline int __multimap_node_comp(const void *a, const void *b) {
    pair *pa = (pair *)a;
    pair *pb = (pair *)b;

    void *key_a = pair_first(pa);
    void *key_b = pair_first(pb);

    if (pa->first_attr->comp) {
        return pa->first_attr->comp(key_a, key_b);
    } else {
        return memcmp(key_a, key_b, pa->first_attr->size);
    }
}

/*!
 * Copy function for multimap nodes (pairs).
 * @param[out] dest  Destination pair to copy into
 * @param[in]  src   Source pair to copy from
 */
static inline void __multimap_node_copy(void *dest, const void *src) {
    CS_RETURN_IF(dest == NULL || src == NULL);
    const pair *s = (const pair *)src;
    memcpy(dest, src, sizeof(pair) + s->first_attr->size + s->second_attr->size);
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

/*!
 * Initialize a multimap structure
 * @param[in] key_attr Attributes of the key datatype
 * @param[in] value_attr Attributes of the value datatype
 * @param[out] mm Pointer to the multimap to initialize
 * @return CS_SUCCESS on success, or an error code on failure
 */
cs_codes multimap_init(multimap *mm, elem_attr_t key_attr,
                           elem_attr_t value_attr);

/*!
 * Insert a key-value pair into the multimap
 * @param[in,out] mm    Pointer to the multimap
 * @param[in]     key   Pointer to the key
 * @param[in]     value Pointer to the value
 * @return CS_SUCCESS on success, or an error code on failure
 */
static inline cs_codes multimap_insert(multimap *mm, const void *key, const void *value) {
    CS_RETURN_IF(mm == NULL || key == NULL || value == NULL, CS_NULL);
    int k_sz = mm->key_attr->size;
    int v_sz = sizeof(vector);
    char buffer[sizeof(pair) + k_sz + v_sz];
    
    memset(buffer, 0, sizeof(buffer));

    pair *p_stack = (pair *)buffer;
    p_stack->header.magic = CS_PAIR_MAGIC;
    p_stack->first_attr = mm->key_attr;
    p_stack->second_attr = mm->vec_attr;
    p_stack->has_first = 1;
    p_stack->has_second = 0;
    
    if (mm->key_attr->copy) mm->key_attr->copy(p_stack->data, key);
    else memcpy(p_stack->data, key, k_sz);

    __rbt_node *node = __rbt_insert_internal(mm->t, p_stack);
    
    pair *p_tree = (pair *)node->data;
    vector *vec = (vector *)(p_tree->data + k_sz);

    if (p_tree->has_second == 0) {
        p_tree->second_attr = mm->vec_attr;
        vector_attr_t v_attr = { .shrink_factor = 1, .min_cap = 2 };
        vector_init(vec, *mm->value_attr, v_attr);
        p_tree->has_second = 1;
    }

    return vector_push_back(vec, value);
}

/*!
 * Remove all values associated with a key from the multimap
 * @param[in,out] mm   Pointer to the multimap
 * @param[in]     key  Pointer to the key
 * @return CS_SUCCESS on success, or an error code on failure
 */
static inline cs_codes multimap_delete(multimap *mm, const void *key) {
    CS_RETURN_IF(mm == NULL || key == NULL, CS_NULL);
    int k_sz = mm->key_attr->size;
    int total_sz = sizeof(pair) + k_sz + sizeof(vector);
    char buffer[total_sz];
    pair *p = (pair *)buffer;

    p->header.magic = CS_PAIR_MAGIC;
    p->first_attr = mm->key_attr;
    p->second_attr = mm->vec_attr;
    p->has_first = 1;
    p->has_second = 0;
    memcpy(p->data, key, k_sz);

    __rbt_node *node = __rbt_node_find(mm->t, p);
    if (node == NULL) {
        return CS_ELEM;
    }

    vector *vec = (vector *)((char *)node->data + sizeof(pair) + k_sz);
    mm->size -= vector_size(vec);
    return __rbt_delete_internal(mm->t, node);
}

/*!
 * Retrieve all values associated with a key from the multimap
 * @param[in] mm   Pointer to the multimap
 * @param[in] key  Pointer to the key
 * @return Pointer to a vector of values, or NULL if the key is not found
 */
static inline vector* multimap_get(multimap *mm, const void *key) {
    CS_RETURN_IF(NULL == mm || NULL == key, NULL);
    int k_sz = mm->key_attr->size;
    int total_sz = sizeof(pair) + k_sz + sizeof(vector);
    char buffer[total_sz];
    pair *p = (pair *)buffer;

    p->header.magic = CS_PAIR_MAGIC;
    p->first_attr = mm->key_attr;
    p->second_attr = mm->vec_attr;
    p->has_first = 1;
    p->has_second = 0;
    memcpy(p->data, key, k_sz);

    pair *found_pair = __rbt_find(mm->t, p);
    if (found_pair == NULL) {
        return NULL;
    }
    return (vector *)((char *)found_pair + sizeof(pair) + k_sz);
}

/*!
 * Check if the multimap is empty
 * @param[in] mm The multimap to check
 * @return 1 if the multimap is empty, 0 otherwise
 */
static inline int multimap_empty(multimap *mm) { return mm->size == 0; }

/*!
 * Get the number of key-value pairs in the multimap
 * @param[in] mm Pointer to the multimap
 * @return The number of key-value pairs in the multimap
 */
static inline int multimap_size(multimap *mm) { return mm->size; }

/*!
 * Clear all entries from the multimap
 * @param[in,out] mm Pointer to the multimap
 */
void multimap_clear(multimap *mm);

/*!
 * Swap the contents of two multimaps
 * @param[in,out] mm1 Pointer to the first multimap
 * @param[in,out] mm2 Pointer to the second multimap
 */
void multimap_swap(multimap *mm1, multimap *mm2);

/*!
 * Print the contents of the multimap
 * @param[in] stream Output stream
 * @param[in] v_mm  Pointer to the multimap
 */
void multimap_print(FILE *stream, void *v_mm);

/*!
 * Free the multimap and its resources
 * @param[in] v_mm Pointer to the multimap
 */
void multimap_free(void *v_mm);

#endif