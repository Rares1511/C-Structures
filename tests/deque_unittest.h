#include <cs/deque.h>
#include <unittest.h>

#include <sys/time.h>
#include <valgrind/valgrind.h>

// ============================================================================
// deque_init
// ============================================================================
test_res test_deque_init(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");
    
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_push_back
// ============================================================================
test_res test_deque_push_back(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int value = 42;
    UNITTEST_ASSERT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed",
        arg->logger, "Successfully pushed back value %d\n", value);
    
    int *retrieved = UNITTEST_ASSERT(deque_back(dq), !=, NULL, "Failed to retrieve back element after push back",
        arg->logger, "Successfully retrieved back element after push back\n");
    UNITTEST_ASSERT(*retrieved, ==, value, "Back element mismatch after push back",
        arg->logger, "Back element matches expected value %d\n", value);

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_push_back_grow(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque with small block size for growth testing\n");

    int total = dq->dq_attr.block_size + 10; // Push more than block size to trigger growth
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during growth test");
    }
    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_push_back_deepcopy(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_test_struct_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque for deepcopy testing\n");

    test_struct ts = create_test_struct(42, "Test", 3.14);
    UNITTEST_ASSERT(deque_push_back(dq, &ts), ==, CS_SUCCESS, "Push back failed for struct with deepcopy",
        arg->logger, "Successfully pushed back struct with deepcopy\n");
    free_test_struct(&ts); // Free original struct since deque should have made a copy

    test_struct *retrieved = UNITTEST_ASSERT(deque_back(dq), !=, NULL, "Failed to retrieve back element after push back with deepcopy",
        arg->logger, "Successfully retrieved back element after push back with deepcopy\n");
    UNITTEST_ASSERT(comp_test_struct_max(retrieved, &ts), ==, 0, "Retrieved struct does not match original after push back with deepcopy",
        arg->logger, "Retrieved struct matches original after push back with deepcopy\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// deque_push_front
// ============================================================================
test_res test_deque_push_front(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int value = 42;
    UNITTEST_ASSERT(deque_push_front(dq, &value), ==, CS_SUCCESS, "Push front failed",
        arg->logger, "Successfully pushed front value %d\n", value);

    int *retrieved = UNITTEST_ASSERT(deque_front(dq), !=, NULL, "Failed to retrieve front element after push front",
        arg->logger, "Successfully retrieved front element after push front\n");
    UNITTEST_ASSERT(*retrieved, ==, value, "Front element mismatch after push front",
        arg->logger, "Front element matches expected value %d\n", value);

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_push_front_grow(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque with small block size for growth testing\n");

    int total = dq->dq_attr.block_size + 10; // Push more than block size to trigger growth
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_front(dq, &value), ==, CS_SUCCESS, "Push front failed during growth test");
    }
    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_push_front_deepcopy(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_test_struct_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque for deepcopy testing\n");

    test_struct ts = create_test_struct(42, "Test", 3.14);
    UNITTEST_ASSERT(deque_push_front(dq, &ts), ==, CS_SUCCESS, "Push front failed for struct with deepcopy",
        arg->logger, "Successfully pushed front struct with deepcopy\n");
    free_test_struct(&ts); // Free original struct since deque should have made a copy

    test_struct *retrieved = UNITTEST_ASSERT(deque_front(dq), !=, NULL, "Failed to retrieve front element after push front with deepcopy",
        arg->logger, "Successfully retrieved front element after push front with deepcopy\n");
    UNITTEST_ASSERT(comp_test_struct_max(retrieved, &ts), ==, 0, "Retrieved struct does not match original after push front with deepcopy",
        arg->logger, "Retrieved struct matches original after push front with deepcopy\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_insert_at
// ============================================================================
test_res test_deque_insert_at(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &values[i]), ==, CS_SUCCESS, "Initial push back failed");
    }

    int new_value = 15;
    for (int i = 0; i <= dq->dq_attr.block_size; i++) {
        UNITTEST_ASSERT_SILENT(deque_insert_at(dq, &new_value, 2), ==, CS_SUCCESS, "Insert at failed");
        int *retrieved = UNITTEST_ASSERT_SILENT(deque_at(dq, 2), !=, NULL, "Failed to retrieve element at index 2 after insert");
        UNITTEST_ASSERT_SILENT(*retrieved, ==, new_value, "Element mismatch at index 2 after insert");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted value %d at every even index\n", new_value);

    int new_value_end = 25;
    for (int i = 0; i <= dq->dq_attr.block_size; i++) {
        UNITTEST_ASSERT_SILENT(deque_insert_at(dq, &new_value_end, dq->size - 2), ==, CS_SUCCESS, "Insert at end failed");
        int *retrieved = UNITTEST_ASSERT_SILENT(deque_at(dq, dq->size - 3), !=, NULL, "Failed to retrieve element at end after insert");
        UNITTEST_ASSERT_SILENT(*retrieved, ==, new_value_end, "Element mismatch at end after insert");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted value %d at every odd index\n", new_value_end);

    int new_value_front = 5;
    UNITTEST_ASSERT(deque_insert_at(dq, &new_value_front, 0), ==, CS_SUCCESS, "Insert at front failed",
        arg->logger, "Successfully inserted value %d at index 0\n", new_value_front);
    int *retrieved = UNITTEST_ASSERT(deque_at(dq, 0), !=, NULL, "Failed to retrieve element at index 0 after insert",
        arg->logger, "Successfully retrieved element at index 0 after insert\n");
    UNITTEST_ASSERT(*retrieved, ==, new_value_front, "Element mismatch at index 0 after insert",
        arg->logger, "Element at index 0 matches expected value %d\n", new_value_front);

    int new_value_back = 35;
    UNITTEST_ASSERT(deque_insert_at(dq, &new_value_back, dq->size), ==, CS_SUCCESS, "Insert at back failed",
        arg->logger, "Successfully inserted value %d at index %d\n", new_value_back, dq->size - 1);
    retrieved = UNITTEST_ASSERT(deque_at(dq, dq->size - 1), !=, NULL, "Failed to retrieve element at back after insert",
        arg->logger, "Successfully retrieved element at back after insert\n");
    UNITTEST_ASSERT(*retrieved, ==, new_value_back, "Element mismatch at back after insert",
        arg->logger, "Element at back matches expected value %d\n", new_value_back);

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

test_res test_deque_insert_at_deepcopy(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_test_struct_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque for deepcopy testing\n");

    test_struct ts;
    for (int i = 0; i < 10; i++) {
        ts = create_test_struct(i, "Test", 3.14 + i);
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &ts), ==, CS_SUCCESS, "Initial push back failed for struct with deepcopy");
        free_test_struct(&ts); // Free original struct since deque should have made a copy
    }

    int insert_index = 3;
    test_struct ts2 = create_test_struct(99, "Inserted", 9.99);
    UNITTEST_ASSERT(deque_insert_at(dq, &ts2, insert_index), ==, CS_SUCCESS, "Insert at failed for struct with deepcopy",
        arg->logger, "Successfully inserted struct at index %d with deepcopy\n", insert_index);
    free_test_struct(&ts2);

    test_struct *retrieved = UNITTEST_ASSERT(deque_at(dq, insert_index), !=, NULL, "Failed to retrieve element at index %d after insert with deepcopy",
        arg->logger, "Successfully retrieved element at index %d after insert with deepcopy\n", insert_index);
    UNITTEST_ASSERT(comp_test_struct_max(retrieved, &ts2), ==, 0, "Retrieved struct does not match original after insert with deepcopy",
        arg->logger, "Retrieved struct matches original after insert with deepcopy\n");

    int insert_index_end = dq->size - 2;
    test_struct ts3 = create_test_struct(88, "Inserted End", 8.88);
    UNITTEST_ASSERT(deque_insert_at(dq, &ts3, insert_index_end), ==, CS_SUCCESS, "Insert at end failed for struct with deepcopy",
        arg->logger, "Successfully inserted struct at index %d with deepcopy\n", insert_index_end);
    free_test_struct(&ts3);
    retrieved = UNITTEST_ASSERT(deque_at(dq, insert_index_end), !=, NULL, "Failed to retrieve element at index %d after insert at end with deepcopy",
        arg->logger, "Successfully retrieved element at index %d after insert at end with deepcopy\n", insert_index_end);
    UNITTEST_ASSERT(comp_test_struct_max(retrieved, &ts3), ==, 0, "Retrieved struct does not match original after insert at end with deepcopy",
        arg->logger, "Retrieved struct matches original after insert at end with deepcopy\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_pop_front
// ============================================================================
test_res test_deque_pop_front(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int num_values = 10;
    for (int i = 0; i < num_values; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Initial push back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque\n", num_values);

    for (int i = 0; i < num_values - 1; i++) {
        int expected = i;
        UNITTEST_ASSERT_SILENT(deque_pop_front(dq), ==, CS_SUCCESS, "Pop front failed");
        int *retrieved = UNITTEST_ASSERT_SILENT(deque_front(dq), !=, NULL, "Failed to retrieve front element after pop front");
        if (i < num_values - 1) {
            UNITTEST_ASSERT_SILENT(*retrieved, ==, expected + 1, "Front element mismatch after pop front");
        } else {
            UNITTEST_ASSERT_SILENT(retrieved, ==, NULL, "Expected deque to be empty after popping all elements");
        }
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully popped all elements from front and verified order\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_pop_front_shrink(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int total = dq->dq_attr.block_size / 2 + dq->dq_attr.block_size * 2; // Push more than block size to trigger growth
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque to trigger growth\n", total);

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_front(dq), ==, CS_SUCCESS, "Pop front failed during shrink test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped all values from front of deque to trigger shrink\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

test_res test_deque_pop_front_deepfree(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_test_struct_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque for deepcopy testing\n");

    test_struct ts;
    for (int i = 0; i < 10; i++) {
        ts = create_test_struct(i, "Test", 3.14 + i);
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &ts), ==, CS_SUCCESS, "Initial push back failed for struct with deepcopy");
        free_test_struct(&ts); // Free original struct since deque should have made a copy
    }

    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_front(dq), ==, CS_SUCCESS, "Pop front failed for struct with deepcopy");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully popped all elements from front of deque with deepcopy\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_pop_back
// ============================================================================
test_res test_deque_pop_back(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int num_values = 10;
    for (int i = 0; i < num_values; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Initial push back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque\n", num_values);

    for (int i = num_values - 1; i >= 0; i--) {
        int expected = i;
        UNITTEST_ASSERT_SILENT(deque_pop_back(dq), ==, CS_SUCCESS, "Pop back failed");
        if (i > 0) {
            int *retrieved = UNITTEST_ASSERT_SILENT(deque_back(dq), !=, NULL, "Failed to retrieve back element after pop back");
            UNITTEST_ASSERT_SILENT(*retrieved, ==, expected - 1, "Back element mismatch after pop back");
        } else {
            UNITTEST_ASSERT_SILENT(deque_back(dq), ==, NULL, "Expected deque to be empty after popping all elements");
        }
    }
    deque_free(dq);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully popped all elements from back and verified order\n");
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_pop_back_shrink(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int total = dq->dq_attr.block_size / 2 + dq->dq_attr.block_size * 2; // Push more than block size to trigger growth
    int front_total = dq->dq_attr.block_size / 2; // Leave some elements at front to ensure we test shrinking from back
    for (int i = 0; i < front_total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_front(dq, &value), ==, CS_SUCCESS, "Push front failed during growth test");
    }
    for (int i = 0; i < total - front_total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque to trigger growth\n", total);

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_back(dq), ==, CS_SUCCESS, "Pop back failed during shrink test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped all values from back of deque to trigger shrink\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

test_res test_deque_pop_back_deepfree(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_test_struct_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque for deepcopy testing\n");

    test_struct ts;
    for (int i = 0; i < 10; i++) {
        ts = create_test_struct(i, "Test", 3.14 + i);
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &ts), ==, CS_SUCCESS, "Initial push back failed for struct with deepcopy");
        free_test_struct(&ts); // Free original struct since deque should have made a copy
    }

    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_back(dq), ==, CS_SUCCESS, "Pop back failed for struct with deepcopy");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully popped all elements from back of deque with deepcopy\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_erase
// ============================================================================
test_res test_deque_erase(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int total = 100;
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Initial push back failed");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque\n", total);

    UNITTEST_ASSERT(deque_erase(dq, 0), ==, CS_SUCCESS, "Erase failed at index 5",
        arg->logger, "Successfully erased element at index 5\n");
    UNITTEST_ASSERT(deque_erase(dq, deque_size(dq) - 1), ==, CS_SUCCESS, "Erase failed at index 4",
        arg->logger, "Successfully erased element at index 4\n");
    UNITTEST_ASSERT(deque_erase(dq, deque_size(dq) / 2 - 1), ==, CS_SUCCESS, "Erase failed at index 2",
        arg->logger, "Successfully erased element at index 2\n");
    UNITTEST_ASSERT(deque_erase(dq, deque_size(dq) / 2 + 1), ==, CS_SUCCESS, "Expected erase to fail with out of bounds index",
        arg->logger, "Erase correctly failed with out of bounds index\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_empty
// ============================================================================
test_res test_deque_empty(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");
    
    UNITTEST_ASSERT(deque_empty(dq), ==, 1, "Expected deque to be empty after initialization",
        arg->logger, "Deque is empty after initialization\n");

    int value = 42;
    UNITTEST_ASSERT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed",
        arg->logger, "Successfully pushed back value %d\n", value);
    UNITTEST_ASSERT(deque_empty(dq), ==, 0, "Expected deque to not be empty after pushing an element",
        arg->logger, "Deque is not empty after pushing an element\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// deque_size
// ============================================================================
test_res test_deque_size(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int total = 100;
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during size test");
        UNITTEST_ASSERT_SILENT(dq->size, ==, i + 1, "Size mismatch after push back");
    } 
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque and verified size\n", total);

    for (int i = total - 1; i >= 0; i--) {
        UNITTEST_ASSERT_SILENT(deque_pop_back(dq), ==, CS_SUCCESS, "Pop back failed during size test");
        UNITTEST_ASSERT_SILENT(dq->size, ==, i, "Size mismatch after pop back");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped %d values from back of deque and verified size\n", total);

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_print
// ============================================================================
test_res test_deque_print(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int total = 5;
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during print test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque for print test\n", total);

    char buffer[256];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    deque_print(stream, dq);
    fprintf(stream, "\n"); // Ensure newline at end of print output
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Deque print output:\n%s", buffer);

    for (int i = 0; i < total; i++) {
        char expected[16];
        snprintf(expected, sizeof(expected), "%d", i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Expected value not found in deque print output");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Verified all expected values are present in deque print output\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_grow_internal
// ============================================================================
test_res test_deque_grow_internal_back(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque with small block size for growth testing\n");

    int total = dq->dq_attr.block_size * 100; // Push more than block size to trigger multiple growths
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque to trigger multiple growths\n", total);

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_back(dq), ==, CS_SUCCESS, "Pop back failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped all values from back of deque after growth\n");

    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_front(dq, &value), ==, CS_SUCCESS, "Push front failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to front of deque to trigger multiple growths\n", total);

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_front(dq), ==, CS_SUCCESS, "Pop front failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped all values from front of deque after growth\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_grow_internal_front(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque with small block size for growth testing\n");

    int total = dq->dq_attr.block_size * 100 + dq->dq_attr.block_size / 2; // Push more than block size to trigger multiple growths
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_front(dq, &value), ==, CS_SUCCESS, "Push front failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to front of deque to trigger multiple growths\n", total);

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_front(dq), ==, CS_SUCCESS, "Pop front failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped all values from front of deque after growth\n");

    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque to trigger multiple growths\n", total);

    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(deque_pop_back(dq), ==, CS_SUCCESS, "Pop back failed during growth test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped all values from back of deque after growth\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_clear
// ============================================================================
test_res test_deque_clear(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");

    int total = 100;
    for (int i = 0; i < total; i++) {
        int value = i;
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &value), ==, CS_SUCCESS, "Push back failed during clear test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed %d values to back of deque for clear test\n", total);

    deque_clear(dq);
    UNITTEST_ASSERT(deque_empty(dq), ==, 1, "Expected deque to be empty after clear",
        arg->logger, "Deque is empty after clear\n");
    UNITTEST_ASSERT(deque_front(dq), ==, NULL, "Expected front element to be NULL after clear", 
        arg->logger, "Front element is NULL after clear\n");
    UNITTEST_ASSERT(deque_back(dq), ==, NULL, "Expected back element to be NULL after clear", 
        arg->logger, "Back element is NULL after clear\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
}

test_res test_deque_clear_deepfree(test_arg *arg) {
    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_test_struct_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque for deepcopy testing\n");

    test_struct ts;
    for (int i = 0; i < 10; i++) {
        ts = create_test_struct(i, "Test", 3.14 + i);
        UNITTEST_ASSERT_SILENT(deque_push_back(dq, &ts), ==, CS_SUCCESS, "Initial push back failed for struct with deepcopy");
        free_test_struct(&ts); // Free original struct since deque should have made a copy
    }

    deque_clear(dq);
    UNITTEST_ASSERT(deque_empty(dq), ==, 1, "Expected deque to be empty after clear",
        arg->logger, "Deque is empty after clear\n");

    deque_free(dq);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// deque_swap
// ============================================================================
test_res test_deque_swap(test_arg *arg) {
    deque *dq1 = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque 1 initialization failed",
        arg->logger, "Successfully initialized deque 1\n");
    deque *dq2 = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque 2 initialization failed",
        arg->logger, "Successfully initialized deque 2\n");

    int values1[] = {1, 2, 3};
    int values2[] = {4, 5, 6, 7};
    for (int i = 0; i < 3; i++) {
        UNITTEST_ASSERT_SILENT(deque_push_back(dq1, &values1[i]), ==, CS_SUCCESS, "Push back failed for deque 1 during swap test");
    }
    for (int i = 0; i < 4; i++) {
        UNITTEST_ASSERT_SILENT(deque_push_back(dq2, &values2[i]), ==, CS_SUCCESS, "Push back failed for deque 2 during swap test");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Initialized deque 1 with 3 values and deque 2 with 4 values for swap test\n");

    deque_swap(dq1, dq2);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Swapped deques\n");

    UNITTEST_ASSERT_SILENT(deque_size(dq1), ==, 4, "Expected deque 1 to have size 4 after swap");
    UNITTEST_ASSERT_SILENT(deque_size(dq2), ==, 3, "Expected deque 2 to have size 3 after swap");
    for (int i = 0; i < 4; i++) {
        int *retrieved = UNITTEST_ASSERT_SILENT(deque_at(dq1, i), !=, NULL, "Failed to retrieve element from deque 1 after swap");
        UNITTEST_ASSERT_SILENT(*retrieved, ==, values2[i], "Element mismatch in deque 1 after swap");
    }
    for (int i = 0; i < 3; i++) {
        int *retrieved = UNITTEST_ASSERT_SILENT(deque_at(dq2, i), !=, NULL, "Failed to retrieve element from deque 2 after swap");
        UNITTEST_ASSERT_SILENT(*retrieved, ==, values1[i], "Element mismatch in deque 2 after swap");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Verified contents of both deques after swap\n");

    deque_free(dq1);
    deque_free(dq2);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// Stress test with time measurement
// ============================================================================

test_res test_deque_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping stress time test on Valgrind\n");
        return (test_res){(char*)__func__, "Skipped on Valgrind", CS_SUCCESS};
    }

    deque *dq = UNITTEST_ASSERT(deque_init(NULL, get_int_attr(), (deque_attr_t){0, 0}), !=, NULL, "Deque initialization failed",
        arg->logger, "Successfully initialized deque\n");
    struct timeval start, end;
    double elapsed_time;
    int total = __DEQUE_STRESS_TEST_SIZE;

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        deque_push_front(dq, &key);
    }
    gettimeofday(&end, NULL);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "insert_front", elapsed_time);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Completed %d push_fronts in %.4f seconds\n", total, elapsed_time);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        deque_pop_front(dq);
    }
    gettimeofday(&end, NULL);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "delete_front", elapsed_time);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Completed %d pop_fronts in %.4f seconds\n", total, elapsed_time);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        deque_push_back(dq, &key);
    }
    gettimeofday(&end, NULL);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "insert_back", elapsed_time);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Completed %d push_backs in %.4f seconds\n", total, elapsed_time);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        deque_pop_back(dq);
    }
    gettimeofday(&end, NULL);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "delete_back", elapsed_time);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Completed %d pop_backs in %.4f seconds\n", total, elapsed_time);
    
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test deque_tests[] = {
    // deque_init
    test_deque_init,

    // deque_push_back
    test_deque_push_back,
    test_deque_push_back_grow,
    test_deque_push_back_deepcopy,

    // deque_push_front
    test_deque_push_front,
    test_deque_push_front_grow,
    test_deque_push_front_deepcopy,

    // deque_insert_at
    test_deque_insert_at,
    test_deque_insert_at_deepcopy,

    // deque_pop_front
    test_deque_pop_front,
    test_deque_pop_front_shrink,
    test_deque_pop_front_deepfree,

    // deque_pop_back
    test_deque_pop_back,
    test_deque_pop_back_shrink,
    test_deque_pop_back_deepfree,

    // deque_grow_internal
    test_deque_grow_internal_back,
    test_deque_grow_internal_front,

    // deque_erase
    test_deque_erase,

    // deque_empty
    test_deque_empty,

    // deque_size
    test_deque_size,

    // deque_print
    test_deque_print,

    // deque_clear
    test_deque_clear,
    test_deque_clear_deepfree,

    // deque_swap
    test_deque_swap,

    // Stress test with time
    test_deque_stress_time,
};
