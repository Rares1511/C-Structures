#include "heap.h"

int heap_compare ( heap *h, void *el1, void *el2 ) {
    if ( h->compare == NULL )
        return universal_compare ( el1, el2, h->dim );
    return h->compare ( el1, el2 );
}

/* initializes the heap knowing the dimension of the elements it will hold and the 
compare function between them */
heap *heap_initiate ( size_t dim ) {
    heap *h = malloc ( HEAP_SIZE );
    if ( !h ) return NULL;
    h->dim = dim;
    h->size = 0;
    h->cap = INIT_CAPACITY;
    h->vec = malloc ( h->dim * h->cap );
    h->compare = NULL;
    return h;
}

/* sets the function for the heap and sorts the heap if it's not empty */
void heap_set_compare ( heap *h, comparer compare ) {
    h->compare = compare;
    if ( heap_empty ( h ) ) return;
    heap *haux = heap_initiate ( h->dim );
    haux->compare = compare;
    for ( size_t i = 0; i < h->size; i++ )
        heap_push ( haux, h->vec + i * h->dim );
    universal_swap ( h, haux, HEAP_SIZE );
    heap_free ( haux );
}

/* add the element given in the given heap */
enum return_codes heap_push ( heap *h, void *el ) {
    if ( h->size == h->cap ) {
        h->vec = realloc ( h->vec, ( h->cap + INIT_CAPACITY ) * h->dim );
        if ( !h->vec ) {
            free ( h );
            return MEMORY_PROBLEM;
        }
        h->cap += INIT_CAPACITY;
    }
    memcpy ( h->vec + h->size * h->dim, el, h->dim );
    h->size++;
    size_t pos = h->size;
    while ( pos > 1 && heap_compare ( h, h->vec + h->dim * ( pos - 1 ), h->vec + h->dim * ( pos / 2 - 1 ) ) < 0 ) {
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
            if ( heap_compare ( h, h->vec + ( 2 * pos - 1 ) * h->dim, h->vec + 2 * pos * h->dim ) < 0 )
                pos = 2 * pos;
            else
                pos = 2 * pos + 1;
        }
        else
            pos = 2 * pos;
        if ( heap_compare ( h, h->vec + ( pos / 2 - 1 ) * h->dim, h->vec + ( pos - 1 ) * h->dim ) <= 0 ) break;
        universal_swap ( h->vec + ( pos / 2 - 1 ) * h->dim, h->vec + ( pos - 1 ) * h->dim, h->dim );
    }
    return SUCCESSFUL_DELETION;
}

/* returns a reference to the top element of the heap, or NULL if the heap is empty */
void *heap_top ( heap *h ) {
    if ( heap_empty ( h ) ) return NULL;
    void *ptr = malloc ( h->dim );
    memcpy ( ptr, h->vec, h->dim );
    return ptr;
}

/* swaps the two heaps, return UNMATCHING DATA if the given heaps contains different data types, 
or SUCCESSFUL REPLACEMENT upon a successful swap */
void heap_swap ( heap *h1, heap *h2 ) {
    universal_swap ( h1, h2, HEAP_SIZE );
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
}