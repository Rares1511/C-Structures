#ifndef __CS_FLAT_SET_H__
#define __CS_FLAT_SET_H__

#include <cs/universal.h>

typedef univ_attr_t flat_set_attr_t;

typedef enum flat_set_type {
    CS_FLAT_SET_DEQUE,
    CS_FLAT_SET_VECTOR,
    CS_FLAT_SET_DEFAULT = CS_FLAT_SET_VECTOR,
} flat_set_type;

typedef struct flat_set {
    void *container;       /*!< Pointer to the underlying container (e.g., vector, deque). */
    flat_set_type type;    /*!< The underlying container type (e.g., vector, deque). */
} flat_set;

/*!
 * Initializes the flat set with the specified attributes and type.
 * @param fs Pointer to the flat_set structure to initialize.
 * @param attr Attributes for the flat set (e.g., element size, comparator function).
 * @param type The underlying container type to use (e.g., vector, deque).
 * @return CS_SUCCESS on success, or an appropriate error code on failure.
 */
cs_codes flat_set_init(flat_set *fs, flat_set_attr_t attr, flat_set_type type);

/*!
 * Inserts an element into the flat set. If the element already exists, it will not be inserted again.
 * @param fs Pointer to the flat_set structure.
 * @param data Pointer to the data to insert.
 * @return CS_SUCCESS on successful insertion, CS_EXISTS if the element already exists, or an appropriate error code on failure.
 */
cs_codes flat_set_insert(flat_set *fs, void *data);

/*!
 * Erases an element from the flat set.
 * @param fs Pointer to the flat_set structure.
 * @param data Pointer to the data to erase.
 * @return CS_SUCCESS on successful erasure, CS_NOT_FOUND if the element does not exist, or an appropriate error code on failure.
 */
cs_codes flat_set_erase(flat_set *fs, void *data);

/*!
 * Finds an element in the flat set.
 * @param fs Pointer to the flat_set structure.
 * @param data Pointer to the data to find.
 * @return The index of the found element, or -1 if the element is not found.
 */
int flat_set_find(flat_set fs, void *data);

/*!
 * Checks if the flat set is empty.
 * @param fs The flat_set structure to check.
 * @return 1 if the flat set is empty, 0 otherwise.
 */
int flat_set_empty(flat_set fs);

/*!
 * Returns the number of elements in the flat set.
 * @param fs The flat_set structure to check.
 * @return The number of elements in the flat set.
 */
int flat_set_size(flat_set fs);

/*!
 * Returns a pointer to the element at the specified index in the flat set.
 * @param fs The flat_set structure to check.
 * @param index The index of the element to retrieve.
 * @return A pointer to the element at the specified index, or NULL if the index is out of bounds.
 */
void* flat_set_at(flat_set fs, int index);

/*!
 * Clears all elements from the flat set.
 * @param fs Pointer to the flat_set structure to clear.
 */
void flat_set_clear(flat_set *fs);

/*!
 * Swaps the contents of two flat sets.
 * @param fs1 Pointer to the first flat_set structure.
 * @param fs2 Pointer to the second flat_set structure.
 */
void flat_set_swap(flat_set *fs1, flat_set *fs2);

/*!
 * Prints the contents of the flat set to the specified stream.
 * @param stream The output stream to print to.
 * @param v_fs Pointer to the flat_set structure to print (passed as void* for generic handling).
 */
void flat_set_print(FILE *stream, void *v_fs);

/*!
 * Frees the resources associated with the flat set.
 * @param v_fs Pointer to the flat_set structure to free (passed as void* for generic handling).
 */
void flat_set_free(void *v_fs);

#endif