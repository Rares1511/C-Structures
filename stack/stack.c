#include "stack.h"

/* initializes the stack knowing the dimension of data and returns a pointer */
stack *stack_initiate ( size_t dim ) {
    stack *s = malloc ( STACK_SIZE );
    s->cap = INIT_CAPACITY;
    s->size = 0;
    s->dim = dim;
    s->vec = malloc ( s->dim * s->cap );
    return s;
}

/* returns 1 if the stack is empty or 0 otherwise */
int stack_empty ( stack *s ) {
    return s->size == 0;
}

/* returns a reference to the top of the stack or NULL if empty */
void *stack_top ( stack *s ) {
    if ( stack_empty ( s ) ) return NULL;
    return s->vec + ( s->size - 1 ) * s->dim;
}

/* pushes the element in the given stack */
enum return_codes stack_push ( stack *s, void *el ) {
    if ( s->size == s->cap ) {
        s->vec = realloc ( s->vec, ( s->cap + INIT_CAPACITY ) * s->dim );
        if ( !s->vec ) return MEMORY_PROBLEM;
        s->cap += INIT_CAPACITY;
    }
    memcpy ( s->vec + s->size * s->dim, el, s->dim );
    s->size++;
    return SUCCESSFUL_ADDITION;
}

/* pops the last element of the stack or returns EMPTY if the stack was empty */
enum return_codes stack_pop ( stack *s ) {
    if ( stack_empty ( s ) ) return EMPTY;
    s->size--;
    return SUCCESSFUL_DELETION;
}

/* swaps the two stacks given */
void stack_swap ( stack *s1, stack *s2 ) {
    universal_swap ( s1, s2, STACK_SIZE );
}

/* frees the memory that the stack uses */
void stack_free ( stack *s ) {
    free ( s->vec );
    free ( s );
}

/* prints the elements of the stack using the printer function given */
void stack_print ( stack *s, printer print ) {
    for ( int i = s->size - 1; i >= 0; i-- )
        print ( s->vec + i * s->dim );
    printf ( "\n" );
}