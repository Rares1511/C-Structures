#ifndef __CS_MULTISET_H__
#define __CS_MULTISET_H__

#include <cs/universal.h>

typedef univ_attr_t multiset_attr_t;
typedef struct rbt rbt;

typedef struct multiset {
    rbt *t;               /*!< red black tree containing the multiset data */
    multiset_attr_t el_attr; /*!< attributes of the multiset elements */
    multiset_attr_t count_attr; /*!< attributes of the count elements */
} multiset;

/*!
 * Initialize a multiset structure
 * @param[in] attr Attributes of the multiset element datatype
 * @return Pointer to the initialized multiset structure, or NULL on failure
 */
multiset *multiset_init(multiset_attr_t attr);

/*! 
 * Insert an element into the multiset
 * @param[in,out] ms    Pointer to the multiset
 * @param[in]     elem  Pointer to the element to insert
 * @return CS_SUCCESS on success, or an error code on failure
 */
cs_codes multiset_insert(multiset *ms, const void *elem);

/*! 
 * Delete an element from the multiset
 * @param[in,out] ms    Pointer to the multiset
 * @param[in]     elem  Pointer to the element to delete
 * @return CS_SUCCESS on success, or an error code on failure
 */
cs_codes multiset_delete(multiset *ms, const void *elem);

/*! 
 * Count the occurrences of an element in the multiset
 * @param[in] ms    Pointer to the multiset
 * @param[in] elem  Pointer to the element to count
 * @return Number of occurrences of the element in the multiset
 */
int multiset_count(multiset *ms, const void *elem);

/*! 
 * Clear all entries from the multiset
 * @param[in,out] ms Pointer to the multiset
 */
void multiset_clear(multiset *ms);

/*! 
 * Print the contents of the multiset
 * @param[in] stream Output stream
 * @param[in] v_ms  Pointer to the multiset
 */
void multiset_print(FILE *stream, void *v_ms);

/*! 
 * Free the multiset and its resources
 * @param[in] v_ms Pointer to the multiset
 */
void multiset_free(void *v_ms);

#endif