#ifndef QUEUE_H
#define QUEUE_H

#include "universal.h"

typedef univ_attr_t queue_attr_t;

typedef struct queue_node
{
    void *data;              /*!< data of the queue node */
    struct queue_node *next; /*!< next element in the queue */
} queue_node;

typedef struct queue
{
    queue_node *start; /*!< start node of the queue */
    queue_node *end;   /*!< end node of the queue */
    int size;          /*!< current size of the queue */
    queue_attr_t attr; /*!< attributes for the elements inside the queue */
} queue;

/*!
 * Initializes the given queue with the given attributes for its elements
 * @param[out] q        The queue which will be initialized
 * @param[in]  el_attr  The attributes for the elements inside the queue
 * @return CS_SIZE if a negative or too big value is given for the size or CS_SUCCESS upon a succesful
 * initalization
 */
cs_codes queue_init(queue *q, queue_attr_t el_attr);

/*!
 * Tests if the queue is empty
 * @param[in] q The queue which will be analyzed if its empty
 * @return 1 if the queue is empty or 0 otherwise
 */
int queue_empty(queue q);

/*!
 * Pushes the element at the back of the queue
 * @param[out] q   The queue in which the element will be added
 * @param[in]  el  The element which will be added in the queue
 * @return CS_MEM if there is a memory problem or CS_SUCCESS upon a succesful addition
 */
cs_codes queue_push(queue *q, void *el);

/*!
 * Pops the element at the front of the list
 * @param[in] q The queue in which the element will be popped
 * @return CS_EMPTY if the queue is empty or CS_SUCCESS upon a successful deletion
 */
cs_codes queue_pop(queue *q);

/*!
 * Gives a reference to the element at the front of the queue
 * @param[in] q The queue whose front element will be given
 * @return pointer to the first element of the queue
 */
void *queue_front(queue q);

/*!
 * Gives a reference to the element at the back of the queue
 * @param[in] q The queue whose back element will be given
 * @return pointer to the last element of the queue
 */
void *queue_back(queue q);

/*!
 * Cleans the queue and frees any memory that was used in it
 * @param[in] q The queue which will be cleaned
 */
void queue_clear(queue *q);

/*!
 * Cleans the queue and frees any memory that was used in it
 * @param[in] q The queue whose memory will be freed
 */
void queue_free(void *q_p);

/*!
 * Sets the new attributes for the queue
 * @param[out] q      Queue whose attributes will be changed
 * @param[in]  attr   New attributes for the elements inside the queue
 */
void queue_set_attr(queue *q, queue_attr_t attr);

/*!
 * Sets the new free function for the queue
 * @param[out] q   Queue whose free function will be changed
 * @param[in]  fr  New free function for the datatype inside the queue
 */
void queue_set_free(queue *q, freer fr);

/*!
 * Sets the new print function for the queue
 * @param[out] q      Queue whose print function will be changed
 * @param[in]  print  New print function for the datatype inside the queue
 */
void queue_set_print(queue *q, printer print);

/*!
 * Sets the new copy function for the queue
 * @param[out] q     Queue whose copy function will be changed
 * @param[in]  copy  New copy function for the elements inside the queue
 */
void queue_set_copy(queue *q, deepcopy cp);

/*!
 * Swaps the two queues
 * @param[in] q1,q2 The two queues that will be swapped
 */
void queue_swap(queue *q1, queue *q2);

/*!
 * Prints the elements of the queue
 * @param[in] q The queue whose elements willbe printed
 */
void queue_print(void *q_p);

#endif