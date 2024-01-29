#ifndef STACK_H
#define STACK_H

#include "../universal/universal.h"

typedef struct stack {
    void* vec;
    size_t cap, dim, size;
} stack;

#define STACK_SIZE sizeof ( stack )

#define stack_empty(s) s->size == 0

stack *stack_initiate ( size_t dim );
void *stack_top ( stack *s );
enum return_codes stack_push ( stack *s, void *el );
enum return_codes stack_pop ( stack *s );
void stack_swap ( stack *s1, stack *s2 );
void stack_free ( stack *s );
void stack_print ( stack *s, printer print );

#endif