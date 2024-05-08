#ifndef HEAP_H
#define HEAP_H

#include "universal.h"

typedef xuniv_attr_t heap_attr_t;

typedef struct heap
{
    void *vec;        /*!< vector holing the elements of the heap */
    int size;         /*!< current size of the heap */
    int cap;          /*!< current capacity of the heap */
    heap_attr_t attr; /*!< attributes for the datatype inside the heap */
} heap;

/*!
 * Initializes the heap structure with the given attributes for the elements inside
 * @param[out] h        The heap that will be initialized
 * @param[in]  el_attr  The attributes for the elements inside
 * @return CS_SIZE if a negative or too big size has been given or CS_SUCCESS upon a successful
 * initialization
 */
cs_codes heap_init(heap *h, heap_attr_t el_attr);

/*!
 * Pushes the element in the heap
 * @param[out] h   The heap in which the element will be added
 * @param[in]  el  The element to be added
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS upon a successful addition
 */
cs_codes heap_push(heap *h, void *el);

/*!
 * Pops the element at the top of the heap
 * @param[out] h  Heap whose top element will be popped
 * @return CS_EMPTY if the heap is empty or CS_SUCCESS upon a successful deletion
 */
cs_codes heap_pop(heap *h);

/*!
 * Checks if the heap given is empty
 * @param[in] h  Heap whose size will be checked
 * @return 1 if the heap is empty, or 0 otherwise
 */
int heap_empty(heap h);

/*!
 * Gives a pointer to the top element inside the heap
 * @param[in] h  Heap whose top element will be accessed
 */
void *heap_top(heap h);

/*!
 * Sets the new attributes for the heap
 * @param[out] h     Heap whose attributes will be changed
 * @param[in]  attr  New attributes for the elements of the heap
 */
void heap_set_attr(heap *h, heap_attr_t attr);

/*!
 * Sets the new free function for the heap
 * @param[out] h   Heap whose free function will be changed
 * @param[in]  fr  New free function for the datatype inside the heap
 */
void heap_set_free(heap *h, freer fr);

/*!
 * Sets the new print function for the heap
 * @param[out] h      Heap whose print function will be changed
 * @param[in]  print  New print function for the datatype inside the heap
 */
void heap_set_print(heap *h, printer print);

/*!
 * Sets the new copy function for the elements of the heap
 * @param[out] h   Heap whose copy function will be changed
 * @param[in]  cp  New copy function for copying the elements inside the heap
 */
void heap_set_copy(heap *h, deepcopy cp);

/*!
 * Sets the new compare function for the heap, additionally it will reorganize the heap
 * with the new function
 * @param[out] h     Heap whose function will be changed
 * @param[in]  comp  New compare function for the datatype inside the heap
 */
void heap_set_comp(heap *h, comparer comp);

/*!
 * Swaps the two given heaps
 * @param h1,h2 The two heaps whose info will be swapped
 */
void heap_swap(heap *h1, heap *h2);

/*!
 * Clears the heap and prepares it for another use
 * @param[out] h  Heap that will be cleared
 */
void heap_clear(heap *h);

/*!
 * Frees the memory that the heap used
 * @param[in] h Heap that will be freed
 */
void heap_free(heap *h);

/*!
 * Prints the elements inide the vector of the heap
 * @param[in] h  Heap whose elements will be accesed
 */
void heap_print(heap h);

#endif