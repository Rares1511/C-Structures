#include <cs/unordered_multimap.h>

#include <unittest.h>

// ============================================================================
// unordered_multimap_init
// ============================================================================
test_res test_unordered_multimap_init(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap",
        arg->logger, "Successfully initialized unordered multimap\n");

    UNITTEST_ASSERT(umap->ht, !=, NULL, "Hash table inside unordered multimap is NULL after initialization",
        arg->logger, "Hash table inside unordered multimap is properly initialized\n");

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multimap_add_entry
// ============================================================================
test_res test_unordered_multimap_add_entry(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for add_entry test",
        arg->logger, "Successfully initialized unordered multimap for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap");
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap, &i), !=, NULL, "Failed to find added entry in unordered multimap");
        int *vec_val = UNITTEST_ASSERT_SILENT(vector_at(found, 0), !=, NULL, "Failed to get value from vector in unordered multimap");
        UNITTEST_ASSERT_SILENT(*vec_val, ==, i, "Found entry does not match the added key in unordered multimap");
    }

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multimap_add_entry_no_comp(test_arg *arg) {
    elem_attr_t no_comp_attr = get_int_attr();
    no_comp_attr.comp = NULL; // Remove the compare function to test default byte-wise comparison
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, no_comp_attr, no_comp_attr, hash_int), !=, NULL, "Failed to initialize unordered multimap for add_entry test",
        arg->logger, "Successfully initialized unordered multimap for add_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap");
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap, &i), !=, NULL, "Failed to find added entry in unordered multimap");
        int *vec_val = UNITTEST_ASSERT_SILENT(vector_at(found, 0), !=, NULL, "Failed to get value from vector in unordered multimap");
        UNITTEST_ASSERT_SILENT(*vec_val, ==, i, "Found entry does not match the added key in unordered multimap");
    }

    int duplicate = 500; // Attempt to add a duplicate entry
    UNITTEST_ASSERT(unordered_multimap_add_entry(umap, &duplicate, &duplicate), ==, CS_SUCCESS, "Failed to add duplicate entry to unordered multimap",
        arg->logger, "Properly added duplicate entry to unordered multimap\n");
    vector *vec = UNITTEST_ASSERT(unordered_multimap_get_entry(umap, &duplicate), !=, NULL, "Failed to find entry for duplicate key in unordered multimap", 
        arg->logger, "Successfully found entry for duplicate key in unordered multimap\n");
    UNITTEST_ASSERT(vec->size, ==, 2, "Duplicate entry did not create a new value vector in unordered multimap", 
        arg->logger, "Duplicate entry correctly created a new value vector in unordered multimap\n");

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multimap_add_entry_deepcopy(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_test_struct_attr(), get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered multimap with NULL hash function",
        arg->logger, "Successfully initialized unordered multimap with NULL hash function\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &ts, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered multimap");
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap, &ts), !=, NULL, "Failed to find added entry in unordered multimap");
        int *vec_val = UNITTEST_ASSERT_SILENT(vector_at(found, 0), !=, NULL, "Failed to get value from vector in unordered multimap");
        UNITTEST_ASSERT_SILENT(*vec_val, ==, i, "Found entry does not match the added key in unordered multimap");
        free_test_struct(&ts); // Free the original struct since the unordered multimap should have made a deep copy
    }

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multimap_add_entry_tombstone(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for add_entry tombstone test",
        arg->logger, "Successfully initialized unordered multimap for add_entry tombstone test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for tombstone test");
    }

    // Remove some entries to create tombstones
    for (int i = 0; i < total; i += 2) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered multimap to create tombstone");
    }

    // Now add new entries that should reuse the tombstone slots
    for (int i = 0; i < total; i+=2) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap after creating tombstones");
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap, &i), !=, NULL, "Failed to find added entry in unordered multimap after creating tombstones");
        int *vec_val = UNITTEST_ASSERT_SILENT(vector_at(found, 0), !=, NULL, "Failed to get value from vector in unordered multimap");
        UNITTEST_ASSERT_SILENT(*vec_val, ==, i, "Found entry does not match the added key in unordered multimap after creating tombstones");
    }

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_multimap_remove_entry
// ============================================================================
test_res test_unordered_multimap_remove_entry(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for remove_entry test",
        arg->logger, "Successfully initialized unordered multimap for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for remove_entry test");
    }

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry from unordered multimap");
        UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap, &i), ==, NULL, "Entry still found in unordered multimap after removal");
    }

    int non_existent = 2000; // Attempt to remove a non-existent entry
    UNITTEST_ASSERT(unordered_multimap_remove_entry(umap, &non_existent), !=, CS_SUCCESS, "Failed to handle removal of non-existent entry from unordered multimap",
        arg->logger, "Properly handled attempt to remove non-existent entry from unordered multimap\n");

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multimap_remove_entry_deepfree(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_test_struct_attr(), get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered multimap with NULL hash function for remove_entry test",
        arg->logger, "Successfully initialized unordered multimap with NULL hash function for remove_entry test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &ts, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for remove_entry test");
        free_test_struct(&ts); // Free the original struct since the unordered multimap should have made a deep copy
    }

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(umap, &ts), ==, CS_SUCCESS, "Failed to remove entry from unordered multimap");
        UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap, &ts), ==, NULL, "Entry still found in unordered multimap after removal");
        free_test_struct(&ts); // Free the struct used for removal
    }

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multimap_remove_entry_hits(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int_mod), !=, NULL, "Failed to initialize unordered multimap with mod hash function for add_entry hits test",
        arg->logger, "Successfully initialized unordered multimap with mod hash function for add_entry hits test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for add_entry hits test");
    }

    // Check that all entries are present despite the hash collisions
    for (int i = total - 1; i >= 0; i--) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to find added entry in unordered multimap with hash collisions");
    }

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// unordered_multimap_count
// ============================================================================
test_res test_unordered_multimap_count(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for count test",
        arg->logger, "Successfully initialized unordered multimap for count test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for count test");
    }

    for (int i = 0; i < total; i++) {
        int count = UNITTEST_ASSERT_SILENT(unordered_multimap_count(umap, &i), ==, 1, "Count for existing entry in unordered multimap should be 1");
        UNITTEST_ASSERT_SILENT(count, ==, 1, "Count for existing entry in unordered multimap should be 1");
    }

    int non_existent = 2000; // Count for a non-existent entry
    UNITTEST_ASSERT(unordered_multimap_count(umap, &non_existent), ==, 0, "Count for non-existent entry in unordered multimap should be 0",
        arg->logger, "Properly returned count of 0 for non-existent entry in unordered multimap\n");

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multimap_empty
// ============================================================================
test_res test_unordered_multimap_empty(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for empty test",
        arg->logger, "Successfully initialized unordered multimap for empty test\n");

    UNITTEST_ASSERT(unordered_multimap_empty(umap), ==, 1, "Newly initialized unordered multimap should be empty", 
        arg->logger, "Newly initialized unordered multimap is correctly reported as empty\n");

    int val = 42;
    UNITTEST_ASSERT(unordered_multimap_add_entry(umap, &val, &val), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for empty test", 
        arg->logger, "Successfully added entry to unordered multimap for empty test\n");

    UNITTEST_ASSERT(unordered_multimap_empty(umap), ==, 0, "Unordered multimap with entries should not be empty", 
        arg->logger, "Unordered multimap with entries is correctly reported as not empty\n");

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multimap_size
// ============================================================================
test_res test_unordered_multimap_size(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for size test",
        arg->logger, "Successfully initialized unordered multimap for size test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for size test");
        UNITTEST_ASSERT_SILENT(unordered_multimap_size(umap), ==, (size_t)(i + 1), "Size of unordered multimap does not match number of added entries");
    }

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multimap_clear
// ============================================================================
test_res test_unordered_multimap_clear(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for clear test",
        arg->logger, "Successfully initialized unordered multimap for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &i), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for clear test");
    }

    unordered_multimap_clear(umap);
    UNITTEST_ASSERT(unordered_multimap_empty(umap), ==, 1, "Unordered multimap should be empty after clear", 
        arg->logger, "Unordered multimap is correctly reported as empty after clear\n");

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

test_res test_unordered_multimap_clear_deepfree(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_test_struct_attr(), get_test_struct_attr(), hash_test_struct), !=, NULL, "Failed to initialize unordered multimap with NULL hash function for clear test",
        arg->logger, "Successfully initialized unordered multimap with NULL hash function for clear test\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "test", 2.5 * i);
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &ts, &ts), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for clear test");
        free_test_struct(&ts); // Free the original struct since the unordered multimap should have made a deep copy
    }

    unordered_multimap_clear(umap);
    UNITTEST_ASSERT(unordered_multimap_empty(umap), ==, 1, "Unordered multimap should be empty after clear", 
        arg->logger, "Unordered multimap is correctly reported as empty after clear\n");

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multimap_swap
// ============================================================================
test_res test_unordered_multimap_swap(test_arg *arg) {
    unordered_multimap *umap1 = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize first unordered multimap for swap test",
        arg->logger, "Successfully initialized first unordered multimap for swap test\n");
    unordered_multimap *umap2 = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize second unordered multimap for swap test",
        arg->logger, "Successfully initialized second unordered multimap for swap test\n");

    int total1 = 1000, total2 = 500;
    for (int i = 0; i < total1; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap1, &i, &i), ==, CS_SUCCESS, "Failed to add entry to first unordered multimap for swap test");
    }
    for (int i = 0; i < total2; i++) {
        int val = 10000 + i; // Use different values to distinguish between the two multimaps
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap2, &val, &val), ==, CS_SUCCESS, "Failed to add entry to second unordered multimap for swap test");
    }

    unordered_multimap_swap(umap1, umap2);

    UNITTEST_ASSERT(unordered_multimap_size(umap1), ==, (size_t)total2, "First unordered multimap size does not match expected size after swap", 
        arg->logger, "First unordered multimap size matches expected size after swap\n");
    UNITTEST_ASSERT(unordered_multimap_size(umap2), ==, (size_t)total1, "Second unordered multimap size does not match expected size after swap", 
        arg->logger, "Second unordered multimap size matches expected size after swap\n");

    for (int i = 0; i < total1; i++) {
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap2, &i), !=, NULL, "Failed to find entry in second unordered multimap after swap");
        int *vec_val = UNITTEST_ASSERT_SILENT(vector_at(found, 0), !=, NULL, "Failed to get value from vector in second unordered multimap after swap");
        UNITTEST_ASSERT_SILENT(*vec_val, ==, i, "Found entry does not match expected key in second unordered multimap after swap");
    }

    for (int i = 0; i < total2; i++) {
        int val = 10000 + i;
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(umap1, &val), !=, NULL, "Failed to find entry in first unordered multimap after swap");
        int *vec_val = UNITTEST_ASSERT_SILENT(vector_at(found, 0), !=, NULL, "Failed to get value from vector in first unordered multimap after swap");
        UNITTEST_ASSERT_SILENT(*vec_val, ==, val, "Found entry does not match expected key in first unordered multimap after swap");
    }

    unordered_multimap_free(umap1);
    unordered_multimap_free(umap2);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// unordered_multimap_print
// ============================================================================
test_res test_unordered_multimap_print(test_arg *arg) {
    unordered_multimap *umap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered multimap for print test",
        arg->logger, "Successfully initialized unordered multimap for print test\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        int val1 = i, val2 = i * 10, val3 = i * 100; // Add multiple values for each key to test printing of vectors
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &val1), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for print test");
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &val2), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for print test");
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(umap, &i, &val3), ==, CS_SUCCESS, "Failed to add entry to unordered multimap for print test");
    }
    int remove_val = 5;
    UNITTEST_ASSERT(unordered_multimap_remove_entry(umap, &remove_val), ==, CS_SUCCESS, "Failed to remove entry from unordered multimap for print test",
        arg->logger, "Successfully removed entry from unordered multimap for print test\n");

    char buffer[1024];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    unordered_multimap_print(stream, umap);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Unordered multimap printed output:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected[16];
        snprintf(expected, sizeof(expected), "%d", i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Printed output does not contain expected entry from unordered multimap");
    }

    unordered_multimap_free(umap);
    return SUCCESSFUL_TEST_RES;
};

// =============================================================================
// Stress test with timing
// =============================================================================
test_res test_unordered_multimap_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time stress test on Valgrind");
        return SUCCESSFUL_TEST_RES;
    }

    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(NULL, get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for stress time test\n");
    struct timeval start, end;
    double elapsed;

    int total = __UNORDERED_MULTIMAP_STRESS_TEST_SIZE;
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_multimap_add_entry(ummap, &i, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Inserted %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_multimap_get_entry(ummap, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Found %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        unordered_multimap_remove_entry(ummap, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Erased %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "delete", elapsed);

    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Test Registery
// ============================================================================
test unordered_multimap_tests[] = {
    // unordered_multimap_init
    test_unordered_multimap_init,

    // unordered_multimap_add_entry
    test_unordered_multimap_add_entry,
    test_unordered_multimap_add_entry_no_comp,
    test_unordered_multimap_add_entry_deepcopy,
    test_unordered_multimap_add_entry_tombstone,

    // unordered_multimap_remove_entry
    test_unordered_multimap_remove_entry,
    test_unordered_multimap_remove_entry_deepfree,
    test_unordered_multimap_remove_entry_hits,

    // unordered_multimap_count
    test_unordered_multimap_count,

    // unordered_multimap_empty
    test_unordered_multimap_empty,

    // unordered_multimap_size
    test_unordered_multimap_size,

    // unordered_multimap_clear
    test_unordered_multimap_clear,
    test_unordered_multimap_clear_deepfree,

    // unordered_multimap_swap
    test_unordered_multimap_swap,

    // unordered_multimap_print
    test_unordered_multimap_print,

    // Stress test with timing
    test_unordered_multimap_stress_time,
};