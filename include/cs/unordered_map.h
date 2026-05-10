#ifndef __CS_UNORDERED_MAP_H__
#define __CS_UNORDERED_MAP_H__

#include <cs/universal.h>
#include <cs/hash_table.h>
#include <cs/pair.h>

typedef struct unordered_map {
    __hash_table *ht;        /*<! Underlying hash table for storing entries */
    __hash_func_t hash_func; /*<! Hash function for the keys */
    elem_attr_t *key_attr;   /*<! Attributes for keys and values */
    elem_attr_t *value_attr; /*<! Attributes for keys and values */
    char *buffer;            /*<! Buffer for temporary entry construction */ 
} unordered_map;

#pragma region Helper Structs
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        START OF HELPER STRUCT SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

typedef struct {
    __hash_func_t hash_func;
    char data[]; /* Flexible array member to hold the pair */
} __unordered_map_entry;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                         END OF HELPER STRUCT SECTION                                       ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline void __unordered_map_entry_copy(void *dest, const void *src) {
    __unordered_map_entry *d = (__unordered_map_entry *)dest;
    const __unordered_map_entry *s = (const __unordered_map_entry *)src;

    d->hash_func = s->hash_func;
    
    pair *ps = (pair *)s->data;
    pair *pd = (pair *)d->data;

    memcpy(pd, ps, sizeof(pair));

    void *src_key = pair_first(ps);
    void *dest_key = pair_first(pd);
    void *src_val = pair_second(ps);
    void *dest_val = pair_second(pd);

    if (ps->first_attr->copy) 
        ps->first_attr->copy(dest_key, src_key);
    else 
        memcpy(dest_key, src_key, ps->first_attr->size);

    if (ps->second_attr->copy) 
        ps->second_attr->copy(dest_val, src_val);
    else 
        memcpy(dest_val, src_val, ps->second_attr->size);
}

static inline void __unordered_map_entry_print(FILE *stream, const void *el) {
    CS_RETURN_IF(NULL == el || stream == NULL);
    const __unordered_map_entry *entry = (const __unordered_map_entry *)el;
    pair_print(stream, (pair *)entry->data);
}

static inline void __unordered_map_entry_free(void *el) {
    CS_RETURN_IF(NULL == el);
    __unordered_map_entry *entry = (__unordered_map_entry *)el;
    pair_free((pair *)entry->data);
}

static inline int __unordered_map_entry_comp(const void *a, const void *b) {
    __unordered_map_entry* ea = (__unordered_map_entry*)a;
    __unordered_map_entry* eb = (__unordered_map_entry*)b;
    pair* pa = (pair*)ea->data;
    pair* pb = (pair*)eb->data;

    void *key_a = pair_first(pa);
    void *key_b = pair_first(pb);
    
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(key_a, key_b);
    }
    return memcmp(key_a, key_b, pa->first_attr->size);
}

static inline size_t __unordered_map_entry_hash(const void *el) {
    CS_RETURN_IF(NULL == el, 0);
    __unordered_map_entry *entry = (__unordered_map_entry *)el;
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
 * Initialize an unordered map with the specified attributes and hash function.
 * @param[in] key_attr Attributes for the keys.
 * @param[in] umap Pointer to the unordered map to initialize.
 * @param[in] value_attr Attributes for the values.
 * @param[in] hash_func Hash function for the keys.
 * @return CS_SUCCESS on success, error code otherwise.
 */
cs_codes unordered_map_init(unordered_map *umap,
                                 elem_attr_t key_attr,
                                 elem_attr_t value_attr,
                                 __hash_func_t hash_func);

/*!
 * Adds an entry to the unordered map.
 * @param[in,out] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @param[in] value Pointer to the value.
 * @return CS_SUCCESS on success, error code otherwise.
 */
static inline cs_codes unordered_map_add_entry(unordered_map *umap, const void *key, const void *value) {
    CS_RETURN_IF(NULL == umap || NULL == key || NULL == value, CS_NULL);
    int rc;
    __unordered_map_entry *entry = (__unordered_map_entry *)umap->buffer;
    pair *p = (pair *)entry->data;

    size_t k_sz = umap->key_attr->size;
    size_t v_sz = umap->value_attr->size;
    memcpy(pair_first(p), key, k_sz);
    memcpy(pair_second(p), value, v_sz);

    __hash_table_add_entry(umap->ht, entry, &rc);
    return rc;
}

/*!
 * Removes an entry from the unordered map.
 * @param[in,out] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @return CS_SUCCESS on success, error code otherwise.
 */
static inline cs_codes unordered_map_remove_entry(unordered_map *umap, const void *key) {
    CS_RETURN_IF(NULL == umap || NULL == key, CS_NULL);
    __unordered_map_entry *entry = (__unordered_map_entry *)umap->buffer;
    pair *p = (pair *)entry->data;
    size_t k_sz = umap->key_attr->size;
    memcpy(pair_first(p), key, k_sz);
    
    return __hash_table_remove_entry(umap->ht, entry);
}

/*!
 * Retrieves an entry from the unordered map.
 * @param[in] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @return Pointer to the value associated with the key, or NULL if not found.
 */
static inline void *unordered_map_get_entry(unordered_map *umap, const void *key) {
    CS_RETURN_IF(NULL == key, NULL);
    __unordered_map_entry *entry = (__unordered_map_entry *)umap->buffer;
    pair *p = (pair *)entry->data;
    size_t k_sz = umap->key_attr->size;
    memcpy(pair_first(p), key, k_sz);
    
    void *found_entry = __hash_table_get_entry(umap->ht, entry);
    CS_RETURN_IF(NULL == found_entry, NULL);
    return pair_second((pair *)(((__unordered_map_entry *)found_entry)->data));
}

/*!
 * Shows if the unordered map is empty, its size, count of a specific key,
 * @param[in] umap Pointer to the unordered map.
 * @return 1 if empty, 0 otherwise.
 */
static inline int unordered_map_empty(unordered_map *umap) {
    CS_RETURN_IF(umap == NULL, 1);
    return __hash_table_empty(umap->ht);
}

/*!
 * Retrieves the size of the unordered map.
 * @param[in] umap Pointer to the unordered map.
 * @return Size of the unordered map.
 */
static inline size_t unordered_map_size(unordered_map *umap) {
    CS_RETURN_IF(umap == NULL, 0);
    return __hash_table_size(umap->ht);
}

/*!
 * Retrieves the count of a specific key in the unordered map.
 * @param[in] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @return Count of the key in the unordered map.
 */
static inline size_t unordered_map_count(unordered_map *umap, const void *key) {
    CS_RETURN_IF(NULL == key, 0);
    size_t k_sz = umap->key_attr->size;
    char buffer[sizeof(__unordered_map_entry) + sizeof(pair) + k_sz];
    __unordered_map_entry *entry = (__unordered_map_entry *)buffer;
    pair *p = (pair *)entry->data;

    entry->hash_func = umap->hash_func;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    p->has_first = 1;
    p->has_second = 0;
    p->first_attr = umap->key_attr;
    p->second_attr = umap->value_attr;
    memcpy(pair_first(p), key, k_sz);

    return __hash_table_count(umap->ht, entry);
}

/*!
 * Swaps the contents of two unordered maps.
 * @param[in,out] umap1 Pointer to the first unordered map.
 * @param[in,out] umap2 Pointer to the second unordered map.
 */
void unordered_map_swap(unordered_map *umap1, unordered_map *umap2);

/*!
 * Clears all entries from the unordered map.
 * @param[in,out] umap Pointer to the unordered map.
 */
void unordered_map_clear(unordered_map *umap);

/*!
 * Prints the contents of the unordered map to the specified stream.
 * @param[in] stream File stream to print to.
 * @param[in] v_umap Pointer to the unordered map.
 */
void unordered_map_print(FILE *stream, void *v_umap);

/*!
 * Frees the memory allocated for the unordered map.
 * @param[in,out] v_umap Pointer to the unordered map.
 */
void unordered_map_free(void *v_umap);

#endif