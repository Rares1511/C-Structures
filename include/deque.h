#ifndef DEQUE_H
#define DEQUE_H

#include <cs/universal.h>

#define DEQUE_INIT_BLOCKS 64
#define DEQUE_BLOCK_SIZE 64

typedef xuniv_attr_t deque_attr_t;

typedef struct deque_block_t {
    void *data;
    int front;
    int back;
} deque_block_t;

typedef struct deque {
    deque_block_t *blocks;
    deque_attr_t attr;
    int size;
    int front;
    int back;
} deque;

/*!
 * Initializes a deque with the specified attributes.
  * @param dq Pointer to the deque to initialize.
  * @param attr Attributes for the deque.
  * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_init(deque *dq, deque_attr_t attr);

/*! 
 * Pushes an element to the back of the deque.
 * @param dq Pointer to the deque.
 * @param el Pointer to the element to push.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_push_back(deque *dq, void *el);

/*! 
 * Pushes an element to the front of the deque.
 * @param dq Pointer to the deque.
 * @param el Pointer to the element to push.
 * @return CS_SUCCESS on success, or an error code on failure.
 */
cs_codes deque_push_front(deque *dq, void *el);

#endif