#include <cs/vector.h>

#include <unittest.h>
#include <benchmark.h>

#include <sys/time.h>
#include <valgrind/valgrind.h>

// ============================================================================
// vector_init
// ============================================================================
test_res test_vector_init(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// vector_push_back
// ============================================================================
test_res test_vector_push_back(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val = 42;
    UNITTEST_ASSERT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed", arg->logger,
        "Successfully pushed value to back of vector\n");

    int *retrieved = (int*)vector_at(v, 0);
    UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index 0", arg->logger,
        "Successfully retrieved element at index 0\n");
    UNITTEST_ASSERT(*retrieved, ==, val, "Retrieved value does not match inserted value", arg->logger,
        "Retrieved value matches inserted value\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_push_back_grow(test_arg *arg) {
    int cap = 32;
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){2, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes and min_cap of 2\n");

    for (int i = 0; i < cap + 1; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back at index failed");
    }

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

test_res test_vector_push_back_deepcopy(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_test_struct_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with test_struct attributes\n");

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "Test", i * 10.0);
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &ts), ==, CS_SUCCESS, "Push back at index failed");
        free_test_struct(&ts); // Free the original struct since vector should have made a deep copy
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed 10 test_struct elements into vector\n");

    for (int i = 0; i < 10; i++) {
        test_struct *retrieved = UNITTEST_ASSERT_SILENT((test_struct*)vector_at(v, i), !=, NULL, "Failed to retrieve element at index");
        test_struct expected = create_test_struct(i, "Test", i * 10.0);
        UNITTEST_ASSERT_SILENT(comp_test_struct_max(retrieved, &expected), ==, 0, "Retrieved struct does not match expected struct");
        free_test_struct(&expected); // Free the expected struct since it was dynamically allocated
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully retrieved and verified 10 test_struct elements from vector\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_insert_at
// ============================================================================
test_res test_vector_insert_at(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val = 42;
    UNITTEST_ASSERT(vector_insert_at(v, &val, 0), ==, CS_SUCCESS, "Insert at index 0 failed", arg->logger,
        "Successfully inserted value at index 0\n");

    int *retrieved = (int*)vector_at(v, 0);
    UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index 0", arg->logger,
        "Successfully retrieved element at index 0\n");
    UNITTEST_ASSERT(*retrieved, ==, val, "Retrieved value does not match inserted value", arg->logger,
        "Retrieved value matches inserted value\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

test_res test_vector_insert_at_grow(test_arg *arg) {
    int cap = 32;
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){2, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes and min_cap of 2\n");

    for (int i = 0; i < cap + 1; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(vector_insert_at(v, &val, i / 2), ==, CS_SUCCESS, "Insert at index failed");
    }

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

test_res test_vector_insert_at_deepcopy(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_test_struct_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with test_struct attributes\n");

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "Test", i * 10.0);
        UNITTEST_ASSERT_SILENT(vector_insert_at(v, &ts, i), ==, CS_SUCCESS, "Insert at index failed");
        free_test_struct(&ts); // Free the original struct since vector should have made a deep copy
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully inserted 10 test_struct elements into vector\n");

    for (int i = 0; i < 10; i++) {
        test_struct *retrieved = UNITTEST_ASSERT_SILENT((test_struct*)vector_at(v, i), !=, NULL, "Failed to retrieve element at index");
        test_struct expected = create_test_struct(i, "Test", i * 10.0);
        UNITTEST_ASSERT_SILENT(comp_test_struct_max(retrieved, &expected), ==, 0, "Retrieved struct does not match expected struct");
        free_test_struct(&expected); // Free the expected struct since it was dynamically allocated
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully retrieved and verified 10 test_struct elements from vector\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_pop_back
// ============================================================================
test_res test_vector_pop_back(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val = 42;
    UNITTEST_ASSERT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed", arg->logger,
        "Successfully pushed value to back of vector\n");

    UNITTEST_ASSERT(vector_pop_back(v), ==, CS_SUCCESS, "Pop back failed", arg->logger,
        "Successfully popped value from back of vector\n");
    
    UNITTEST_ASSERT(vector_size(v), ==, 0, "Vector size should be 0 after pop_back", arg->logger,
        "Vector size is correct after pop_back\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_pop_back_shrink(test_arg *arg) {
    int cap = 32;
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){8, 3}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes and min_cap of 8 and shrink_factor of 3\n");

    for (int i = 0; i < cap; i++) {
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &i), ==, CS_SUCCESS, "Push back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d values to vector\n", cap);

    for (int i = 0; i < cap; i++) {
        UNITTEST_ASSERT_SILENT(vector_pop_back(v), ==, CS_SUCCESS, "Pop back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully popped %d values from vector\n", cap);

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

test_res test_vector_pop_back_deepfree(test_arg *arg) {
    int cap = 32;
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_test_struct_attr(), (vector_attr_t){8, 2}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with test_struct attributes\n");

    for (int i = 0; i < cap; i++) {
        test_struct ts = create_test_struct(i, "Test", i * 10.0);
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &ts), ==, CS_SUCCESS, "Push back failed");
        free_test_struct(&ts); // Free the original struct since vector should have made a deep copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d test_struct values to vector\n", cap);

    for (int i = 0; i < cap - 1; i++) {
        UNITTEST_ASSERT_SILENT(vector_pop_back(v), ==, CS_SUCCESS, "Pop back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully popped %d test_struct values from vector\n", cap - 1);

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_erase
// ============================================================================
test_res test_vector_erase(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val2), ==, CS_SUCCESS, "Push back of second value failed", arg->logger,
        "Successfully pushed second value to back of vector\n");

    UNITTEST_ASSERT(vector_erase(v, 0), ==, CS_SUCCESS, "Erase at index 0 failed", arg->logger,
        "Successfully erased element at index 0\n");
    
    int *retrieved = (int*)vector_at(v, 0);
    UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index 0 after erase", arg->logger,
        "Successfully retrieved element at index 0 after erase\n");
    UNITTEST_ASSERT(*retrieved, ==, val2, "Retrieved value does not match expected value after erase", arg->logger,
        "Retrieved value matches expected value after erase\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_erase_shrink(test_arg *arg) {
    int cap = 32;
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){8, 3}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes and min_cap of 8 and shrink_factor of 3\n");

    for (int i = 0; i < cap; i++) {
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &i), ==, CS_SUCCESS, "Push back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d values to vector\n", cap);

    for (int i = 0; i < cap; i++) {
        UNITTEST_ASSERT_SILENT(vector_erase(v, 0), ==, CS_SUCCESS, "Erase at index 0 failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully erased %d values from vector\n", cap);

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

test_res test_vector_erase_deepfree(test_arg *arg) {
    int cap = 32;
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_test_struct_attr(), (vector_attr_t){8, 2}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with test_struct attributes\n");

    for (int i = 0; i < cap; i++) {
        test_struct ts = create_test_struct(i, "Test", i * 10.0);
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &ts), ==, CS_SUCCESS, "Push back failed");
        free_test_struct(&ts); // Free the original struct since vector should have made a deep copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d test_struct values to vector\n", cap);

    for (int i = 0; i < cap - 1; i++) {
        UNITTEST_ASSERT_SILENT(vector_erase(v, 0), ==, CS_SUCCESS, "Erase at index 0 failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully erased %d test_struct values from vector\n", cap - 1);

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// vector_replace
// ============================================================================
test_res test_vector_replace(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");

    UNITTEST_ASSERT(vector_replace(v, &val2, 0), ==, CS_SUCCESS, "Replace at index 0 failed", arg->logger,
        "Successfully replaced element at index 0\n");
    
    int *retrieved = (int*)vector_at(v, 0);
    UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index 0 after replace", arg->logger,
        "Successfully retrieved element at index 0 after replace\n");
    UNITTEST_ASSERT(*retrieved, ==, val2, "Retrieved value does not match expected value after replace", arg->logger,
        "Retrieved value matches expected value after replace\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_replace_deep(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_test_struct_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with test_struct attributes\n");

    test_struct ts1 = create_test_struct(1, "Test1", 10.0);
    test_struct ts2 = create_test_struct(2, "Test2", 20.0);
    UNITTEST_ASSERT(vector_push_back(v, &ts1), ==, CS_SUCCESS, "Push back of first struct failed", arg->logger,
        "Successfully pushed first test_struct to back of vector\n");
    free_test_struct(&ts1); // Free the original struct since vector should have made a deep copy

    UNITTEST_ASSERT(vector_replace(v, &ts2, 0), ==, CS_SUCCESS, "Replace at index 0 failed", arg->logger,
        "Successfully replaced element at index 0 with new test_struct\n");
    free_test_struct(&ts2); // Free the original struct since vector should have made a deep copy
    
    test_struct *retrieved = (test_struct*)vector_at(v, 0);
    UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index 0 after replace", arg->logger,
        "Successfully retrieved element at index 0 after replace\n");
    UNITTEST_ASSERT_SILENT(comp_test_struct_max(retrieved, &ts2), ==, 0, "Retrieved struct does not match expected struct after replace");

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully verified replaced test_struct element in vector\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// vector_at
// ============================================================================
test_res test_vector_at(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val = 42;
    UNITTEST_ASSERT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed", arg->logger,
        "Successfully pushed value to back of vector\n");

    int *retrieved = (int*)vector_at(v, 0);
    UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index 0", arg->logger,
        "Successfully retrieved element at index 0\n");
    UNITTEST_ASSERT(*retrieved, ==, val, "Retrieved value does not match inserted value", arg->logger,
        "Retrieved value matches inserted value\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_find
// ============================================================================
test_res test_vector_find(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val2), ==, CS_SUCCESS, "Push back of second value failed", arg->logger,
        "Successfully pushed second value to back of vector\n");

    size_t found_index = vector_find(v, &val2);
    UNITTEST_ASSERT(found_index, !=, vector_size(v), "Failed to find existing element", arg->logger,
        "Successfully found existing element in vector\n");
    UNITTEST_ASSERT(found_index, ==, 1, "Found index does not match expected index", arg->logger,
        "Found index matches expected index\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_find_no_comp(test_arg *arg) {
    elem_attr_t attr = get_int_attr();
    attr.comp = NULL; // Force vector_find to use memcmp for finding
    vector *v = UNITTEST_ASSERT(vector_init(NULL, attr, (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val2), ==, CS_SUCCESS, "Push back of second value failed", arg->logger,
        "Successfully pushed second value to back of vector\n");

    size_t found_index = vector_find(v, &val2);
    UNITTEST_ASSERT(found_index, !=, vector_size(v), "Failed to find existing element", arg->logger,
        "Successfully found existing element in vector\n");
    UNITTEST_ASSERT(found_index, ==, 1, "Found index does not match expected index", arg->logger,
        "Found index matches expected index\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_find_not_found(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");

    size_t found_index = vector_find(v, &val2);
    UNITTEST_ASSERT(found_index, ==, vector_size(v), "Finding non-existent element should return size of vector", arg->logger,
        "Finding non-existent element correctly returns size of vector\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_count
// ============================================================================
test_res test_vector_count(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val2), ==, CS_SUCCESS, "Push back of second value failed", arg->logger,
        "Successfully pushed second value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of duplicate value failed", arg->logger,
        "Successfully pushed duplicate value to back of vector\n");

    size_t count_val1 = vector_count(v, &val1);
    size_t count_val2 = vector_count(v, &val2);
    UNITTEST_ASSERT(count_val1, ==, 2, "Count for val1 does not match expected count", arg->logger,
        "Count for val1 matches expected count\n");
    UNITTEST_ASSERT(count_val2, ==, 1, "Count for val2 does not match expected count", arg->logger,
        "Count for val2 matches expected count\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_count_no_comp(test_arg *arg) {
    elem_attr_t attr = get_int_attr();
    attr.comp = NULL; // Force vector_count to use memcmp for counting
    vector *v = UNITTEST_ASSERT(vector_init(NULL, attr, (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val2), ==, CS_SUCCESS, "Push back of second value failed", arg->logger,
        "Successfully pushed second value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of duplicate value failed", arg->logger,
        "Successfully pushed duplicate value to back of vector\n");

    size_t count_val1 = vector_count(v, &val1);
    size_t count_val2 = vector_count(v, &val2);
    UNITTEST_ASSERT(count_val1, ==, 2, "Count for val1 does not match expected count", arg->logger,
        "Count for val1 matches expected count\n");
    UNITTEST_ASSERT(count_val2, ==, 1, "Count for val2 does not match expected count", arg->logger,
        "Count for val2 matches expected count\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_empty
// ============================================================================
test_res test_vector_empty(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    UNITTEST_ASSERT(vector_empty(v), ==, 1, "Newly initialized vector should be empty", arg->logger,
        "Newly initialized vector is empty\n");

    int val = 42;
    UNITTEST_ASSERT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed", arg->logger,
        "Successfully pushed value to back of vector\n");
    
    UNITTEST_ASSERT(vector_empty(v), ==, 0, "Vector should not be empty after adding an element", arg->logger,
        "Vector is not empty after adding an element\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_size
// ============================================================================
test_res test_vector_size(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    UNITTEST_ASSERT(vector_size(v), ==, 0, "Newly initialized vector should have size 0", arg->logger,
        "Newly initialized vector has size 0\n");

    int val = 42;
    UNITTEST_ASSERT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed", arg->logger,
        "Successfully pushed value to back of vector\n");
    
    UNITTEST_ASSERT(vector_size(v), ==, 1, "Vector size should be 1 after adding an element", arg->logger,
        "Vector size is correct after adding an element\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_swap
// ============================================================================
test_res test_vector_swap(test_arg *arg) {
    vector *v1 = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");
    vector *v2 = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v1, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v2, &val2), ==, CS_SUCCESS, "Push back of second value failed", arg->logger,
        "Successfully pushed second value to back of vector\n");

    vector_swap(v1, v2);
    
    int *retrieved0 = (int*)vector_at(v1, 0);
    int *retrieved1 = (int*)vector_at(v2, 0);
    UNITTEST_ASSERT(retrieved0, !=, NULL, "Failed to retrieve element at index 0 of v1 after swap", arg->logger,
        "Successfully retrieved element at index 0 of v1 after swap\n");
    UNITTEST_ASSERT(retrieved1, !=, NULL, "Failed to retrieve element at index 0 of v2 after swap", arg->logger,
        "Successfully retrieved element at index 0 of v2 after swap\n");
    UNITTEST_ASSERT(*retrieved0, ==, val2, "Element at index 0 of v1 does not match expected value after swap", arg->logger,
        "Element at index 0 matches expected value of v1 after swap\n");
    UNITTEST_ASSERT(*retrieved1, ==, val1, "Element at index 0 of v2 does not match expected value after swap", arg->logger,
        "Element at index 0 matches expected value of v2 after swap\n");

    vector_free(v1);
    vector_free(v2);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_clear
// ============================================================================
test_res test_vector_clear(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(vector_push_back(v, &val1), ==, CS_SUCCESS, "Push back of first value failed", arg->logger,
        "Successfully pushed first value to back of vector\n");
    UNITTEST_ASSERT(vector_push_back(v, &val2), ==, CS_SUCCESS, "Push back of second value failed", arg->logger,
        "Successfully pushed second value to back of vector\n");

    vector_clear(v);
    
    UNITTEST_ASSERT(vector_size(v), ==, 0, "Vector size should be 0 after clear", arg->logger,
        "Vector size is correct after clear\n");
    UNITTEST_ASSERT(vector_empty(v), ==, 1, "Vector should be empty after clear", arg->logger,
        "Vector is empty after clear\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_clear_deep(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_test_struct_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with test_struct attributes\n");

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "Test", i * 10.0);
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &ts), ==, CS_SUCCESS, "Push back failed");
        free_test_struct(&ts); // Free the original struct since vector should have made a deep copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed 10 test_struct values to vector\n");

    vector_clear(v);
    
    UNITTEST_ASSERT(vector_size(v), ==, 0, "Vector size should be 0 after clear", arg->logger,
        "Vector size is correct after clear\n");
    UNITTEST_ASSERT(vector_empty(v), ==, 1, "Vector should be empty after clear", arg->logger,
        "Vector is empty after clear\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_sort
// ============================================================================
test_res test_vector_sort(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int vals[] = {84, 42, 126};
    int sorted_vals[] = {42, 84, 126};
    for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); i++) {
        UNITTEST_ASSERT(vector_push_back(v, &vals[i]), ==, CS_SUCCESS, "Push back failed", arg->logger,
            "Successfully pushed value to back of vector\n");
    }

    vector_sort(v);
    
    for (size_t i = 0; i < sizeof(sorted_vals)/sizeof(sorted_vals[0]); i++) {
        int *retrieved = (int*)vector_at(v, i);
        UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index after sort", arg->logger,
            "Successfully retrieved element at index after sort\n");
        UNITTEST_ASSERT(*retrieved, ==, sorted_vals[i], "Element at index does not match expected sorted value", arg->logger,
            "Element at index matches expected sorted value\n");
    }

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_sort_no_comp(test_arg *arg) {
    elem_attr_t attr = get_int_attr();
    attr.comp = NULL; // Force vector_sort to use qsort with memcmp for sorting
    vector *v = UNITTEST_ASSERT(vector_init(NULL, attr, (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int vals[] = {84, 42, 126};
    int sorted_vals[] = {42, 84, 126};
    for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); i++) {
        UNITTEST_ASSERT(vector_push_back(v, &vals[i]), ==, CS_SUCCESS, "Push back failed", arg->logger,
            "Successfully pushed value to back of vector\n");
    }

    vector_sort(v);
    
    for (size_t i = 0; i < sizeof(sorted_vals)/sizeof(sorted_vals[0]); i++) {
        int *retrieved = (int*)vector_at(v, i);
        UNITTEST_ASSERT(retrieved, !=, NULL, "Failed to retrieve element at index after sort", arg->logger,
            "Successfully retrieved element at index after sort\n");
        UNITTEST_ASSERT(*retrieved, ==, sorted_vals[i], "Element at index does not match expected sorted value", arg->logger,
            "Element at index matches expected sorted value\n");
    }

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_print
// ============================================================================
test_res test_vector_print(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    int vals[] = {42, 84, 126};
    for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); i++) {
        UNITTEST_ASSERT(vector_push_back(v, &vals[i]), ==, CS_SUCCESS, "Push back failed", arg->logger,
            "Successfully pushed value to back of vector\n");
    }

    // capture the output of vector_print
    char buffer[256];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for capturing output", arg->logger,
        "Successfully opened memory stream for capturing output\n");
    vector_print(stream, v);
    fprintf(stream, "\n"); // Add newline to ensure output is flushed to buffer
    fclose(stream);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Captured output of vector_print:%s\n", buffer);
    
    // Check if the output contains the expected values (this is a simple check, you can enhance it as needed)
    for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); i++) {
        char val_str[16];
        snprintf(val_str, sizeof(val_str), "%d", vals[i]);
        UNITTEST_ASSERT_SILENT(strstr(buffer, val_str), !=, NULL, "Output does not contain expected value");
    }

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_reserve
// ============================================================================
test_res test_vector_reserve(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){32, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    size_t reserve_size = 100;
    UNITTEST_ASSERT(vector_reserve(v, reserve_size), ==, CS_SUCCESS, "Vector reserve failed", arg->logger,
        "Successfully reserved capacity for %zu elements\n", reserve_size);
    
    // We can't directly check the internal capacity, but we can check that we can insert up to the reserved size without failure
    for (size_t i = 0; i < reserve_size; i++) {
        int val = (int)i;
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed after reserve");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %zu values to vector after reserve\n", reserve_size);

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_reserve_small_cap(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){0, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    size_t reserve_size = 100;
    UNITTEST_ASSERT(vector_reserve(v, reserve_size), ==, CS_SUCCESS, "Vector reserve failed", arg->logger,
        "Successfully reserved capacity for %zu elements\n", reserve_size);
    
    // We can't directly check the internal capacity, but we can check that we can insert up to the reserved size without failure
    for (size_t i = 0; i < reserve_size; i++) {
        int val = (int)i;
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed after reserve");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %zu values to vector after reserve\n", reserve_size);

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// vector_shrink_to_fit
// ============================================================================
test_res test_vector_shrink_to_fit(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){64, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    for (int i = 0; i < 32; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed 32 values to vector\n");

    UNITTEST_ASSERT(vector_shrink_to_fit(v), ==, CS_SUCCESS, "Shrink to fit failed on empty vector", arg->logger,
        "Successfully shrunk empty vector to fit\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

test_res test_vector_shrink_to_fit_size_equal_cap(test_arg *arg) {
    vector *v = UNITTEST_ASSERT(vector_init(NULL, get_int_attr(), (vector_attr_t){32, 0}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");

    for (int i = 0; i < 32; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(vector_push_back(v, &val), ==, CS_SUCCESS, "Push back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed 32 values to vector\n");

    UNITTEST_ASSERT(vector_shrink_to_fit(v), ==, CS_SUCCESS, "Shrink to fit failed when size equals capacity", arg->logger,
        "Successfully shrunk vector to fit when size equals capacity\n");

    vector_free(v);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// Stress test with timing
// ============================================================================
test_res test_vector_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Valgrind detected - skipping stress timing test\n");
        return (test_res){(char*)__func__, "Valgrind active - skipping stress test", CS_SUCCESS};
    }

    elem_attr_t attr = get_int_attr();
    attr.comp = NULL;
    vector *v = UNITTEST_ASSERT(vector_init(NULL, attr, (vector_attr_t){0, 1}), !=, NULL, "Vector initialization failed",
        arg->logger, "Successfully initialized vector with int attributes\n");
    struct timeval start, end;
    double elapsed;

    if (v == NULL) {
        return (test_res){(char*)__func__, "Vector initialization failed", CS_UNKNOWN};
    }
    int total = __VECTOR_STRESS_TEST_SIZE;

    /* INSERT timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        vector_push_back(v, &i); // We won't check the return value here to avoid overhead of assertions in the loop
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Insert Time = %.9f sec\n", elapsed);

    /* FIND timing */
    gettimeofday(&start, NULL);
    int search_val = total - 1;
    vector_find(v, &search_val); // Call once to warm up any caches
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Find Time = %.9f sec\n", elapsed);

    gettimeofday(&start, NULL);
    vector_set_comp(v, comp_int_max); // Set the comparator for sorting
    vector_sort(v);
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "sort", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Sort Time = %.9f sec\n", elapsed);

    /* DELETE timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        vector_pop_back(v); // We won't check the return value here to avoid overhead of assertions in the loop
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "delete", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Delete Time = %.9f sec\n", elapsed);

    vector_free(v);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test vector_tests[] = {
    // vector_init
    test_vector_init,

    // vector_push_back
    test_vector_push_back,
    test_vector_push_back_grow,
    test_vector_push_back_deepcopy,

    // vector_insert_at
    test_vector_insert_at,
    test_vector_insert_at_grow,
    test_vector_insert_at_deepcopy,

    // vector_pop_back
    test_vector_pop_back,
    test_vector_pop_back_shrink,
    test_vector_pop_back_deepfree,

    // vector_erase
    test_vector_erase,
    test_vector_erase_shrink,
    test_vector_erase_deepfree,

    // vector_replace
    test_vector_replace,
    test_vector_replace_deep,

    // vector_at
    test_vector_at,

    // vector_find
    test_vector_find,
    test_vector_find_no_comp,
    test_vector_find_not_found,

    // vector_count
    test_vector_count,
    test_vector_count_no_comp,

    // vector_reserve
    test_vector_reserve,
    test_vector_reserve_small_cap,

    // vector_shrink_to_fit
    test_vector_shrink_to_fit,
    test_vector_shrink_to_fit_size_equal_cap,

    // vector_empty
    test_vector_empty,

    // vector_size
    test_vector_size,

    // vector_swap
    test_vector_swap,

    // vector_clear
    test_vector_clear,
    test_vector_clear_deep,

    // vector_sort
    test_vector_sort,
    test_vector_sort_no_comp,

    // vector_print
    test_vector_print,

    // Timing test
    test_vector_stress_time,
};