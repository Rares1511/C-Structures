#include <cs/multimap.h>

#include <unittest.h>

// ============================================================================
// multimap_init
// ============================================================================
test_res test_multimap_init(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    UNITTEST_ASSERT(mm->t, !=, NULL, "Multimap's internal tree is NULL after initialization", arg->logger, "Multimap's internal tree initialized successfully\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_insert
// ============================================================================
test_res test_multimap_insert(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_test_struct_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", val);
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &i, &ts), ==, CS_SUCCESS, "Failed to insert element into multimap");
        free_test_struct(&ts); // Free original struct since multimap should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multimap\n", total);

    UNITTEST_ASSERT(multimap_size(mm), ==, total, "Multimap size mismatch after insertions", arg->logger, "Multimap size is correct after insertions\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multimap_insert_duplicate(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_double_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int key = 42;
    double val = 123.45;
    UNITTEST_ASSERT(multimap_insert(mm, &key, &val), ==, CS_SUCCESS, "Failed to insert element into multimap", arg->logger, "First insertion successful\n");
    UNITTEST_ASSERT(multimap_insert(mm, &key, &val), ==, CS_SUCCESS, "Failed to insert duplicate element into multimap", arg->logger, "Duplicate insertion correctly inserted\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multimap_insert_deepcopy(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, attr, get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &ts, &i), ==, CS_SUCCESS, "Failed to insert element into multimap");
        free_test_struct(&ts); // Free original struct since multimap should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the multimap\n", total);
    
    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_delete
// ============================================================================
test_res test_multimap_delete(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_double_attr(), get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000, counts = 3;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        for (int j = 0; j < counts; j++) {
            UNITTEST_ASSERT_SILENT(multimap_insert(mm, &val, &i), ==, CS_SUCCESS, "Failed to insert element into multimap");
        }
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multimap\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        // Only delete once since multimap_delete should remove all values for the key
        UNITTEST_ASSERT_SILENT(multimap_delete(mm, &key), ==, CS_SUCCESS, "Failed to delete existing element from multimap");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the multimap\n", total);

    UNITTEST_ASSERT(multimap_empty(mm), ==, 1, "Multimap is not empty after deleting all elements", arg->logger, "Multimap is empty after deletions\n");

    UNITTEST_ASSERT(multimap_delete(mm, &(double){123.45}), ==, CS_ELEM, "Deleted non-existent element from multimap", arg->logger, "Correctly failed to delete non-existent element\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multimap_delete_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, attr, get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &ts, &i), ==, CS_SUCCESS, "Failed to insert element into multimap");
        free_test_struct(&ts); // Free original struct since multimap should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the multimap\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        test_struct temp = create_test_struct(0, NULL, key); // Create temp struct with just the score for comparison
        UNITTEST_ASSERT_SILENT(multimap_delete(mm, &temp), ==, CS_SUCCESS, "Failed to delete existing element from multimap");
        free_test_struct(&temp);
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the multimap\n", total);

    UNITTEST_ASSERT(multimap_empty(mm), ==, 1, "Multimap is not empty after deleting all elements", arg->logger, "Multimap is empty after deletions\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_clear
// ============================================================================
test_res test_multimap_clear(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &val, &i), ==, CS_SUCCESS, "Failed to insert element into multimap");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multimap\n", total);

    multimap_clear(mm);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the multimap\n");

    UNITTEST_ASSERT(multimap_empty(mm), ==, 1, "Multimap is not empty after clear", arg->logger, "Multimap is empty after clear\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

test_res test_multimap_clear_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, attr, get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &ts, &i), ==, CS_SUCCESS, "Failed to insert element into multimap");
        free_test_struct(&ts); // Free original struct since multimap should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the multimap\n", total);

    multimap_clear(mm);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the multimap\n");

    UNITTEST_ASSERT(multimap_empty(mm), ==, 1, "Multimap is not empty after clear", arg->logger, "Multimap is empty after clear\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_get
// =============================================================================
test_res test_multimap_get(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_double_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000;
    int key = 42;
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &key, &val), ==, CS_SUCCESS, "Failed to insert element into multimap");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multimap\n", total);

    vector *vec = UNITTEST_ASSERT(multimap_get(mm, &key), !=, NULL, "Failed to get existing element from multimap", arg->logger, "Successfully retrieved vector for existing key\n");
    UNITTEST_ASSERT(vector_size(vec), ==, (size_t)total, "Vector size mismatch for existing key in multimap", arg->logger, "Vector size is correct for existing key\n");

    int not_found_key = 9999;
    UNITTEST_ASSERT(multimap_get(mm, &not_found_key), ==, NULL, "Multimap get mismatch for non-existent element", arg->logger, "Multimap get is correct for non-existent element\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_size
// ============================================================================
test_res test_multimap_size(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_double_attr(), get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    int total = 1000;
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &val, &i), ==, CS_SUCCESS, "Failed to insert element into multimap");
        UNITTEST_ASSERT_SILENT(multimap_size(mm), ==, i + 1, "Multimap size mismatch after insertion");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multimap and verified size after each insertion\n", total);

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_empty
// ============================================================================
test_res test_multimap_empty(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_double_attr(), get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    UNITTEST_ASSERT(multimap_empty(mm), ==, 1, "Newly initialized multimap is not empty", arg->logger, "Newly initialized multimap is empty\n");

    int val = 42;
    double key = 3.14;
    UNITTEST_ASSERT(multimap_insert(mm, &key, &val), ==, CS_SUCCESS, "Failed to insert element into multimap", arg->logger, "Inserted one element into the multimap\n");
    UNITTEST_ASSERT(multimap_empty(mm), ==, 0, "Multimap is empty after inserting an element", arg->logger, "Multimap is not empty after inserting an element\n");

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_print
// ============================================================================
test_res test_multimap_print(test_arg *arg) {
    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_double_attr(), get_int_attr()), !=, NULL, "Multimap initialization failed", arg->logger, "Multimap initialized successfully\n");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    multimap_print(stream, mm);
    fclose(stream);
    UNITTEST_ASSERT(buffer[0], ==, '\0', "Multimap print output is not empty for an empty multimap", arg->logger, "Multimap print output is empty for an empty multimap\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        int val = i * i;
        double key = i / 3.0 * i + 1.0;
        UNITTEST_ASSERT_SILENT(multimap_insert(mm, &key, &val), ==, CS_SUCCESS, "Failed to insert element into multimap");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the multimap\n", total);

    stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    multimap_print(stream, mm);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Multimap contents:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected[64];
        snprintf(expected, sizeof(expected), "%.2f", i / 3.0 * i + 1.0);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Multimap print output is missing expected key");
        snprintf(expected, sizeof(expected), "%d", i * i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Multimap print output is missing expected element");
    }

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// multimap_swap
// ============================================================================
test_res test_multimap_swap(test_arg *arg) {
    multimap *mm1 = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_int_attr()), !=, NULL, "Multimap 1 initialization failed", arg->logger, "Multimap 1 initialized successfully\n");
    multimap *mm2 = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_int_attr()), !=, NULL, "Multimap 2 initialization failed", arg->logger, "Multimap 2 initialized successfully\n");

    int total1 = 1000, total2 = 500;
    for (int i = 0; i < total1; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(multimap_insert(mm1, &val, &i), ==, CS_SUCCESS, "Failed to insert element into multimap 1");
    }
    for (int i = 0; i < total2; i++) {
        int val = i + 1000;
        UNITTEST_ASSERT_SILENT(multimap_insert(mm2, &val, &i), ==, CS_SUCCESS, "Failed to insert element into multimap 2");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into multimap 1 and %d elements into multimap 2\n", total1, total2);

    multimap_swap(mm1, mm2);

    UNITTEST_ASSERT(multimap_size(mm1), ==, total2, "Multimap 1 size mismatch after swap", arg->logger, "Multimap 1 size is correct after swap\n");
    UNITTEST_ASSERT(multimap_size(mm2), ==, total1, "Multimap 2 size mismatch after swap", arg->logger, "Multimap 2 size is correct after swap\n");

    for (int i = 0; i < total2; i++) {
        int key = i + 1000;
        vector *vec = UNITTEST_ASSERT_SILENT(multimap_get(mm1, &key), !=, NULL, "Failed to find element in multimap 1 after swap");
        UNITTEST_ASSERT_SILENT(vector_size(vec), ==, 1, "Vector size mismatch for existing key in multimap 1");
    }

    for (int i = 0; i < total1; i++) {
        int key = i;
        vector *vec = UNITTEST_ASSERT_SILENT(multimap_get(mm2, &key), !=, NULL, "Failed to find element in multimap 2 after swap");
        UNITTEST_ASSERT_SILENT(vector_size(vec), ==, 1, "Vector size mismatch for existing key in multimap 2");
    }

    multimap_free(mm1);
    multimap_free(mm2);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Stress test with timing
// ============================================================================
test_res test_multimap_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        return SUCCESSFUL_TEST_RES;
    }

    multimap *mm = UNITTEST_ASSERT(multimap_init(NULL, get_int_attr(), get_int_attr()), !=, NULL, "Multimap init failed", arg->logger, 
        "Successfully initialized multimap for stress test\n");

    struct timeval start, end;
    double elapsed;

    int total = __MULTIMAP_STRESS_TEST_SIZE;

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        int value = i;
        multimap_insert(mm, &key, &value);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Insert Time = %.9f sec\n", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        multimap_get(mm, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Find Time = %.9f sec\n", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        multimap_delete(mm, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "erase", elapsed);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Erase Time = %.9f sec\n", elapsed);

    multimap_free(mm);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Test array
// ============================================================================
test multimap_tests[] = {
    // multimap_init
    test_multimap_init,

    // multimap_insert
    test_multimap_insert,
    test_multimap_insert_duplicate,
    test_multimap_insert_deepcopy,

    // multimap_delete
    test_multimap_delete,
    test_multimap_delete_deepfree,

    // multimap_clear
    test_multimap_clear,
    test_multimap_clear_deepfree,

    // multimap_get
    test_multimap_get,

    // multimap_size
    test_multimap_size,

    // multimap_empty
    test_multimap_empty,

    // multimap_print
    test_multimap_print,

    // multimap_swap
    test_multimap_swap,

    // Stress tests
    test_multimap_stress_time,
};