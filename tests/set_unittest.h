#include <cs/set.h>

#include <unittest.h>

// ============================================================================
// set_init
// ============================================================================
test_res test_set_init(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    UNITTEST_ASSERT(s->t, !=, NULL, "Set's internal tree is NULL after initialization", arg->logger, "Set's internal tree initialized successfully\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_insert
// ============================================================================
test_res test_set_insert(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_double_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the set\n", total);

    UNITTEST_ASSERT(set_size(s), ==, total, "Set size mismatch after insertions", arg->logger, "Set size is correct after insertions\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

test_res test_set_insert_duplicate(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int val = 42;
    UNITTEST_ASSERT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set", arg->logger, "First insertion successful\n");
    UNITTEST_ASSERT(set_insert(s, &val), ==, CS_ELEM, "Duplicate insertion did not return expected error code", arg->logger, "Duplicate insertion correctly returned error code\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

test_res test_set_insert_deepcopy(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    set *s = UNITTEST_ASSERT(set_init(NULL, attr), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(set_insert(s, &ts), ==, CS_SUCCESS, "Failed to insert element into set");
        free_test_struct(&ts); // Free original struct since set should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the set\n", total);
    
    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_delete
// ============================================================================
test_res test_set_delete(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_double_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the set\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(set_delete(s, &key), ==, CS_SUCCESS, "Failed to delete existing element from set");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the set\n", total);

    UNITTEST_ASSERT(set_empty(s), ==, 1, "Set is not empty after deleting all elements", arg->logger, "Set is empty after deletions\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

test_res test_set_delete_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    set *s = UNITTEST_ASSERT(set_init(NULL, attr), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(set_insert(s, &ts), ==, CS_SUCCESS, "Failed to insert element into set");
        free_test_struct(&ts); // Free original struct since set should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the set\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        test_struct temp = create_test_struct(0, NULL, key); // Create temp struct with just the score for comparison
        UNITTEST_ASSERT_SILENT(set_delete(s, &temp), ==, CS_SUCCESS, "Failed to delete existing element from set");
        free_test_struct(&temp);
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the set\n", total);

    UNITTEST_ASSERT(set_empty(s), ==, 1, "Set is not empty after deleting all elements", arg->logger, "Set is empty after deletions\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_clear
// ============================================================================
test_res test_set_clear(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the set\n", total);

    set_clear(s);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the set\n");

    UNITTEST_ASSERT(set_empty(s), ==, 1, "Set is not empty after clear", arg->logger, "Set is empty after clear\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

test_res test_set_clear_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    set *s = UNITTEST_ASSERT(set_init(NULL, attr), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(set_insert(s, &ts), ==, CS_SUCCESS, "Failed to insert element into set");
        free_test_struct(&ts); // Free original struct since set should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the set\n", total);

    set_clear(s);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the set\n");

    UNITTEST_ASSERT(set_empty(s), ==, 1, "Set is not empty after clear", arg->logger, "Set is empty after clear\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_find
// =============================================================================
test_res test_set_find(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_double_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the set\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        double *found = UNITTEST_ASSERT_SILENT(set_find(s, &key), !=, NULL, "Failed to find existing element in set");
        UNITTEST_ASSERT_SILENT(*found, ==, key, "Incorrect element found in set");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully found all %d elements in the set\n", total);

    double not_found_key = -12345.67;
    UNITTEST_ASSERT(set_find(s, &not_found_key), ==, NULL, "Finding non-existent element did not return NULL", arg->logger, "Finding non-existent element correctly returned NULL\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_size
// ============================================================================
test_res test_set_size(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set");
        UNITTEST_ASSERT_SILENT(set_size(s), ==, i + 1, "Set size mismatch after insertion");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the set and verified size after each insertion\n", total);

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_empty
// ============================================================================
test_res test_set_empty(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    UNITTEST_ASSERT(set_empty(s), ==, 1, "Newly initialized set is not empty", arg->logger, "Newly initialized set is empty\n");

    int val = 42;
    UNITTEST_ASSERT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set", arg->logger, "Inserted one element into the set\n");
    UNITTEST_ASSERT(set_empty(s), ==, 0, "Set is empty after inserting an element", arg->logger, "Set is not empty after inserting an element\n");

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_print
// ============================================================================
test_res test_set_print(test_arg *arg) {
    set *s = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set initialization failed", arg->logger, "Set initialized successfully\n");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    set_print(stream, s);
    fclose(stream);
    UNITTEST_ASSERT(buffer[0], ==, '\0', "Set print output is not empty for an empty set", arg->logger, "Set print output is empty for an empty set\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        int val = i * i;
        UNITTEST_ASSERT_SILENT(set_insert(s, &val), ==, CS_SUCCESS, "Failed to insert element into set");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the set\n", total);

    stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    set_print(stream, s);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Set contents:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected_line[64];
        snprintf(expected_line, sizeof(expected_line), "%d", i * i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected_line), !=, NULL, "Set print output is missing expected element");
    }

    set_free(s);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// set_swap
// ============================================================================
test_res test_set_swap(test_arg *arg) {
    set *s1 = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set 1 initialization failed", arg->logger, "Set 1 initialized successfully\n");
    set *s2 = UNITTEST_ASSERT(set_init(NULL, get_int_attr()), !=, NULL, "Set 2 initialization failed", arg->logger, "Set 2 initialized successfully\n");

    int total1 = 1000, total2 = 500;
    for (int i = 0; i < total1; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(set_insert(s1, &val), ==, CS_SUCCESS, "Failed to insert element into set 1");
    }
    for (int i = 0; i < total2; i++) {
        int val = i + 1000;
        UNITTEST_ASSERT_SILENT(set_insert(s2, &val), ==, CS_SUCCESS, "Failed to insert element into set 2");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into set 1 and %d elements into set 2\n", total1, total2);

    set_swap(s1, s2);

    UNITTEST_ASSERT(set_size(s1), ==, total2, "Set 1 size mismatch after swap", arg->logger, "Set 1 size is correct after swap\n");
    UNITTEST_ASSERT(set_size(s2), ==, total1, "Set 2 size mismatch after swap", arg->logger, "Set 2 size is correct after swap\n");

    for (int i = 0; i < total2; i++) {
        int key = i + 1000;
        int *found = UNITTEST_ASSERT_SILENT(set_find(s1, &key), !=, NULL, "Failed to find element in set 1 after swap");
        UNITTEST_ASSERT_SILENT(*found, ==, key, "Incorrect element found in set 1 after swap");
    }

    for (int i = 0; i < total1; i++) {
        int key = i;
        int *found = UNITTEST_ASSERT_SILENT(set_find(s2, &key), !=, NULL, "Failed to find element in set 2 after swap");
        UNITTEST_ASSERT_SILENT(*found, ==, key, "Incorrect element found in set 2 after swap");
    }

    set_free(s1);
    set_free(s2);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Stress test with timing
// ============================================================================
test_res test_set_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        return (test_res){(char*)__func__, "Valgrind active - skipping stress test", CS_SUCCESS};
    }

    set *s = set_init(NULL, get_int_attr());
    if (s == NULL) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    struct timeval start, end;
    double elapsed;
    int total = __SET_STRESS_TEST_SIZE;

    /* INSERT timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int val = i;
        set_insert(s, &val);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Insert Time = %.9f sec\n", elapsed);

    /* FIND timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        set_find(s, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Find Time = %.9f sec\n", elapsed);

    /* DELETE timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        set_delete(s, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "delete", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Delete Time = %.9f sec\n", elapsed);

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Test registry
// ============================================================================
test set_tests[] = {
    // set_init
    test_set_init,

    // set_insert
    test_set_insert,
    test_set_insert_duplicate,
    test_set_insert_deepcopy,

    // set_delete
    test_set_delete,
    test_set_delete_deepfree,

    // set_find
    test_set_find,

    // set_clear
    test_set_clear,
    test_set_clear_deepfree,

    // set_size
    test_set_size,

    // set_empty
    test_set_empty,

    // set_print
    test_set_print,

    // set_swap
    test_set_swap,

    // Stress tests with timing
    test_set_stress_time,
};