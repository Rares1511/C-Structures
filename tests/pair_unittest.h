#include <cs/pair.h>
#include <unittest.h>   

// ============================================================================
// pair_init
// ============================================================================
test_res test_pair_init(test_arg *arg) {
    elem_attr_t int_attr = get_int_attr();
    elem_attr_t double_attr = get_double_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&int_attr, &double_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with int and double attributes\n");

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// pair_set
// ============================================================================
test_res test_pair_set(test_arg *arg) {
    elem_attr_t int_attr = get_int_attr();
    elem_attr_t double_attr = get_double_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&int_attr, &double_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with int and double attributes\n");

    int first_val = 42;
    double second_val = 3.14;
    UNITTEST_ASSERT(pair_set(p, &first_val, &second_val), ==, CS_SUCCESS, "Failed to set pair values",
        arg->logger, "Successfully set pair values: first=%d, second=%.2f\n", first_val, second_val);

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
};

test_res test_pair_set_null(test_arg *arg) {
    elem_attr_t int_attr = get_int_attr();
    elem_attr_t double_attr = get_double_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&int_attr, &double_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with int and double attributes\n");

    UNITTEST_ASSERT(pair_set(p, NULL, NULL), ==, CS_ELEM, "pair_set should fail when both first and second are NULL",
        arg->logger, "pair_set correctly failed when both first and second are NULL\n");

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
};

test_res test_pair_set_null_first(test_arg *arg) {
    elem_attr_t int_attr = get_int_attr();
    elem_attr_t double_attr = get_double_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&int_attr, &double_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with int and double attributes\n");

    double second_val = 3.14;
    UNITTEST_ASSERT(pair_set(p, NULL, &second_val), ==, CS_SUCCESS, "pair_set should succeed when first is NULL but second is not",
        arg->logger, "pair_set correctly succeeded when first is NULL but second is not\n");

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
};

test_res test_pair_set_null_second(test_arg *arg) {
    elem_attr_t int_attr = get_int_attr();
    elem_attr_t double_attr = get_double_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&int_attr, &double_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with int and double attributes\n");

    int first_val = 42;
    UNITTEST_ASSERT(pair_set(p, &first_val, NULL), ==, CS_SUCCESS, "pair_set should succeed when second is NULL but first is not",
        arg->logger, "pair_set correctly succeeded when second is NULL but first is not\n");

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
};

test_res test_pair_set_deepcopy(test_arg *arg) {
    elem_attr_t first_attr = get_test_struct_attr();
    elem_attr_t second_attr = get_test_struct_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&first_attr, &second_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with test_struct attributes\n");

    test_struct first_val = create_test_struct(1, "Test Name", 99.9);
    test_struct second_val = create_test_struct(2, "Another Name", 88.8);
    UNITTEST_ASSERT(pair_set(p, &first_val, &second_val), ==, CS_SUCCESS, "Failed to set pair values with test_struct", 
        arg->logger, "Successfully set pair values with test_struct\n");
    free_test_struct(&first_val); // Free the original struct since pair should have made a deep copy
    free_test_struct(&second_val); // Free the original struct since pair should have made a deep copy

    test_struct *retrieved_first = UNITTEST_ASSERT(pair_first(p), !=, NULL, "Failed to retrieve first element after setting test_struct", 
        arg->logger, "Successfully retrieved first element after setting test_struct\n");
    test_struct *retrieved_second = UNITTEST_ASSERT(pair_second(p), !=, NULL, "Failed to retrieve second element after setting test_struct", 
        arg->logger, "Successfully retrieved second element after setting test_struct\n");

    UNITTEST_ASSERT(comp_test_struct_max(retrieved_first, &first_val), ==, 0, "Retrieved first test_struct does not match expected", 
        arg->logger, "Retrieved first test_struct matches expected\n");
    UNITTEST_ASSERT(comp_test_struct_max(retrieved_second, &second_val), ==, 0, "Retrieved second test_struct does not match expected", 
        arg->logger, "Retrieved second test_struct matches expected\n");

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
}

test_res test_pair_set_deepfree(test_arg *arg) {
    elem_attr_t first_attr = get_test_struct_attr();
    elem_attr_t second_attr = get_test_struct_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&first_attr, &second_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with test_struct attributes\n");

    test_struct first_val = create_test_struct(1, "Test Name", 99.9);
    test_struct second_val = create_test_struct(2, "Another Name", 88.8);
    UNITTEST_ASSERT(pair_set(p, &first_val, &second_val), ==, CS_SUCCESS, "Failed to set pair values with test_struct", 
        arg->logger, "Successfully set pair values with test_struct\n");
    free_test_struct(&first_val); // Free the original struct since pair should have made a deep copy
    free_test_struct(&second_val); // Free the original struct since pair should have made a deep copy

    test_struct other_first_val = create_test_struct(3, "Other Name", 77.7);
    test_struct other_second_val = create_test_struct(4, "Yet Another Name", 66.6);
    UNITTEST_ASSERT(pair_set(p, &other_first_val, &other_second_val), ==, CS_SUCCESS, "Failed to overwrite pair values with test_struct", 
        arg->logger, "Successfully overwrote pair values with test_struct\n");
    free_test_struct(&other_first_val); // Free the original struct since pair should have made a deep copy
    free_test_struct(&other_second_val); // Free the original struct since pair should have made a deep copy

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// pair_first and pair_second
// ============================================================================
test_res test_pair_first_second(test_arg *arg) {
    elem_attr_t int_attr = get_int_attr();
    elem_attr_t double_attr = get_double_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&int_attr, &double_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with int and double attributes\n");

    int first_val = 42;
    double second_val = 3.14;
    UNITTEST_ASSERT(pair_set(p, &first_val, &second_val), ==, CS_SUCCESS, "Failed to set pair values",
        arg->logger, "Successfully set pair values: first=%d, second=%.2f\n", first_val, second_val);

    int *retrieved_first = (int*)pair_first(p);
    double *retrieved_second = (double*)pair_second(p);
    UNITTEST_ASSERT(retrieved_first, !=, NULL, "Failed to retrieve first element", arg->logger, "Successfully retrieved first element\n");
    UNITTEST_ASSERT(retrieved_second, !=, NULL, "Failed to retrieve second element", arg->logger, "Successfully retrieved second element\n");
    UNITTEST_ASSERT(*retrieved_first, ==, first_val, "Retrieved first value does not match expected", 
        arg->logger, "Retrieved first value matches expected: %d\n", *retrieved_first);
    UNITTEST_ASSERT(*retrieved_second, ==, second_val, "Retrieved second value does not match expected", 
        arg->logger, "Retrieved second value matches expected: %.2f\n", *retrieved_second);

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// pair_print
// ============================================================================
test_res test_pair_print(test_arg *arg) {
    elem_attr_t int_attr = get_int_attr();
    elem_attr_t double_attr = get_double_attr();
    pair *p = UNITTEST_ASSERT(pair_init(&int_attr, &double_attr), !=, NULL, "Pair initialization failed",
        arg->logger, "Successfully initialized pair with int and double attributes\n");

    int first_val = 42;
    double second_val = 3.14;
    UNITTEST_ASSERT(pair_set(p, &first_val, &second_val), ==, CS_SUCCESS, "Failed to set pair values",
        arg->logger, "Successfully set pair values: first=%d, second=%.2f\n", first_val, second_val);

    char buffer[256];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open log file for writing", arg->logger, "Successfully opened log file for writing\n");
    pair_print(stream, p);
    fclose(stream);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Pair print output: %s\n", buffer);

    UNITTEST_ASSERT(strstr(buffer, "42"), !=, NULL, "Pair print output does not contain expected first value", 
        arg->logger, "Pair print output contains expected first value\n");
    UNITTEST_ASSERT(strstr(buffer, "3.14"), !=, NULL, "Pair print output does not contain expected second value", 
        arg->logger, "Pair print output contains expected second value\n");

    pair_free(p);
    return SUCCESSFUL_TEST_RES;
};

test pair_tests[] = {
    // pair_init
    test_pair_init,

    // pair_set
    test_pair_set,
    test_pair_set_null,
    test_pair_set_null_first,
    test_pair_set_null_second,
    test_pair_set_deepcopy,
    test_pair_set_deepfree,

    // pair_first and pair_second
    test_pair_first_second,

    // pair_print
    test_pair_print,
};
