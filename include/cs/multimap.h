#ifndef __CS_MULTIMAP_H__
#define __CS_MULTIMAP_H__

#include <cs/universal.h>

typedef univ_attr_t multimap_attr_t;
typedef struct rbt rbt;

typedef struct multimap {
    rbt *t;
    multimap_attr_t key_attr;
    multimap_attr_t value_attr;
    multimap_attr_t vec_attr;
} multimap;

/*!
 * Initialize a multimap structure
 * @param[in] key_attr Attributes of the key datatype
 * @param[in] value_attr Attributes of the value datatype
 * @return Pointer to the initialized multimap structure, or NULL on failure
 */
multimap *multimap_init(multimap_attr_t key_attr,
                           multimap_attr_t value_attr);

/*!
 * Insert a key-value pair into the multimap
 * @param[in,out] mm    Pointer to the multimap
 * @param[in]     key   Pointer to the key
 * @param[in]     value Pointer to the value
 * @return CS_SUCCESS on success, or an error code on failure
 */
cs_codes multimap_insert(multimap *mm, const void *key, const void *value);

/*!
 * Remove all values associated with a key from the multimap
 * @param[in,out] mm   Pointer to the multimap
 * @param[in]     key  Pointer to the key
 * @return CS_SUCCESS on success, or an error code on failure
 */
cs_codes multimap_delete(multimap *mm, const void *key);

/*!
 * Clear all entries from the multimap
 * @param[in,out] mm Pointer to the multimap
 */
void multimap_clear(multimap *mm);

/*!
 * Print the contents of the multimap
 * @param[in] stream Output stream
 * @param[in] v_mm  Pointer to the multimap
 */
void multimap_print(FILE *stream, void *v_mm);

/*!
 * Free the multimap and its resources
 * @param[in] v_mm Pointer to the multimap
 */
void multimap_free(void *v_mm);

#endif