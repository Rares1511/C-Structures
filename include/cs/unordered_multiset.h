#ifndef __CS_UNORDERED_MULTISET_H__
#define __CS_UNORDERED_MULTISET_H__

#include <cs/universal.h>
#include <cs/hash_table.h>
#include <cs/pair.h>

typedef struct unordered_multiset {
    __hash_table *ht;        /*<! Pointer to the underlying hash table that stores the elements of the unordered multiset. */
    size_t size;             /*<! The number of elements in the unordered multiset. */
    elem_attr_t *attr;       /*<! The attributes of the elements stored in the unordered multiset (size, copy, compare, free, print functions). */
    elem_attr_t *count_attr; /*<! The attributes of the count (integer) stored in the unordered multiset (size, copy, compare, free, print functions). */
    char *buffer;             /*<! A buffer used for temporary storage during operations like insert, erase, and find. */
} unordered_multiset;

#pragma region Helper Structs
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        START OF HELPER STRUCT SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

typedef struct {
    __hash_func_t hash_func;
    char data[]; /* Flexible array member to hold the pair */
} __unordered_multiset_entry;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                         END OF HELPER STRUCT SECTION                                       ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline void __unordered_multiset_entry_copy(void *dest, const void *src) {
    __unordered_multiset_entry *d = (__unordered_multiset_entry *)dest;
    const __unordered_multiset_entry *s = (const __unordered_multiset_entry *)src;

    d->hash_func = s->hash_func;
    
    pair *ps = (pair *)s->data;
    pair *pd = (pair *)d->data;

    memcpy(pd, ps, sizeof(pair));

    void *src_key = pair_first(ps);
    void *dest_key = pair_first(pd);
    int *src_val = (int *)pair_second(ps);
    int *dest_val = (int *)pair_second(pd);

    if (ps->first_attr->copy) 
        ps->first_attr->copy(dest_key, src_key);
    else 
        memcpy(dest_key, src_key, ps->first_attr->size);

    *dest_val = *src_val;
}

static inline void __unordered_multiset_entry_print(FILE *restrict stream, const void *restrict el) {
    CS_RETURN_IF(NULL == el || stream == NULL);
    const __unordered_multiset_entry *entry = (const __unordered_multiset_entry *)el;
    pair *p = (pair *)entry->data;
    CS_RETURN_IF(p->header.magic != CS_PAIR_MAGIC || p->first_attr->print == NULL);

    void *key = pair_first(p);
    int *count = (int *)pair_second(p);
    fprintf(stream, "Key: ");
    p->first_attr->print(stream, key);
    fprintf(stream, " | Count: %d", *count);
}

static inline void __unordered_multiset_entry_free(void *el) {
    CS_RETURN_IF(NULL == el);
    __unordered_multiset_entry *entry = (__unordered_multiset_entry *)el;
    __pair_free_internal((pair *)entry->data);
}

static inline int __unordered_multiset_entry_comp(const void *a, const void *b) {
    __unordered_multiset_entry* ea = (__unordered_multiset_entry*)a;
    __unordered_multiset_entry* eb = (__unordered_multiset_entry*)b;
    pair* pa = (pair*)ea->data;
    pair* pb = (pair*)eb->data;

    void *key_a = pair_first(pa);
    void *key_b = pair_first(pb);
    
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(key_a, key_b);
    }
    return memcmp(key_a, key_b, pa->first_attr->size);
}

static inline size_t __unordered_multiset_entry_hash(const void *restrict el) {
    CS_RETURN_IF(NULL == el, 0);
    __unordered_multiset_entry *entry = (__unordered_multiset_entry *)el;
    pair *p = (pair *)entry->data;
    void *key = pair_first(p);
    if (entry->hash_func == NULL) {
        return universal_hash_bytes(key, p->first_attr->size);
    }
    return entry->hash_func(key);
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

/*!
 * @brief Initializes an unordered multiset with the given attributes, hash function, and initial capacity.
 * @param[in] attr The attributes of the unordered multiset (element size, copy, compare, free, print functions).
 * @param[in] hash_func The hash function to be used for hashing the elements.
 * @return A pointer to the initialized unordered multiset, or NULL on failure.
 */
unordered_multiset* unordered_multiset_init(unordered_multiset *pool,
                                elem_attr_t attr, 
                                __hash_func_t hash_func);

/*!
 * @brief Inserts an element into the unordered multiset.
 * @param[in,out] umset Pointer to the unordered multiset.
 * @param[in] key Pointer to the element to be inserted.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
static inline cs_codes unordered_multiset_add_entry(unordered_multiset *restrict umset, const void *restrict key) {
    CS_RETURN_IF(umset == NULL || key == NULL || umset->ht == NULL, CS_NULL);
    int rc = CS_SUCCESS;
    __unordered_multiset_entry *entry = (__unordered_multiset_entry *)umset->buffer;
    pair *p = (pair *)entry->data;

    memcpy(p->data, key, umset->attr->size);

    __unordered_multiset_entry *existing_entry = __hash_table_add_entry(umset->ht, entry, &rc);
    if (existing_entry == NULL) {
        return rc;
    }

    int *existing_count = (int *)pair_second((pair *)existing_entry->data);
    (*existing_count)++;
    umset->size++;
    return CS_SUCCESS;
}

/*!
 * @brief Erases all occurrences of an element from the unordered multiset.
 * @param[in,out] umset Pointer to the unordered multiset.
 * @param[in] key Pointer to the element to be erased.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
static inline cs_codes unordered_multiset_remove_entry(unordered_multiset *restrict umset, const void *restrict key) {
    CS_RETURN_IF(umset == NULL || key == NULL || umset->ht == NULL, CS_NULL);
    __unordered_multiset_entry *entry = (__unordered_multiset_entry *)umset->buffer;
    pair *p = (pair *)entry->data;

    memcpy(p->data, key, umset->attr->size);
    size_t idx = __hash_table_find_index(umset->ht, entry);
    if (idx == __HASH_TABLE_TOMBSTONE_VALUE) {
        return CS_ELEM; // Element not found
    }
    umset->size--;
    __unordered_multiset_entry *found_entry = (__unordered_multiset_entry *)((char *)umset->ht->keys + (idx * umset->ht->attr.size));
    int *count = (int *)pair_second((pair *)found_entry->data);
    (*count)--;
    if (*count == 0) {
        __hash_table_remove_at_index(umset->ht, idx);
    }
    return CS_SUCCESS;
}

/*!
 * @brief Counts the number of occurrences of an element in the unordered multiset.
 * @param[in] umset The unordered multiset.
 * @param[in] key Pointer to the element to be counted.
 * @return The number of occurrences of the element.
 */
static inline size_t unordered_multiset_count(unordered_multiset *restrict umset, const void *restrict key) {
    CS_RETURN_IF(umset == NULL || key == NULL, 0);
    __unordered_multiset_entry *entry = (__unordered_multiset_entry *)umset->buffer;
    pair *p = (pair *)entry->data;

    memcpy(p->data, key, umset->attr->size);
    __unordered_multiset_entry *found_entry = __hash_table_get_entry(umset->ht, entry);
    if (found_entry == NULL) {
        return 0;
    }
    p = (pair *)found_entry->data;
    int *count = (int *)pair_second(p);
    return (size_t)(*count);
}

/*!
 * @brief Checks if the unordered multiset is empty.
 * @param[in] umset The unordered multiset.
 * @return 1 if the unordered multiset is empty, 0 otherwise.
 */
static inline int unordered_multiset_empty(unordered_multiset *restrict umset) {
    CS_RETURN_IF(umset == NULL, 1);
    return umset->size == 0;
}

/*!
 * @brief Returns the number of elements in the unordered multiset.
 * @param[in] umset The unordered multiset.
 * @return The number of elements in the unordered multiset.
 */
static inline size_t unordered_multiset_size(unordered_multiset *restrict umset) {
    CS_RETURN_IF(umset == NULL || umset->ht == NULL, 0);
    return umset->size;
}

/*!
 * @brief Clears all elements from the unordered multiset.
 * @param[in,out] umset Pointer to the unordered multiset.
 */
void unordered_multiset_clear(unordered_multiset *restrict umset);

void unordered_multiset_swap(unordered_multiset *umset1, unordered_multiset *umset2);

/*!
 * @brief Prints the contents of the unordered multiset to the specified stream.
 * @param[in] stream The output stream.
 * @param[in] v_umset Pointer to the unordered multiset.
 */
void unordered_multiset_print(FILE *restrict stream, void *restrict v_umset);

/*!
 * @brief Frees the memory allocated for the unordered multiset.
 * @param[in,out] v_umset Pointer to the unordered multiset.
 */
void unordered_multiset_free(void *restrict v_umset);

#endif