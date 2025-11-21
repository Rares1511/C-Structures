#include <cs/queue.h>

#include "../include/unittest.h"

#define TEST_SIZE 1000

FILE *DEBUG_OUT = NULL;

test_res test_queue_init() {
    queue q;
    queue_attr_t attr = {
        .size = sizeof(int),
        .print = print_int,
        .copy = NULL,
        .fr = NULL,
        .stream = DEBUG_OUT
    };

    cs_codes rc = queue_init(&q, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize queue",
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

test_res test_queue_push() {
    queue q;
    queue_attr_t attr = {
        .size = sizeof(int),
        .print = print_int,
        .copy = NULL,
        .fr = NULL,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = queue_init(&q, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize queue",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = queue_push(&q, &values[i]);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push element to queue",
                .return_code = rc
            };
        }

        if (*(int *)queue_back(q) != values[i]) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Queue back element incorrect after push",
                .return_code = CS_UNKNOWN
            };
        }
    }

    if (q.size != TEST_SIZE) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue size incorrect after push",
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

test_res test_queue_pop() {
    queue q;
    queue_attr_t attr = {
        .size = sizeof(int),
        .print = print_int,
        .copy = NULL,
        .fr = NULL,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = queue_init(&q, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize queue",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = queue_push(&q, &values[i]);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to push element to queue",
                .return_code = rc
            };
        }
    }

    while (!queue_empty(q)) {
        if (*(int *)queue_front(q) != values[TEST_SIZE - q.size]) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Queue front element incorrect before pop",
                .return_code = CS_UNKNOWN
            };
        }

        rc = queue_pop(&q);
        if (rc != CS_SUCCESS) {
            queue_free(&q);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to pop element from queue",
                .return_code = rc
            };
        }
    }

    if (q.size != 0) {
        queue_free(&q);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Queue size incorrect after pops",
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

int main (int argc, char** argv) {
    test tests[] = {
        test_queue_init,
        test_queue_push,
        test_queue_pop,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);

    return 0;
}