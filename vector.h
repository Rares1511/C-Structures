#ifndef VECTOR_H
#define VECTOR_H

#include "universal.h"

typedef struct vector {
    void* vec;
    size_t cap, dim, size;
} vector;

#define VECTOR_SIZE sizeof ( vector )

vector *vector_initiate ( size_t elem_size );
enum return_codes vector_insert_at ( vector *vec, void* el, size_t pos );
enum return_codes vector_push_back ( vector *vec, void *el );
enum return_codes vector_erase ( vector *vec, size_t pos );
enum return_codes vector_replace ( vector *vec, void *el, size_t pos );
void vector_swap ( vector *vec1, vector *vec2 );
size_t vector_find ( vector *vec, void *el );
void *vector_at ( vector *vec, size_t pos );
void vector_sort ( vector *vec, comparer compare );
void vector_free ( vector* vec );
void vector_print ( vector *vec, printer func );

#endif
