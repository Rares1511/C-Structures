#ifndef __CS_PRIORITY_QUEUE_H__
#define __CS_PRIORITY_QUEUE_H__

#include <cs/universal.h>

typedef enum priority_queue_type {
    CS_PRIORITY_QUEUE_DEQUE,
    CS_PRIORITY_QUEUE_VECTOR,
    CS_PRIORITY_QUEUE_DEFAULT = CS_PRIORITY_QUEUE_VECTOR
} priority_queue_type;

typedef struct priority_queue {
    void *container;
    priority_queue_type type;
} priority_queue;

/*!
 * Initializes a priority queue with the specified attributes and type.
 * @param[out] pq A pointer to the priority queue to initialize.
 * @param[in] attr The attributes for the priority queue.
 * @param[in] type The underlying container type for the priority queue.
 * @return A cs_codes value indicating success or failure.
 */
cs_codes priority_queue_init(priority_queue *pq, elem_attr_t attr, priority_queue_type type);

/*!
 * Pushes an element onto the priority queue.
 * @param[in,out] pq A pointer to the priority queue.
 * @param[in] data A pointer to the data to be pushed.
 * @return A cs_codes value indicating success or failure.
 */
cs_codes priority_queue_push(priority_queue *pq, void *data);

/*!
 * Pops the top element from the priority queue.
 * @param[in,out] pq A pointer to the priority queue.
 * @return A cs_codes value indicating success or failure.
 */
cs_codes priority_queue_pop(priority_queue *pq);

/*!
 * Retrieves the top element of the priority queue without removing it.
 * @param[in] pq The priority queue.
 * @return A pointer to the top element, or NULL if the queue is empty.
 */
void* priority_queue_top(priority_queue pq);

/*!
 * Checks if the priority queue is empty.
 * @param[in] pq The priority queue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int priority_queue_empty(priority_queue pq);

/*!
 * Retrieves the size of the priority queue.
 * @param[in] pq The priority queue.
 * @return The number of elements in the queue.
 */
int priority_queue_size(priority_queue pq);

/*!
 * Clears all elements from the priority queue.
 * @param[in,out] pq A pointer to the priority queue.
 */
void priority_queue_clear(priority_queue *pq);

/*!
 * Swaps the contents of two priority queues.
 * @param[in,out] pq1 A pointer to the first priority queue.
 * @param[in,out] pq2 A pointer to the second priority queue.
 */
void priority_queue_swap(priority_queue *pq1, priority_queue *pq2);

/*!
 * Prints the contents of the priority queue to the specified stream.
 * @param[in] stream The output stream.
 * @param[in] v_pq A pointer to the priority queue.
 */
void priority_queue_print(FILE *stream, void *v_pq);

/*!
 * Frees the memory allocated for the priority queue.
 * @param[in] v_pq A pointer to the priority queue.
 */
void priority_queue_free(void *v_pq);

#endif