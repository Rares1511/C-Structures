#ifndef HEAP_H
#define HEAP_H

#include "..\universal\universal.h"

typedef struct heap {
    void* vec;
    size_t cap, dim, size;
    comparer compare;
} heap;

#define HEAP_SIZE sizeof ( heap )

heap *heap_initiate ( size_t dim, comparer compare );
heap *heap_sort ( heap *h, comparer compare );
enum return_codes heap_push ( heap *h, void *el );
enum return_codes heap_pop ( heap *h );
void heap_swap ( heap *h1, heap *h2 );
int heap_empty ( heap *h );
void *heap_top ( heap *h );
void heap_free ( heap *h );
void heap_print ( heap *h, printer print );

#endif