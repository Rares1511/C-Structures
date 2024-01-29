#include "vector.h"

/* returns a pointer to an initialized vector structure */
vector *vector_init ( size_t dim ) { 
    vector *vec = malloc ( VECTOR_SIZE );
    if ( !vec ) return NULL;
    vec->cap = INIT_CAPACITY;
    vec->dim = dim;
    vec->size = 0;
    vec->vec = malloc ( vec->dim * vec->cap );
    return vec;
}

/* inserts the element at the position, returns SUCCESSFUL_ADDITION if done correctly,
WRONG_POSITION if the position offered is higher than the size */
enum return_codes vector_insert_at ( vector *vec, void* el, size_t pos ) {
    if ( pos > vec->size ) return WRONG_POSITION;
    if ( vec->size == vec->cap ) {
        vec->vec = realloc ( vec->vec, ( vec->cap + INIT_CAPACITY ) * vec->dim );
        if ( !vec->vec ) return MEMORY_PROBLEM;
        vec->cap += INIT_CAPACITY;
    }
    if ( pos != vec->size )
        memcpy ( vec->vec + vec->dim * ( pos + 1 ), vec->vec + vec->dim * pos, ( vec->size - pos ) * vec->dim );
    memcpy ( vec->vec + vec->dim * pos, el, vec->dim );
    vec->size++;
    return SUCCESSFUL_ADDITION;
}

/* inserts the element el at the end of the vector */
enum return_codes vector_push_back ( vector *vec, void *el ) { 
    return vector_insert_at ( vec, el, vec->size );
}

/* deletes the element that sits at the position pos, returns SUCCESSFUL_DELETION if the
operation has been done correctly, WRONG_POSITION if pos is higher than the size, EMPTY
if the vector is empty*/
enum return_codes vector_erase ( vector *vec, size_t pos ) {
    if ( vector_empty ( vec ) ) return EMPTY;
    if ( pos >= vec->size ) return WRONG_POSITION;
    memcpy ( vec->vec + vec->dim * pos, vec->vec + vec->dim * ( pos + 1 ), ( vec->size - pos ) * vec->dim );
    vec->size--;
    return SUCCESSFUL_DELETION;
}

/* returns a reference to the element at the position pos in the vector, or NULL if it
doesn't exist */
void *vector_at ( vector *vec, size_t pos ) {
    if ( pos >= vec->size || vector_empty ( vec ) ) return NULL;
    return vec->vec + vec->dim * pos;
}

/* replaces the element at pos with the element offered, returns SUCCESSFUL_REPLACE upon
a successful replacement, EMPTY if the vector is empty, WRONG_POSITION if the position
offered is higher than the size of the vector */
enum return_codes vector_replace ( vector *vec, void *el, size_t pos ) {
    if ( vector_empty ( vec ) ) return EMPTY;
    if ( pos >= vec->size ) return WRONG_POSITION;
    memcpy ( vec->vec + vec->dim * pos, el, vec->dim );
    return SUCCESSFUL_REPLACEMENT;
}

/* returns the position that the element offered can be find at if it is in the vector, 
or -1 of the element is not in the vector, it uses the compare function to compare
the elements of the vector */
size_t vector_find ( vector *vec, void *el, comparer comp ) {
    for ( size_t i = 0; i < vec->size; i++ ) {
        if ( comp ( vec->vec + i * vec->dim, el ) == 0 )
            return i;
    }
    return vec->size + 1;
}

/* swaps the two vectors */
void vector_swap ( vector *vec1, vector *vec2 ) { 
    universal_swap ( vec1, vec2, VECTOR_SIZE );
}

void vector_sort ( vector* vec, comparer comp ) {
    qsort ( vec->vec, vec->size, vec->dim, comp );
}

/* frees the memory that the vector uses */
void vector_free ( vector* vec ) { 
    free ( vec->vec );
    free ( vec );
}

/* uses the printer function offered to print the elements of the vector */
void vector_print ( vector *vec, printer print ) { 
    for ( size_t i = 0; i < vec->size; i++ )
        print ( vec->vec + i * vec->dim );
}