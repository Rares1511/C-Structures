#include <cs/unordered_multiset.h>

#include <unittest.h>

// ============================================================================
// unordered_multiset_init
// ============================================================================
test_res test_unordered_multiset_init(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset",
        arg->logger, "Successfully initialized unordered multiset\n");

    UNITTEST_ASSERT(umset->ht, !=, NULL, "Hash table inside unordered multiset is NULL after initialization",
        arg->logger, "Hash table inside unordered multiset is properly initialized\n");

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multiset_add_entry
// ============================================================================
test_res test_unordered_multiset_add_entry(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for add_entry test",
        arg->logger, "Successfully initialized unordered multiset for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset");
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &i), ==, 1, "Failed to find added entry in unordered multiset");
    }

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multiset_add_entry_no_comp(test_arg *arg) {
    elem_attr_t no_comp_attr = get_int_attr();
    no_comp_attr.comp = NULL; // Remove the compare function to test default byte-wise comparison
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, no_comp_attr, hash_int), !=, NULL, "Failed to initialize unordered multiset for add_entry test",
        arg->logger, "Successfully initialized unordered multiset for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset");
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &i), ==, 1, "Failed to find added entry in unordered multiset");
    }

    int duplicate = 500; // Attempt to add a duplicate entry
    UNITTEST_ASSERT(unordered_multiset_add_entry(umset, &duplicate), ==, CS_SUCCESS, "Failed to add duplicate entry to unordered multiset",
        arg->logger, "Properly handled attempt to add duplicate entry to unordered multiset\n");
    UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &duplicate), ==, 2, "Count for duplicate entry in unordered multiset should be 2");

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multiset_add_entry_deepcopy(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered multiset with NULL hash function",
        arg->logger, "Successfully initialized unordered multiset with NULL hash function\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered multiset");
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &ts), ==, 1, "Failed to find added entry in unordered multiset");
        free_test_struct(&ts); // Free the original struct since the unordered multiset should have made a deep copy
    }

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multiset_add_entry_tombstone(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for add_entry tombstone test",
        arg->logger, "Successfully initialized unordered multiset for add_entry tombstone test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset");
    }

    // Remove some entries to create tombstones
    for (int i = 0; i < total; i += 2) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_remove_entry(umset, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered multiset to create tombstone");
    }

    // Now add new entries that should reuse the tombstone slots
    for (int i = 0; i < total; i+=2) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset after creating tombstones");
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &i), ==, 1, "Failed to find added entry in unordered multiset after creating tombstones");
    }

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_multiset_remove_entry
// ============================================================================
test_res test_unordered_multiset_remove_entry(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for remove_entry test",
        arg->logger, "Successfully initialized unordered multiset for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for remove_entry test");
    }

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_remove_entry(umset, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered multiset");
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &i), ==, 0, "Entry still found in unordered multiset after removal");
    }

    int non_existent = 2000; // Attempt to remove a non-existent entry
    UNITTEST_ASSERT(unordered_multiset_remove_entry(umset, &non_existent), !=, CS_SUCCESS, "Failed to handle removal of non-existent entry from unordered multiset",
        arg->logger, "Properly handled attempt to remove non-existent entry from unordered multiset\n");

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multiset_remove_entry_deepfree(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered multiset with NULL hash function for remove_entry test",
        arg->logger, "Successfully initialized unordered multiset with NULL hash function for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for remove_entry test");
        free_test_struct(&ts); // Free the original struct since the unordered multiset should have made a deep copy
    }

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multiset_remove_entry(umset, &ts), ==, CS_SUCCESS, "Failed to remove entry from unordered multiset");
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &ts), ==, 0, "Entry still found in unordered multiset after removal");
        free_test_struct(&ts); // Free the struct used for removal
    }

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multiset_remove_entry_hits(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int_mod), !=, NULL, "Failed to initialize unordered multiset with mod hash function for add_entry hits test",
        arg->logger, "Successfully initialized unordered multiset with mod hash function for add_entry hits test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for add_entry hits test");
    }

    // Check that all entries are present despite the hash collisions
    for (int i = total - 1; i >= 0; i--) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_remove_entry(umset, &i), ==, CS_SUCCESS, "Failed to find added entry in unordered multiset with hash collisions");
    }

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_multiset_count
// ============================================================================
test_res test_unordered_multiset_count(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for count test",
        arg->logger, "Successfully initialized unordered multiset for count test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for count test");
    }

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset, &i), ==, 1, "Count for existing entry in unordered multiset should be 1");
    }

    int non_existent = 2000; // Count for a non-existent entry
    UNITTEST_ASSERT(unordered_multiset_count(umset, &non_existent), ==, 0, "Count for non-existent entry in unordered multiset should be 0",
        arg->logger, "Properly returned count of 0 for non-existent entry in unordered multiset\n");

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multiset_empty
// ============================================================================
test_res test_unordered_multiset_empty(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for empty test",
        arg->logger, "Successfully initialized unordered multiset for empty test\n");

    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 1, "Newly initialized unordered multiset should be empty", 
        arg->logger, "Newly initialized unordered multiset is correctly reported as empty\n");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_add_entry(umset, &val), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for empty test", 
        arg->logger, "Successfully added entry to unordered multiset for empty test\n");

    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 0, "Unordered multiset with entries should not be empty", 
        arg->logger, "Unordered multiset with entries is correctly reported as not empty\n");

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multiset_size
// ============================================================================
test_res test_unordered_multiset_size(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for size test",
        arg->logger, "Successfully initialized unordered multiset for size test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for size test");
        UNITTEST_ASSERT_SILENT(unordered_multiset_size(umset), ==, (size_t)(i + 1), "Size of unordered multiset does not match number of added entries");
    }

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multiset_clear
// ============================================================================
test_res test_unordered_multiset_clear(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for clear test",
        arg->logger, "Successfully initialized unordered multiset for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for clear test");
    }

    unordered_multiset_clear(umset);
    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 1, "Unordered multiset should be empty after clear", 
        arg->logger, "Unordered multiset is correctly reported as empty after clear\n");

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multiset_clear_deepfree(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered multiset with NULL hash function for clear test",
        arg->logger, "Successfully initialized unordered multiset with NULL hash function for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for clear test");
        free_test_struct(&ts); // Free the original struct since the unordered multiset should have made a deep copy
    }

    unordered_multiset_clear(umset);
    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 1, "Unordered multiset should be empty after clear", 
        arg->logger, "Unordered multiset is correctly reported as empty after clear\n");

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multiset_swap
// ============================================================================
test_res test_unordered_multiset_swap(test_arg *arg) {
    unordered_multiset *umset1 = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize first unordered multiset for swap test",
        arg->logger, "Successfully initialized first unordered multiset for swap test\n");
    unordered_multiset *umset2 = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize second unordered multiset for swap test",
        arg->logger, "Successfully initialized second unordered multiset for swap test\n");

    int total1 = 1000, total2 = 500;
    for (int i = 0; i < total1; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset1, &i), ==, CS_SUCCESS, "Failed to add entry to first unordered multiset for swap test");
    }
    for (int i = 0; i < total2; i++) {
        int val = 10000 + i; // Use different values to distinguish between the two sets
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset2, &val), ==, CS_SUCCESS, "Failed to add entry to second unordered multiset for swap test");
    }

    unordered_multiset_swap(umset1, umset2);

    UNITTEST_ASSERT(unordered_multiset_size(umset1), ==, (size_t)total2, "First unordered multiset size does not match expected size after swap", 
        arg->logger, "First unordered multiset size matches expected size after swap\n");
    UNITTEST_ASSERT(unordered_multiset_size(umset2), ==, (size_t)total1, "Second unordered multiset size does not match expected size after swap", 
        arg->logger, "Second unordered multiset size matches expected size after swap\n");

    for (int i = 0; i < total1; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset2, &i), ==, 1, "Failed to find entry in second unordered multiset after swap");
    }

    for (int i = 0; i < total2; i++) {
        int val = 10000 + i;
        UNITTEST_ASSERT_SILENT(unordered_multiset_count(umset1, &val), ==, 1, "Failed to find entry in first unordered multiset after swap");
    }

    unordered_multiset_free(umset1);
    unordered_multiset_free(umset2);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multiset_print
// ============================================================================
test_res test_unordered_multiset_print(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multiset for print test",
        arg->logger, "Successfully initialized unordered multiset for print test\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_add_entry(umset, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multiset for print test");
    }
    int remove_val = 5;
    UNITTEST_ASSERT(unordered_multiset_remove_entry(umset, &remove_val), ==, CS_SUCCESS, "Failed to remove entry from unordered multiset for print test", 
        arg->logger, "Successfully removed entry from unordered multiset for print test\n");

    char buffer[1024];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    unordered_multiset_print(stream, umset);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Unordered multiset printed output:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected[16];
        snprintf(expected, sizeof(expected), "%d", i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Printed output does not contain expected entry from unordered multiset");
    }

    unordered_multiset_free(umset);
    return SUCCESSFUL_TEST_RES;
};

// ===============================================================================
// Stress test with timing
// ===============================================================================
test_res test_unordered_multiset_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time stress test on Valgrind\n");
        return SUCCESSFUL_TEST_RES;
    }

    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(NULL, get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    struct timeval start, end;
    double elapsed;

    int total = __UNORDERED_MULTISET_STRESS_TEST_SIZE;
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_multiset_add_entry(umset, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Inserted %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_multiset_count(umset, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Counted %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "count", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_multiset_remove_entry(umset, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Erased %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "delete", elapsed);

    return SUCCESSFUL_TEST_RES;
}

// ===============================================================================
// Test Registery
// ===============================================================================
test unordered_multiset_tests[] = {
    // unordered_multiset_init
    test_unordered_multiset_init,

    // unordered_multiset_add_entry
    test_unordered_multiset_add_entry,
    test_unordered_multiset_add_entry_no_comp,
    test_unordered_multiset_add_entry_deepcopy,
    test_unordered_multiset_add_entry_tombstone,

    // unordered_multiset_remove_entry
    test_unordered_multiset_remove_entry,
    test_unordered_multiset_remove_entry_deepfree,
    test_unordered_multiset_remove_entry_hits,

    // unordered_multiset_count
    test_unordered_multiset_count,

    // unordered_multiset_empty
    test_unordered_multiset_empty,

    // unordered_multiset_size
    test_unordered_multiset_size,

    // unordered_multiset_clear
    test_unordered_multiset_clear,
    test_unordered_multiset_clear_deepfree,

    // unordered_multiset_swap
    test_unordered_multiset_swap,

    // unordered_multiset_print
    test_unordered_multiset_print,

    // Stress test with timing
    test_unordered_multiset_stress_time,
};