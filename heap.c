#include "heap.h"

/* initializes the heap knowing the dimension of the elements it will hold and the 
compare function between them */
heap *heap_initiate ( size_t dim, comparer compare ) {
    heap *h = malloc ( HEAP_SIZE );
    if ( !h ) return NULL;
    h->dim = dim;
    h->size = 0;
    h->cap = INIT_CAPACITY;
    h->vec = malloc ( h->dim * h->cap );
    h->compare = compare;
    return h;
}

/* returns 1 if the heap is empty, 0 otherwise */
int heap_empty ( heap *h ) {
    return h->size == 0;
}

/* add the element given in the given heap */
enum return_codes heap_push ( heap *h, void *el ) {
    if ( h->size == h->cap ) {
        h->vec = realloc ( h->vec, ( h->cap + INIT_CAPACITY ) * h->dim );
        if ( !h->vec ) return MEMORY_PROBLEM;
        h->cap += INIT_CAPACITY;
    }
    memcpy ( h->vec + h->size * h->dim, el, h->dim );
    h->size++;
    size_t pos = h->size;
    while ( pos > 1 && h->compare ( h->vec + h->dim * ( pos - 1 ), h->vec + h->dim * ( pos / 2 - 1 ) ) < 0 ) {
        universal_swap ( h->vec + h->dim * ( pos / 2 - 1 ), h->vec + h->dim * ( pos - 1 ), h->dim );
        pos /= 2;
    }
    return SUCCESSFUL_ADDITION;
}

/* deletes the first element of the heap, or returns EMPTY if offered an empty heap */
enum return_codes heap_pop ( heap *h ) {
    if ( heap_empty ( h ) ) return EMPTY;
    h->size--;
    universal_swap ( h->vec, h->vec + h->size * h->dim, h->dim );
    size_t pos = 1;
    while ( 2 * pos - 1 < h->size ) {
        if ( 2 * pos < h->size ) {
            if ( h->compare ( h->vec + ( 2 * pos - 1 ) * h->dim, h->vec + 2 * pos * h->dim ) < 0 )
                pos = 2 * pos;
            else
                pos = 2 * pos + 1;
        }
        else
            pos = 2 * pos;
        if ( h->compare ( h->vec + ( pos / 2 - 1 ) * h->dim, h->vec + ( pos - 1 ) * h->dim ) <= 0 ) break;
        universal_swap ( h->vec + ( pos / 2 - 1 ) * h->dim, h->vec + ( pos - 1 ) * h->dim, h->dim );
    }
    return SUCCESSFUL_DELETION;
}

/* returns a reference to the top element of the heap, or NULL if the heap is empty */
void *heap_top ( heap *h ) {
    if ( heap_empty ( h ) ) return NULL;
    return h->vec;
}

/* swaps the two heaps, return UNMATCHING DATA if the given heaps contains different data types, 
or SUCCESSFUL REPLACEMENT upon a successful swap */
void heap_swap ( heap *h1, heap *h2 ) {
    universal_swap ( h1, h2, HEAP_SIZE );
}

/* returns a reference to a new heap that is ordered with the compare function and contains 
the elements of the offered heap */
heap *heap_sort ( heap *h, comparer compare ) {
    heap *haux = heap_initiate ( h->dim, compare );
    for ( size_t i = 0; i < h->size; i++ )
        heap_push ( haux, h->vec + i * h->dim );
    return haux;
}

/* frees the memory used for the heap */
void heap_free ( heap *h ) {
    free ( h->vec );
    free ( h );
}

/* prints the heap with the given printer function */
void heap_print ( heap *h, printer print ) {
    for ( size_t i = 0; i < h->size; i++ ) 
        print ( h->vec + h->dim * i );
    printf ( "\n" );
}