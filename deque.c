#include "deque.h"

int check_size ( deque *dq ) {
    if ( dq->size == dq->cap ) {
        dq->vec = realloc ( dq->vec, ( dq->cap + INIT_CAPACITY ) * dq->dim );
        if ( !dq->vec ) {
            free ( dq );
            return MEMORY_PROBLEM;
        }
        dq->cap += INIT_CAPACITY;
    }
    return 1;
}

/* initializes the deque structure with the elements having dim size */
deque *deque_initiate ( size_t dim ) {
    deque *dq = malloc ( DEQUE_SIZE );
    if ( !dq ) return NULL;
    dq->size = 0;
    dq->cap = INIT_CAPACITY;
    dq->dim = dim;
    dq->vec = malloc ( dq->dim * dq->cap );
    return dq;
}

/* pushes the element at the back of the deque */
enum return_codes deque_push_back ( deque *dq, void *el ) {
    int rc = check_size ( dq );
    if ( rc == MEMORY_PROBLEM ) return MEMORY_PROBLEM;
    memcpy ( dq->vec + dq->size * dq->dim, el, dq->dim );
    dq->size++;
    return SUCCESSFUL_ADDITION;
}

/* pushes the element at the start of the deque */
enum return_codes deque_push_front ( deque *dq, void *el ) {
    int rc = check_size ( dq );
    if ( rc == MEMORY_PROBLEM ) return MEMORY_PROBLEM;
    memcpy ( dq->vec + dq->dim, dq->vec, dq->size * dq->dim );
    memcpy ( dq->vec, el, dq->dim );
    dq->size++;
    return SUCCESSFUL_ADDITION;
}

/* pops the element at the front of the deque */
enum return_codes deque_pop_front ( deque *dq ) {
    if ( dq->size == 0 ) return EMPTY;
    dq->vec += dq->dim;
    dq->size--;
    dq->cap--;
    return SUCCESSFUL_DELETION;
}

/* pops the element at the back of the deque */
enum return_codes deque_pop_back ( deque *dq ) {
    if ( dq->size == 0 ) return EMPTY;
    dq->size--;
}

/* returns a reference to the first element of the deque, or NULL if the deque is empty */
void *deque_front ( deque *dq ) {
    if ( dq->size == 0 ) return NULL;
    return dq->vec;
}

/* returns a reference to the back element of the deque, or NULL if the deque is empty */
void *deque_back ( deque *dq ) {
    if ( dq->size == 0 ) return NULL;
    return dq->vec + ( dq->size - 1 ) * dq->dim;
}

/* frees the memory that the structure uses */
void deque_free ( deque *dq ) {
    free ( dq->vec );
    free ( dq );
}

/* prints the deque using the printer function for the individual elements */
void deque_print ( deque *dq, printer print ) {
    for ( size_t i = 0; i < dq->size; i++ )
        print ( dq->vec + i * dq->dim );
    printf ( "\n" );
}