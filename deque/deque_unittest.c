#include <cs/deque.h>

#include "../include/unittest.h"

#define TEST_SIZE DEQUE_BLOCK_SIZE * DEQUE_INIT_BLOCKS * 2

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
            .test_name = (char *) __func__, .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    deque_free(&dq);

    return (test_res){
        .test_name = (char *) __func__, .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_push_pop_front() {
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
            .test_name = (char *) __func__, .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = deque_push_front(&dq, &values[i]);
        if (rc != CS_SUCCESS) {
            deque_free(&dq);
            return (test_res){
                .test_name = (char *) __func__, .reason = "Failed to push to front", .return_code = rc
            };
        }
    }

    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        int *front = (int *)deque_front(dq);
        if (!front || *front != values[i]) {
            deque_free(&dq);
            return (test_res){
                .test_name = (char *) __func__, .reason = "Front element mismatch", .return_code = CS_UNKNOWN
            };
        }

        rc = deque_pop_front(&dq);
        if (rc != CS_SUCCESS) {
            deque_free(&dq);
            return (test_res){
                .test_name = (char *) __func__, .reason = "Failed to pop from front", .return_code = rc
            };
        }
    }

    deque_free(&dq);

    return (test_res){
        .test_name = (char *) __func__, .reason = NULL, .return_code = CS_SUCCESS
    };
}

test_res test_deque_push_pop_back() {
    deque dq;
    deque_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .size = sizeof(int),
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = deque_init(&dq, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *) __func__, .reason = "Failed to initialize deque", .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = deque_push_back(&dq, &values[i]);
        if (rc != CS_SUCCESS) {
            deque_free(&dq);
            return (test_res){
                .test_name = (char *) __func__, .reason = "Failed to push to back", .return_code = rc
            };
        }
    }

    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        int *front = (int *)deque_back(dq);
        if (!front || *front != values[i]) {
            deque_free(&dq);
            return (test_res){
                .test_name = (char *) __func__, .reason = "Back element mismatch", .return_code = CS_UNKNOWN
            };
        }
        rc = deque_pop_back(&dq);
        if (rc != CS_SUCCESS) {
            deque_free(&dq);
            return (test_res){
                .test_name = (char *) __func__, .reason = "Failed to pop from back", .return_code = rc
            };
        }
    }

    deque_free(&dq);

    return (test_res){
        .test_name = (char *) __func__, .reason = NULL, .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_deque_init,
        test_deque_push_pop_front,
        test_deque_push_pop_back,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}