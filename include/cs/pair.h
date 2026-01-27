#ifndef __CS_PAIR_H__
#define __CS_PAIR_H__

#include <cs/universal.h>

typedef univ_attr_t pair_attr_t;

typedef struct pair {
    void* first;
    void* second;
    pair_attr_t* first_attr;
    pair_attr_t* second_attr;
} pair;

/*!
 * Initializes a pair structure with the provided elements and their attributes.
 * @param p Pointer to the pair structure to be initialized.
 * @param first Pointer to the first element.
 * @param first_attr Attributes for the first element (size, copy, free functions).
 * @param second Pointer to the second element.
 * @param second_attr Attributes for the second element (size, copy, free functions).
 * @return CS_SUCCESS on success, or an appropriate error code on failure.
 */
cs_codes pair_init(pair* p, pair_attr_t* first_attr, pair_attr_t* second_attr);

/*!
 * Sets the values of the pair's elements.
 * @param p Pointer to the pair structure.
 * @param first Pointer to the new value for the first element.
 * @param second Pointer to the new value for the second element.
 * @return CS_SUCCESS on success, or an appropriate error code on failure.
 */
cs_codes pair_set(pair* p, const void* first, const void* second);

/*!
 * Retrieves the first element of the pair.
 * @param p The pair structure.
 * @return Pointer to the first element.
 */
void* pair_first(pair p);

/*!
 * Retrieves the second element of the pair.
 * @param p The pair structure.
 * @return Pointer to the second element.
 */
void* pair_second(pair p);

/*!
 * Prints the contents of the pair to the specified output streams.
 * @param p The pair structure to be printed.
 */
void pair_print(FILE *stream, const void *v_p);

/*!
 * Frees the resources allocated for the pair.
 * @param v_p Pointer to the pair structure to be freed.
 */
void pair_free(void *v_p);

#endif