#ifndef _STACK_H_
#define _STACK_H_

#include <cs/universal.h>

typedef xuniv_attr_t stack_attr;

typedef enum stack_type {
    CS_STACK_ARRAY,
    CS_STACK_DEQUE,
    CS_STACK_LIST,
    CS_STACK_DEFAULT
} stack_type;

typedef struct stack {
    void *container;
    stack_type type;
} stack;

/*!
 * @brief Initializes a stack with the specified type and attributes.
 * @param[in] s Pointer to the stack to initialize.
 * @param[in] type The type of the stack (array, deque, list).
 * @param[in] attr Attributes for the underlying container.
 * @return CS_OK on success, error code otherwise.
 */
cs_codes stack_init(stack *s, stack_type type, stack_attr attr);

/*!
 * @brief Pushes an element onto the stack.
 * @param[in] s Pointer to the stack.
 * @param[in] data Pointer to the data to push onto the stack.
 * @return CS_OK on success, error code otherwise.
 */
cs_codes stack_push(stack *s, const void *data);

/*!
 * @brief Pops an element from the stack.
 * @param[in] s Pointer to the stack.
 * @return CS_OK on success, error code otherwise.
 */
cs_codes stack_pop(stack *s);

/*!
 * @brief Retrieves the top element of the stack without removing it.
 * @param[in] s Pointer to the stack.
 * @return Pointer to the top element, or NULL if the stack is empty or on error.
 */
void *stack_top(const stack *s);

/*!
 * @brief Returns the number of elements in the stack.
 * @param[in] s Pointer to the stack.
 * @return Number of elements in the stack, or -1 on error.
 */
int stack_size(const stack *s);

/*!
 * @brief Checks if the stack is empty.
 * @param[in] s Pointer to the stack.
 * @return 1 if the stack is empty, 0 if not, -1 on error.
 */
int stack_empty(const stack *s);

/*!
 * @brief Clears all elements from the stack.
 * @param[in] s Pointer to the stack.
 */
void stack_clear(stack *s);

/*!
 * @brief Frees the resources associated with the stack.
 * @param[in] v_s Pointer to the stack to free.
 */
void stack_free(void *v_s);

/*!
 * @brief Prints the contents of the stack to the specified stream.
 * @param[in] stream The output stream.
 * @param[in] v_s Pointer to the stack.
 */
void stack_print(FILE *stream, void *v_s);

#endif