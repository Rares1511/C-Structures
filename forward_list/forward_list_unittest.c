#include <cs/forward_list.h>

#include "../include/unittest.h"

#define TEST_SIZE 1000

FILE *DEBUG_OUT = NULL;

test_res test_forward_list_init() {
    forward_list list;
    forward_list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL,
        .stream = DEBUG_OUT
    };

    cs_codes rc = forward_list_init(&list, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "forward_list_init failed",
            .return_code = rc
        };
    }

    forward_list_free(&list);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_forward_list_push_front() {
    forward_list list;
    forward_list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL,
        .stream = DEBUG_OUT
    };
    int value;

    cs_codes rc = forward_list_init(&list, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "forward_list_init failed",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % 10000;
        rc = forward_list_push_front(&list, &value);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "forward_list_push_front failed",
                .return_code = rc
            };
        }

        if (*(int *)forward_list_front(list) != value) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "front element mismatch after push_front",
                .return_code = CS_UNKNOWN
            };
        }
    }

    if (list.size != TEST_SIZE) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size after push_front is incorrect",
            .return_code = CS_UNKNOWN
        };
    }

    forward_list_free(&list);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_forward_list_pop_front() {
    forward_list list;
    forward_list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL,
        .stream = DEBUG_OUT
    };
    int values[TEST_SIZE];

    cs_codes rc = forward_list_init(&list, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "forward_list_init failed",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = forward_list_push_front(&list, &values[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "forward_list_push_front failed",
                .return_code = rc
            };
        }
    }

    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        if (*(int *)forward_list_front(list) != values[i]) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "front element mismatch before pop_front",
                .return_code = CS_UNKNOWN
            };
        }

        rc = forward_list_pop_front(&list);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "forward_list_pop_front failed",
                .return_code = rc
            };
        }
    }

    forward_list_free(&list);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_forward_list_swap() {
    forward_list list1, list2;
    forward_list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL,
        .stream = DEBUG_OUT
    };
    int values1[TEST_SIZE], values2[TEST_SIZE];

    cs_codes rc = forward_list_init(&list1, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "forward_list_init for list1 failed",
            .return_code = rc
        };
    }

    rc = forward_list_init(&list2, attr);
    if (rc != CS_SUCCESS) {
        forward_list_free(&list1);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "forward_list_init for list2 failed",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values1[i] = rand() % 10000;
        values2[i] = rand() % 10000;

        rc = forward_list_push_front(&list1, &values1[i]);
        if (rc != CS_SUCCESS) {
            forward_list_free(&list1);
            forward_list_free(&list2);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "forward_list_push_front for list1 failed",
                .return_code = rc
            };
        }

        rc = forward_list_push_front(&list2, &values2[i]);
        if (rc != CS_SUCCESS) {
            forward_list_free(&list1);
            forward_list_free(&list2);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "forward_list_push_front for list2 failed",
                .return_code = rc
            };
        }
    }

    forward_list_swap(&list1, &list2);

    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        if (*(int *)forward_list_front(list1) != values2[i]) {
            forward_list_free(&list1);
            forward_list_free(&list2);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list1 front element mismatch after swap",
                .return_code = CS_UNKNOWN
            };
        }
        forward_list_pop_front(&list1);
    }

    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        if (*(int *)forward_list_front(list2) != values1[i]) {
            forward_list_free(&list1);
            forward_list_free(&list2);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list2 front element mismatch after swap",
                .return_code = CS_UNKNOWN
            };
        }
        forward_list_pop_front(&list2);
    }

    forward_list_free(&list1);
    forward_list_free(&list2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char** argv) {
    test tests[] = {
        test_forward_list_init,
        test_forward_list_push_front,
        test_forward_list_pop_front,
        test_forward_list_swap,
    };

    unittest(tests, sizeof(tests)/sizeof(test), argc, argv);
    return 0;
}