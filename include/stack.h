#ifndef STACK_H
#define STACK_H

#include "universal.h"

typedef univ_attr_t stack_attr_t;

typedef struct stack {
    void*        vec;   /*!< array of elements of the stack */
    int          size;  /*!< current size of the stack */
    int          cap;   /*!< current maximum capacity of the stack */
    stack_attr_t attr;  /*!< attributes for the elements inside the stack */
} stack;

/*!
 * Initializes the given stack with the offered attributes
 * @param[out] s        Stack that will be initialized
 * @param[in]  el_attr  Attributes for the elements inside the stack
 * @return CS_SIZE if the size for the elements is negative or too big otherwise CS_SUCCESS upon
 * a successful initialization
 */
cs_codes stack_init ( stack *s, stack_attr_t el_attr );

/*!
 * Gives a pointer to the top element of the stack
 * @param[in] s The stack whose element will be accessed
 * @return Pointer to the top element
 */
void* stack_top ( stack s );

/*!
 * Pushes the element at the top of the stack
 * @param[out] s  The stack in which the element will be added
 * @param[in]  el The element which will be added
 * @return CS_MEM if a memory problem occured or CS_SUCCESS for a successful addition
 */
cs_codes stack_push ( stack *s, void *el );

/*!
 * Pops the element at the top of the stack if there is any
 * @param[in] s The stack whose last element will be popped
 * @return CS_EMPTY if the stack is empty otherwise CS_SUCCESS for a successful deletion
 */
cs_codes stack_pop ( stack *s );

/*!
 * Sets the new free function for the elements of the stack
 * @param[out] s   The stack whose free function will be changed
 * @param[in]  fr  The new free function for the elements of the stack
 */
void stack_set_free ( stack *s, freer fr );

/*!
 * Sets the new print function for the elements of the stack
 * @param[out] s      The stack whose print function will be changed
 * @param[in]  print  The new print function for the elements of the stack
 */
void stack_set_print ( stack *s, printer print );

/*!
 * Swaps the two given stacks
 * @param[in] s1,s2 The stacks that will be swapped
 */
void stack_swap ( stack *s1, stack *s2 );

/*!
 * Cleans the stack for further use
 * @param[in] s The stack that will be cleaned
 */
void stack_clear ( stack *s );

/*!
 * Stack whose memory will be freed
 * @param[in] s The stack whose memory will be freed
 */
void stack_free ( stack *s );

/*!
 * Prints the elements of the stack
 * @param[in] s The stack whose items will be printed 
 */
void stack_print ( stack s );

#endif