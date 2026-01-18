#ifndef _UNORDERED_SET_H
#define _UNORDERED_SET_H

#include <cs/universal.h>

typedef xuniv_attr_t unordered_set_attr_t;
typedef struct hash_table hash_table;
typedef size_t (*hash_func_t)(const void *key);

typedef struct unordered_set {
    hash_table *ht;
} unordered_set;

/*!
 * @brief Initializes an unordered set.
 * @param[in] uset Pointer to the unordered set to initialize.
 * @param[in] attr Attributes for the unordered set.
 * @param[in] hash_func Hash function to use for the unordered set.
 * @param[in] initial_capacity Initial capacity of the unordered set.
 * @return CS_OK on success, error code otherwise.
 */
cs_codes unordered_set_init(unordered_set *uset, unordered_set_attr_t attr, hash_func_t hash_func, int initial_capacity);

/*!
 * @brief Inserts a key into the unordered set.
 * @param[in] uset Pointer to the unordered set.
 * @param[in] key Pointer to the key to insert.
 * @return CS_OK on success, error code otherwise.
 */
cs_codes unordered_set_insert(unordered_set *uset, const void *key);

/*!
 * @brief Erases a key from the unordered set.
 * @param[in] uset Pointer to the unordered set.
 * @param[in] key Pointer to the key to erase.
 * @return CS_OK on success, error code otherwise.
 */
cs_codes unordered_set_erase(unordered_set *uset, const void *key);

/*!
 * @brief Finds a key in the unordered set.
 * @param[in] uset The unordered set.
 * @param[in] key Pointer to the key to find.
 * @return Pointer to the key if found, NULL otherwise.
 */
void* unordered_set_find(unordered_set uset, const void *key);

/*!
 * @brief Counts occurrences of a key in the unordered set.
 * @param[in] uset The unordered set.
 * @param[in] key Pointer to the key to count.
 * @return Number of occurrences of the key.
 */
int unordered_set_count(unordered_set uset, const void *key);

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