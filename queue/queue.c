#include "queue.h"

/* initiates the queue knowing that the elements have a sizeof of dim   */
queue *queue_initiate ( size_t dim ) {
    queue *q = malloc ( QUEUE_SIZE );
    if ( !q ) return NULL;
    q->cap = INIT_CAPACITY;
    q->dim = dim;
    q->size = 0;
    q->vec = malloc ( q->dim * q->cap );
    return q;
}

/* returns a reference to the first element of the queue */
void *queue_front ( queue *q ) {
    if ( queue_empty ( q ) ) return EMPTY;
    void *ptr = malloc ( q->dim );
    memcpy ( ptr, q->vec, q->dim );
    return ptr;
}

/* returns a reference to the last element of the queue */
void *queue_back ( queue *q ) {
    if ( queue_empty ( q ) ) return EMPTY;
    void *ptr = malloc ( q->dim );
    memcpy ( ptr, q->vec + ( q->size - 1 ) * q->dim, q->dim );
    return ptr;
}

/* adds the element at the back of the queue */
enum return_codes queue_push ( queue *q, void *el ) {
    if ( q->size == q->cap ) {
        q->vec = realloc ( q->vec, ( q->cap + INIT_CAPACITY ) * q->dim );
        if ( !q->vec ) return MEMORY_PROBLEM;
        q->cap += INIT_CAPACITY;
    }
    memcpy ( q->vec + q->size * q->dim, el, q->dim );
    q->size++;
    return SUCCESSFUL_ADDITION;
}

/* deletes the first element of the queue */
enum return_codes queue_pop ( queue *q ) {
    if ( queue_empty ( q ) ) return EMPTY;
    q->vec += q->dim;
    q->cap--;
    q->size--;
    return SUCCESSFUL_DELETION;
}

/* swaps the two queues */
void queue_swap ( queue *q1, queue *q2 ) { 
    universal_swap ( q1, q2, QUEUE_SIZE );
}

/* frees the memory used for the queue */
void queue_free ( queue *q ) {
    free ( q->vec );
    free ( q );
}

/* prints the queue using the printing functions offered for the elements */
void queue_print ( queue *q, printer print ) {
    for ( size_t i = 0; i < q->size; i++ ) 
        print ( q->vec + i * q->dim );
}