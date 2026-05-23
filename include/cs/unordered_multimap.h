#ifndef __CS_UNORDERED_MULTIMAP_H__
#define __CS_UNORDERED_MULTIMAP_H__

#include <cs/universal.h>
#include <cs/pair.h>
#include <cs/hash_table.h>
#include <cs/vector.h>

typedef struct unordered_multimap {
    __hash_table *ht;          /*!< Hash table to store the key-value pairs */
    __hash_func_t hash_func;   /*!< Hash function for hashing keys */
    elem_attr_t *key_attr;     /*!< Attribute descriptor for keys */
    elem_attr_t *value_attr;   /*!< Attribute descriptor for values */
    size_t size;               /*!< Number of entries in the unordered multimap */
    char *buffer;              /*!< Buffer for temporary storage during operations */
} unordered_multimap;

#pragma region Helper Structs
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        START OF HELPER STRUCT SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

typedef struct {
    __hash_func_t hash_func;  /*!< Hash function for hashing the key */
    char data[];              /*!< Flexible array member to hold the key-value pair data */
} __unordered_multimap_entry;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                         END OF HELPER STRUCT SECTION                                       ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline void __unordered_multimap_entry_copy(void *dest, const void *src) {
    __unordered_multimap_entry *d = (__unordered_multimap_entry *)dest;
    __unordered_multimap_entry *s = (__unordered_multimap_entry *)src;

    d->hash_func = s->hash_func;
    
    pair *pd = (pair *)d->data;
    pair *ps = (pair *)s->data;

    memcpy(pd, ps, sizeof(pair));

    void *pd_first = pd->data;
    void *ps_first = ps->data;

    if (ps->first_attr->copy) {
        ps->first_attr->copy(pd_first, ps_first);
    } else {
        memcpy(pd_first, ps_first, ps->first_attr->size);
    }

    vector *pd_second = (vector *) (pd->data + pd->first_attr->size);
    vector *ps_second = (vector *) (ps->data + ps->first_attr->size);

    memcpy(pd_second, ps_second, sizeof(vector));
    pd_second->vec = malloc(pd_second->attr.size * pd_second->cap);
}

static inline void __unordered_multimap_entry_print(FILE *stream, const void *el) {
    CS_RETURN_IF(NULL == stream || NULL == el);
    const __unordered_multimap_entry *entry = (const __unordered_multimap_entry *)el;
    pair_print(stream, entry->data);
}

static inline void __unordered_multimap_entry_free(void *el) {
    CS_RETURN_IF(NULL == el);
    __unordered_multimap_entry *entry = (__unordered_multimap_entry *)el;
    __pair_free_internal((pair *)entry->data);
}

static inline int __unordered_multimap_entry_comp(const void *a, const void *b) {
    __unordered_multimap_entry* ea = (__unordered_multimap_entry*)a;
    __unordered_multimap_entry* eb = (__unordered_multimap_entry*)b;
    pair* pa = (pair*) ea->data;
    pair* pb = (pair*) eb->data;
    void* key_a = pair_first(pa);
    void* key_b = pair_first(pb);
    
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(key_a, key_b);
    }
    return memcmp(key_a, key_b, pa->first_attr->size);
}

static inline size_t __unordered_multimap_entry_hash(const void *el) {
    CS_RETURN_IF(NULL == el, 0);
    __unordered_multimap_entry *entry = (__unordered_multimap_entry *)el;
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
 * @brief Initializes an unordered multimap with the specified attributes and hash function.
 * @param[in] key_attr The attribute descriptor for the keys.
 * @param[in] value_attr The attribute descriptor for the values.
 * @param[in] hash_func The hash function to be used for hashing keys.
 * @return A pointer to the initialized unordered multimap.
 */
unordered_multimap* unordered_multimap_init(unordered_multimap *pool,
                                elem_attr_t key_attr,
                                elem_attr_t value_attr,
                                __hash_func_t hash_func);

/*!
 * @brief Adds a key-value pair to the unordered multimap.
 * @param[in,out] ummap A pointer to the unordered multimap.
 * @param[in] key A pointer to the key to be added.
 * @param[in] value A pointer to the value to be added.
 * @return A cs_codes value indicating success or failure.
 */
static inline cs_codes unordered_multimap_add_entry(unordered_multimap *ummap, const void *key, const void *value) {
    CS_RETURN_IF(NULL == ummap || NULL == key || NULL == value, CS_NULL);
    __unordered_multimap_entry *entry = (__unordered_multimap_entry *)ummap->buffer;
    pair *p = (pair *)entry->data;
    int rc = CS_SUCCESS;

    memcpy(p->data, key, ummap->key_attr->size);

    __unordered_multimap_entry *existing_entry = __hash_table_add_entry(ummap->ht, entry, &rc);
    if (existing_entry == NULL) {
        return rc;
    }

    p = (pair *)existing_entry->data;
    vector *vec = (vector *)pair_second(p);
    rc = vector_push_back(vec, value);
    if (rc != CS_SUCCESS) {
        return rc;
    }
    ummap->size++;
    return CS_SUCCESS;
}

/*!
 * @brief Removes all entries with the specified key from the unordered multimap.
 * @param[in,out] ummap A pointer to the unordered multimap.
 * @param[in] key A pointer to the key whose entries are to be removed.
 * @return A cs_codes value indicating success or failure.
 */
static inline cs_codes unordered_multimap_remove_entry(unordered_multimap *ummap, const void *key) {
    CS_RETURN_IF(NULL == ummap || NULL == key, CS_NULL);
    __unordered_multimap_entry *entry = (__unordered_multimap_entry *)ummap->buffer;
    pair *p = (pair *)entry->data;
    int rc;

    memcpy(p->data, key, ummap->key_attr->size);

    rc = __hash_table_remove_entry(ummap->ht, entry);
    if (rc != CS_SUCCESS) {
        return rc;
    }
    ummap->size--;
    return CS_SUCCESS;
}

/*!
 * @brief Retrieves an entry with the specified key from the unordered multimap.
 * @param[in] ummap A pointer to the unordered multimap.
 * @param[in] key A pointer to the key whose entry is to be retrieved.
 * @return A pointer to the entry if found, NULL otherwise.
 */
static inline vector *unordered_multimap_get_entry(unordered_multimap *ummap, const void *key) {
    CS_RETURN_IF(NULL == ummap || NULL == key || NULL == ummap->ht, NULL);
    __unordered_multimap_entry *entry = (__unordered_multimap_entry *)ummap->buffer;
    pair *p = (pair *)entry->data;

    memcpy(p->data, key, ummap->key_attr->size);
    __unordered_multimap_entry *found_entry = __hash_table_get_entry(ummap->ht, entry);
    if (!found_entry) {
        return NULL;
    }
    p = (pair *)found_entry->data;
    return (vector *)pair_second(p);
}

/*!
 * @brief Checks if the unordered multimap is empty.
 * @param[in] ummap The unordered multimap.
 * @return 1 if empty, 0 otherwise.
 */
static inline int unordered_multimap_empty(unordered_multimap *ummap) {
    CS_RETURN_IF(NULL == ummap, 1);
    return ummap->size == 0;
}

/*!
 * @brief Returns the number of entries in the unordered multimap.
 * @param[in] ummap The unordered multimap.
 * @return The number of entries.
 */
static inline size_t unordered_multimap_size(unordered_multimap *ummap) {
    CS_RETURN_IF(NULL == ummap, 0);
    return ummap->size;
}

/*!
 * @brief Returns the number of entries with the specified key in the unordered multimap.
 * @param[in] ummap The unordered multimap.
 * @param[in] key A pointer to the key whose count is to be retrieved.
 * @return The number of entries with the specified key.
 */
static inline size_t unordered_multimap_count(unordered_multimap *ummap, const void *key) {
    CS_RETURN_IF(NULL == ummap || NULL == key || NULL == ummap->ht, 0);
    __unordered_multimap_entry *entry = (__unordered_multimap_entry *)ummap->buffer;
    pair *p = (pair *)entry->data;

    memcpy(p->data, key, ummap->key_attr->size);
    __unordered_multimap_entry *found_entry = __hash_table_get_entry(ummap->ht, entry);
    if (!found_entry) {
        return 0;
    }
    p = (pair *)found_entry->data;
    vector *vec = (vector *)pair_second(p);
    return vector_size(vec);
}

/*!
 * @brief Swaps the contents of two unordered multimaps.
 * @param[in,out] ummap1 A pointer to the first unordered multimap.
 * @param[in,out] ummap2 A pointer to the second unordered multimap.
 */
void unordered_multimap_swap(unordered_multimap *ummap1, unordered_multimap *ummap2);

/*!
 * @brief Clears all entries from the unordered multimap.
 * @param[in,out] ummap A pointer to the unordered multimap.
 */
void unordered_multimap_clear(unordered_multimap *ummap);

/*!
 * @brief Prints the contents of the unordered multimap to the specified stream.
 * @param[in] stream The output stream.
 * @param[in] v_ummap A pointer to the unordered multimap.
 */
void unordered_multimap_print(FILE *stream, void *v_ummap);

/*!
 * @brief Frees the memory allocated for the unordered multimap.
 * @param[in,out] v_ummap A pointer to the unordered multimap.
 */
void unordered_multimap_free(void *v_ummap);

#endif