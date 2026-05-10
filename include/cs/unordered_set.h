#ifndef __CS_UNORDERED_SET_H__
#define __CS_UNORDERED_SET_H__

#include <cs/universal.h>
#include <cs/hash_table.h>

typedef struct unordered_set {
    __hash_table *ht;
} unordered_set;

/*!
 * @brief Initializes an unordered set.
 * @param[out] uset Pointer to the unordered set.
 * @param[in] attr Attributes for the unordered set.
 * @param[in] hash_func Hash function to use for the unordered set.
 * @return Pointer to the initialized unordered set, or NULL on failure.
 */
cs_codes unordered_set_init(unordered_set *uset, elem_attr_t attr, __hash_func_t hash_func);

/*!
 * @brief Inserts a key into the unordered set.
 * @param[in] uset Pointer to the unordered set.
 * @param[in] key Pointer to the key to insert.
 * @return CS_OK on success, error code otherwise.
 */
static inline cs_codes unordered_set_insert(unordered_set *uset, const void *key) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL || key == NULL, CS_NULL);
    int rc;
    __hash_table_add_entry(uset->ht, key, &rc);
    return rc;
}

/*!
 * @brief Erases a key from the unordered set.
 * @param[in] uset Pointer to the unordered set.
 * @param[in] key Pointer to the key to erase.
 * @return CS_OK on success, error code otherwise.
 */
static inline cs_codes unordered_set_erase(unordered_set *uset, const void *key) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL || key == NULL, CS_NULL);
    return __hash_table_remove_entry(uset->ht, key);
}

/*!
 * @brief Finds a key in the unordered set.
 * @param[in] uset The unordered set.
 * @param[in] key Pointer to the key to find.
 * @return Pointer to the key if found, NULL otherwise.
 */
static inline void* unordered_set_find(unordered_set *uset, const void *key) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL || key == NULL, NULL);
    return __hash_table_get_entry(uset->ht, key);
}

/*!
 * @brief Counts occurrences of a key in the unordered set.
 * @param[in] uset The unordered set.
 * @param[in] key Pointer to the key to count.
 * @return Number of occurrences of the key.
 */
static inline int unordered_set_count(unordered_set *uset, const void *key) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL || key == NULL, 0);
    return __hash_table_count(uset->ht, key);
}

/*!
 * @brief Checks if the unordered set is empty.
 * @param[in] uset The unordered set.
 * @return 1 if empty, 0 otherwise.
 */
static inline int unordered_set_empty(unordered_set *uset) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL, 1);
    return __hash_table_empty(uset->ht);
}

/*!
 * @brief Gets the size of the unordered set.
 * @param[in] uset The unordered set.
 * @return The number of elements in the unordered set.
 */
static inline int unordered_set_size(unordered_set *uset) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL, 0);
    return __hash_table_size(uset->ht);
}

/*!
 * @brief Clears the unordered set.
 * @param[in] uset Pointer to the unordered set to clear.
 */
void unordered_set_clear(unordered_set *uset);

/*!
 * @brief Frees the unordered set.
 * @param[in] v_uset Pointer to the unordered set to free.
 */
void unordered_set_free(void *v_uset);

/*!
 * @brief Prints the unordered set to the given stream.
 * @param[in] stream The output stream.
 * @param[in] v_uset Pointer to the unordered set to print.
 */
void unordered_set_print(FILE *stream, void *v_uset);

#endif