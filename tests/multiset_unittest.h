#include <cs/multiset.h>

#include <unittest.h>

// ============================================================================
// multiset_init
// ============================================================================
test_res test_multiset_init(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    UNITTEST_ASSERT(ms->t, !=, NULL, "Multiset's internal tree is NULL after initialization", arg->logger, "Multiset's internal tree initialized successfully\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_insert
// ============================================================================
test_res test_multiset_insert(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_double_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multiset\n", total);

    UNITTEST_ASSERT(multiset_size(ms), ==, total, "Multiset size mismatch after insertions", arg->logger, "Multiset size is correct after insertions\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multiset_insert_duplicate(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int val = 42;
    UNITTEST_ASSERT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset", arg->logger, "First insertion successful\n");
    UNITTEST_ASSERT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert duplicate element into multiset", arg->logger, "Duplicate insertion correctly inserted\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multiset_insert_deepcopy(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, attr), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &ts), ==, CS_SUCCESS, "Failed to insert element into multiset");
        free_test_struct(&ts); // Free original struct since multiset should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the multiset\n", total);
    
    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_delete
// ============================================================================
test_res test_multiset_delete(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_double_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000, counts = 3;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        for (int j = 0; j < counts; j++) {
            UNITTEST_ASSERT_SILENT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset");
        }
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multiset\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        for (int j = 0; j < counts; j++) {
            UNITTEST_ASSERT_SILENT(multiset_delete(ms, &key), ==, CS_SUCCESS, "Failed to delete existing element from multiset");
        }
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the multiset\n", total);

    UNITTEST_ASSERT(multiset_empty(ms), ==, 1, "Multiset is not empty after deleting all elements", arg->logger, "Multiset is empty after deletions\n");

    UNITTEST_ASSERT(multiset_delete(ms, &(double){123.45}), ==, CS_ELEM, "Deleted non-existent element from multiset", arg->logger, "Correctly failed to delete non-existent element\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multiset_delete_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, attr), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &ts), ==, CS_SUCCESS, "Failed to insert element into multiset");
        free_test_struct(&ts); // Free original struct since multiset should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the multiset\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        test_struct temp = create_test_struct(0, NULL, key); // Create temp struct with just the score for comparison
        UNITTEST_ASSERT_SILENT(multiset_delete(ms, &temp), ==, CS_SUCCESS, "Failed to delete existing element from multiset");
        free_test_struct(&temp);
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the multiset\n", total);

    UNITTEST_ASSERT(multiset_empty(ms), ==, 1, "Multiset is not empty after deleting all elements", arg->logger, "Multiset is empty after deletions\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_clear
// ============================================================================
test_res test_multiset_clear(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multiset\n", total);

    multiset_clear(ms);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the multiset\n");

    UNITTEST_ASSERT(multiset_empty(ms), ==, 1, "Multiset is not empty after clear", arg->logger, "Multiset is empty after clear\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multiset_clear_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, attr), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &ts), ==, CS_SUCCESS, "Failed to insert element into multiset");
        free_test_struct(&ts); // Free original struct since multiset should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the multiset\n", total);

    multiset_clear(ms);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the multiset\n");

    UNITTEST_ASSERT(multiset_empty(ms), ==, 1, "Multiset is not empty after clear", arg->logger, "Multiset is empty after clear\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_count
// =============================================================================
test_res test_multiset_count(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_double_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multiset\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(multiset_count(ms, &key), ==, 1, "Multiset count mismatch for existing element");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully found all %d elements in the multiset\n", total);

    double not_found_key = -12345.67;
    UNITTEST_ASSERT(multiset_count(ms, &not_found_key), ==, 0, "Multiset count mismatch for non-existent element", arg->logger, "Multiset count is correct for non-existent element\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_size
// ============================================================================
test_res test_multiset_size(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset");
        UNITTEST_ASSERT_SILENT(multiset_size(ms), ==, i + 1, "Multiset size mismatch after insertion");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multiset and verified size after each insertion\n", total);

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_empty
// ============================================================================
test_res test_multiset_empty(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    UNITTEST_ASSERT(multiset_empty(ms), ==, 1, "Newly initialized multiset is not empty", arg->logger, "Newly initialized multiset is empty\n");

    int val = 42;
    UNITTEST_ASSERT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset", arg->logger, "Inserted one element into the multiset\n");
    UNITTEST_ASSERT(multiset_empty(ms), ==, 0, "Multiset is empty after inserting an element", arg->logger, "Multiset is not empty after inserting an element\n");

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_print
// ============================================================================
test_res test_multiset_print(test_arg *arg) {
    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset initialization failed", arg->logger, "Multiset initialized successfully\n");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    multiset_print(stream, ms);
    fclose(stream);
    UNITTEST_ASSERT(buffer[0], ==, '\0', "Multiset print output is not empty for an empty multiset", arg->logger, "Multiset print output is empty for an empty multiset\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        int val = i * i;
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset");
        UNITTEST_ASSERT_SILENT(multiset_insert(ms, &val), ==, CS_SUCCESS, "Failed to insert element into multiset");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multiset\n", total);

    stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    multiset_print(stream, ms);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Multiset contents:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected_line[64];
        snprintf(expected_line, sizeof(expected_line), "%d", i * i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected_line), !=, NULL, "Multiset print output is missing expected element");
    }

    multiset_free(ms);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multiset_swap
// ============================================================================
test_res test_multiset_swap(test_arg *arg) {
    multiset *ms1 = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset 1 initialization failed", arg->logger, "Multiset 1 initialized successfully\n");
    multiset *ms2 = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset 2 initialization failed", arg->logger, "Multiset 2 initialized successfully\n");

    int total1 = 1000, total2 = 500;
    for (int i = 0; i < total1; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(multiset_insert(ms1, &val), ==, CS_SUCCESS, "Failed to insert element into multiset 1");
    }
    for (int i = 0; i < total2; i++) {
        int val = i + 1000;
        UNITTEST_ASSERT_SILENT(multiset_insert(ms2, &val), ==, CS_SUCCESS, "Failed to insert element into multiset 2");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into multiset 1 and %d elements into multiset 2\n", total1, total2);

    multiset_swap(ms1, ms2);

    UNITTEST_ASSERT(multiset_size(ms1), ==, total2, "Multiset 1 size mismatch after swap", arg->logger, "Multiset 1 size is correct after swap\n");
    UNITTEST_ASSERT(multiset_size(ms2), ==, total1, "Multiset 2 size mismatch after swap", arg->logger, "Multiset 2 size is correct after swap\n");

    for (int i = 0; i < total2; i++) {
        int key = i + 1000;
        UNITTEST_ASSERT_SILENT(multiset_count(ms1, &key), ==, 1, "Failed to find element in multiset 1 after swap");
    }

    for (int i = 0; i < total1; i++) {
        int key = i;
        UNITTEST_ASSERT_SILENT(multiset_count(ms2, &key), ==, 1, "Failed to find element in multiset 2 after swap");
    }

    multiset_free(ms1);
    multiset_free(ms2);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Stress time test
// ============================================================================
test_res test_multiset_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        return (test_res){(char*)__func__, "Valgrind active - skipping stress test", CS_SUCCESS};
    }

    struct timeval start, end;
    double elapsed;

    multiset *ms = UNITTEST_ASSERT(multiset_init(NULL, get_int_attr()), !=, NULL, "Multiset init failed", 
        arg->logger, "Successfully initialized multiset for stress test\n");
    if (ms == NULL) {
        return (test_res){(char*)__func__, "Multiset init failed", CS_MEM};
    }
    int total = __MULTISET_STRESS_TEST_SIZE;

    /* INSERT timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int val = i;
        multiset_insert(ms, &val);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Insert Time = %.9f sec\n", elapsed);

    /* COUNT timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        multiset_count(ms, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Count Time = %.9f sec\n", elapsed);

    /* DELETE timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        multiset_delete(ms, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "erase", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Delete Time = %.9f sec\n", elapsed);

    multiset_free(ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Test array
// ============================================================================
test multiset_tests[] = {
    // multiset_init
    test_multiset_init,

    // multiset_insert
    test_multiset_insert,
    test_multiset_insert_duplicate,
    test_multiset_insert_deepcopy,

    // multiset_delete
    test_multiset_delete,
    test_multiset_delete_deepfree,

    // multiset_clear
    test_multiset_clear,
    test_multiset_clear_deepfree,

    // multiset_count
    test_multiset_count,

    // multiset_size
    test_multiset_size,

    // multiset_empty
    test_multiset_empty,

    // multiset_print
    test_multiset_print,

    // multiset_swap
    test_multiset_swap,

    // Stress time
    test_multiset_stress_time,
};