#ifndef QUEUE_H
#define QUEUE_H

#include "universal.h"

typedef struct queue {
    void* vec;
    int cap, dim, size;
} queue;

#define QUEUE_SIZE sizeof ( queue )

#define queue_empty(q) q->size == 0

queue *queue_init ( int dim );
void *queue_front ( queue *q );
void *queue_back ( queue *q );
void queue_free ( queue *q );
void queue_print ( queue *q, printer print );
enum return_codes queue_push ( queue *q, void *el );
void queue_swap ( queue *q1, queue *q2 );
enum return_codes queue_pop ( queue *q );

#endif