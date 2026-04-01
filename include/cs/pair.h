#ifndef __CS_PAIR_H__
#define __CS_PAIR_H__

#include <cs/universal.h>

typedef struct pair {
    void *data;
    char has_first;
    char has_second;
    elem_attr_t* first_attr;
    elem_attr_t* second_attr;
} pair;

static inline void *pair_first_value(pair p) {
    return p.has_first ? p.data : NULL;
}

static inline void *pair_first_ptr(const pair *p) {
    return (p != NULL && p->has_first) ? p->data : NULL;
}

static inline void *pair_second_value(pair p) {
    return p.has_second ? ((char*)p.data + p.first_attr->size) : NULL;
}

static inline void *pair_second_ptr(const pair *p) {
    return (p != NULL && p->has_second) ? ((char*)p->data + p->first_attr->size) : NULL;
}

#define pair_first(p) _Generic((p), \
    pair: pair_first_value, \
    const pair: pair_first_value, \
    volatile pair: pair_first_value, \
    const volatile pair: pair_first_value, \
    pair*: pair_first_ptr, \
    const pair*: pair_first_ptr, \
    volatile pair*: pair_first_ptr, \
    const volatile pair*: pair_first_ptr \
)(p)

#define pair_second(p) _Generic((p), \
    pair: pair_second_value, \
    const pair: pair_second_value, \
    volatile pair: pair_second_value, \
    const volatile pair: pair_second_value, \
    pair*: pair_second_ptr, \
    const pair*: pair_second_ptr, \
    volatile pair*: pair_second_ptr, \
    const volatile pair*: pair_second_ptr \
)(p)

/*!
 * Initializes a pair structure with the provided elements and their attributes.
 * @param p Pointer to the pair structure to be initialized.
 * @param first Pointer to the first element.
 * @param first_attr Attributes for the first element (size, copy, free functions).
 * @param second Pointer to the second element.
 * @param second_attr Attributes for the second element (size, copy, free functions).
 * @return CS_SUCCESS on success, or an appropriate error code on failure.
 */
cs_codes pair_init(pair* p, elem_attr_t* first_attr, elem_attr_t* second_attr);

/*!
 * Sets the values of the pair's elements.
 * @param p Pointer to the pair structure.
 * @param first Pointer to the new value for the first element.
 * @param second Pointer to the new value for the second element.
 * @return CS_SUCCESS on success, or an appropriate error code on failure.
 */
cs_codes pair_set(pair* p, const void* first, const void* second);

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