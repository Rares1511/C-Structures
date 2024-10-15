#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/unittest.h"
#include "../include/vector.h"

test_res test_vector_init() {
    vector v;
    vector_attr_t attr = {sizeof(int), NULL, NULL, NULL, NULL};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_vector_init", .reason = "vector_init failed", .return_code = rc};
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = "test_vector_init",
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){.test_name = "test_vector_init", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_init_neg_attr_size() {
    vector v;
    vector_attr_t attr = {-1, NULL, NULL, NULL, NULL};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = "test_vector_init_wrong_attr",
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = "test_vector_init_wrong_attr", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_init_max_attr_size() {
    vector v;
    vector_attr_t attr = {SIZE_TH + 1, NULL, NULL, NULL, NULL};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = "test_vector_init_wrong_attr",
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = "test_vector_init_wrong_attr", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_insert_at() {
    vector v;
    vector_attr_t attr = {sizeof(int), NULL, print_int, NULL, NULL};
    int insert_elements[] = {1, 2, 3, 4, 5};
    int insert_positions[] = {0, 0, 1, 1, 3};
    int correct_elements[] = {2, 4, 3, 5, 1};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){.test_name = "test_vector_insert_at",
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    for (int i = 0; i < 5; i++) {
        rc = vector_insert_at(&v, &insert_elements[i], insert_positions[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){.test_name = "test_vector_insert_at",
                              .reason = "failed to insert elements",
                              .return_code = rc};
        }
    }

    vector_print(&v);
    printf("\n");

    for (int i = 0; i < 5; i++) {
        if (*(int *)vector_at(v, i) != correct_elements[i]) {
            return (test_res){.test_name = "test_vector_insert_at",
                              .reason = "elements were not introduced correctly",
                              .return_code = CS_UNKNOWN};
        }
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = "test_vector_insert_at",
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){
        .test_name = "test_vector_insert_at", .reason = "none", .return_code = CS_SUCCESS};
}

int main() {
    int i;
    test tests[] = {test_vector_init, test_vector_init_neg_attr_size,
                    test_vector_init_max_attr_size, test_vector_insert_at};
    test_res res;

    for (i = 0; i < MATRIX_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++) {
        res = tests[i]();
        char buffer[1024];
        strcpy(buffer, res.test_name);
        strncat(buffer,
                "..........................................................................",
                MAX_PRINT_SIZE - strlen(res.test_name));
        if (res.return_code != CS_SUCCESS) {
            printf("%sFAILED: %s\n", buffer, res.reason);
            exit(-(int)(sizeof(tests) / sizeof(test) - i));
        } else {
            printf("%sSUCCESS: %d/%d\n", buffer, i + 1, (int)(sizeof(tests) / sizeof(test)));
        }
    }
    return 0;
}