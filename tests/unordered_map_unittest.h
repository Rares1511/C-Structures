#include <cs/unordered_map.h>

#include <unittest.h>

// ============================================================================
// unordered_map_init
// ============================================================================
test_res test_unordered_map_init(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map",
        arg->logger, "Successfully initialized unordered map\n");

    UNITTEST_ASSERT(umap->ht, !=, NULL, "Hash table inside unordered map is NULL after initialization",
        arg->logger, "Hash table inside unordered map is properly initialized\n");

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_map_add_entry
// ============================================================================
test_res test_unordered_map_add_entry(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for add_entry test",
        arg->logger, "Successfully initialized unordered map for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map");
        void *found = UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &i), !=, NULL, "Failed to find added entry in unordered map");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match the added key in unordered map");
    }

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_map_add_entry_no_comp(test_arg *arg) {
    elem_attr_t no_comp_attr = get_int_attr();
    no_comp_attr.comp = NULL; // Remove the compare function to test default byte-wise comparison
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, no_comp_attr, no_comp_attr, hash_int), !=, NULL, "Failed to initialize unordered map for add_entry test",
        arg->logger, "Successfully initialized unordered map for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map");
        void *found = UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &i), !=, NULL, "Failed to find added entry in unordered map");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match the added key in unordered map");
    }

    int duplicate = 500; // Attempt to add a duplicate entry
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &duplicate, &duplicate), !=, CS_SUCCESS, "Failed to add duplicate entry to unordered map",
        arg->logger, "Properly handled attempt to add duplicate entry to unordered map\n");

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_map_add_entry_deepcopy(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_test_struct_attr(), get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered map with NULL hash function",
        arg->logger, "Successfully initialized unordered map with NULL hash function\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &ts, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered map");
        void *found = UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &ts), !=, NULL, "Failed to find added entry in unordered map");
        UNITTEST_ASSERT_SILENT(comp_test_struct_max(found, &ts), ==, 0, "Found entry does not match the added key in unordered map");
        free_test_struct(&ts); // Free the original struct since the unordered map should have made a deep copy
    }

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_map_add_entry_tombstone(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for add_entry tombstone test",
        arg->logger, "Successfully initialized unordered map for add_entry tombstone test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map for tombstone test");
    }

    // Remove some entries to create tombstones
    for (int i = 0; i < total; i += 2) {
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered map to create tombstone");
    }

    // Now add new entries that should reuse the tombstone slots
    for (int i = 0; i < total; i+=2) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map after creating tombstones");
        void *found = UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &i), !=, NULL, "Failed to find added entry in unordered map after creating tombstones");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match the added key in unordered map after creating tombstones");
    }

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_map_remove_entry
// ============================================================================
test_res test_unordered_map_remove_entry(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for remove_entry test",
        arg->logger, "Successfully initialized unordered map for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map for remove_entry test");
    }

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered map");
        UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &i), ==, NULL, "Entry still found in unordered map after removal");
    }

    int non_existent = 2000; // Attempt to remove a non-existent entry
    UNITTEST_ASSERT(unordered_map_remove_entry(umap, &non_existent), !=, CS_SUCCESS, "Failed to handle removal of non-existent entry from unordered map",
        arg->logger, "Properly handled attempt to remove non-existent entry from unordered map\n");

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_map_remove_entry_deepfree(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_test_struct_attr(), get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered map with NULL hash function for remove_entry test",
        arg->logger, "Successfully initialized unordered map with NULL hash function for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &ts, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered map for remove_entry test");
        free_test_struct(&ts); // Free the original struct since the unordered map should have made a deep copy
    }

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &ts), ==, CS_SUCCESS, "Failed to remove entry from unordered map");
        UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &ts), ==, NULL, "Entry still found in unordered map after removal");
        free_test_struct(&ts); // Free the struct used for removal
    }

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_map_remove_entry_hits(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int_mod), !=, NULL, "Failed to initialize unordered map with mod hash function for add_entry hits test",
        arg->logger, "Successfully initialized unordered map with mod hash function for add_entry hits test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map");
    }

    // Check that all entries are present despite the hash collisions
    for (int i = total - 1; i >= 0; i--) {
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to find added entry in unordered map with hash collisions");
    }

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_map_count
// ============================================================================
test_res test_unordered_map_count(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for count test",
        arg->logger, "Successfully initialized unordered map for count test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map for count test");
    }

    for (int i = 0; i < total; i++) {
        int count = UNITTEST_ASSERT_SILENT(unordered_map_count(umap, &i), ==, 1, "Count for existing entry in unordered map should be 1");
        UNITTEST_ASSERT_SILENT(count, ==, 1, "Count for existing entry in unordered map should be 1");
    }

    int non_existent = 2000; // Count for a non-existent entry
    UNITTEST_ASSERT(unordered_map_count(umap, &non_existent), ==, 0, "Count for non-existent entry in unordered map should be 0",
        arg->logger, "Properly returned count of 0 for non-existent entry in unordered map\n");

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_map_empty
// ============================================================================
test_res test_unordered_map_empty(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for empty test",
        arg->logger, "Successfully initialized unordered map for empty test\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 1, "Newly initialized unordered map should be empty", 
        arg->logger, "Newly initialized unordered map is correctly reported as empty\n");

    int val = 42;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &val, &val), ==, CS_SUCCESS, "Failed to add entry to unordered map for empty test", 
        arg->logger, "Successfully added entry to unordered map for empty test\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 0, "Unordered map with entries should not be empty", 
        arg->logger, "Unordered map with entries is correctly reported as not empty\n");

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_map_size
// ============================================================================
test_res test_unordered_map_size(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for size test",
        arg->logger, "Successfully initialized unordered map for size test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map for size test");
        UNITTEST_ASSERT_SILENT(unordered_map_size(umap), ==, (size_t)(i + 1), "Size of unordered map does not match number of added entries");
    }

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_map_clear
// ============================================================================
test_res test_unordered_map_clear(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for clear test",
        arg->logger, "Successfully initialized unordered map for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map for clear test");
    }

    unordered_map_clear(umap);
    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 1, "Unordered map should be empty after clear", 
        arg->logger, "Unordered map is correctly reported as empty after clear\n");

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_map_clear_deepfree(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_test_struct_attr(), get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered map with NULL hash function for clear test",
        arg->logger, "Successfully initialized unordered map with NULL hash function for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &ts, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered map for clear test");
        free_test_struct(&ts); // Free the original struct since the unordered map should have made a deep copy
    }

    unordered_map_clear(umap);
    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 1, "Unordered map should be empty after clear", 
        arg->logger, "Unordered map is correctly reported as empty after clear\n");

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_map_swap
// ============================================================================
test_res test_unordered_map_swap(test_arg *arg) {
    unordered_map *umap1 = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize first unordered map for swap test",
        arg->logger, "Successfully initialized first unordered map for swap test\n");
    unordered_map *umap2 = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize second unordered map for swap test",
        arg->logger, "Successfully initialized second unordered map for swap test\n");

    int total1 = 1000, total2 = 500;
    for (int i = 0; i < total1; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap1, &i, &i), ==, CS_SUCCESS, "Failed to add entry to first unordered map for swap test");
    }
    for (int i = 0; i < total2; i++) {
        int val = 10000 + i; // Use different values to distinguish between the two maps
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap2, &val, &val), ==, CS_SUCCESS, "Failed to add entry to second unordered map for swap test");
    }

    unordered_map_swap(umap1, umap2);

    UNITTEST_ASSERT(unordered_map_size(umap1), ==, (size_t)total2, "First unordered map size does not match expected size after swap", 
        arg->logger, "First unordered map size matches expected size after swap\n");
    UNITTEST_ASSERT(unordered_map_size(umap2), ==, (size_t)total1, "Second unordered map size does not match expected size after swap", 
        arg->logger, "Second unordered map size matches expected size after swap\n");

    for (int i = 0; i < total1; i++) {
        void *found = UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap2, &i), !=, NULL, "Failed to find entry in second unordered map after swap");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, i, "Found entry does not match expected key in second unordered map after swap");
    }

    for (int i = 0; i < total2; i++) {
        int val = 10000 + i;
        void *found = UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap1, &val), !=, NULL, "Failed to find entry in first unordered map after swap");
        UNITTEST_ASSERT_SILENT(*(int*)found, ==, val, "Found entry does not match expected key in first unordered map after swap");
    }

    unordered_map_free(umap1);
    unordered_map_free(umap2);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_map_print
// ============================================================================
test_res test_unordered_map_print(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered map for print test",
        arg->logger, "Successfully initialized unordered map for print test\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered map for print test");
    }
    int remove_val = 5;
    UNITTEST_ASSERT(unordered_map_remove_entry(umap, &remove_val), ==, CS_SUCCESS, "Failed to remove entry from unordered map for print test",
        arg->logger, "Successfully removed entry from unordered map for print test\n");

    char buffer[1024];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    unordered_map_print(stream, umap);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Unordered map printed output:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected[16];
        snprintf(expected, sizeof(expected), "%d", i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Printed output does not contain expected entry from unordered map");
    }

    unordered_map_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// =============================================================================
// Stress test with timing
// =============================================================================
test_res test_unordered_map_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time-based stress test on Valgrind\n");
        return SUCCESSFUL_TEST_RES;
    }

    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(NULL, get_int_attr(), get_string_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for stress time test\n");
    struct timeval start, end;
    double elapsed;

    int total = __UNORDERED_MAP_STRESS_TEST_SIZE;
    char value[25] = "StressValGeneric";

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_map_add_entry(umap, &i, value);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_map_get_entry(umap, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test get completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_map_remove_entry(umap, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test remove completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "delete", elapsed);

    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Test Registry
// ============================================================================
test unordered_map_tests[] = {
    // unordered_map_init
    test_unordered_map_init,

    // unordered_map_add_entry
    test_unordered_map_add_entry,
    test_unordered_map_add_entry_no_comp,
    test_unordered_map_add_entry_deepcopy,
    test_unordered_map_add_entry_tombstone,

    // unordered_map_remove_entry
    test_unordered_map_remove_entry,
    test_unordered_map_remove_entry_deepfree,
    test_unordered_map_remove_entry_hits,

    // unordered_map_count
    test_unordered_map_count,

    // unordered_map_empty
    test_unordered_map_empty,

    // unordered_map_size
    test_unordered_map_size,

    // unordered_map_clear
    test_unordered_map_clear,
    test_unordered_map_clear_deepfree,

    // unordered_map_swap
    test_unordered_map_swap,

    // unordered_map_print
    test_unordered_map_print,

    // Sress test with timing
    test_unordered_map_stress_time,
};