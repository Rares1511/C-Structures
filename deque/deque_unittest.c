#include <cs/deque.h>

#include "../include/unittest.h"

#define TEST_SIZE 1000

FILE *DEBUG_OUT = NULL;

test_res test_deque_init() {
    deque dq;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_init", .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    if (dq.size != 0 || dq.front != NULL || dq.back != NULL) {
        return (test_res){
            .test_name = "test_deque_init", .reason = "Deque not initialized correctly", .return_code = CS_UNKNOWN
        };
    }

    deque_free(&dq);

    return (test_res){
        .test_name = "test_deque_init", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_push_back() {
    deque dq;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_push_back", .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 1000;
        rc = deque_push_back(&dq, &values[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_push_back", .reason = "Failed to push back element", .return_code = rc
            };
        }
    }

    if (dq.size != TEST_SIZE) {
        return (test_res){
            .test_name = "test_deque_push_back", .reason = "Deque size incorrect after pushes", .return_code = CS_UNKNOWN
        };
    }

    deque_node_t *current = dq.front;
    for (int i = 0; i < TEST_SIZE; i++) {
        if (*(int *)current->data != values[i]) {
            return (test_res){
                .test_name = "test_deque_push_back", .reason = "Deque element mismatch", .return_code = CS_UNKNOWN
            };
        }
        current = current->next;
    }

    deque_free(&dq);

    return (test_res){
        .test_name = "test_deque_push_back", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_push_front() {
    deque dq;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_push_front", .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 1000;
        rc = deque_push_front(&dq, &values[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_push_front", .reason = "Failed to push front element", .return_code = rc
            };
        }
    }

    if (dq.size != TEST_SIZE) {
        return (test_res){
            .test_name = "test_deque_push_front", .reason = "Deque size incorrect after pushes", .return_code = CS_UNKNOWN
        };
    }

    deque_node_t *current = dq.front;
    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        if (*(int *)current->data != values[i]) {
            return (test_res){
                .test_name = "test_deque_push_front", .reason = "Deque element mismatch", .return_code = CS_UNKNOWN
            };
        }
        current = current->next;
    }

    deque_free(&dq);

    return (test_res){
        .test_name = "test_deque_push_front", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_pop_back() {
    deque dq;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_pop_back", .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 1000;
        rc = deque_push_back(&dq, &values[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_pop_back", .reason = "Failed to push back element", .return_code = rc
            };
        }
    }

    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        int *back_value = (int *)deque_back(dq);
        if (*back_value != values[i]) {
            return (test_res){
                .test_name = "test_deque_pop_back", .reason = "Back element mismatch before pop", .return_code = CS_UNKNOWN
            };
        }
        rc = deque_pop_back(&dq);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_pop_back", .reason = "Failed to pop back element", .return_code = rc
            };
        }
    }

    if (dq.size != 0) {
        return (test_res){
            .test_name = "test_deque_pop_back", .reason = "Deque not empty after pops", .return_code = CS_UNKNOWN
        };
    }

    deque_free(&dq);

    return (test_res){
        .test_name = "test_deque_pop_back", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_pop_front() {
    deque dq;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_pop_front", .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 1000;
        rc = deque_push_back(&dq, &values[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_pop_front", .reason = "Failed to push back element", .return_code = rc
            };
        }
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int *front_value = (int *)deque_front(dq);
        if (*front_value != values[i]) {
            return (test_res){
                .test_name = "test_deque_pop_front", .reason = "Front element mismatch before pop", .return_code = CS_UNKNOWN
            };
        }
        rc = deque_pop_front(&dq);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_pop_front", .reason = "Failed to pop front element", .return_code = rc
            };
        }
    }

    if (dq.size != 0) {
        return (test_res){
            .test_name = "test_deque_pop_front", .reason = "Deque not empty after pops", .return_code = CS_UNKNOWN
        };
    }

    deque_free(&dq);

    return (test_res){
        .test_name = "test_deque_pop_front", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_clone() {
    deque dq1, dq2;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = deque_init(&dq1, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_clone", .reason = "Failed to initialize source deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 1000;
        rc = deque_push_back(&dq1, &values[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_clone", .reason = "Failed to push back element to source deque", .return_code = rc
            };
        }
    }

    rc = deque_clone(&dq2, dq1);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_clone", .reason = "Failed to clone deque", .return_code = rc
        };
    }

    if (dq2.size != dq1.size) {
        return (test_res){
            .test_name = "test_deque_clone", .reason = "Cloned deque size mismatch", .return_code = CS_UNKNOWN
        };
    }

    deque_node_t *current1 = dq1.front;
    deque_node_t *current2 = dq2.front;
    int index = 0;
    while (current1 != NULL && current2 != NULL) {
        if (*(int *)current1->data != *(int *)current2->data && *(int *)current2->data != values[index]) {
            return (test_res){
                .test_name = "test_deque_clone", .reason = "Cloned deque element mismatch", .return_code = CS_UNKNOWN
            };
        }
        current1 = current1->next;
        current2 = current2->next;
        index++;
    }

    deque_free(&dq1);
    deque_free(&dq2);

    return (test_res){
        .test_name = "test_deque_clone", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_front() {
    deque dq;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int value;

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_front", .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % 1000;
        rc = deque_push_front(&dq, &value);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_front", .reason = "Failed to push front element", .return_code = rc
            };
        }

        int value_at_front = *(int *)deque_front(dq);
        if (value_at_front != value) {
            return (test_res){
                .test_name = "test_deque_front", .reason = "Front element mismatch after push", .return_code = CS_UNKNOWN
            };
        }
    }

    deque_free(&dq);

    return (test_res){
        .test_name = "test_deque_front", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_back() {
    deque dq;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int value;

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_back", .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % 1000;
        rc = deque_push_back(&dq, &value);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_deque_back", .reason = "Failed to push back element", .return_code = rc
            };
        }

        int value_at_back = *(int *)deque_back(dq);
        if (value_at_back != value) {
            return (test_res){
                .test_name = "test_deque_back", .reason = "Back element mismatch after push", .return_code = CS_UNKNOWN
            };
        }
    }

    deque_free(&dq);

    return (test_res){
        .test_name = "test_deque_back", .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_swap() {
    deque dq1, dq2;
    deque_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int values1[TEST_SIZE], values2[TEST_SIZE];

    cs_codes rc = deque_init(&dq1, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_swap", .reason = "Failed to initialize first deque", .return_code = rc
        };
    }

    rc = deque_init(&dq2, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_deque_swap", .reason = "Failed to initialize second deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values1[i] = rand() % 1000;
        values2[i] = rand() % 1000;
        deque_push_back(&dq1, &values1[i]);
        deque_push_back(&dq2, &values2[i]);
    }

    deque_swap(&dq1, &dq2);

    for (int i = 0; i < TEST_SIZE; i++) {
        int *val1 = (int *)deque_front(dq1);
        int *val2 = (int *)deque_front(dq2);
        if (*val1 != values2[i] || *val2 != values1[i]) {
            return (test_res){
                .test_name = "test_deque_swap", .reason = "Deque swap failed", .return_code = CS_UNKNOWN
            };
        }
        deque_pop_front(&dq1);
        deque_pop_front(&dq2);
    }

    deque_free(&dq1);
    deque_free(&dq2);

    return (test_res){
        .test_name = "test_deque_swap", .reason = NULL, .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_deque_init,
        test_deque_push_back,
        test_deque_push_front,
        test_deque_pop_back,
        test_deque_pop_front,
        test_deque_clone,
        test_deque_front,
        test_deque_back,
        test_deque_swap,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}