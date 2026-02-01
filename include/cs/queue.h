#ifndef __CS_QUEUE_H__
#define __CS_QUEUE_H__

#include <cs/universal.h>

typedef univ_attr_t queue_attr;

typedef enum queue_type {
    CS_QUEUE_LIST,
    CS_QUEUE_DEQUE,
    CS_QUEUE_DEFAULT = CS_QUEUE_DEQUE
} queue_type;

typedef struct queue {
    void *container;
    queue_type type;
} queue;

/*!
 * Initialize a queue with given attributes and underlying container type.
 * @param[out] q Pointer to the queue to be initialized.
 * @param[in] attr Attributes for the queue (currently unused, set to 0).
 * @param[in] type The underlying container type for the queue.
 * @return Pointer to the initialized queue structure.
 */
cs_codes queue_init(queue *q, queue_attr attr, queue_type type);

/*! Push an element to the back of the queue.
 * @param[in,out] q Pointer to the queue.
 * @param[in] data Pointer to the data to be pushed.
 * @return cs_codes indicating success or failure.
 */
cs_codes queue_push(queue *q, void *data);

/*! Pop an element from the front of the queue.
 * @param[in,out] q Pointer to the queue.
 * @return cs_codes indicating success or failure.
 */
cs_codes queue_pop(queue *q);

/*! Get the front element of the queue without removing it.
 * @param[in] q Pointer to the queue.
 * @return Pointer to the front element data.
 */
void* queue_front(queue *q);

/*! Get the back element of the queue without removing it.
 * @param[in] q Pointer to the queue.
 * @return Pointer to the back element data.
 */
void* queue_back(queue *q);

/*! Check if the queue is empty.
 * @param[in] q Pointer to the queue.
 * @return 1 if empty, 0 otherwise.
 */
int queue_empty(queue q);

/*! Get the size of the queue.
 * @param[in] q Pointer to the queue.
 * @return Number of elements in the queue.
 */
int queue_size(queue q);

/*! Clear all elements from the queue.
 * @param[in,out] q Pointer to the queue.
 */
void queue_clear(queue *q);

/*! Swap the contents of two queues.
 * @param[in,out] q1 Pointer to the first queue.
 * @param[in,out] q2 Pointer to the second queue.
 */
void queue_swap(queue *q1, queue *q2);

/*! Print the contents of the queue to the given stream.
 * @param[in] stream The output stream.
 * @param[in] v_q Pointer to the queue to be printed.
 */
void queue_print(FILE *stream, void *v_q);

/*! Free the memory allocated for the queue.
 * @param[in,out] v_q Pointer to the queue to be freed.
 */
void queue_free(void *v_q);

#endif