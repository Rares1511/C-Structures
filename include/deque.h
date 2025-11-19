#ifndef DEQUE_H
#define DEQUE_H

#include "universal.h"

typedef univ_attr_t deque_attr_t;

typedef struct deque_node_t {
    void *data;                /*!< data inside the deque node */
    struct deque_node_t *next; /*!< next element in the deque */
    struct deque_node_t *prev; /*!< previous element in the deque */
} deque_node_t;

typedef struct deque {
    int size;            /*!< current size of the deque */
    deque_node_t *front; /*!< first element in the deque */
    deque_node_t *back;  /*!< last element in the deque */
    deque_attr_t attr;   /*!< attributes for the elements inside the deque */
} deque;

/*!
 * Initializes the deque structure given with the attributes offered
 * @param[out] dq       The deque variable that will be initialized
 * @param[in]  el_attr  The attributes for the elements inside of the deque
 * @return CS_SIZE if a negative or too big size is given for the elements or CS_SUCCESS upon
 * a successful initialization
 */
cs_codes deque_init(deque *dq, deque_attr_t el_attr);

/*!
 * Pushes the element at the back of the deque
 * @param[out] dq  The deque in which the element will be added
 * @param[in]  el  The element whose value will be added in the deque
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes deque_push_back(deque *dq, void *el);

/*!
 * Pushes the element at the front of the deque
 * @param[out] dq  The deque in which the element will be added
 * @param[in]  el  The element whose value will be added in the deque
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes deque_push_front(deque *dq, void *el);

/*!
 * Pops the element at the back of the deque
 * @param[out] dq  The deque whose last element will be deleted
 * @return CS_SIZE if the deque given is empty or CS_SUCCESS for a successful deletion
 */
cs_codes deque_pop_back(deque *dq);

/*!
 * Pops the element at the front of the deque
 * @param[out] dq  The deque whose first element will be deleted
 * @return CS_SIZE if the deque given is empty or CS_SUCCESS for a successful deletion
 */
cs_codes deque_pop_front(deque *dq);

/*!
 * Copies the elements from the source deque to the destination deque
 * @param[out] dest  The deque that will be copied to
 * @param[in]  src   The deque that will be copied from
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful copy
 */
cs_codes deque_clone(deque *dest, deque src);

/*!
 * Returns the element at the front of the deque
 * @param[in] dq Deque whose element will be accessed
 */
void *deque_front(deque dq);

/*!
 * Returns the element at the back of the deque
 * @param[in] dq Deque whose element will be accessed
 */
void *deque_back(deque dq);

/*!
 * Sets the new attributes for the deque
 * @param[out] dq    Deque whose attributes will be changed
 * @param[in]  attr  New attributes of the deque
 */
static inline void deque_set_attr(deque *dq, deque_attr_t attr) { dq->attr = attr; }

/*!
 * Sets the new free function for the deque
 * @param[out] dq  Deque whose free function will be changed
 * @param[in]  fr  New free function for the datatype inside of the deque
 */
static inline void deque_set_free(deque *dq, freer fr) { dq->attr.fr = fr; }

/*!
 * Sets the new print function for the deque
 * @param[out] dq     Deque whose print function will be changed
 * @param[in]  print  New print function for the datatype inside of the deque
 */
static inline void deque_set_print(deque *dq, printer print) { dq->attr.print = print; }

/*!
 * Sets the new copy function for the deque
 * @param[out] dq  Deque whose copy function will be set or changed
 * @param[in]  cp  New copy function used in copying the elements in the deque
 */
static inline void deque_set_copy(deque *dq, deepcopy cp) { dq->attr.copy = cp; }

/*!
 * Sets the new output stream for the deque
 * @param[out] dq      Deque whose output stream will be changed
 * @param[in]  stream  New output stream for the printer function
 */
static inline void deque_set_stream(deque *dq, FILE *stream) { dq->attr.stream = stream; }

/*!
 * Clears the deque, frees the memory that it was used in it and empties for a next use
 * @param[out] dq  Deque that will be emptied
 */
void deque_clear(deque *dq);

/*!
 * Swaps the two given deques
 * @param dq1,dq2  The deques that will be swapped
 */
void deque_swap(deque *dq1, deque *dq2);

/*!
 * Frees the memory that the deque used
 * @param v_dq Deque that will be freed
 */
void deque_free(void *v_dq);

/*!
 * Prints the elements inside of the deque
 * @param[in] v_dq Deque that will be printed
 */
void deque_print(void *v_dq);

#endif