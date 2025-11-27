#include <cs/list.h>

#include "../include/unittest.h"

#define TEST_SIZE 1000

FILE *DEBUG_OUT = NULL;

test_res test_list_init() {
    list l;
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .print = NULL,
        .fr = NULL,
    };

    cs_codes rc = list_init(&l, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_init returned error code",
            .return_code = rc
        };
    }

    list_free(&l);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_list_push_front() {
    list l;
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .print = NULL,
        .fr = NULL,
    };
    int value;

    cs_codes rc = list_init(&l, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % 10000;
        rc = list_push_front(&l, &value);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_push_front returned error code",
                .return_code = rc
            };
        }

        if (*(int *)list_front(l) != value) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_front did not return the correct value",
                .return_code = CS_UNKNOWN
            };
        }
    }

    list_free(&l);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_list_push_back() {
    list l;
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .print = NULL,
        .fr = NULL,
    };
    int value;

    cs_codes rc = list_init(&l, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % 10000;
        rc = list_push_back(&l, &value);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_push_back returned error code",
                .return_code = rc
            };
        }

        if (*(int *)list_back(l) != value) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_back did not return the correct value",
                .return_code = CS_UNKNOWN
            };
        }
    }

    list_free(&l);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_list_pop_front() {
    list l;
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .print = NULL,
        .fr = NULL,
    };
    int values[TEST_SIZE];

    cs_codes rc = list_init(&l, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = list_push_back(&l, &values[i]);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_push_back returned error code",
                .return_code = rc
            };
        }
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        if (*(int *)list_front(l) != values[i]) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_front did not return the correct value",
                .return_code = CS_UNKNOWN
            };
        }

        rc = list_pop_front(&l);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_pop_front returned error code",
                .return_code = rc
            };
        }
    }

    list_free(&l);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_list_pop_back() {
    list l;
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .print = NULL,
        .fr = NULL,
    };
    int values[TEST_SIZE];

    cs_codes rc = list_init(&l, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = list_push_back(&l, &values[i]);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_push_back returned error code",
                .return_code = rc
            };
        }
    }

    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        if (*(int *)list_back(l) != values[i]) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_back did not return the correct value",
                .return_code = CS_UNKNOWN
            };
        }

        rc = list_pop_back(&l);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_pop_back returned error code",
                .return_code = rc
            };
        }
    }

    list_free(&l);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_list_erase() {
    list l;
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .print = NULL,
        .fr = NULL,
    };
    int values[TEST_SIZE], erase_pos;

    cs_codes rc = list_init(&l, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        values[i] = rand() % 10000;
        rc = list_push_back(&l, &values[i]);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_push_back returned error code",
                .return_code = rc
            };
        }
    }

    erase_pos = rand() % TEST_SIZE;
    rc = list_erase(&l, erase_pos);
    if (rc != CS_SUCCESS) {
        list_free(&l);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_erase returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < l.size; i++) {
        int expected_value = (i < erase_pos) ? values[i] : values[i + 1];
        if (*(int *)list_front(l) != expected_value) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_erase did not remove the correct element",
                .return_code = CS_UNKNOWN
            };
        }
        rc = list_pop_front(&l);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_pop_front returned error code during verification",
                .return_code = rc
            };
        }
    }

    list_free(&l);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_list_sort() {
    list l;
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = comp_int_max,
        .print = NULL,
        .fr = NULL,
    };
    int prev_value;

    cs_codes rc = list_init(&l, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int value = rand() % 10000;
        rc = list_push_back(&l, &value);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_push_back returned error code",
                .return_code = rc
            };
        }
    }

    rc = list_sort(&l);
    if (rc != CS_SUCCESS) {
        list_free(&l);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_sort returned error code",
            .return_code = rc
        };
    }

    prev_value = *(int *)list_front(l);
    rc = list_pop_front(&l);
    if (rc != CS_SUCCESS) {
        list_free(&l);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "list_pop_front returned error code during verification",
            .return_code = rc
        };
    }

    while (!list_empty(l)) {
        int current_value = *(int *)list_front(l);
        if (current_value < prev_value) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_sort did not sort the list correctly",
                .return_code = CS_UNKNOWN
            };
        }
        prev_value = current_value;
        rc = list_pop_front(&l);
        if (rc != CS_SUCCESS) {
            list_free(&l);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "list_pop_front returned error code during verification",
                .return_code = rc
            };
        }
    }

    list_free(&l);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_list_init,
        test_list_push_front,
        test_list_push_back,
        test_list_pop_front,
        test_list_pop_back,
        test_list_erase,
        test_list_sort,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}