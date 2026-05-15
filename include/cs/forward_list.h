#ifndef __CS_FORWARD_LIST_H__
#define __CS_FORWARD_LIST_H__

#include <cs/universal.h>

#define CS_FORWARD_LIST_MAGIC 0x464C5354 /* "FLST" in hexadecimal */

typedef struct forward_list_node{
    struct forward_list_node* next; /*!< Pointer to the next node in the list */
    char data[];                    /*<! Data storage for the node */
} forward_list_node;

typedef struct forward_list {
    cs_header_t header;      /*!< Header for validation and type identification */
    forward_list_node* head; /*!< Pointer to the first node in the list */
    forward_list_node* tail; /*!< Pointer to the last node in the list */
    elem_attr_t attr;        /*!< Attributes for the elements stored in the list */
    int size;                /*!< Number of elements currently in the list */
} forward_list;

/*!
 * Initializes a forward list with the given attributes.
 * @param attr Attributes for the forward list (e.g., element size).
 * @return Pointer to the initialized forward list on success, or NULL on failure.
 */
forward_list* forward_list_init(elem_attr_t attr);

/*! 
 * Checks if the forward list is empty.
 * @param list The forward list to check.
 * @return 1 if the list is empty, 0 otherwise.
 */
static inline int forward_list_empty(forward_list *list) { return list->size == 0; }

/*! 
 * Retrieves the size of the forward list.
 * @param list The forward list.
 * @return The number of elements in the list.
 */
static inline int forward_list_size(forward_list *list) { return list->size; }

/*! 
 * Inserts a new element at the front of the forward list.
 * @param list Pointer to the forward list.
 * @param data Pointer to the data to insert.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes forward_list_push_front(forward_list* list, const void* data) {
    CS_RETURN_IF(list == NULL || data == NULL, CS_NULL);
    CS_RETURN_IF(list->header.magic != CS_FORWARD_LIST_MAGIC, CS_UNINITIALIZED);

    forward_list_node *node = malloc(sizeof(forward_list_node) + list->attr.size);
    CS_RETURN_IF(node == NULL, CS_MEM);

    // We copy the bytes directly into the node's tail
    if (list->attr.copy)
        list->attr.copy(node->data, data);
    else
        memcpy(node->data, data, list->attr.size);

    node->next = NULL; // Initialize next pointer to NULL

    if (list->size == 0) {
        list->tail = node;
    }

    node->next = list->head;
    list->head = node;
    list->size++;

    return CS_SUCCESS;
}

/*! 
 * Removes the element at the front of the forward list.
 * @param list Pointer to the forward list.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes forward_list_pop_front(forward_list* list) {
    CS_RETURN_IF(list == NULL, CS_NULL);
    CS_RETURN_IF(list->header.magic != CS_FORWARD_LIST_MAGIC, CS_UNINITIALIZED);
    CS_RETURN_IF(list->size == 0, CS_EMPTY);

    forward_list_node* temp = list->head;
    freer fr = list->attr.fr;
    list->head = list->head->next;

    if (fr)
        fr(temp->data);
    free(temp);
    list->size--;

    if (list->size == 0) {
        list->tail = NULL; // If the list is now empty, reset the tail pointer
        list->head = NULL; // Also reset the head pointer for safety
    }

    return CS_SUCCESS;
}

/*! 
 * Finds the index of the first occurrence of the specified data in the forward list.
 * @param list The forward list to search.
 * @param data Pointer to the data to find.
 * @return The index of the element if found, or -1 if not found or on error.
 */
int forward_list_find(forward_list *list, const void* data);

/*! 
 * Retrieves the size of the forward list.
 * @param list The forward list.
 * @return The number of elements in the list.
 */
static inline void forward_list_set_attr(forward_list* list, elem_attr_t attr) {
    CS_RETURN_IF(list == NULL || attr.size <= 0 || attr.size > SIZE_TH || list->header.magic != CS_FORWARD_LIST_MAGIC);
    list->attr = attr;
}

/*! 
 * Sets the size of the forward list.
 * @param list Pointer to the forward list.
 * @param size The new size to set.
 */
static inline void forward_list_set_size(forward_list* list, int size) {
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC);
    list->attr.size = size;
}

/*! 
 * Sets the freer function for the forward list.
 * @param list Pointer to the forward list.
 * @param fr The freer function to set.
 */
static inline void forward_list_set_free(forward_list* list, freer fr) {
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC);
    list->attr.fr = fr;
}

/*! 
 * Sets the copy function for the forward list.
 * @param list Pointer to the forward list.
 * @param cp The copy function to set.
 */
static inline void forward_list_set_copy(forward_list* list, deepcopy cp) {
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC);
    list->attr.copy = cp;
}

/*! 
 * Sets the print function for the forward list.
 * @param list Pointer to the forward list.
 * @param pr The print function to set.
 */
static inline void forward_list_set_print(forward_list* list, printer pr) {
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC);
    list->attr.print = pr;
}

/*! 
 * Sets the compare function for the forward list.
 * @param list Pointer to the forward list.
 * @param cmp The compare function to set.
 */
static inline void forward_list_set_compare(forward_list* list, comparer cmp) {
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC);
    list->attr.comp = cmp;
}

/*! 
 * Retrieves the data at the front of the forward list.
 * @param list The forward list.
 * @return Pointer to the data at the front of the list, or NULL if the list is empty.
 */
void* forward_list_front(forward_list *list) {
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC || list->size == 0, NULL);
    return list->head->data;
}

void forward_list_sort(forward_list *list);

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