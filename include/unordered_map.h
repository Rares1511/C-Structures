#ifndef __CS_UNORDERED_MAP_H__
#define __CS_UNORDERED_MAP_H__

#include <cs/universal.h>

typedef univ_attr_t unordered_map_attr_t;
typedef struct hash_table hash_table;
typedef size_t (*hash_func_t)(const void *key);

typedef struct unordered_map {
    hash_table *ht;
    hash_func_t hash_func;
    unordered_map_attr_t key_attr;
    unordered_map_attr_t value_attr;
} unordered_map;

/*!
 * Initialize an unordered map with the specified attributes and hash function.
 * @param[in] key_attr Attributes for the keys.
 * @param[in] value_attr Attributes for the values.
 * @param[in] hash_func Hash function for the keys.
 * @param[in] initial_capacity Initial capacity of the unordered map.
 * @return Pointer to the initialized unordered map.
 */
unordered_map *unordered_map_init(unordered_map_attr_t key_attr,
                                 unordered_map_attr_t value_attr,
                                 hash_func_t hash_func,
                                 int initial_capacity);

/*!
 * Adds an entry to the unordered map.
 * @param[in,out] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @param[in] value Pointer to the value.
 * @return CS_SUCCESS on success, error code otherwise.
 */
cs_codes unordered_map_add_entry(unordered_map *umap, const void *key, const void *value);

/*!
 * Removes an entry from the unordered map.
 * @param[in,out] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @return CS_SUCCESS on success, error code otherwise.
 */
cs_codes unordered_map_remove_entry(unordered_map *umap, const void *key);

/*!
 * Retrieves an entry from the unordered map.
 * @param[in] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @return Pointer to the value associated with the key, or NULL if not found.
 */
void *unordered_map_get_entry(unordered_map umap, const void *key);

/*!
 * Shows if the unordered map is empty, its size, count of a specific key,
 * @param[in] umap Pointer to the unordered map.
 * @return 1 if empty, 0 otherwise.
 */
int unordered_map_empty(unordered_map umap);

/*!
 * Retrieves the size of the unordered map.
 * @param[in] umap Pointer to the unordered map.
 * @return Size of the unordered map.
 */
int unordered_map_size(unordered_map umap);

/*!
 * Retrieves the count of a specific key in the unordered map.
 * @param[in] umap Pointer to the unordered map.
 * @param[in] key Pointer to the key.
 * @return Count of the key in the unordered map.
 */
int unordered_map_count(unordered_map umap, const void *key);

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