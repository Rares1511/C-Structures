#ifndef __CS_UNORDERED_MULTISET_H__
#define __CS_UNORDERED_MULTISET_H__

#include <cs/universal.h>

typedef elem_attr_t unordered_multiset_attr_t;
typedef struct hash_table hash_table;
typedef size_t (*hash_func_t)(const void *key);

typedef struct unordered_multiset {
    hash_table *ht;
} unordered_multiset;

/*!
 * @brief Initializes an unordered multiset with the given attributes, hash function, and initial capacity.
 * @param[out] umset Pointer to the unordered multiset to be initialized.
 * @param[in] attr The attributes of the unordered multiset (element size, copy, compare, free, print functions).
 * @param[in] hash_func The hash function to be used for hashing the elements.
 * @param[in] initial_capacity The initial capacity of the unordered multiset.
 * @return A pointer to the initialized unordered multiset, or NULL on failure.
 */
cs_codes unordered_multiset_init(unordered_multiset *umset, 
                                unordered_multiset_attr_t attr, 
                                hash_func_t hash_func, 
                                int initial_capacity);

/*!
 * @brief Inserts an element into the unordered multiset.
 * @param[in,out] umset Pointer to the unordered multiset.
 * @param[in] key Pointer to the element to be inserted.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes unordered_multiset_insert(unordered_multiset *umset, const void *key);

/*!
 * @brief Erases all occurrences of an element from the unordered multiset.
 * @param[in,out] umset Pointer to the unordered multiset.
 * @param[in] key Pointer to the element to be erased.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes unordered_multiset_erase(unordered_multiset *umset, const void *key);

/*!
 * @brief Finds an element in the unordered multiset.
 * @param[in] umset The unordered multiset.
 * @param[in] key Pointer to the element to be found.
 * @return Pointer to the found element, or NULL if not found.
 */
void* unordered_multiset_find(unordered_multiset umset, const void *key);

/*!
 * @brief Counts the number of occurrences of an element in the unordered multiset.
 * @param[in] umset The unordered multiset.
 * @param[in] key Pointer to the element to be counted.
 * @return The number of occurrences of the element.
 */
int unordered_multiset_count(unordered_multiset umset, const void *key);

/*!
 * @brief Checks if the unordered multiset is empty.
 * @param[in] umset The unordered multiset.
 * @return 1 if the unordered multiset is empty, 0 otherwise.
 */
int unordered_multiset_empty(unordered_multiset umset);

/*!
 * @brief Returns the number of elements in the unordered multiset.
 * @param[in] umset The unordered multiset.
 * @return The number of elements in the unordered multiset.
 */
int unordered_multiset_size(unordered_multiset umset);

/*!
 * @brief Clears all elements from the unordered multiset.
 * @param[in,out] umset Pointer to the unordered multiset.
 */
void unordered_multiset_clear(unordered_multiset *umset);

/*!
 * @brief Prints the contents of the unordered multiset to the specified stream.
 * @param[in] stream The output stream.
 * @param[in] v_umset Pointer to the unordered multiset.
 */
void unordered_multiset_print(FILE *stream, void *v_umset);

/*!
 * @brief Frees the memory allocated for the unordered multiset.
 * @param[in,out] v_umset Pointer to the unordered multiset.
 */
void unordered_multiset_free(void *v_umset);

#endif