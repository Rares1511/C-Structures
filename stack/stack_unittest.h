#include <cs/stack.h>

#include <unittest.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_stack_init_array() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize array stack",
            .return_code = rc
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_init_deque() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_DEQUE, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque stack",
            .return_code = rc
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_init_list() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_LIST, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize list stack",
            .return_code = rc
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_init_default() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_DEFAULT, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize default stack",
            .return_code = rc
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_init_null_pointer() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    cs_codes rc = stack_init(NULL, CS_STACK_ARRAY, attr);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL pointer",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_init_invalid_size() {
    elem_attr_t attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_SIZE for zero element size",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              PUSH TESTS                                    */
/******************************************************************************/

test_res test_stack_push_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    rc = stack_push(&s, &val);

    if (rc != CS_SUCCESS) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to push element",
            .return_code = rc
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_push_multiple() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        rc = stack_push(&s, &i);
        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push element",
                .return_code = rc
            };
        }
    }

    if (stack_size(&s) != 100) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after multiple pushes",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_push_null_stack() {
    int val = 42;
    cs_codes rc = stack_push(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL stack",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_push_null_data() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = stack_push(&s, NULL);

    if (rc != CS_NULL) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL data",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              POP TESTS                                     */
/******************************************************************************/

test_res test_stack_pop_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    stack_push(&s, &val);

    rc = stack_pop(&s);

    if (rc != CS_SUCCESS) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to pop element",
            .return_code = rc
        };
    }

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Stack should be empty after pop",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_pop_multiple() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        stack_push(&s, &i);
    }

    for (int i = 0; i < 25; i++) {
        rc = stack_pop(&s);
        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to pop element",
                .return_code = rc
            };
        }
    }

    if (stack_size(&s) != 25) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 25 after popping 25 elements",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_pop_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = stack_pop(&s);

    // Pop on empty should fail
    if (rc == CS_SUCCESS) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Pop on empty stack should fail",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_pop_null_stack() {
    cs_codes rc = stack_pop(NULL);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL stack",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TOP TESTS                                     */
/******************************************************************************/

test_res test_stack_top_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    stack_push(&s, &val);

    void *top = stack_top(&s);

    if (top == NULL) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    if (*(int *)top != 42) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_top_lifo_order() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push 0, 1, 2, 3, 4
    for (int i = 0; i < 5; i++) {
        stack_push(&s, &i);
    }

    // Top should be 4 (last pushed)
    void *top = stack_top(&s);
    if (top == NULL || *(int *)top != 4) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 4 (LIFO order)",
            .return_code = CS_UNKNOWN
        };
    }

    // Pop and check order
    for (int i = 4; i >= 0; i--) {
        top = stack_top(&s);
        if (top == NULL || *(int *)top != i) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "LIFO order violation",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_top_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    void *top = stack_top(&s);

    if (top != NULL) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top on empty stack should return NULL",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_top_null_stack() {
    void *top = stack_top(NULL);

    if (top != NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top on NULL stack should return NULL",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_top_does_not_pop() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    stack_push(&s, &val);

    // Call top multiple times
    stack_top(&s);
    stack_top(&s);
    stack_top(&s);

    // Size should still be 1
    if (stack_size(&s) != 1) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should not remove elements",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_stack_size_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (stack_size(&s) != 0) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 for empty stack",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_size_after_push() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 25; i++) {
        stack_push(&s, &i);
    }

    if (stack_size(&s) != 25) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after pushes",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_size_after_pop() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        stack_push(&s, &i);
    }

    for (int i = 0; i < 5; i++) {
        stack_pop(&s);
    }

    if (stack_size(&s) != 5) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 5 after popping 5 elements",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_size_null_stack() {
    int size = stack_size(NULL);

    if (size != -1) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size of NULL stack should return -1",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_empty_true() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Newly created stack should be empty",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_empty_false() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    stack_push(&s, &val);

    if (stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Stack with element should not be empty",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_empty_after_pop_all() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        stack_push(&s, &i);
    }

    for (int i = 0; i < 10; i++) {
        stack_pop(&s);
    }

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Stack should be empty after popping all elements",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_empty_null_stack() {
    int empty = stack_empty(NULL);

    if (empty != -1) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty on NULL stack should return -1",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_stack_clear_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        stack_push(&s, &i);
    }

    stack_clear(&s);

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Stack should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    if (stack_size(&s) != 0) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 after clear",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_clear_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    stack_clear(&s);

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty stack should remain empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_clear_null() {
    // Clear NULL should not crash
    stack_clear(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_stack_free_null() {
    // Free NULL should not crash
    stack_free(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_free_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_free_with_elements() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        stack_push(&s, &i);
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          CONTAINER TYPE TESTS                              */
/******************************************************************************/

test_res test_stack_deque_operations() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_DEQUE, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque stack",
            .return_code = rc
        };
    }

    // Push elements
    for (int i = 0; i < 50; i++) {
        stack_push(&s, &i);
    }

    // Verify LIFO order
    for (int i = 49; i >= 0; i--) {
        void *top = stack_top(&s);
        if (top == NULL || *(int *)top != i) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Deque stack LIFO order violation",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_list_operations() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_LIST, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize list stack",
            .return_code = rc
        };
    }

    // Push elements
    for (int i = 0; i < 50; i++) {
        stack_push(&s, &i);
    }

    // Verify LIFO order
    for (int i = 49; i >= 0; i--) {
        void *top = stack_top(&s);
        if (top == NULL || *(int *)top != i) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "List stack LIFO order violation",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_stack_large_capacity() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 5000; i++) {
        rc = stack_push(&s, &i);
        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push during stress test",
                .return_code = rc
            };
        }
    }

    if (stack_size(&s) != 5000) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch in stress test",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_push_pop_cycle() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Multiple cycles of push and pop
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            stack_push(&s, &i);
        }
        for (int i = 0; i < 20; i++) {
            stack_pop(&s);
        }
    }

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Stack should be empty after cycles",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_negative_values() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = -50; i < 50; i++) {
        rc = stack_push(&s, &i);
        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push negative value",
                .return_code = rc
            };
        }
    }

    // Top should be 49 (last pushed)
    void *top = stack_top(&s);
    if (top == NULL || *(int *)top != 49) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 49",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_clear_and_reuse() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push, clear, and push again
    for (int i = 0; i < 50; i++) {
        stack_push(&s, &i);
    }

    stack_clear(&s);

    for (int i = 100; i < 150; i++) {
        rc = stack_push(&s, &i);
        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push after clear",
                .return_code = rc
            };
        }
    }

    if (stack_size(&s) != 50) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 50 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    // Top should be 149
    void *top = stack_top(&s);
    if (top == NULL || *(int *)top != 149) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 149 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          TEST STRUCT TESTS                                 */
/******************************************************************************/

test_res test_stack_struct_basic() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    test_struct ts = create_test_struct(1, "TestStack", 10.5);
    rc = stack_push(&s, &ts);
    free_test_struct(&ts);

    if (rc != CS_SUCCESS) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to push struct",
            .return_code = rc
        };
    }

    if (stack_size(&s) != 1) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 1",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_multiple() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "MultiTest", (double)i * 1.5);
        rc = stack_push(&s, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push struct",
                .return_code = rc
            };
        }
    }

    if (stack_size(&s) != 100) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_top() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "TopTest", (double)i);
        stack_push(&s, &ts);
        free_test_struct(&ts);
    }

    // Top should be id=9 (last pushed)
    test_struct *top = (test_struct *)stack_top(&s);
    if (top == NULL || top->id != 9) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should have id=9",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_lifo_order() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push structs with increasing IDs
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "LIFOTest", (double)i);
        stack_push(&s, &ts);
        free_test_struct(&ts);
    }

    // Pop and verify LIFO order (should be 49, 48, 47, ...)
    for (int i = 49; i >= 0; i--) {
        test_struct *top = (test_struct *)stack_top(&s);
        if (top == NULL || top->id != i) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "LIFO order violated with struct",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
    }

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Stack should be empty after popping all",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_simple() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Test with simple structs (no nested allocations)
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct_simple(i);
        rc = stack_push(&s, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push simple struct",
                .return_code = rc
            };
        }
    }

    // Verify LIFO order with simple structs
    for (int i = 99; i >= 50; i--) {
        test_struct *top = (test_struct *)stack_top(&s);
        if (top == NULL || top->id != i) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "LIFO order violated with simple struct",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_deque_container() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_DEQUE, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque stack",
            .return_code = rc
        };
    }

    // Push complex structs
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "DequeTest", (double)i * 2.5);
        rc = stack_push(&s, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push struct to deque stack",
                .return_code = rc
            };
        }
    }

    // Verify LIFO order
    for (int i = 49; i >= 0; i--) {
        test_struct *top = (test_struct *)stack_top(&s);
        if (top == NULL || top->id != i) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "LIFO order violated in deque stack",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_list_container() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_LIST, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize list stack",
            .return_code = rc
        };
    }

    // Push complex structs
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ListTest", (double)i * 2.5);
        rc = stack_push(&s, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push struct to list stack",
                .return_code = rc
            };
        }
    }

    // Verify LIFO order
    for (int i = 49; i >= 0; i--) {
        test_struct *top = (test_struct *)stack_top(&s);
        if (top == NULL || top->id != i) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "LIFO order violated in list stack",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_clear() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push complex structs
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        stack_push(&s, &ts);
        free_test_struct(&ts);
    }

    // Clear should free all struct memory
    stack_clear(&s);

    if (!stack_empty(&s)) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Stack should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    // Push again to verify clear worked
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        rc = stack_push(&s, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push after clear",
                .return_code = rc
            };
        }
    }

    // Verify top is id=149
    test_struct *top = (test_struct *)stack_top(&s);
    if (top == NULL || top->id != 149) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should have id=149 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_deep_copy_verify() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Create a complex struct
    test_struct ts = create_test_struct(42, "DeepCopyTest", 99.99);
    char *original_name = ts.name;

    stack_push(&s, &ts);

    // The stack should have deep copied the struct
    test_struct *stacked = (test_struct *)stack_top(&s);
    if (stacked == NULL) {
        free_test_struct(&ts);
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    // The name pointers should be different (deep copy)
    if (stacked->name == original_name) {
        free_test_struct(&ts);
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Name was not deep copied",
            .return_code = CS_UNKNOWN
        };
    }

    // But the values should be equal
    if (stacked->id != 42 || strcmp(stacked->name, "DeepCopyTest") != 0) {
        free_test_struct(&ts);
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Deep copied values don't match",
            .return_code = CS_UNKNOWN
        };
    }

    free_test_struct(&ts);
    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_stack_struct_interleaved() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .comp = NULL,
        .print = print_test_struct_compact
    };

    stack s;
    cs_codes rc = stack_init(&s, CS_STACK_ARRAY, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int next_push = 0;
    int expected_top = -1;

    for (int i = 0; i < 50; i++) {
        // Push 2 structs
        test_struct ts1 = create_test_struct(next_push++, "Interleaved", 1.0);
        test_struct ts2 = create_test_struct(next_push++, "Interleaved", 2.0);
        stack_push(&s, &ts1);
        stack_push(&s, &ts2);
        expected_top = next_push - 1;
        free_test_struct(&ts1);
        free_test_struct(&ts2);

        // Pop 1 struct and verify it was the last pushed
        test_struct *top = (test_struct *)stack_top(&s);
        if (top == NULL || top->id != expected_top) {
            stack_free(&s);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "LIFO order violated in interleaved operations",
                .return_code = CS_UNKNOWN
            };
        }
        stack_pop(&s);
        expected_top--;
    }

    // Should have 50 elements remaining
    if (stack_size(&s) != 50) {
        stack_free(&s);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after interleaved operations",
            .return_code = CS_UNKNOWN
        };
    }

    stack_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test stack_tests[] = {
    // Init tests
    test_stack_init_array,
    test_stack_init_deque,
    test_stack_init_list,
    test_stack_init_default,
    test_stack_init_null_pointer,
    test_stack_init_invalid_size,

    // Push tests
    test_stack_push_basic,
    test_stack_push_multiple,
    test_stack_push_null_stack,
    test_stack_push_null_data,

    // Pop tests
    test_stack_pop_basic,
    test_stack_pop_multiple,
    test_stack_pop_empty,
    test_stack_pop_null_stack,

    // Top tests
    test_stack_top_basic,
    test_stack_top_lifo_order,
    test_stack_top_empty,
    test_stack_top_null_stack,
    test_stack_top_does_not_pop,

    // Size/Empty tests
    test_stack_size_empty,
    test_stack_size_after_push,
    test_stack_size_after_pop,
    test_stack_size_null_stack,
    test_stack_empty_true,
    test_stack_empty_false,
    test_stack_empty_after_pop_all,
    test_stack_empty_null_stack,

    // Clear tests
    test_stack_clear_basic,
    test_stack_clear_empty,
    test_stack_clear_null,

    // Free tests
    test_stack_free_null,
    test_stack_free_empty,
    test_stack_free_with_elements,

    // Container type tests
    test_stack_deque_operations,
    test_stack_list_operations,

    // Stress/Edge tests
    test_stack_large_capacity,
    test_stack_push_pop_cycle,
    test_stack_negative_values,
    test_stack_clear_and_reuse,

    // Struct tests
    test_stack_struct_basic,
    test_stack_struct_multiple,
    test_stack_struct_top,
    test_stack_struct_lifo_order,
    test_stack_struct_simple,
    test_stack_struct_deque_container,
    test_stack_struct_list_container,
    test_stack_struct_clear,
    test_stack_struct_deep_copy_verify,
    test_stack_struct_interleaved,
};