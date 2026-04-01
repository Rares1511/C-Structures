#ifndef __CS_UNORDERED_MULTIMAP_H__
#define __CS_UNORDERED_MULTIMAP_H__

#include <cs/universal.h>

typedef struct hash_table hash_table;
typedef size_t (*hash_func_t)(const void *key);

typedef struct unordered_multimap {
    hash_table *ht;
    hash_func_t hash_func;
    elem_attr_t key_attr;
    elem_attr_t value_attr;
} unordered_multimap;

/*!
 * @brief Initializes an unordered multimap with the specified attributes and hash function.
 * @param[in] key_attr The attribute descriptor for the keys.
 * @param[in] value_attr The attribute descriptor for the values.
 * @param[in] hash_func The hash function to be used for hashing keys.
 * @param[in] initial_capacity The initial capacity of the unordered multimap.
 * @return A pointer to the initialized unordered multimap.
 */
cs_codes unordered_multimap_init(unordered_multimap *ummap,
                                elem_attr_t key_attr,
                                elem_attr_t value_attr,
                                hash_func_t hash_func,
                                int initial_capacity);

/*!
 * @brief Adds a key-value pair to the unordered multimap.
 * @param[in,out] umap A pointer to the unordered multimap.
 * @param[in] key A pointer to the key to be added.
 * @param[in] value A pointer to the value to be added.
 * @return A cs_codes value indicating success or failure.
 */
cs_codes unordered_multimap_add_entry(unordered_multimap *umap, const void *key, const void *value);

/*!
 * @brief Removes all entries with the specified key from the unordered multimap.
 * @param[in,out] umap A pointer to the unordered multimap.
 * @param[in] key A pointer to the key whose entries are to be removed.
 * @return A cs_codes value indicating success or failure.
 */
cs_codes unordered_multimap_remove_entry(unordered_multimap *umap, const void *key);

/*!
 * @brief Retrieves an entry with the specified key from the unordered multimap.
 * @param[in] umap A pointer to the unordered multimap.
 * @param[in] key A pointer to the key whose entry is to be retrieved.
 * @return A pointer to the entry if found, NULL otherwise.
 */
void *unordered_multimap_get_entry(unordered_multimap umap, const void *key);

/*!
 * @brief Checks if the unordered multimap is empty.
 * @param[in] umap The unordered multimap.
 * @return 1 if empty, 0 otherwise.
 */
int unordered_multimap_empty(unordered_multimap umap);

/*!
 * @brief Returns the number of entries in the unordered multimap.
 * @param[in] umap The unordered multimap.
 * @return The number of entries.
 */
int unordered_multimap_size(unordered_multimap umap);

/*!
 * @brief Returns the number of entries with the specified key in the unordered multimap.
 * @param[in] umap The unordered multimap.
 * @param[in] key A pointer to the key whose count is to be retrieved.
 * @return The number of entries with the specified key.
 */
int unordered_multimap_count(unordered_multimap umap, const void *key);

/*!
 * @brief Swaps the contents of two unordered multimaps.
 * @param[in,out] umap1 A pointer to the first unordered multimap.
 * @param[in,out] umap2 A pointer to the second unordered multimap.
 */
void unordered_multimap_swap(unordered_multimap *umap1, unordered_multimap *umap2);

/*!
 * @brief Clears all entries from the unordered multimap.
 * @param[in,out] umap A pointer to the unordered multimap.
 */
void unordered_multimap_clear(unordered_multimap *umap);

/*!
 * @brief Prints the contents of the unordered multimap to the specified stream.
 * @param[in] stream The output stream.
 * @param[in] v_umap A pointer to the unordered multimap.
 */
void unordered_multimap_print(FILE *stream, void *v_umap);

/*!
 * @brief Frees the memory allocated for the unordered multimap.
 * @param[in,out] v_umap A pointer to the unordered multimap.
 */
void unordered_multimap_free(void *v_umap);

#endif