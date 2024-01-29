#ifndef HEAP_H
#define HEAP_H

#include "../universal/universal.h"

typedef struct heap {
    void* vec;
    size_t cap, dim, size;
    comparer compare;
} heap;

#define HEAP_SIZE sizeof ( heap )

#define heap_empty(h) h->size == 0
#define heap_size(h) h->size
#define heap_value_dimension(h) h->dim

heap* heap_init ( size_t dim );
enum return_codes heap_push ( heap *h, void *el );
enum return_codes heap_pop ( heap *h );
enum return_codes heap_clear ( heap *h );
void* heap_top ( heap *h );
void heap_swap ( heap *h1, heap *h2 );
void heap_set_compare ( heap *h, comparer compare );
void heap_free ( heap *h );
void heap_print ( heap *h, printer print );

#endif