#ifndef LIST_H
#define LIST_H

#include "universal.h"

typedef xuniv_attr_t list_attr_t;

typedef int (*condition)(void *);

typedef struct list_node
{
    void *data;             /*!< information held inside the node */
    struct list_node *next; /*!< next node in the list */
    struct list_node *prev; /*!< previous node in the list */
} list_node;

typedef struct list
{
    list_node *front; /*!< front element of the list */
    int size;         /*!< current size of the list */
    list_attr_t attr; /*!< attributes for the datatype inside the list */
} list;

/*!
 * Initializes the list with th given attributes for the datatype inside
 * @param[out] l        List that will be initialized
 * @param[in]  el_attr  Attributes for the datatype inside the list
 * @return CS_SIZE if given a negative or too big size for the elements or CS_SUCCESS
 * for a successful initialization
 */
cs_codes list_init(list *l, list_attr_t el_attr);

/*!
 * Pushes the element at the front of the list
 * @param[out] l   List in which the element will be added into
 * @param[in]  el  Element that will be added
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes list_push_front(list *l, void *el);

/*!
 * Pushes the element at the back of the list
 * @param[out] l   List in which the element will be added into
 * @param[in]  el  Element that will be added
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes list_push_back(list *l, void *el);

/*!
 * Pops the element at the front list
 * @param[out] l  List whose first element will be deleted
 * @return CS_SIZE if the list is empty or CS_SUCCESS for a successful deletion
 */
cs_codes list_pop_front(list *l);

/*!
 * Pops the element at the back list
 * @param[out] l  List whose last element will be deleted
 * @return CS_SIZE if the list is empty or CS_SUCCESS for a successful deletion
 */
cs_codes list_pop_back(list *l);

/*!
 * Inserts count elements from the given array from the starting position in the list
 * @param[out] l      List that will have new elements inserted in
 * @param[in]  start  Starting position for the addition of new elements
 * @param[in]  count  Number of elements that will be added from the array into the list
 * @param[in]  vec    Array holding at least count datatype elements that will be added in the list
 */
cs_codes list_insert(list *l, int start, int count, void *vec);

/*!
 * Erases a number of elements from the given position
 * @param[out] l      List whose elements will be deleted
 * @param[in]  start  Starting position for starting deleting elements
 * @param[in]  count  Number of elements to be deleted
 * @return CS_SIZE if the list is empty or CS_SUCCESS for a successful deletion
 */
cs_codes list_erase(list *l, int start, int count);

/*!
 * Removes all apparitions of the given element in the list
 * @param[out] l   List in which all of the apparitions of the given element will be deleted
 * @param[in]  el  Element whose apparitions will be deleted
 * @return CS_SIZE if the list is empty or CS_SUCCESS for a successful deletion
 */
cs_codes list_remove(list *l, void *el);

/*!
 * Will delete all elements in the list that respect the given condition function
 * @param[out] l     List whose elements will be deleted if they respect the condition
 * @param[in]  cond  Condition function, will remove elements that return 0
 * @return CS_SIZE if the list is empty or CS_SUCCESS for a successful deletion
 */
cs_codes list_remove_if(list *l, condition cond);

/*!
 * Keeps only the unique elements in the list by using its own comp function
 * @param[out] l  List whose non-unique elements will be deleted all but one
 * @return CS_FUNC if the comp function is not set or CS_SUCCESS for a successful
 * deletion
 */
cs_codes list_unique(list *l);

/*!
 * Merges the two list by inserting the second list from the starting position given
 * in the first list
 * @param[out] l1     List in which the second one will be inserted into
 * @param[in]  l2     List to be inserted
 * @param[in]  start  Starting position from which the list will be inserted
 * @return CS_POS if th position given is negative or bigger that the first list size or
 * CS_SUCCESS for a successful addition
 */
cs_codes list_merge(list *l1, list *l2, int start);

/*!
 * Gives a pointer to the information the front element in the list holds
 * @param[in] l  List whose front element will be accessed
 */
void *list_front(list l);

/*!
 * Gives a pointer to the information the back element in the list holds
 * @param[in] l  List whose back element will be accessed
 */
void *list_back(list l);

/*!
 * Sorts the list if a comp function has been set
 * @param[in] l  List to be sorted
 * @return CS_FUNC if comp function is NULL, CS_MEM if a memory problem occured,
 * CS_SIZE if the list contains elements with a negative or too big size or CS_SUCCESS
 * for a successful sort
 */
cs_codes list_sort(list *l);

/*!
 * Sets the new attributes for the elements of the list
 * @param[out] l     List whose attributes will be changed
 * @param[in]  attr  New attributes for the elements of the list
 */
void list_set_attr(list *l, list_attr_t attr);

/*!
 * Sets the new free function for the datatype inside the list
 * @param[out] l   List whose free function will be changed
 * @param[in]  fr  New free function for the datatype inside the list
 */
void list_set_free(list *l, freer fr);

/*!
 * Sets the new print function for the datatype inside the list
 * @param[out] l      List whose print function will be changed
 * @param[in]  print  New print function for the datatype inside the list
 */
void list_set_print(list *l, printer print);

/*!
 * Sets the new copy function for the datatype inside the list
 * @param[out] l   List whose copy function will be changed
 * @param[in]  cp  New copy function for copying the elements inside the list
 */
void list_set_copy(list *l, deepcopy cp);

/*!
 * Sets the new comp function for the datatype inside the list
 * @param[out] l      List whose comp function will be changed
 * @param[in]  print  New comp function for the datatype inside the list
 */
void list_set_comp(list *l, comparer comp);

/*!
 * Reverses the list
 * @param[out] l  List that will be reversed
 */
void list_reverse(list *l);

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
 * Frees the memory that the list uses
 * @param[out] l  List whose memory will be freed
 */
void list_free(void *l_p);

/*!
 * Prints the elements of the list
 * @param[in] l  List whose elements will be printed
 */
void list_print(void *l_p);

#endif