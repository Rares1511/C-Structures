#ifndef __CS_FORWARD_LIST_H__
#define __CS_FORWARD_LIST_H__

#include <cs/universal.h>

typedef univ_attr_t forward_list_attr_t;

typedef struct forward_list_node{
    void* data;
    struct forward_list_node* next;
} forward_list_node;

typedef struct forward_list{
    forward_list_node* head;
    forward_list_attr_t attr;
    int size;
} forward_list;

/*!
 * Initializes a forward list with the given attributes.
 * @param attr Attributes for the forward list (e.g., element size).
 * @param[out] list Pointer to the forward list to initialize.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes forward_list_init(forward_list *list, forward_list_attr_t attr);

/*! 
 * Checks if the forward list is empty.
 * @param list The forward list to check.
 * @return 1 if the list is empty, 0 otherwise.
 */
static inline int forward_list_empty(forward_list list) { return list.size == 0; }

/*! 
 * Retrieves the size of the forward list.
 * @param list The forward list.
 * @return The number of elements in the list.
 */
static inline int forward_list_size(forward_list list) { return list.size; }

/*! 
 * Inserts a new element at the front of the forward list.
 * @param list Pointer to the forward list.
 * @param data Pointer to the data to insert.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes forward_list_push_front(forward_list* list, const void* data);

/*! 
 * Removes the element at the front of the forward list.
 * @param list Pointer to the forward list.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes forward_list_pop_front(forward_list* list);

/*! 
 * Retrieves the size of the forward list.
 * @param list The forward list.
 * @return The number of elements in the list.
 */
static inline void forward_list_set_attr(forward_list* list, forward_list_attr_t attr) {
    CS_RETURN_IF(list == NULL || attr.size <= 0 || attr.size > SIZE_TH);
    list->attr = attr;
}

/*! 
 * Sets the size of the forward list.
 * @param list Pointer to the forward list.
 * @param size The new size to set.
 */
static inline void forward_list_set_size(forward_list* list, int size) {
    CS_RETURN_IF(list == NULL);
    list->attr.size = size;
}

/*! 
 * Sets the freer function for the forward list.
 * @param list Pointer to the forward list.
 * @param fr The freer function to set.
 */
static inline void forward_list_set_free(forward_list* list, freer fr) {
    CS_RETURN_IF(list == NULL);
    list->attr.fr = fr;
}

/*! 
 * Sets the copy function for the forward list.
 * @param list Pointer to the forward list.
 * @param cp The copy function to set.
 */
static inline void forward_list_set_copy(forward_list* list, deepcopy cp) {
    CS_RETURN_IF(list == NULL);
    list->attr.copy = cp;
}

/*! 
 * Sets the print function for the forward list.
 * @param list Pointer to the forward list.
 * @param pr The print function to set.
 */
static inline void forward_list_set_print(forward_list* list, printer pr) {
    CS_RETURN_IF(list == NULL);
    list->attr.print = pr;
}

/*! 
 * Sets the compare function for the forward list.
 * @param list Pointer to the forward list.
 * @param cmp The compare function to set.
 */
static inline void forward_list_set_compare(forward_list* list, comparer cmp) {
    CS_RETURN_IF(list == NULL);
    list->attr.comp = cmp;
}

/*! 
 * Retrieves the data at the front of the forward list.
 * @param list The forward list.
 * @return Pointer to the data at the front of the list, or NULL if the list is empty.
 */
void* forward_list_front(forward_list list);

/*! 
 * Swaps the contents of two forward lists.
 * @param list1 Pointer to the first forward list.
 * @param list2 Pointer to the second forward list.
 */
void forward_list_swap(forward_list* list1, forward_list* list2);

/*! 
 * Clears all elements from the forward list.
 * @param list Pointer to the forward list to clear.
 */
void forward_list_clear(forward_list* list);

/*! 
 * Prints the contents of the forward list.
 * @param v_l Pointer to the forward list to print.
 */
void forward_list_print(FILE *stream, const void* v_l);

/*! 
 * Frees the forward list and its resources.
 * @param v_l Pointer to the forward list to free.
 */
void forward_list_free(void *v_l);

#endif