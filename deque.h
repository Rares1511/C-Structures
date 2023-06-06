#ifndef DEQUE_H
#define DEQUE_H

#include "universal.h"

typedef struct deque {
    void *vec;
    size_t size, cap, dim;
} deque;

#define DEQUE_SIZE sizeof ( deque )

deque *deque_initiate ( size_t dim );
enum return_codes deque_push_back ( deque *dq, void *el );
enum return_codes deque_push_front ( deque *dq, void *el );
enum return_codes deque_pop_front ( deque *dq );
enum return_codes deque_pop_back ( deque *dq );
void *deque_front ( deque *dq );
void *deque_back ( deque *dq );
void deque_free ( deque *dq );
void deque_print ( deque *dq, printer print );

#endif