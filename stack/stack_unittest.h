#include <cs/stack.h>

#include <unittest.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_stack_init_array() {
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
    stack_attr_t attr = {
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
};