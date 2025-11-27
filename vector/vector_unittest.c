#include <cs/vector.h>

#include "../include/unittest.h"

FILE *DEBUG_OUT = NULL;

test_res test_vector_init() {
    vector v;
    vector_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .size = sizeof(int),
    };

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__, .reason = "vector_init failed", .return_code = rc};
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){.test_name = (char *)__func__, .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_init_neg_attr_size() {
    vector v;
    vector_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .size = -1,
    };

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = (char *)__func__, .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_init_max_attr_size() {
    vector v;
    vector_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .size = SIZE_TH + 1,
    };

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SIZE) {
        return (test_res){.test_name = "test_vector_init_wrong_attr",
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    return (test_res){
        .test_name = (char *)__func__, .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_insert_at() {
    vector v;
    vector_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .size = sizeof(int),
    };
    int insert_elements[] = {1, 2, 3, 4, 5};
    int insert_positions[] = {0, 0, 1, 1, 3};
    int correct_elements[] = {2, 4, 3, 5, 1};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    for (int i = 0; i < 5; i++) {
        rc = vector_insert_at(&v, &insert_elements[i], insert_positions[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "failed to insert elements",
                              .return_code = rc};
        }
    }

    for (int i = 0; i < 5; i++) {
        if (*(int *)vector_at(v, i) != correct_elements[i]) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "elements were not introduced correctly",
                              .return_code = CS_UNKNOWN};
        }
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){
        .test_name = "test_vector_insert_at", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_push_back() {
    vector v;
    vector_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .size = sizeof(int),
    };
    int insert_elements[] = {1, 2, 3, 4, 5};
    int correct_elements[] = {1, 2, 3, 4, 5};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    for (int i = 0; i < 5; i++) {
        rc = vector_push_back(&v, &insert_elements[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "failed to insert elements",
                              .return_code = rc};
        }
    }

    for (int i = 0; i < 5; i++) {
        if (*(int *)vector_at(v, i) != correct_elements[i]) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "elements were not introduced correctly",
                              .return_code = CS_UNKNOWN};
        }
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){
        .test_name = (char *)__func__, .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_erase() {
    vector v;
    vector_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .size = sizeof(int),
    };
    int insert_elements[] = {1, 2, 3, 4, 5};
    int correct_elements[] = {1, 2, 4, 5};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    for (int i = 0; i < 5; i++) {
        rc = vector_push_back(&v, &insert_elements[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "failed to insert elements",
                              .return_code = rc};
        }
    }

    rc = vector_erase(&v, 2);
    if (rc != CS_SUCCESS) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "failed to erase element",
                          .return_code = rc};
    }

    for (int i = 0; i < 4; i++) {
        if (*(int *)vector_at(v, i) != correct_elements[i]) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "elements were not erased correctly",
                              .return_code = CS_UNKNOWN};
        }
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){
        .test_name = (char *)__func__, .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_vector_replace() {
    vector v;
    vector_attr_t attr = {
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .size = sizeof(int),
    };
    int insert_elements[] = {1, 2, 3, 4, 5};
    int correct_elements[] = {1, 2, 99, 4, 5};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_init failed",
                          .return_code = rc};
    }

    for (int i = 0; i < 5; i++) {
        rc = vector_push_back(&v, &insert_elements[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "failed to insert elements",
                              .return_code = rc};
        }
    }

    int new_element = 99;
    rc = vector_replace(&v, &new_element, 2);
    if (rc != CS_SUCCESS) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "failed to replace element",
                          .return_code = rc};
    }

    for (int i = 0; i < 5; i++) {
        if (*(int *)vector_at(v, i) != correct_elements[i]) {
            return (test_res){.test_name = (char *)__func__,
                              .reason = "elements were not replaced correctly",
                              .return_code = CS_UNKNOWN};
        }
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = (char *)__func__,
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){
        .test_name = (char *)__func__, .reason = "none", .return_code = CS_SUCCESS};
}

int main(int argc, char **argv) {
    test tests[] = {
        test_vector_init,
        test_vector_init_neg_attr_size,
        test_vector_init_max_attr_size, 
        test_vector_insert_at, 
        test_vector_push_back,
        test_vector_erase,
        test_vector_replace
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}