#include <cs/priority_queue.h>

#include <unittest.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_priority_queue_init_vector() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize vector priority queue",
            .return_code = rc
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_init_deque() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque priority queue",
            .return_code = rc
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_init_default() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_DEFAULT);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize default priority queue",
            .return_code = rc
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_init_null_pointer() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    cs_codes rc = priority_queue_init(NULL, attr, CS_PRIORITY_QUEUE_VECTOR);

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

test_res test_priority_queue_init_invalid_size() {
    priority_queue_attr_t attr = {
        .size = 0,
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);

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

test_res test_priority_queue_push_basic() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    rc = priority_queue_push(&pq, &val);
    if (rc != CS_SUCCESS) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to push element",
            .return_code = rc
        };
    }

    if (priority_queue_size(pq) != 1) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 1 after push",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_push_multiple() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        rc = priority_queue_push(&pq, &i);
        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push element",
                .return_code = rc
            };
        }
    }

    if (priority_queue_size(pq) != 100) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_push_null_pq() {
    int val = 42;
    cs_codes rc = priority_queue_push(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL priority queue",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_push_null_data() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = priority_queue_push(&pq, NULL);
    priority_queue_free(&pq);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL data",
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
/*                              POP TESTS                                     */
/******************************************************************************/

test_res test_priority_queue_pop_basic() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    priority_queue_push(&pq, &val);

    rc = priority_queue_pop(&pq);
    if (rc != CS_SUCCESS) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to pop element",
            .return_code = rc
        };
    }

    if (priority_queue_size(pq) != 0) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 after pop",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_pop_multiple() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        priority_queue_push(&pq, &i);
    }

    for (int i = 0; i < 25; i++) {
        rc = priority_queue_pop(&pq);
        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to pop element",
                .return_code = rc
            };
        }
    }

    if (priority_queue_size(pq) != 25) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 25 after popping half",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_pop_empty() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = priority_queue_pop(&pq);
    priority_queue_free(&pq);

    if (rc != CS_EMPTY) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_EMPTY for pop on empty queue",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_pop_null_pq() {
    cs_codes rc = priority_queue_pop(NULL);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL priority queue",
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

test_res test_priority_queue_top_basic() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    priority_queue_push(&pq, &val);

    void *top = priority_queue_top(pq);
    if (top == NULL) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    if (*(int *)top != 42) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top value should be 42",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_top_max_heap() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push in random order
    int vals[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq, &vals[i]);
    }

    // Top should be maximum (9)
    void *top = priority_queue_top(pq);
    if (top == NULL || *(int *)top != 9) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be maximum value (9)",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_top_min_heap() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_min,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push in random order
    int vals[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq, &vals[i]);
    }

    // Top should be minimum (0)
    void *top = priority_queue_top(pq);
    if (top == NULL || *(int *)top != 0) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be minimum value (0)",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_top_empty() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    void *top = priority_queue_top(pq);
    priority_queue_free(&pq);

    if (top != NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should return NULL for empty queue",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_top_does_not_pop() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    priority_queue_push(&pq, &val);

    // Call top multiple times
    priority_queue_top(pq);
    priority_queue_top(pq);
    priority_queue_top(pq);

    if (priority_queue_size(pq) != 1) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should not modify size",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          HEAP ORDER TESTS                                  */
/******************************************************************************/

test_res test_priority_queue_max_heap_order() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push 0-99 in order
    for (int i = 0; i < 100; i++) {
        priority_queue_push(&pq, &i);
    }

    // Pop and verify descending order (99, 98, 97, ...)
    for (int expected = 99; expected >= 0; expected--) {
        void *top = priority_queue_top(pq);
        if (top == NULL || *(int *)top != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Max heap order violated",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_min_heap_order() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_min,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push 99-0 in reverse order
    for (int i = 99; i >= 0; i--) {
        priority_queue_push(&pq, &i);
    }

    // Pop and verify ascending order (0, 1, 2, ...)
    for (int expected = 0; expected < 100; expected++) {
        void *top = priority_queue_top(pq);
        if (top == NULL || *(int *)top != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Min heap order violated",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_random_order_max() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push in pseudo-random order
    int vals[] = {37, 12, 89, 3, 56, 78, 24, 91, 45, 67, 8, 99, 15, 42, 63};
    for (int i = 0; i < 15; i++) {
        priority_queue_push(&pq, &vals[i]);
    }

    // Pop and verify descending order
    int prev = 100;
    while (!priority_queue_empty(pq)) {
        void *top = priority_queue_top(pq);
        int curr = *(int *)top;
        if (curr > prev) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Max heap order violated with random input",
                .return_code = CS_UNKNOWN
            };
        }
        prev = curr;
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          SIZE/EMPTY TESTS                                  */
/******************************************************************************/

test_res test_priority_queue_size_empty() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (priority_queue_size(pq) != 0) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 for empty queue",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_size_after_push() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 1; i <= 10; i++) {
        priority_queue_push(&pq, &i);
        if (priority_queue_size(pq) != i) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Size mismatch after push",
                .return_code = CS_UNKNOWN
            };
        }
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_size_after_pop() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq, &i);
    }

    for (int i = 9; i >= 0; i--) {
        priority_queue_pop(&pq);
        if (priority_queue_size(pq) != i) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Size mismatch after pop",
                .return_code = CS_UNKNOWN
            };
        }
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_empty_true() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (!priority_queue_empty(pq)) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty should return true for new queue",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_empty_false() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    priority_queue_push(&pq, &val);

    if (priority_queue_empty(pq)) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty should return false for non-empty queue",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_empty_after_pop_all() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq, &i);
    }

    for (int i = 0; i < 10; i++) {
        priority_queue_pop(&pq);
    }

    if (!priority_queue_empty(pq)) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty should return true after popping all",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_priority_queue_clear_basic() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        priority_queue_push(&pq, &i);
    }

    priority_queue_clear(&pq);

    if (!priority_queue_empty(pq)) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    if (priority_queue_size(pq) != 0) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 after clear",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_clear_empty() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Clear an already empty queue - should not crash
    priority_queue_clear(&pq);

    if (!priority_queue_empty(pq)) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should still be empty after clearing empty queue",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_clear_null() {
    // Clear on NULL should not crash
    priority_queue_clear(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SWAP TESTS                                    */
/******************************************************************************/

test_res test_priority_queue_swap_basic() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq1, pq2;
    cs_codes rc1 = priority_queue_init(&pq1, attr, CS_PRIORITY_QUEUE_VECTOR);
    cs_codes rc2 = priority_queue_init(&pq2, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc1 != CS_SUCCESS || rc2 != CS_SUCCESS) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize priority queues",
            .return_code = CS_UNKNOWN
        };
    }

    // Push 10 elements to pq1
    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq1, &i);
    }

    // Push 5 elements to pq2
    for (int i = 100; i < 105; i++) {
        priority_queue_push(&pq2, &i);
    }

    priority_queue_swap(&pq1, &pq2);

    // pq1 should now have 5 elements with max 104
    if (priority_queue_size(pq1) != 5) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq1 should have 5 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    void *top1 = priority_queue_top(pq1);
    if (top1 == NULL || *(int *)top1 != 104) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq1 top should be 104 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    // pq2 should now have 10 elements with max 9
    if (priority_queue_size(pq2) != 10) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    void *top2 = priority_queue_top(pq2);
    if (top2 == NULL || *(int *)top2 != 9) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq2 top should be 9 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq1);
    priority_queue_free(&pq2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_swap_with_empty() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq1, pq2;
    cs_codes rc1 = priority_queue_init(&pq1, attr, CS_PRIORITY_QUEUE_VECTOR);
    cs_codes rc2 = priority_queue_init(&pq2, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc1 != CS_SUCCESS || rc2 != CS_SUCCESS) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize priority queues",
            .return_code = CS_UNKNOWN
        };
    }

    // Push to pq1 only
    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq1, &i);
    }

    priority_queue_swap(&pq1, &pq2);

    if (!priority_queue_empty(pq1)) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq1 should be empty after swap with empty queue",
            .return_code = CS_UNKNOWN
        };
    }

    if (priority_queue_size(pq2) != 10) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq1);
    priority_queue_free(&pq2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_priority_queue_free_null() {
    // Free on NULL should not crash
    priority_queue_free(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_free_empty() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_free_with_elements() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        priority_queue_push(&pq, &i);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          CONTAINER TYPE TESTS                              */
/******************************************************************************/

test_res test_priority_queue_deque_operations() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque priority queue",
            .return_code = rc
        };
    }

    // Push in random order
    int vals[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq, &vals[i]);
    }

    // Pop and verify max heap order
    for (int expected = 9; expected >= 0; expected--) {
        void *top = priority_queue_top(pq);
        if (top == NULL || *(int *)top != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Deque container: max heap order violated",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          STRESS/EDGE TESTS                                 */
/******************************************************************************/

test_res test_priority_queue_large_capacity() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10000; i++) {
        rc = priority_queue_push(&pq, &i);
        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push during large capacity test",
                .return_code = rc
            };
        }
    }

    if (priority_queue_size(pq) != 10000) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 10000",
            .return_code = CS_UNKNOWN
        };
    }

    // Verify max is 9999
    void *top = priority_queue_top(pq);
    if (top == NULL || *(int *)top != 9999) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 9999",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_push_pop_cycle() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push 100, pop 50, push 100, pop all
    for (int i = 0; i < 100; i++) {
        priority_queue_push(&pq, &i);
    }

    for (int i = 0; i < 50; i++) {
        priority_queue_pop(&pq);
    }

    // Top should now be 49
    void *top = priority_queue_top(pq);
    if (top == NULL || *(int *)top != 49) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 49 after popping 50 from 0-99",
            .return_code = CS_UNKNOWN
        };
    }

    for (int i = 100; i < 200; i++) {
        priority_queue_push(&pq, &i);
    }

    // Now top should be 199
    top = priority_queue_top(pq);
    if (top == NULL || *(int *)top != 199) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 199 after pushing 100-199",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_negative_values() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push negative values
    for (int i = -50; i <= 50; i++) {
        priority_queue_push(&pq, &i);
    }

    // Max should be 50
    void *top = priority_queue_top(pq);
    if (top == NULL || *(int *)top != 50) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 50 (max of -50 to 50)",
            .return_code = CS_UNKNOWN
        };
    }

    // Pop and verify order
    for (int expected = 50; expected >= -50; expected--) {
        top = priority_queue_top(pq);
        if (top == NULL || *(int *)top != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Max heap order violated with negative values",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_clear_and_reuse() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        priority_queue_push(&pq, &i);
    }

    priority_queue_clear(&pq);

    // Push new values
    for (int i = 100; i < 150; i++) {
        rc = priority_queue_push(&pq, &i);
        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push after clear",
                .return_code = rc
            };
        }
    }

    if (priority_queue_size(pq) != 50) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 50 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    // Top should be 149
    void *top = priority_queue_top(pq);
    if (top == NULL || *(int *)top != 149) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should be 149 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_duplicate_values() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push duplicate values
    int val = 42;
    for (int i = 0; i < 10; i++) {
        priority_queue_push(&pq, &val);
    }

    if (priority_queue_size(pq) != 10) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 10 with duplicates",
            .return_code = CS_UNKNOWN
        };
    }

    // All tops should be 42
    for (int i = 0; i < 10; i++) {
        void *top = priority_queue_top(pq);
        if (top == NULL || *(int *)top != 42) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "All tops should be 42",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_interleaved_push_pop() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = print_int,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Interleaved push and pop
    for (int i = 0; i < 100; i++) {
        // Push 3 elements
        int a = i * 3, b = i * 3 + 1, c = i * 3 + 2;
        priority_queue_push(&pq, &a);
        priority_queue_push(&pq, &b);
        priority_queue_push(&pq, &c);

        // Pop 1 element (should be the max so far)
        priority_queue_pop(&pq);
    }

    // Should have 200 elements remaining
    if (priority_queue_size(pq) != 200) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after interleaved operations",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          TEST STRUCT TESTS                                 */
/******************************************************************************/

test_res test_priority_queue_struct_basic() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    test_struct ts = create_test_struct(1, "TestPQ", 10.5);
    rc = priority_queue_push(&pq, &ts);
    free_test_struct(&ts);

    if (rc != CS_SUCCESS) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to push struct",
            .return_code = rc
        };
    }

    if (priority_queue_size(pq) != 1) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 1",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_multiple() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "MultiTest", (double)i * 1.5);
        rc = priority_queue_push(&pq, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push struct",
                .return_code = rc
            };
        }
    }

    if (priority_queue_size(pq) != 100) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_max_heap_order() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push structs with increasing IDs in random order
    int ids[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(ids[i], "MaxHeap", (double)ids[i]);
        priority_queue_push(&pq, &ts);
        free_test_struct(&ts);
    }

    // Pop and verify descending order by ID (9, 8, 7, ...)
    for (int expected = 9; expected >= 0; expected--) {
        test_struct *top = (test_struct *)priority_queue_top(pq);
        if (top == NULL || top->id != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Max heap order violated with struct",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_min_heap_order() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_min,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push structs with increasing IDs in random order
    int ids[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(ids[i], "MinHeap", (double)ids[i]);
        priority_queue_push(&pq, &ts);
        free_test_struct(&ts);
    }

    // Pop and verify ascending order by ID (0, 1, 2, ...)
    for (int expected = 0; expected < 10; expected++) {
        test_struct *top = (test_struct *)priority_queue_top(pq);
        if (top == NULL || top->id != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Min heap order violated with struct",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_simple() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
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
        rc = priority_queue_push(&pq, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push simple struct",
                .return_code = rc
            };
        }
    }

    // Verify max heap order (99, 98, 97, ...)
    for (int expected = 99; expected >= 0; expected--) {
        test_struct *top = (test_struct *)priority_queue_top(pq);
        if (top == NULL || top->id != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Max heap order violated with simple struct",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_deque_container() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque priority queue",
            .return_code = rc
        };
    }

    // Push complex structs
    int ids[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(ids[i], "DequeTest", (double)ids[i] * 2.5);
        rc = priority_queue_push(&pq, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push struct to deque priority queue",
                .return_code = rc
            };
        }
    }

    // Verify max heap order
    for (int expected = 9; expected >= 0; expected--) {
        test_struct *top = (test_struct *)priority_queue_top(pq);
        if (top == NULL || top->id != expected) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Max heap order violated in deque priority queue",
                .return_code = CS_UNKNOWN
            };
        }
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_clear() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
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
        priority_queue_push(&pq, &ts);
        free_test_struct(&ts);
    }

    // Clear should free all struct memory
    priority_queue_clear(&pq);

    if (!priority_queue_empty(pq)) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Priority queue should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    // Push again to verify clear worked
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        rc = priority_queue_push(&pq, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push after clear",
                .return_code = rc
            };
        }
    }

    // Verify top is id=149
    test_struct *top = (test_struct *)priority_queue_top(pq);
    if (top == NULL || top->id != 149) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top should have id=149 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_swap() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq1, pq2;
    cs_codes rc1 = priority_queue_init(&pq1, attr, CS_PRIORITY_QUEUE_VECTOR);
    cs_codes rc2 = priority_queue_init(&pq2, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc1 != CS_SUCCESS || rc2 != CS_SUCCESS) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize priority queues",
            .return_code = CS_UNKNOWN
        };
    }

    // Push to pq1
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "PQ1", (double)i);
        priority_queue_push(&pq1, &ts);
        free_test_struct(&ts);
    }

    // Push to pq2
    for (int i = 100; i < 105; i++) {
        test_struct ts = create_test_struct(i, "PQ2", (double)i);
        priority_queue_push(&pq2, &ts);
        free_test_struct(&ts);
    }

    priority_queue_swap(&pq1, &pq2);

    // pq1 should now have 5 elements with top id=104
    if (priority_queue_size(pq1) != 5) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq1 should have 5 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    test_struct *top1 = (test_struct *)priority_queue_top(pq1);
    if (top1 == NULL || top1->id != 104) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq1 top should have id=104 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    // pq2 should now have 10 elements with top id=9
    if (priority_queue_size(pq2) != 10) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    test_struct *top2 = (test_struct *)priority_queue_top(pq2);
    if (top2 == NULL || top2->id != 9) {
        priority_queue_free(&pq1);
        priority_queue_free(&pq2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "pq2 top should have id=9 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq1);
    priority_queue_free(&pq2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_deep_copy_verify() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
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

    priority_queue_push(&pq, &ts);

    // The priority queue should have deep copied the struct
    test_struct *queued = (test_struct *)priority_queue_top(pq);
    if (queued == NULL) {
        free_test_struct(&ts);
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Top returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    // The name pointers should be different (deep copy)
    if (queued->name == original_name) {
        free_test_struct(&ts);
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Name was not deep copied",
            .return_code = CS_UNKNOWN
        };
    }

    // But the values should be equal
    if (queued->id != 42 || strcmp(queued->name, "DeepCopyTest") != 0) {
        free_test_struct(&ts);
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Deep copied values don't match",
            .return_code = CS_UNKNOWN
        };
    }

    free_test_struct(&ts);
    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_interleaved() {
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Interleaved push and pop
    for (int i = 0; i < 50; i++) {
        // Push 2 structs
        test_struct ts1 = create_test_struct(i * 2, "Interleaved", 1.0);
        test_struct ts2 = create_test_struct(i * 2 + 1, "Interleaved", 2.0);
        priority_queue_push(&pq, &ts1);
        priority_queue_push(&pq, &ts2);
        free_test_struct(&ts1);
        free_test_struct(&ts2);

        // Pop 1 struct (should be the max)
        priority_queue_pop(&pq);
    }

    // Should have 50 elements remaining
    if (priority_queue_size(pq) != 50) {
        priority_queue_free(&pq);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after interleaved operations",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_priority_queue_struct_by_score() {
    // Test with score-based comparison
    priority_queue_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_by_score,
        .print = print_test_struct_compact,
        .fr = free_test_struct,
        .copy = copy_test_struct
    };

    priority_queue pq;
    cs_codes rc = priority_queue_init(&pq, attr, CS_PRIORITY_QUEUE_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push structs with different scores
    double scores[] = {3.5, 1.2, 9.8, 4.6, 7.3, 2.1, 8.9, 5.4, 6.7, 0.5};
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "ScoreTest", scores[i]);
        priority_queue_push(&pq, &ts);
        free_test_struct(&ts);
    }

    // Pop and verify descending score order
    double prev_score = 100.0;
    while (!priority_queue_empty(pq)) {
        test_struct *top = (test_struct *)priority_queue_top(pq);
        if (top->score > prev_score) {
            priority_queue_free(&pq);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Score-based heap order violated",
                .return_code = CS_UNKNOWN
            };
        }
        prev_score = top->score;
        priority_queue_pop(&pq);
    }

    priority_queue_free(&pq);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test priority_queue_tests[] = {
    // Init tests
    test_priority_queue_init_vector,
    test_priority_queue_init_deque,
    test_priority_queue_init_default,
    test_priority_queue_init_null_pointer,
    test_priority_queue_init_invalid_size,

    // Push tests
    test_priority_queue_push_basic,
    test_priority_queue_push_multiple,
    test_priority_queue_push_null_pq,
    test_priority_queue_push_null_data,

    // Pop tests
    test_priority_queue_pop_basic,
    test_priority_queue_pop_multiple,
    test_priority_queue_pop_empty,
    test_priority_queue_pop_null_pq,

    // Top tests
    test_priority_queue_top_basic,
    test_priority_queue_top_max_heap,
    test_priority_queue_top_min_heap,
    test_priority_queue_top_empty,
    test_priority_queue_top_does_not_pop,

    // Heap order tests
    test_priority_queue_max_heap_order,
    test_priority_queue_min_heap_order,
    test_priority_queue_random_order_max,

    // Size/Empty tests
    test_priority_queue_size_empty,
    test_priority_queue_size_after_push,
    test_priority_queue_size_after_pop,
    test_priority_queue_empty_true,
    test_priority_queue_empty_false,
    test_priority_queue_empty_after_pop_all,

    // Clear tests
    test_priority_queue_clear_basic,
    test_priority_queue_clear_empty,
    test_priority_queue_clear_null,

    // Swap tests
    test_priority_queue_swap_basic,
    test_priority_queue_swap_with_empty,

    // Free tests
    test_priority_queue_free_null,
    test_priority_queue_free_empty,
    test_priority_queue_free_with_elements,

    // Container type tests
    test_priority_queue_deque_operations,

    // Stress/Edge tests
    test_priority_queue_large_capacity,
    test_priority_queue_push_pop_cycle,
    test_priority_queue_negative_values,
    test_priority_queue_clear_and_reuse,
    test_priority_queue_duplicate_values,
    test_priority_queue_interleaved_push_pop,

    // Struct tests
    test_priority_queue_struct_basic,
    test_priority_queue_struct_multiple,
    test_priority_queue_struct_max_heap_order,
    test_priority_queue_struct_min_heap_order,
    test_priority_queue_struct_simple,
    test_priority_queue_struct_deque_container,
    test_priority_queue_struct_clear,
    test_priority_queue_struct_swap,
    test_priority_queue_struct_deep_copy_verify,
    test_priority_queue_struct_interleaved,
    test_priority_queue_struct_by_score,
};