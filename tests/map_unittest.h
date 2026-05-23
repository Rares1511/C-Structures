#include <cs/map.h>

#include <unittest.h>

// ============================================================================
// map_init
// ============================================================================
test_res test_map_init(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_string_attr()), !=, NULL, "Map initialization failed",
        arg->logger, "Map initialization successful\n");

    UNITTEST_ASSERT(m->t, !=, NULL, "Map's internal tree initialization failed", arg->logger, "Map's internal tree initialization successful\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_insert
// ============================================================================
test_res test_map_insert(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_double_attr(), get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Value");
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(map_insert(m, &val, buffer), ==, CS_SUCCESS, "Failed to insert element into map");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the map\n", total);

    UNITTEST_ASSERT(map_size(m), ==, total, "Map size mismatch after insertions", arg->logger, "Map size is correct after insertions\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

test_res test_map_insert_duplicate(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_double_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int val = 42;
    double value = 3.14;
    UNITTEST_ASSERT(map_insert(m, &val, &value), ==, CS_SUCCESS, "Failed to insert element into map", arg->logger, "First insertion successful\n");
    UNITTEST_ASSERT(map_insert(m, &val, &value), ==, CS_ELEM, "Duplicate insertion did not return expected error code", arg->logger, "Duplicate insertion correctly returned error code\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

test_res test_map_insert_deepcopy(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    map *m = UNITTEST_ASSERT(map_init(NULL, attr, get_double_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(map_insert(m, &ts, &score), ==, CS_SUCCESS, "Failed to insert element into map");
        free_test_struct(&ts); // Free original struct since map should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the map\n", total);
    
    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_delete
// ============================================================================
test_res test_map_delete(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_double_attr(), get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    char value_buffer[64];
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(map_insert(m, &val, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the map\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(map_delete(m, &key), ==, CS_SUCCESS, "Failed to delete existing element from map");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the map\n", total);

    UNITTEST_ASSERT(map_empty(m), ==, 1, "Map is not empty after deleting all elements", arg->logger, "Map is empty after deletions\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

test_res test_map_delete_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    map *m = UNITTEST_ASSERT(map_init(NULL, attr, get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    char value_buffer[64];
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(map_insert(m, &ts, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map");
        free_test_struct(&ts); // Free original struct since map should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the map\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        test_struct temp = create_test_struct(0, NULL, key); // Create temp struct with just the score for comparison
        UNITTEST_ASSERT_SILENT(map_delete(m, &temp), ==, CS_SUCCESS, "Failed to delete existing element from map");
        free_test_struct(&temp);
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deleted %d elements from the map\n", total);

    UNITTEST_ASSERT(map_empty(m), ==, 1, "Map is not empty after deleting all elements", arg->logger, "Map is empty after deletions\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_clear
// ============================================================================
test_res test_map_clear(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_double_attr(), get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    char value_buffer[64];
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(map_insert(m, &val, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the map\n", total);

    map_clear(m);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the map\n");

    UNITTEST_ASSERT(map_empty(m), ==, 1, "Map is not empty after clear", arg->logger, "Map is empty after clear\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

test_res test_map_clear_deepfree(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    attr.comp = comp_test_struct_by_score;
    map *m = UNITTEST_ASSERT(map_init(NULL, attr, get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    char value_buffer[64];
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    double a = 1.5, b = 20.0, c = 5.0;
    for (int i = 0; i < total; i++) {
        double score = a * i * i - b * i + c;
        test_struct ts = create_test_struct(i, "Test", score);
        UNITTEST_ASSERT_SILENT(map_insert(m, &ts, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map");
        free_test_struct(&ts); // Free original struct since map should have made a copy
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d test_struct elements into the map\n", total);

    map_clear(m);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared the map\n");

    UNITTEST_ASSERT(map_empty(m), ==, 1, "Map is not empty after clear", arg->logger, "Map is empty after clear\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_find
// =============================================================================
test_res test_map_find(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_double_attr(), get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    char value_buffer[64];
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    double a = 2.3, b = 50.2, c = 11.7;
    for (int i = 0; i < total; i++) {
        double val = a * i * i - b * i + c;
        UNITTEST_ASSERT_SILENT(map_insert(m, &val, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the map\n", total);

    for (int i = 0; i < total; i++) {
        double key = a * i * i - b * i + c;
        char *found = UNITTEST_ASSERT_SILENT(map_find(m, &key), !=, NULL, "Failed to find existing element in map");
        UNITTEST_ASSERT_SILENT(strcmp(found, value_buffer), ==, 0, "Found value does not match expected value");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully found all %d elements in the set\n", total);

    double not_found_key = -12345.67;
    UNITTEST_ASSERT(map_find(m, &not_found_key), ==, NULL, "Finding non-existent element did not return NULL", arg->logger, "Finding non-existent element correctly returned NULL\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_size
// ============================================================================
test_res test_map_size(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    int total = 1000;
    char value_buffer[64];
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    for (int i = 0; i < total; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(map_insert(m, &val, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map");
        UNITTEST_ASSERT_SILENT(map_size(m), ==, i + 1, "Map size mismatch after insertion");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the map and verified size after each insertion\n", total);

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_empty
// ============================================================================
test_res test_map_empty(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    UNITTEST_ASSERT(map_empty(m), ==, 1, "Newly initialized map is not empty", arg->logger, "Newly initialized map is empty\n");

    char value_buffer[64];
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    int val = 42;
    UNITTEST_ASSERT(map_insert(m, &val, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map", arg->logger, "Inserted one element into the map\n");
    UNITTEST_ASSERT(map_empty(m), ==, 0, "Map is empty after inserting an element", arg->logger, "Map is not empty after inserting an element\n");

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_print
// ============================================================================
test_res test_map_print(test_arg *arg) {
    map *m = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_string_attr()), !=, NULL, "Map initialization failed", arg->logger, "Map initialized successfully\n");

    char buffer[1024], value_buffer[64];
    memset(buffer, 0, sizeof(buffer));
    snprintf(value_buffer, sizeof(value_buffer), "Value");
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    map_print(stream, m);
    fclose(stream);
    UNITTEST_ASSERT(buffer[0], ==, '\0', "Map print output is not empty for an empty map", arg->logger, "Map print output is empty for an empty map\n");

    int total = 10;
    for (int i = 0; i < total; i++) {
        int val = i * i;
        UNITTEST_ASSERT_SILENT(map_insert(m, &val, value_buffer), ==, CS_SUCCESS, "Failed to insert element into map");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into the map\n", total);

    stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Memory stream opened successfully for printing\n");
    map_print(stream, m);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Map contents:\n%s\n", buffer);

    for (int i = 0; i < total; i++) {
        char expected_line[64];
        snprintf(expected_line, sizeof(expected_line), "%d", i * i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected_line), !=, NULL, "Map print output is missing expected element");
    }

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// map_swap
// ============================================================================
test_res test_map_swap(test_arg *arg) {
    map *m1 = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_string_attr()), !=, NULL, "Map 1 initialization failed", arg->logger, "Map 1 initialized successfully\n");
    map *m2 = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_string_attr()), !=, NULL, "Map 2 initialization failed", arg->logger, "Map 2 initialized successfully\n");

    int total1 = 1000, total2 = 500;
    char value_buffer1[64], value_buffer2[64];
    snprintf(value_buffer1, sizeof(value_buffer1), "Value1");
    snprintf(value_buffer2, sizeof(value_buffer2), "Value2");
    for (int i = 0; i < total1; i++) {
        int val = i;
        UNITTEST_ASSERT_SILENT(map_insert(m1, &val, value_buffer1), ==, CS_SUCCESS, "Failed to insert element into map 1");
    }
    for (int i = 0; i < total2; i++) {
        int val = i + 1000;
        UNITTEST_ASSERT_SILENT(map_insert(m2, &val, value_buffer2), ==, CS_SUCCESS, "Failed to insert element into map 2");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements into map 1 and %d elements into map 2\n", total1, total2);

    map_swap(m1, m2);

    UNITTEST_ASSERT(map_size(m1), ==, total2, "Map 1 size mismatch after swap", arg->logger, "Map 1 size is correct after swap\n");
    UNITTEST_ASSERT(map_size(m2), ==, total1, "Map 2 size mismatch after swap", arg->logger, "Map 2 size is correct after swap\n");

    for (int i = 0; i < total2; i++) {
        int key = i + 1000;
        char *found = UNITTEST_ASSERT_SILENT(map_find(m1, &key), !=, NULL, "Failed to find element in map 1 after swap");
        UNITTEST_ASSERT_SILENT(strcmp(found, value_buffer2), ==, 0, "Incorrect element found in map 1 after swap");
    }

    for (int i = 0; i < total1; i++) {
        int key = i;
        char *found = UNITTEST_ASSERT_SILENT(map_find(m2, &key), !=, NULL, "Failed to find element in map 2 after swap");
        UNITTEST_ASSERT_SILENT(strcmp(found, value_buffer1), ==, 0, "Incorrect element found in map 2 after swap");
    }

    map_free(m1);
    map_free(m2);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// Stress test with timing
// ============================================================================
test_res test_map_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Valgrind detected - skipping stress timing test to avoid false positives.\n");
        return (test_res){(char*)__func__, "Valgrind active - skipping stress test", CS_SUCCESS};
    }

    map *m = UNITTEST_ASSERT(map_init(NULL, get_int_attr(), get_string_attr()), !=, NULL, "Map initialization failed",
        arg->logger, "Stress test map initialization successful\n");
    if (m == NULL) {
        return (test_res){(char*)__func__, "Map initialization failed", CS_UNKNOWN};
    }
    struct timeval start, end;
    char value_buffer[__MAX_PRINT_SIZE];
    double elapsed;

    snprintf(value_buffer, sizeof(value_buffer), "InitialValue");

    int total = __MAP_STRESS_TEST_SIZE;

    /* INSERT timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        map_insert(m, &key, value_buffer);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Insert Time = %.9f sec\n", elapsed);

    /* FIND timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        map_find(m, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Find Time = %.9f sec\n", elapsed);
    
    /* DELETE timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        map_delete(m, &key);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "delete", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed: Total Delete Time = %.9f sec\n", elapsed);

    map_free(m);
    return SUCCESSFUL_TEST_RES;
}

test map_tests[] = {
    // map_init
    test_map_init,

    // map_insert
    test_map_insert,
    test_map_insert_duplicate,
    test_map_insert_deepcopy,

    // map_delete
    test_map_delete,
    test_map_delete_deepfree,

    // map_clear
    test_map_clear,
    test_map_clear_deepfree,

    // map_find
    test_map_find,

    // map_size
    test_map_size,

    // map_empty
    test_map_empty,

    // map_print
    test_map_print,

    // map_swap
    test_map_swap,

    // Stress test with timing
    test_map_stress_time,
};