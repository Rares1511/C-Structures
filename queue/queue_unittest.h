#include <cs/queue.h>

#include <unittest.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_queue_init_deque() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque queue",
            .return_code = rc
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_init_list() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_LIST);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize list queue",
            .return_code = rc
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_init_default() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEFAULT);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize default queue",
            .return_code = rc
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_init_null_pointer() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    cs_codes rc = queue_init(NULL, attr, CS_QUEUE_DEQUE);

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

test_res test_queue_init_invalid_size() {
    queue_attr attr = {
        .size = 0,
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);

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

test_res test_queue_push_basic() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    rc = queue_push(&q, &val);

    if (rc != CS_SUCCESS) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to push element",
            .return_code = rc
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_push_multiple() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        rc = queue_push(&q, &i);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push element",
                .return_code = rc
            };
        }
    }

    if (queue_size(q) != 100) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after multiple pushes",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_push_null_queue() {
    int val = 42;
    cs_codes rc = queue_push(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL queue",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_push_null_data() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = queue_push(&q, NULL);

    if (rc != CS_NULL) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL data",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              POP TESTS                                     */
/******************************************************************************/

test_res test_queue_pop_basic() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    queue_push(&q, &val);

    rc = queue_pop(&q);

    if (rc != CS_SUCCESS) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to pop element",
            .return_code = rc
        };
    }

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should be empty after pop",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_pop_multiple() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        queue_push(&q, &i);
    }

    for (int i = 0; i < 25; i++) {
        rc = queue_pop(&q);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to pop element",
                .return_code = rc
            };
        }
    }

    if (queue_size(q) != 25) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 25 after popping 25 elements",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_pop_empty() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = queue_pop(&q);

    // Pop on empty should fail
    if (rc == CS_SUCCESS) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Pop on empty queue should fail",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_pop_null_queue() {
    cs_codes rc = queue_pop(NULL);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL queue",
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
/*                              FRONT TESTS                                   */
/******************************************************************************/

test_res test_queue_front_basic() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    queue_push(&q, &val);

    void *front = queue_front(&q);

    if (front == NULL) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    if (*(int *)front != 42) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_front_fifo_order() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push 0, 1, 2, 3, 4
    for (int i = 0; i < 5; i++) {
        queue_push(&q, &i);
    }

    // Front should be 0 (first pushed)
    void *front = queue_front(&q);
    if (front == NULL || *(int *)front != 0) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front should be 0 (FIFO order)",
            .return_code = CS_UNKNOWN
        };
    }

    // Pop and check order
    for (int i = 0; i < 5; i++) {
        front = queue_front(&q);
        if (front == NULL || *(int *)front != i) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "FIFO order violation",
                .return_code = CS_UNKNOWN
            };
        }
        queue_pop(&q);
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_front_empty() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    void *front = queue_front(&q);

    if (front != NULL) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front on empty queue should return NULL",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_front_null_queue() {
    void *front = queue_front(NULL);

    if (front != NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front on NULL queue should return NULL",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_front_does_not_pop() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    queue_push(&q, &val);

    // Call front multiple times
    queue_front(&q);
    queue_front(&q);
    queue_front(&q);

    // Size should still be 1
    if (queue_size(q) != 1) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front should not remove elements",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              BACK TESTS                                    */
/******************************************************************************/

test_res test_queue_back_basic() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    queue_push(&q, &val);

    void *back = queue_back(&q);

    if (back == NULL) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    if (*(int *)back != 42) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_back_last_pushed() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push 0, 1, 2, 3, 4
    for (int i = 0; i < 5; i++) {
        queue_push(&q, &i);
    }

    // Back should be 4 (last pushed)
    void *back = queue_back(&q);
    if (back == NULL || *(int *)back != 4) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back should be 4 (last pushed)",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_back_empty() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    void *back = queue_back(&q);

    if (back != NULL) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back on empty queue should return NULL",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_back_null_queue() {
    void *back = queue_back(NULL);

    if (back != NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back on NULL queue should return NULL",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_front_back_different() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val1 = 10, val2 = 20, val3 = 30;
    queue_push(&q, &val1);
    queue_push(&q, &val2);
    queue_push(&q, &val3);

    void *front = queue_front(&q);
    void *back = queue_back(&q);

    if (front == NULL || *(int *)front != 10) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front should be 10",
            .return_code = CS_UNKNOWN
        };
    }

    if (back == NULL || *(int *)back != 30) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back should be 30",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_queue_size_empty() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (queue_size(q) != 0) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 for empty queue",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_size_after_push() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 25; i++) {
        queue_push(&q, &i);
    }

    if (queue_size(q) != 25) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after pushes",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_size_after_pop() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        queue_push(&q, &i);
    }

    for (int i = 0; i < 5; i++) {
        queue_pop(&q);
    }

    if (queue_size(q) != 5) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 5 after popping 5 elements",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_empty_true() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Newly created queue should be empty",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_empty_false() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    queue_push(&q, &val);

    if (queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue with element should not be empty",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_empty_after_pop_all() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        queue_push(&q, &i);
    }

    for (int i = 0; i < 10; i++) {
        queue_pop(&q);
    }

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should be empty after popping all elements",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_queue_clear_basic() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        queue_push(&q, &i);
    }

    queue_clear(&q);

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    if (queue_size(q) != 0) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 after clear",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_clear_empty() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    queue_clear(&q);

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty queue should remain empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_clear_null() {
    // Clear NULL should not crash
    queue_clear(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SWAP TESTS                                    */
/******************************************************************************/

test_res test_queue_swap_basic() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q1, q2;
    cs_codes rc = queue_init(&q1, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize q1",
            .return_code = rc
        };
    }

    rc = queue_init(&q2, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        queue_free(&q1);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize q2",
            .return_code = rc
        };
    }

    // Add elements to q1
    for (int i = 0; i < 5; i++) {
        queue_push(&q1, &i);
    }

    // Add elements to q2
    for (int i = 100; i < 103; i++) {
        queue_push(&q2, &i);
    }

    int size1_before = queue_size(q1);
    int size2_before = queue_size(q2);

    queue_swap(&q1, &q2);

    if (queue_size(q1) != size2_before) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q1 size should match q2's old size",
            .return_code = CS_UNKNOWN
        };
    }

    if (queue_size(q2) != size1_before) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q2 size should match q1's old size",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q1);
    queue_free(&q2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_swap_with_empty() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q1, q2;
    cs_codes rc = queue_init(&q1, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize q1",
            .return_code = rc
        };
    }

    rc = queue_init(&q2, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        queue_free(&q1);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize q2",
            .return_code = rc
        };
    }

    // Add elements only to q1
    for (int i = 0; i < 10; i++) {
        queue_push(&q1, &i);
    }

    queue_swap(&q1, &q2);

    if (!queue_empty(q1)) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q1 should be empty after swap",
            .return_code = CS_UNKNOWN
        };
    }

    if (queue_size(q2) != 10) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q1);
    queue_free(&q2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_queue_free_null() {
    // Free NULL should not crash
    queue_free(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_free_empty() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_free_with_elements() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        queue_push(&q, &i);
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          CONTAINER TYPE TESTS                              */
/******************************************************************************/

test_res test_queue_list_operations() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_LIST);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize list queue",
            .return_code = rc
        };
    }

    // Push elements
    for (int i = 0; i < 50; i++) {
        queue_push(&q, &i);
    }

    // Verify FIFO order
    for (int i = 0; i < 50; i++) {
        void *front = queue_front(&q);
        if (front == NULL || *(int *)front != i) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "List queue FIFO order violation",
                .return_code = CS_UNKNOWN
            };
        }
        queue_pop(&q);
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_queue_large_capacity() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 5000; i++) {
        rc = queue_push(&q, &i);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push during stress test",
                .return_code = rc
            };
        }
    }

    if (queue_size(q) != 5000) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch in stress test",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_push_pop_cycle() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
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
            queue_push(&q, &i);
        }
        for (int i = 0; i < 20; i++) {
            queue_pop(&q);
        }
    }

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should be empty after cycles",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_negative_values() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = -50; i < 50; i++) {
        rc = queue_push(&q, &i);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push negative value",
                .return_code = rc
            };
        }
    }

    // Front should be -50 (first pushed)
    void *front = queue_front(&q);
    if (front == NULL || *(int *)front != -50) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front should be -50",
            .return_code = CS_UNKNOWN
        };
    }

    // Back should be 49 (last pushed)
    void *back = queue_back(&q);
    if (back == NULL || *(int *)back != 49) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back should be 49",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_clear_and_reuse() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push, clear, and push again
    for (int i = 0; i < 50; i++) {
        queue_push(&q, &i);
    }

    queue_clear(&q);

    for (int i = 100; i < 150; i++) {
        rc = queue_push(&q, &i);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push after clear",
                .return_code = rc
            };
        }
    }

    if (queue_size(q) != 50) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 50 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    // Front should be 100
    void *front = queue_front(&q);
    if (front == NULL || *(int *)front != 100) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front should be 100 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_interleaved_push_pop() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = print_int,
        .copy = NULL,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Interleaved push and pop
    int next_push = 0;
    int next_pop = 0;

    for (int i = 0; i < 100; i++) {
        // Push 2 elements
        queue_push(&q, &next_push);
        next_push++;
        queue_push(&q, &next_push);
        next_push++;

        // Pop 1 element and verify
        void *front = queue_front(&q);
        if (front == NULL || *(int *)front != next_pop) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Interleaved FIFO order violation",
                .return_code = CS_UNKNOWN
            };
        }
        queue_pop(&q);
        next_pop++;
    }

    // Should have 100 elements remaining (200 pushed - 100 popped)
    if (queue_size(q) != 100) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after interleaved operations",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          TEST STRUCT TESTS                                 */
/******************************************************************************/

test_res test_queue_struct_basic() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    test_struct ts = create_test_struct(1, "TestQueue", 10.5);
    rc = queue_push(&q, &ts);
    free_test_struct(&ts);

    if (rc != CS_SUCCESS) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to push struct",
            .return_code = rc
        };
    }

    if (queue_size(q) != 1) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 1",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_multiple() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "MultiTest", (double)i * 1.5);
        rc = queue_push(&q, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push struct",
                .return_code = rc
            };
        }
    }

    if (queue_size(q) != 100) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_front_back() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "FrontBackTest", (double)i);
        queue_push(&q, &ts);
        free_test_struct(&ts);
    }

    // Front should be id=0 (first pushed)
    test_struct *front = (test_struct *)queue_front(&q);
    if (front == NULL || front->id != 0) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front should have id=0",
            .return_code = CS_UNKNOWN
        };
    }

    // Back should be id=9 (last pushed)
    test_struct *back = (test_struct *)queue_back(&q);
    if (back == NULL || back->id != 9) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Back should have id=9",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_fifo_order() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Push structs with increasing IDs
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "FIFOTest", (double)i);
        queue_push(&q, &ts);
        free_test_struct(&ts);
    }

    // Pop and verify FIFO order
    for (int i = 0; i < 50; i++) {
        test_struct *front = (test_struct *)queue_front(&q);
        if (front == NULL || front->id != i) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "FIFO order violated with struct",
                .return_code = CS_UNKNOWN
            };
        }
        queue_pop(&q);
    }

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should be empty after popping all",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_simple() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
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
        rc = queue_push(&q, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push simple struct",
                .return_code = rc
            };
        }
    }

    // Verify FIFO order with simple structs
    for (int i = 0; i < 50; i++) {
        test_struct *front = (test_struct *)queue_front(&q);
        if (front == NULL || front->id != i) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "FIFO order violated with simple struct",
                .return_code = CS_UNKNOWN
            };
        }
        queue_pop(&q);
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_list_container() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_LIST);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize list queue",
            .return_code = rc
        };
    }

    // Push complex structs
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ListTest", (double)i * 2.5);
        rc = queue_push(&q, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push struct to list queue",
                .return_code = rc
            };
        }
    }

    // Verify FIFO order
    for (int i = 0; i < 50; i++) {
        test_struct *front = (test_struct *)queue_front(&q);
        if (front == NULL || front->id != i) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "FIFO order violated in list queue",
                .return_code = CS_UNKNOWN
            };
        }
        queue_pop(&q);
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_clear() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
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
        queue_push(&q, &ts);
        free_test_struct(&ts);
    }

    // Clear should free all struct memory
    queue_clear(&q);

    if (!queue_empty(q)) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    // Push again to verify clear worked
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        rc = queue_push(&q, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push after clear",
                .return_code = rc
            };
        }
    }

    // Verify front is id=100
    test_struct *front = (test_struct *)queue_front(&q);
    if (front == NULL || front->id != 100) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front should have id=100 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_swap() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q1, q2;
    cs_codes rc1 = queue_init(&q1, attr, CS_QUEUE_DEQUE);
    cs_codes rc2 = queue_init(&q2, attr, CS_QUEUE_DEQUE);
    if (rc1 != CS_SUCCESS || rc2 != CS_SUCCESS) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize queues",
            .return_code = CS_UNKNOWN
        };
    }

    // Push to q1
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "Queue1", (double)i);
        queue_push(&q1, &ts);
        free_test_struct(&ts);
    }

    // Push to q2
    for (int i = 100; i < 105; i++) {
        test_struct ts = create_test_struct(i, "Queue2", (double)i);
        queue_push(&q2, &ts);
        free_test_struct(&ts);
    }

    queue_swap(&q1, &q2);

    // q1 should now have 5 elements starting with id=100
    if (queue_size(q1) != 5) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q1 should have 5 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    test_struct *front1 = (test_struct *)queue_front(&q1);
    if (front1 == NULL || front1->id != 100) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q1 front should have id=100 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    // q2 should now have 10 elements starting with id=0
    if (queue_size(q2) != 10) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    test_struct *front2 = (test_struct *)queue_front(&q2);
    if (front2 == NULL || front2->id != 0) {
        queue_free(&q1);
        queue_free(&q2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "q2 front should have id=0 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q1);
    queue_free(&q2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_deep_copy_verify() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
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

    queue_push(&q, &ts);

    // The queue should have deep copied the struct
    test_struct *queued = (test_struct *)queue_front(&q);
    if (queued == NULL) {
        free_test_struct(&ts);
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Front returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    // The name pointers should be different (deep copy)
    if (queued->name == original_name) {
        free_test_struct(&ts);
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Name was not deep copied",
            .return_code = CS_UNKNOWN
        };
    }

    // But the values should be equal
    if (queued->id != 42 || strcmp(queued->name, "DeepCopyTest") != 0) {
        free_test_struct(&ts);
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Deep copied values don't match",
            .return_code = CS_UNKNOWN
        };
    }

    free_test_struct(&ts);
    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_queue_struct_interleaved() {
    queue_attr attr = {
        .size = sizeof(test_struct),
        .fr = free_test_struct,
        .print = print_test_struct_compact,
        .copy = copy_test_struct,
        .comp = NULL
    };

    queue q;
    cs_codes rc = queue_init(&q, attr, CS_QUEUE_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int next_push = 0;
    int next_pop = 0;

    for (int i = 0; i < 50; i++) {
        // Push 2 structs
        test_struct ts1 = create_test_struct(next_push++, "Interleaved", 1.0);
        test_struct ts2 = create_test_struct(next_push++, "Interleaved", 2.0);
        queue_push(&q, &ts1);
        queue_push(&q, &ts2);
        free_test_struct(&ts1);
        free_test_struct(&ts2);

        // Pop 1 struct and verify
        test_struct *front = (test_struct *)queue_front(&q);
        if (front == NULL || front->id != next_pop) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "FIFO order violated in interleaved operations",
                .return_code = CS_UNKNOWN
            };
        }
        queue_pop(&q);
        next_pop++;
    }

    // Should have 50 elements remaining
    if (queue_size(q) != 50) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after interleaved operations",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(&q);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test queue_tests[] = {
    // Init tests
    test_queue_init_deque,
    test_queue_init_list,
    test_queue_init_default,
    test_queue_init_null_pointer,
    test_queue_init_invalid_size,

    // Push tests
    test_queue_push_basic,
    test_queue_push_multiple,
    test_queue_push_null_queue,
    test_queue_push_null_data,

    // Pop tests
    test_queue_pop_basic,
    test_queue_pop_multiple,
    test_queue_pop_empty,
    test_queue_pop_null_queue,

    // Front tests
    test_queue_front_basic,
    test_queue_front_fifo_order,
    test_queue_front_empty,
    test_queue_front_null_queue,
    test_queue_front_does_not_pop,

    // Back tests
    test_queue_back_basic,
    test_queue_back_last_pushed,
    test_queue_back_empty,
    test_queue_back_null_queue,
    test_queue_front_back_different,

    // Size/Empty tests
    test_queue_size_empty,
    test_queue_size_after_push,
    test_queue_size_after_pop,
    test_queue_empty_true,
    test_queue_empty_false,
    test_queue_empty_after_pop_all,

    // Clear tests
    test_queue_clear_basic,
    test_queue_clear_empty,
    test_queue_clear_null,

    // Swap tests
    test_queue_swap_basic,
    test_queue_swap_with_empty,

    // Free tests
    test_queue_free_null,
    test_queue_free_empty,
    test_queue_free_with_elements,

    // Container type tests
    test_queue_list_operations,

    // Stress/Edge tests
    test_queue_large_capacity,
    test_queue_push_pop_cycle,
    test_queue_negative_values,
    test_queue_clear_and_reuse,
    test_queue_interleaved_push_pop,

    // Struct tests
    test_queue_struct_basic,
    test_queue_struct_multiple,
    test_queue_struct_front_back,
    test_queue_struct_fifo_order,
    test_queue_struct_simple,
    test_queue_struct_list_container,
    test_queue_struct_clear,
    test_queue_struct_swap,
    test_queue_struct_deep_copy_verify,
    test_queue_struct_interleaved,
};