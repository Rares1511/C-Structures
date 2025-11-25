#ifndef SET_H
#define SET_H

#include <cs/universal.h>

typedef xuniv_attr_t set_attr_t;

#define SET_NODE_RED_COLOR 0
#define SET_NODE_BLACK_COLOR 1

typedef struct set {
    struct set_node *root; /*!< root of the map */
    int size;              /*!< number of elements in the map */
    set_attr_t attr;       /*!< attributes of the set */
} set;

typedef struct set_node {
    char color;                   /*!< color of the node */
    void *data;                    /*!< data of the node */
    struct set_node *left_child;  /*!< left child of the node */
    struct set_node *right_child; /*!< right child of the node */
    struct set_node *father;      /*!< father of the node */
} set_node;

/*!
 * Initializes a set with the given attributes.
 * @param s Pointer to the set to be initialized.
 * @param attr Attributes for the set.
 * @return One of the following:
 *   @li CS_SUCCESS — Initialization successful.
 *   @li CS_ELEM — The set pointer is NULL.
 *   @li CS_SIZE — The size attribute is invalid.
 */
cs_codes set_init(set *s, set_attr_t attr);

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
void set_print(void *v_s);

/*!
 * Frees the memory allocated for the set and its elements.
 * @param v_s Void pointer to the set to be freed.
 */
void set_free(void *v_s);

#endif