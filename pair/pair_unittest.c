#include <cs/pair.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_pair_init() {
    pair p;
    pair_attr_t first_attr = {
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .comp = NULL,
        .print = NULL
    };
    pair_attr_t second_attr = {
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(double),
        .comp = NULL,
        .print = NULL
    };

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_pair_init",
            .reason = "pair_init failed",
            .return_code = rc
        };
    }

    pair_free(&p);

    return (test_res){
        .test_name = "test_pair_init",
        .reason = "pair_init succeeded",
        .return_code = CS_SUCCESS
    };
}

test_res test_pair_set_get() {
    pair p;
    pair_attr_t first_attr = {
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .comp = NULL,
        .print = NULL
    };
    pair_attr_t second_attr = {
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(double),
        .comp = NULL,
        .print = NULL
    };

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_pair_set_get",
            .reason = "pair_init failed",
            .return_code = rc
        };
    }

    int first_value = 42;
    double second_value = 3.14;

    rc = pair_set(&p, &first_value, &second_value);
    if (rc != CS_SUCCESS) {
        pair_free(&p);
        return (test_res){
            .test_name = "test_pair_set_get",
            .reason = "pair_set failed",
            .return_code = rc
        };
    }

    int* retrieved_first = (int*)pair_first(p);
    double* retrieved_second = (double*)pair_second(p);

    if (*retrieved_first != first_value || *retrieved_second != second_value) {
        pair_free(&p);
        return (test_res){
            .test_name = "test_pair_set_get",
            .reason = "Retrieved values do not match set values",
            .return_code = CS_ELEM
        };
    }

    pair_free(&p);

    return (test_res){
        .test_name = "test_pair_set_get",
        .reason = "pair_set and pair_get succeeded",
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char** argv) {
    test tests[] ={
        test_pair_init,
        test_pair_set_get,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}