#ifndef HEAP_H
#define HEAP_H

#include "..\universal\universal.h"

typedef struct heap {
    void* vec;
    size_t cap, dim, size;
    comparer compare;
} heap;

#define HEAP_SIZE sizeof ( heap )

#define heap_empty(h) h->size == 0

heap *heap_initiate ( size_t dim );
enum return_codes heap_push ( heap *h, void *el );
enum return_codes heap_pop ( heap *h );
void heap_swap ( heap *h1, heap *h2 );
void heap_set_compare ( heap *h, comparer compare );
void *heap_top ( heap *h );
void heap_free ( heap *h );
void heap_print ( heap *h, printer print );

#endif