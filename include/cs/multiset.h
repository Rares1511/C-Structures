#ifndef __CS_MULTISET_H__
#define __CS_MULTISET_H__

#include <cs/universal.h>

typedef struct rbt rbt;

typedef struct multiset {
    rbt *t;               /*!< red black tree containing the multiset data */
    elem_attr_t* el_attr; /*!< attributes of the multiset elements */
    elem_attr_t* count_attr; /*!< attributes of the count elements */
    int size;             /*!< number of unique elements in the multiset */
} multiset;

/*!
 * Initialize a multiset structure
 * @param[in] ms Pointer to the multiset structure to initialize
 * @param[in] attr Attributes of the multiset element datatype
 * @return CS_SUCCESS on success, or an error code on failure
 */
cs_codes multiset_init(multiset *ms, elem_attr_t attr);

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
 * Check if the multiset is empty
 * @param[in] ms Pointer to the multiset
 * @return 1 if the multiset is empty, 0 otherwise
 */
static inline int multiset_empty(multiset ms) { return ms.size == 0; }

/*! 
 * Get the number of unique elements in the multiset
 * @param[in] ms Pointer to the multiset
 * @return Number of unique elements in the multiset
 */
static inline int multiset_size(multiset ms) { return ms.size; }

/*! 
 * Clear all entries from the multiset
 * @param[in,out] ms Pointer to the multiset
 */
void multiset_clear(multiset *ms);

/*! 
 * Swap the contents of two multisets
 * @param[in,out] ms1 Pointer to the first multiset
 * @param[in,out] ms2 Pointer to the second multiset
 */
void multiset_swap(multiset *ms1, multiset *ms2);

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