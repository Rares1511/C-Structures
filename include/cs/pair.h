#ifndef __CS_PAIR_H__
#define __CS_PAIR_H__

#include <cs/universal.h>

#define CS_PAIR_MAGIC 0x50414952 /* 'PAIR' in ASCII */

typedef struct pair {
    cs_header_t header;
    char has_first;           /* Flags to indicate if the first and second elements are set */
    char has_second;          /* Flags to indicate if the first and second elements are set */
    elem_attr_t* first_attr;  /* Pointers to the attributes of the first and second elements */
    elem_attr_t* second_attr; /* Pointers to the attributes of the first and second elements */
    char data[];              /* <! Flexible array member */
} pair;

static inline void *pair_first(pair *p) {
    CS_RETURN_IF(p == NULL || p->header.magic != CS_PAIR_MAGIC || !p->has_first, NULL);
    return (void*)p->data;
}

static inline void *pair_second(pair *p) {
    CS_RETURN_IF(p == NULL || p->header.magic != CS_PAIR_MAGIC || !p->has_second, NULL);
    return (void*)(p->data + p->first_attr->size);
}

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
static inline cs_codes pair_set(pair* p, const void* first, const void* second) {
    CS_RETURN_IF(p == NULL, CS_NULL);
    CS_RETURN_IF(p->header.magic != CS_PAIR_MAGIC, CS_UNINITIALIZED);
    CS_RETURN_IF(first == NULL && second == NULL, CS_ELEM);
    void *first_val = p->data;
    void *second_val = (char*)p->data + p->first_attr->size;
    if (first) {
        if (p->has_first && p->first_attr->fr) {
            p->first_attr->fr(first_val);
        }
        if (p->first_attr->copy) {
            p->first_attr->copy(first_val, first);
        } else {
            memcpy(first_val, first, p->first_attr->size);
        }
        p->has_first = 1;
    }
    if (second) {
        if (p->has_second && p->second_attr->fr) {
            p->second_attr->fr(second_val);
        }
        if (p->second_attr->copy) {
            p->second_attr->copy(second_val, second);
        } else {
            memcpy(second_val, second, p->second_attr->size);
        }
        p->has_second = 1;
    }
    return CS_SUCCESS;
}

/*!
 * Prints the contents of the pair to the specified output streams.
 * @param p The pair structure to be printed.
 * @param stream The output stream to which the pair's contents will be printed.
 */
void pair_print(FILE *stream, const void *v_p);

/*!
 * Frees the resources allocated for the pair.
 * @param v_p Pointer to the pair structure to be freed.
 */
void pair_free(void *v_p);

#endif