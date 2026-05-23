#include <cs/unordered_set.h>

#include <unittest.h>

// ============================================================================
// unordered_set_init
// ============================================================================
test_res test_unordered_set_init(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set",
        arg->logger, "Successfully initialized unordered set\n");

    UNITTEST_ASSERT(uset->ht, !=, NULL, "Hash table inside unordered set is NULL after initialization",
        arg->logger, "Hash table inside unordered set is properly initialized\n");

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_set_add_entry
// ============================================================================
test_res test_unordered_set_add_entry(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for add_entry test",
        arg->logger, "Successfully initialized unordered set for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set");
        void *found = UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset, &i), !=, NULL, "Failed to find added entry in unordered set");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match the added key in unordered set");
    }

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_set_add_entry_no_comp(test_arg *arg) {
    elem_attr_t no_comp_attr = get_int_attr();
    no_comp_attr.comp = NULL; // Remove the compare function to test default byte-wise comparison
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, no_comp_attr, hash_int), !=, NULL, "Failed to initialize unordered set for add_entry test",
        arg->logger, "Successfully initialized unordered set for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set");
        void *found = UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset, &i), !=, NULL, "Failed to find added entry in unordered set");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match the added key in unordered set");
    }

    int duplicate = 500; // Attempt to add a duplicate entry
    UNITTEST_ASSERT(unordered_set_add_entry(uset, &duplicate), !=, CS_SUCCESS, "Failed to add duplicate entry to unordered set",
        arg->logger, "Properly handled attempt to add duplicate entry to unordered set\n");

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_set_add_entry_deepcopy(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered set with NULL hash function",
        arg->logger, "Successfully initialized unordered set with NULL hash function\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered set");
        void *found = UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset, &ts), !=, NULL, "Failed to find added entry in unordered set");
        UNITTEST_ASSERT_SILENT(comp_test_struct_max(found, &ts), ==, 0, "Found entry does not match the added key in unordered set");
        free_test_struct(&ts); // Free the original struct since the unordered set should have made a deep copy
    }

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_set_add_entry_tombstone(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for add_entry tombstone test",
        arg->logger, "Successfully initialized unordered set for add_entry tombstone test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set");
    }

    // Remove some entries to create tombstones
    for (int i = 0; i < total; i += 2) {
        UNITTEST_ASSERT_SILENT(unordered_set_remove_entry(uset, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered set");
    }

    // Now add new entries that should reuse the tombstone slots
    for (int i = 0; i < total; i+=2) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set after creating tombstones");
        void *found = UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset, &i), !=, NULL, "Failed to find added entry in unordered set after creating tombstones");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match the added key in unordered set after creating tombstones");
    }

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_set_remove_entry
// ============================================================================
test_res test_unordered_set_remove_entry(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for remove_entry test",
        arg->logger, "Successfully initialized unordered set for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set for remove_entry test");
    }

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_remove_entry(uset, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered set");
        UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset, &i), ==, NULL, "Entry still found in unordered set after removal");
    }

    int non_existent = 2000; // Attempt to remove a non-existent entry
    UNITTEST_ASSERT(unordered_set_remove_entry(uset, &non_existent), !=, CS_SUCCESS, "Failed to handle removal of non-existent entry from unordered set",
        arg->logger, "Properly handled attempt to remove non-existent entry from unordered set\n");

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_set_remove_entry_deepfree(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered set with NULL hash function for remove_entry test",
        arg->logger, "Successfully initialized unordered set with NULL hash function for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered set for remove_entry test");
        free_test_struct(&ts); // Free the original struct since the unordered set should have made a deep copy
    }

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_set_remove_entry(uset, &ts), ==, CS_SUCCESS, "Failed to remove entry from unordered set");
        UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset, &ts), ==, NULL, "Entry still found in unordered set after removal");
        free_test_struct(&ts); // Free the struct used for removal
    }

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_set_remove_entry_hits(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int_mod), !=, NULL, "Failed to initialize unordered set with mod hash function for add_entry hits test",
        arg->logger, "Successfully initialized unordered set with mod hash function for add_entry hits test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set");
    }

    // Check that all entries are present despite the hash collisions
    for (int i = total - 1; i >= 0; i--) {
        UNITTEST_ASSERT_SILENT(unordered_set_remove_entry(uset, &i), ==, CS_SUCCESS, "Failed to find added entry in unordered set with hash collisions");
    }

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_set_count
// ============================================================================
test_res test_unordered_set_count(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for count test",
        arg->logger, "Successfully initialized unordered set for count test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set for count test");
    }

    for (int i = 0; i < total; i++) {
        int count = UNITTEST_ASSERT_SILENT(unordered_set_count(uset, &i), ==, 1, "Count for existing entry in unordered set should be 1");
        UNITTEST_ASSERT_SILENT(count, ==, 1, "Count for existing entry in unordered set should be 1");
    }

    int non_existent = 2000; // Count for a non-existent entry
    UNITTEST_ASSERT(unordered_set_count(uset, &non_existent), ==, 0, "Count for non-existent entry in unordered set should be 0",
        arg->logger, "Properly returned count of 0 for non-existent entry in unordered set\n");

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_set_empty
// ============================================================================
test_res test_unordered_set_empty(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for empty test",
        arg->logger, "Successfully initialized unordered set for empty test\n");

    UNITTEST_ASSERT(unordered_set_empty(uset), ==, 1, "Newly initialized unordered set should be empty", 
        arg->logger, "Newly initialized unordered set is correctly reported as empty\n");

    int val = 42;
    UNITTEST_ASSERT(unordered_set_add_entry(uset, &val), ==, CS_SUCCESS, "Failed to add entry to unordered set for empty test", 
        arg->logger, "Successfully added entry to unordered set for empty test\n");

    UNITTEST_ASSERT(unordered_set_empty(uset), ==, 0, "Unordered set with entries should not be empty", 
        arg->logger, "Unordered set with entries is correctly reported as not empty\n");

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_set_size
// ============================================================================
test_res test_unordered_set_size(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for size test",
        arg->logger, "Successfully initialized unordered set for size test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set for size test");
        UNITTEST_ASSERT_SILENT(unordered_set_size(uset), ==, (size_t)(i + 1), "Size of unordered set does not match number of added entries");
    }

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_set_clear
// ============================================================================
test_res test_unordered_set_clear(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for clear test",
        arg->logger, "Successfully initialized unordered set for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set for clear test");
    }

    unordered_set_clear(uset);
    UNITTEST_ASSERT(unordered_set_empty(uset), ==, 1, "Unordered set should be empty after clear", 
        arg->logger, "Unordered set is correctly reported as empty after clear\n");

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_set_clear_deepfree(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered set with NULL hash function for clear test",
        arg->logger, "Successfully initialized unordered set with NULL hash function for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered set for clear test");
        free_test_struct(&ts); // Free the original struct since the unordered set should have made a deep copy
    }

    unordered_set_clear(uset);
    UNITTEST_ASSERT(unordered_set_empty(uset), ==, 1, "Unordered set should be empty after clear", 
        arg->logger, "Unordered set is correctly reported as empty after clear\n");

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_set_swap
// ============================================================================
test_res test_unordered_set_swap(test_arg *arg) {
    unordered_set *uset1 = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize first unordered set for swap test",
        arg->logger, "Successfully initialized first unordered set for swap test\n");
    unordered_set *uset2 = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize second unordered set for swap test",
        arg->logger, "Successfully initialized second unordered set for swap test\n");

    int total1 = 1000, total2 = 500;
    for (int i = 0; i < total1; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset1, &i), ==, CS_SUCCESS, "Failed to add entry to first unordered set for swap test");
    }
    for (int i = 0; i < total2; i++) {
        int val = 10000 + i; // Use different values to distinguish between the two sets
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset2, &val), ==, CS_SUCCESS, "Failed to add entry to second unordered set for swap test");
    }

    unordered_set_swap(uset1, uset2);

    UNITTEST_ASSERT(unordered_set_size(uset1), ==, (size_t)total2, "First unordered set size does not match expected size after swap", 
        arg->logger, "First unordered set size matches expected size after swap\n");
    UNITTEST_ASSERT(unordered_set_size(uset2), ==, (size_t)total1, "Second unordered set size does not match expected size after swap", 
        arg->logger, "Second unordered set size matches expected size after swap\n");

    for (int i = 0; i < total1; i++) {
        void *found = UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset2, &i), !=, NULL, "Failed to find entry in second unordered set after swap");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match expected key in second unordered set after swap");
    }

    for (int i = 0; i < total2; i++) {
        int val = 10000 + i;
        void *found = UNITTEST_ASSERT_SILENT(unordered_set_get_entry(uset1, &val), !=, NULL, "Failed to find entry in first unordered set after swap");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, val, "Found entry does not match expected key in first unordered set after swap");
    }

    unordered_set_free(uset1);
    unordered_set_free(uset2);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_set_print
// ============================================================================
test_res test_unordered_set_print(test_arg *arg) {
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for print test",
        arg->logger, "Successfully initialized unordered set for print test\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_set_add_entry(uset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered set for print test");
    }
    int remove_val = 5;
    UNITTEST_ASSERT(unordered_set_remove_entry(uset, &remove_val), ==, CS_SUCCESS, "Failed to remove entry from unordered set for print test", 
        arg->logger, "Successfully removed entry from unordered set for print test\n");

    char buffer[1024];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    unordered_set_print(stream, uset);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Unordered set printed output:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected[16];
        snprintf(expected, sizeof(expected), "%d", i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Printed output does not contain expected entry from unordered set");
    }

    unordered_set_free(uset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// Stress test with timing
// ============================================================================
test_res test_unordered_set_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        return SUCCESSFUL_TEST_RES;
    }
    
    unordered_set *uset = UNITTEST_ASSERT(unordered_set_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered set for stress test",
        arg->logger, "Successfully initialized unordered set for stress test\n");
    struct timeval start, end;
    double elapsed;

    int total = __UNORDERED_SET_STRESS_TEST_SIZE;

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_set_add_entry(uset, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Insert Time = %.9f sec\n", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_set_get_entry(uset, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Find Time = %.9f sec\n", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_set_remove_entry(uset, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "delete", elapsed);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Erase Time = %.9f sec\n", elapsed);

    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Test registry
// ============================================================================
test unordered_set_tests[] = {
    // unordered_set_init
    test_unordered_set_init,

    // unordered_set_add_entry
    test_unordered_set_add_entry,
    test_unordered_set_add_entry_deepcopy,
    test_unordered_set_add_entry_no_comp,
    test_unordered_set_add_entry_tombstone,

    // unordered_set_remove_entry
    test_unordered_set_remove_entry,
    test_unordered_set_remove_entry_deepfree,
    test_unordered_set_remove_entry_hits,

    // unordered_set_count
    test_unordered_set_count,

    // unordered_set_empty
    test_unordered_set_empty,

    // unordered_set_size
    test_unordered_set_size,

    // unordered_set_clear
    test_unordered_set_clear,
    test_unordered_set_clear_deepfree,

    // unordered_set_swap
    test_unordered_set_swap,

    // unordered_set_print
    test_unordered_set_print,

    // Stress tests with timing
    test_unordered_set_stress_time,
};