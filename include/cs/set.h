#ifndef __CS_SET_H__
#define __CS_SET_H__

#include <cs/universal.h>

typedef struct rbt rbt;

typedef struct set {
    rbt *t;                /*!< red black tree containing the set data */
} set;

/*!
 * Initializes a set with the given attributes.
 * @param attr Attributes for the set.
 * @param s Pointer to the set to be initialized.
 * @return cs_codes Status code indicating success or type of error.
 */
cs_codes set_init(set *s, elem_attr_t attr);

/*!
 * Inserts a new element into the set.
 * @param s Pointer to the set.
 * @param data Pointer to the data to be inserted.
 * @return cs_codes Status code indicating success or type of error.
 */
cs_codes set_insert(set *s, void *data);

/*!
 * Deletes an element from the set.
 * @param s Pointer to the set.
 * @param data Pointer to the data to be deleted.
 * @return cs_codes Status code indicating success or type of error.
 */
cs_codes set_delete(set *s, void *data);

/*!
 * Checks if the set is empty.
 * @param s The set to check.
 * @return 1 if the set is empty, 0 otherwise.
 */
int set_empty(set s);

/*!
 * Returns the number of elements in the set.
 * @param s The set.
 * @return The size of the set.
 */
int set_size(set s);

/*!
 * Finds an element in the set.
 * @param s The set to search.
 * @param data Pointer to the data to be found.
 * @return Pointer to the found data, or NULL if not found.
 */
void* set_find(set s, void *data);

/*!
 * Swaps the contents of two sets.
 * @param s1 Pointer to the first set.
 * @param s2 Pointer to the second set.
 */
void set_swap(set *s1, set *s2);

/*!
 * Clears all elements from the set.
 * @param s Pointer to the set to be cleared.
 */
void set_clear(set *s);

/*!
 * Prints the contents of the set.
 * @param v_s Void pointer to the set to be printed.
 */
void set_print(FILE *stream, void *v_s);

/*!
 * Frees the memory allocated for the set and its elements.
 * @param v_s Void pointer to the set to be freed.
 */
void set_free(void *v_s);

#endif