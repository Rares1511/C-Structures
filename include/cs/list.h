#ifndef __CS_LIST_H__
#define __CS_LIST_H__

#include <cs/universal.h>

#define CS_LIST_MAGIC 0x4C495354 /* "LIST" in hexadecimal */

typedef struct list_node {
    struct list_node *next; /*!< next node in the list */
    struct list_node *prev; /*!< previous node in the list */
    char data[];             /*!< flexible array member to hold the data */
} list_node;

typedef struct list {
    cs_header_t header; /*!< header for the list */
    int size;         /*!< size of the list */
    list_node *front; /*!< front element of the list */
    elem_attr_t attr; /*!< attributes for the datatype inside the list */
} list;

static inline list_node* _list_node_init(const void *el, size_t elem_size, deepcopy copy) {
    // We allocate the struct size PLUS the data size in one block
    list_node *node = malloc(sizeof(list_node) + elem_size);
    if (!node) return NULL;

    // We copy the bytes directly into the node's tail
    if (copy)
        copy(node->data, el);
    else
        memcpy(node->data, el, elem_size);

    node->next = node->prev = node; // Initialize as circular
    
    return node;
}

/*!
 * Initializes the list with th given attributes for the datatype inside
 * @param[in] attr Attributes for the datatype inside the list
 * @param[out] l  Pointer to the list that will be initialized
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful initialization
 */
cs_codes list_init(list *l, elem_attr_t attr);

/*!
 * Pushes the element at the front of the list
 * @param[out] l   List in which the element will be added into
 * @param[in]  el  Element that will be added
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
static inline cs_codes list_push_front(list *l, const void *el) {
    CS_RETURN_IF(l == NULL || el == NULL, CS_NULL);
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, CS_UNINITIALIZED);
    list_node *aux = _list_node_init(el, l->attr.size, l->attr.copy);
    CS_RETURN_IF(aux == NULL, CS_MEM);
    if (l->size > 0) {
        aux->prev = l->front->prev;
        l->front->prev->next = aux;
        l->front->prev = aux;
        aux->next = l->front;
    }
    l->front = aux;
    l->size++;
    return CS_SUCCESS;
}

/*!
 * Pushes the element at the back of the list
 * @param[out] l   List in which the element will be added into
 * @param[in]  el  Element that will be added
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
static inline cs_codes list_push_back(list *l, const void *el) {
    CS_RETURN_IF(l == NULL || el == NULL, CS_NULL);
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, CS_UNINITIALIZED);
    list_node *aux = _list_node_init(el, l->attr.size, l->attr.copy);
    CS_RETURN_IF(aux == NULL, CS_MEM);
    if (l->size > 0) {
        l->front->prev->next = aux;
        aux->prev = l->front->prev;
        aux->next = l->front;
        l->front->prev = aux;
    }
    else
        l->front = aux;
    l->size++;
    return CS_SUCCESS;
}

/*!
 * Pops the element at the front list
 * @param[out] l  List whose first element will be deleted
 * @return CS_SIZE if the list is empty or CS_SUCCESS for a successful deletion
 */
static inline cs_codes list_pop_front(list *l) {
    CS_RETURN_IF(l == NULL, CS_NULL);
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, CS_UNINITIALIZED);
    CS_RETURN_IF(l->size == 0, CS_EMPTY);
    freer fr = l->attr.fr;
    l->size--;
    if (l->size == 0) {
        if (fr)
            fr(l->front->data);
        free(l->front);
        l->front = NULL;
        return CS_SUCCESS;
    }
    list_node *aux = l->front;
    l->front->prev->next = l->front->next;
    l->front->next->prev = l->front->prev;
    l->front = l->front->next;
    
    
    if (fr)   
        fr(aux->data);
    free(aux);
    return CS_SUCCESS;
}

/*!
 * Pops the element at the back list
 * @param[out] l  List whose last element will be deleted
 * @return CS_SIZE if the list is empty or CS_SUCCESS for a successful deletion
 */
static inline cs_codes list_pop_back(list *l) {
    CS_RETURN_IF(l == NULL, CS_NULL);
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, CS_UNINITIALIZED);
    CS_RETURN_IF(l->size == 0, CS_EMPTY);
    freer fr = l->attr.fr;
    l->size--;
    if (l->size == 0) {
        if (fr)
            fr(l->front->data);
        free(l->front);
        l->front = NULL;
        return CS_SUCCESS;
    }
    list_node *aux = l->front->prev;
    l->front->prev->prev->next = l->front;
    l->front->prev = l->front->prev->prev;
    if (fr)
        fr(aux->data);
    free(aux);
    return CS_SUCCESS;
}

/*!
 * Erases the element at the given position from the list
 * @param[out] l    List from which the element will be deleted
 * @param[in]  pos  Position of the element that will be deleted
 * @return CS_SIZE if the position is invalid or CS_SUCCESS for a successful deletion
 */
cs_codes list_erase(list *l, int pos);

/*!
 * Finds the position of the first occurrence of the given element in the list
 * @param[in] l  List in which the element will be searched for
 * @param[in] el Element that will be searched for
 * @return Position of the first occurrence of the element or -1 if not found
 */
int list_find(list *l, const void *el);

/*!
 * Returns if the list is empty
 * @param[in] l  List that will be checked
 * @return 1 if the list is empty, 0 otherwise 
 */
static inline int list_empty(list *l) { return l->size == 0; }

/*!
 * Returns the number of elements in the list
 * @param[in] l  List whose size will be returned
 */
static inline int list_size(list *l) { return l->size; }

/*!
 * Gives a pointer to the information the front element in the list holds
 * @param[in] l  List whose front element will be accessed
 */
static inline void *list_front(list *l) {
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, NULL);
    CS_RETURN_IF(l->size == 0, NULL);
    return l->front->data;
}

/*!
 * Gives a pointer to the information the back element in the list holds
 * @param[in] l  List whose back element will be accessed
 */
static inline void *list_back(list *l) {
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, NULL);
    CS_RETURN_IF(l->size == 0, NULL);
    return l->front->prev->data;
}

/*!
 * Sorts the list if a comp function has been set
 * @param[in] l  List to be sorted
 * @return CS_FUNC if comp function is NULL, CS_MEM if a memory problem occured,
 * CS_SIZE if the list contains elements with a negative or too big size or CS_SUCCESS
 * for a successful sort
 */
void list_sort(list *l);

/*!
 * Sets the new attributes for the elements of the list
 * @param[out] l     List whose attributes will be changed
 * @param[in]  attr  New attributes for the elements of the list
 */
static inline void list_set_attr(list *l, elem_attr_t attr) { 
    CS_RETURN_IF(l == NULL || attr.size <= 0 || attr.size > SIZE_TH || l->header.magic != CS_LIST_MAGIC);
    l->attr = attr; 
}

/*!
 * Sets the new free function for the datatype inside the list
 * @param[out] l   List whose free function will be changed
 * @param[in]  fr  New free function for the datatype inside the list
 */
static inline void list_set_free(list *l, freer fr) { 
    CS_RETURN_IF(l == NULL || l->header.magic != CS_LIST_MAGIC);
    l->attr.fr = fr; 
}

/*!
 * Sets the new print function for the datatype inside the list
 * @param[out] l      List whose print function will be changed
 * @param[in]  print  New print function for the datatype inside the list
 */
static inline void list_set_print(list *l, printer print) { 
    CS_RETURN_IF(l == NULL || l->header.magic != CS_LIST_MAGIC);
    l->attr.print = print; 
}

/*!
 * Sets the new copy function for the datatype inside the list
 * @param[out] l   List whose copy function will be changed
 * @param[in]  cp  New copy function for copying the elements inside the list
 */
static inline void list_set_copy(list *l, deepcopy copy) { 
    CS_RETURN_IF(l == NULL || l->header.magic != CS_LIST_MAGIC);
    l->attr.copy = copy; 
}

/*!
 * Sets the new comp function for the datatype inside the list
 * @param[out] l      List whose comp function will be changed
 * @param[in]  print  New comp function for the datatype inside the list
 */
static inline void list_set_comp(list *l, comparer comp) { 
    CS_RETURN_IF(l == NULL || l->header.magic != CS_LIST_MAGIC);
    l->attr.comp = comp; 
}

/*!
 * Swaps the two given lists
 * @param[in] l1,l2  Lists that will be swapped
 */
void list_swap(list *l1, list *l2);

/*!
 * Clears the list and prepares it for further usage
 * @param[out] l  List that will be cleared
 */
void list_clear(list *l);

/*!
 * Prints the elements of the list
 * @param[in] l  List whose elements will be printed
 */
void list_print(FILE *stream, void *l_p);

/*!
 * Frees the memory that the list uses
 * @param[out] l  List whose memory will be freed
 */
void list_free(void *l_p);

#endif