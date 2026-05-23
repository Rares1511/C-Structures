#include <cs/forward_list.h>
#include <unittest.h>

// ============================================================================
// forward_list_init
// ============================================================================
test_res test_forward_list_init(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    UNITTEST_ASSERT(fl->header.magic, ==, CS_FORWARD_LIST_MAGIC, "Invalid magic number in forward list header", 
        arg->logger, "Magic number in forward list header is correct\n");
    UNITTEST_ASSERT(fl->size, ==, 0, "Newly initialized forward list should have size 0", 
        arg->logger, "Newly initialized forward list has size 0\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES; 
};

// ============================================================================
// forward_list_push_front
// ============================================================================
test_res test_forward_list_push_front(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    int value1 = 42, value2 = 84;
    UNITTEST_ASSERT(forward_list_push_front(fl, &value1), ==, CS_SUCCESS, "Failed to push element to forward list", 
        arg->logger, "Successfully pushed first element to forward list\n");
    UNITTEST_ASSERT(fl->size, ==, 1, "Forward list size should be 1 after pushing one element", 
        arg->logger, "Forward list size is 1 after pushing one element\n");

    UNITTEST_ASSERT(forward_list_push_front(fl, &value2), ==, CS_SUCCESS, "Failed to push second element to forward list", 
        arg->logger, "Successfully pushed second element to forward list\n");
    UNITTEST_ASSERT(fl->size, ==, 2, "Forward list size should be 2 after pushing two elements", 
        arg->logger, "Forward list size is 2 after pushing two elements\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

test_res test_forward_list_push_front_deepcopy(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize forward list with test_struct attributes", 
        arg->logger, "Successfully initialized forward list with test_struct attributes\n");

    test_struct ts1 = create_test_struct(42, "Hello", 3.14);
    test_struct ts2 = create_test_struct(84, "World", 6.28);
    UNITTEST_ASSERT(forward_list_push_front(fl, &ts1), ==, CS_SUCCESS, "Failed to push test_struct element to forward list", 
        arg->logger, "Successfully pushed test_struct element to forward list\n");
    free_test_struct(&ts1); // Free original since it should be deep-copied
    UNITTEST_ASSERT(fl->size, ==, 1, "Forward list size should be 1 after pushing one test_struct element", 
        arg->logger, "Forward list size is 1 after pushing one test_struct element\n");

    UNITTEST_ASSERT(forward_list_push_front(fl, &ts2), ==, CS_SUCCESS, "Failed to push second test_struct element to forward list", 
        arg->logger, "Successfully pushed second test_struct element to forward list\n");
    free_test_struct(&ts2); // Free original since it should be deep-copied
    UNITTEST_ASSERT(fl->size, ==, 2, "Forward list size should be 2 after pushing two test_struct elements", 
        arg->logger, "Forward list size is 2 after pushing two test_struct elements\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// forward_list_pop_front
// ============================================================================
test_res test_forward_list_pop_front(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    int value1 = 42, value2 = 84;
    UNITTEST_ASSERT(forward_list_push_front(fl, &value1), ==, CS_SUCCESS, "Failed to push element to forward list", 
        arg->logger, "Successfully pushed first element to forward list\n");
    UNITTEST_ASSERT(forward_list_push_front(fl, &value2), ==, CS_SUCCESS, "Failed to push second element to forward list", 
        arg->logger, "Successfully pushed second element to forward list\n");

    UNITTEST_ASSERT(forward_list_pop_front(fl), ==, CS_SUCCESS, "Failed to pop element from forward list", 
        arg->logger, "Successfully popped element from forward list\n");
    UNITTEST_ASSERT(fl->size, ==, 1, "Forward list size should be 1 after popping one element", 
        arg->logger, "Forward list size is 1 after popping one element\n");
    int *front = UNITTEST_ASSERT(forward_list_front(fl), !=, NULL, "Failed to get front element from forward list", 
        arg->logger, "Successfully got front element from forward list\n");
    UNITTEST_ASSERT(*front, ==, value1, "Front element should be the first pushed element after popping the second element", 
        arg->logger, "Front element is correct after popping the second element\n");

    UNITTEST_ASSERT(forward_list_pop_front(fl), ==, CS_SUCCESS, "Failed to pop second element from forward list", 
        arg->logger, "Successfully popped second element from forward list\n");
    UNITTEST_ASSERT(fl->size, ==, 0, "Forward list size should be 0 after popping all elements", 
        arg->logger, "Forward list size is 0 after popping all elements\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

test_res test_forward_list_pop_front_deepfree(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize forward list with test_struct attributes", 
        arg->logger, "Successfully initialized forward list with test_struct attributes\n");

    test_struct ts1 = create_test_struct(42, "Hello", 3.14);
    test_struct ts2 = create_test_struct(84, "World", 6.28);
    UNITTEST_ASSERT(forward_list_push_front(fl, &ts1), ==, CS_SUCCESS, "Failed to push test_struct element to forward list", 
        arg->logger, "Successfully pushed test_struct element to forward list\n");
    UNITTEST_ASSERT(forward_list_push_front(fl, &ts2), ==, CS_SUCCESS, "Failed to push second test_struct element to forward list", 
        arg->logger, "Successfully pushed second test_struct element to forward list\n");
    free_test_struct(&ts1); // Free originals since they should be deep-copied
    free_test_struct(&ts2);

    UNITTEST_ASSERT(forward_list_pop_front(fl), ==, CS_SUCCESS, "Failed to pop element from forward list", 
        arg->logger, "Successfully popped element from forward list\n");
    UNITTEST_ASSERT(fl->size, ==, 1, "Forward list size should be 1 after popping one element", 
        arg->logger, "Forward list size is 1 after popping one element\n");

    UNITTEST_ASSERT(forward_list_pop_front(fl), ==, CS_SUCCESS, "Failed to pop second element from forward list", 
        arg->logger, "Successfully popped second element from forward list\n");
    UNITTEST_ASSERT(fl->size, ==, 0, "Forward list size should be 0 after popping all elements", 
        arg->logger, "Forward list size is 0 after popping all elements\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// forward_list_find
// ============================================================================
test_res test_forward_list_find(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(forward_list_push_front(fl, &values[i]), ==, CS_SUCCESS, "Failed to push element to forward list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed elements 10, 20, 30, 40, 50 to forward list\n");

    for (int i = 0; i < 5; i++) {
        int target = values[i];
        size_t index = forward_list_find(fl, &target);
        UNITTEST_ASSERT_SILENT(index, !=, fl->size, "Failed to find existing element in forward list");
        UNITTEST_ASSERT_SILENT(index, ==, (size_t)(4 - i), "Element found at incorrect index in forward list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully found all existing elements in forward list\n");

    int not_found = 60;
    size_t index = forward_list_find(fl, &not_found);
    UNITTEST_ASSERT(index, ==, fl->size, "Should not find non-existing element in forward list", 
        arg->logger, "Correctly did not find non-existing element in forward list\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

test_res test_forward_list_find_no_comp(test_arg *arg) {
    elem_attr_t attr = get_int_attr();
    attr.comp = NULL; // Remove the compare function to test fallback to memcmp
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, attr), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(forward_list_push_front(fl, &values[i]), ==, CS_SUCCESS, "Failed to push element to forward list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed elements 10, 20, 30, 40, 50 to forward list\n");

    // Temporarily remove the compare function
    forward_list_set_compare(fl, NULL);

    for (int i = 0; i < 5; i++) {
        int target = values[i];
        size_t index = forward_list_find(fl, &target);
        UNITTEST_ASSERT_SILENT(index, !=, fl->size, "Failed to find existing element in forward list without compare function");
        UNITTEST_ASSERT_SILENT(index, ==, (size_t)(4 - i), "Element found at incorrect index in forward list without compare function");
    }

    int not_found = 60;
    size_t index = forward_list_find(fl, &not_found);
    UNITTEST_ASSERT(index, ==, fl->size, "Should not find non-existing element in forward list without compare function", 
        arg->logger, "Correctly did not find non-existing element in forward list without compare function\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// forward_list_sort
// ============================================================================
test_res test_forward_list_sort(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    int values[] = {91, 22, 104, 34, 20, 50, 10, 40, 30, 60};
    int sorted_values[] = {10, 20, 22, 30, 34, 40, 50, 60, 91, 104};
    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(forward_list_push_front(fl, &values[i]), ==, CS_SUCCESS, "Failed to push element to forward list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed elements 91, 22, 104, 34, 20, 50, 10, 40, 30, 60 to forward list\n");

    forward_list_sort(fl);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Sorted forward list\n");

    for (int i = 0; i < 10; i++) {
        int *front = UNITTEST_ASSERT_SILENT(forward_list_front(fl), !=, NULL, "Failed to get front element from forward list after sorting");
        UNITTEST_ASSERT_SILENT(*front, ==, sorted_values[i], "Elements are not in correct order after sorting");
        UNITTEST_ASSERT_SILENT(forward_list_pop_front(fl), ==, CS_SUCCESS, "Failed to pop element from forward list during verification");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Verified sorted order of elements in forward list\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// forward_list_swap
// ============================================================================
test_res test_forward_list_swap(test_arg *arg) {
    forward_list *fl1 = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize first forward list", 
        arg->logger, "Successfully initialized first forward list\n");
    forward_list *fl2 = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize second forward list", 
        arg->logger, "Successfully initialized second forward list\n");

    int values1[] = {1, 2, 3};
    int values2[] = {4, 5};
    for (int i = 0; i < 3; i++) {
        UNITTEST_ASSERT_SILENT(forward_list_push_front(fl1, &values1[i]), ==, CS_SUCCESS, "Failed to push element to first forward list");
    }
    for (int i = 0; i < 2; i++) {
        UNITTEST_ASSERT_SILENT(forward_list_push_front(fl2, &values2[i]), ==, CS_SUCCESS, "Failed to push element to second forward list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed elements to both forward lists\n");

    forward_list_swap(fl1, fl2);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Swapped forward lists\n");

    UNITTEST_ASSERT(forward_list_size(fl1), ==, 2, "First forward list should have size 2 after swap", 
        arg->logger, "First forward list has correct size after swap\n");   
    UNITTEST_ASSERT(forward_list_size(fl2), ==, 3, "Second forward list should have size 3 after swap", 
        arg->logger, "Second forward list has correct size after swap\n");

    // Verify fl1 now has elements of fl2
    for (int i = 0; i < 2; i++) {
        int *front = UNITTEST_ASSERT_SILENT(forward_list_front(fl1), !=, NULL, "Failed to get front element from first forward list after swap");
        UNITTEST_ASSERT_SILENT(*front, ==, values2[1 - i], "First forward list does not have correct elements after swap");
        UNITTEST_ASSERT_SILENT(forward_list_pop_front(fl1), ==, CS_SUCCESS, "Failed to pop element from first forward list during verification");
    }

    // Verify fl2 now has elements of fl1
    for (int i = 0; i < 3; i++) {
        int *front = UNITTEST_ASSERT_SILENT(forward_list_front(fl2), !=, NULL, "Failed to get front element from second forward list after swap");
        UNITTEST_ASSERT_SILENT(*front, ==, values1[2 - i], "Second forward list does not have correct elements after swap");
        UNITTEST_ASSERT_SILENT(forward_list_pop_front(fl2), ==, CS_SUCCESS, "Failed to pop element from second forward list during verification");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Verified contents of both forward lists after swap\n");

    forward_list_free(fl1);
    forward_list_free(fl2);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// forward_list_clear
// ============================================================================
test_res test_forward_list_clear(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        UNITTEST_ASSERT_SILENT(forward_list_push_front(fl, &values[i]), ==, CS_SUCCESS, "Failed to push element to forward list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed elements 10, 20, 30 to forward list\n");    

    forward_list_clear(fl);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared forward list\n");

    UNITTEST_ASSERT(forward_list_size(fl), ==, 0, "Forward list size should be 0 after clearing", 
        arg->logger, "Forward list size is 0 after clearing\n");
    UNITTEST_ASSERT(forward_list_empty(fl), ==, 1, "Forward list should be empty after clearing", 
        arg->logger, "Forward list is empty after clearing\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

test_res test_forward_list_clear_deepfree(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize forward list with test_struct attributes", 
        arg->logger, "Successfully initialized forward list with test_struct attributes\n");

    test_struct ts1 = create_test_struct(42, "Hello", 3.14);
    test_struct ts2 = create_test_struct(84, "World", 6.28);
    UNITTEST_ASSERT(forward_list_push_front(fl, &ts1), ==, CS_SUCCESS, "Failed to push test_struct element to forward list", 
        arg->logger, "Successfully pushed test_struct element to forward list\n");
    UNITTEST_ASSERT(forward_list_push_front(fl, &ts2), ==, CS_SUCCESS, "Failed to push second test_struct element to forward list", 
        arg->logger, "Successfully pushed second test_struct element to forward list\n");
    free_test_struct(&ts1); // Free originals since they should be deep-copied
    free_test_struct(&ts2);

    forward_list_clear(fl);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Cleared forward list with test_struct elements\n");

    UNITTEST_ASSERT(forward_list_size(fl), ==, 0, "Forward list size should be 0 after clearing", 
        arg->logger, "Forward list size is 0 after clearing\n");
    UNITTEST_ASSERT(forward_list_empty(fl), ==, 1, "Forward list should be empty after clearing", 
        arg->logger, "Forward list is empty after clearing\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// forward_list_print
// ============================================================================
test_res test_forward_list_print(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        UNITTEST_ASSERT_SILENT(forward_list_push_front(fl, &values[i]), ==, CS_SUCCESS, "Failed to push element to forward list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Pushed elements 10, 20, 30 to forward list\n");

    clogger_log(arg->logger, CLOGGER_DEBUG, "Printing forward list contents:\n");
    char buffer[256];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    forward_list_print(stream, fl);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Got string: %s", buffer);

    for (int i = 0; i < 3; i++) {
        char expected[16];
        snprintf(expected, sizeof(expected), "%d", values[2 - i]);
        UNITTEST_ASSERT(strstr(buffer, expected) != NULL, ==, 1, "Printed output should contain all elements in forward list", 
            arg->logger, "Printed output contains all elements in forward list\n");
    }

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// forward_list_empty
// ============================================================================
test_res test_forward_list_empty(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");
    
    UNITTEST_ASSERT(forward_list_empty(fl), ==, 1, "Newly initialized forward list should be empty", 
        arg->logger, "Newly initialized forward list is empty\n");

    int value = 42;
    UNITTEST_ASSERT(forward_list_push_front(fl, &value), ==, CS_SUCCESS, "Failed to push element to forward list", 
        arg->logger, "Successfully pushed element to forward list\n");
    UNITTEST_ASSERT(forward_list_empty(fl), ==, 0, "Forward list should not be empty after adding an element", 
        arg->logger, "Forward list is not empty after adding an element\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// forward_list_size
// ============================================================================
test_res test_forward_list_size(test_arg *arg) {
    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");

    UNITTEST_ASSERT(forward_list_size(fl), ==, 0, "Newly initialized forward list should have size 0", 
        arg->logger, "Newly initialized forward list has size 0\n");

    int value1 = 42, value2 = 84;
    UNITTEST_ASSERT(forward_list_push_front(fl, &value1), ==, CS_SUCCESS, "Failed to push element to forward list", 
        arg->logger, "Successfully pushed first element to forward list\n");
    UNITTEST_ASSERT(forward_list_size(fl), ==, 1, "Forward list size should be 1 after pushing one element", 
        arg->logger, "Forward list size is 1 after pushing one element\n");

    UNITTEST_ASSERT(forward_list_push_front(fl, &value2), ==, CS_SUCCESS, "Failed to push second element to forward list", 
        arg->logger, "Successfully pushed second element to forward list\n");
    UNITTEST_ASSERT(forward_list_size(fl), ==, 2, "Forward list size should be 2 after pushing two elements", 
        arg->logger, "Forward list size is 2 after pushing two elements\n");

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// Stress test with time measurement
// ============================================================================
test_res test_forward_list_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time-based stress test on Valgrind\n");
        return (test_res){(char*)__func__, NULL, CS_SUCCESS};
    }

    forward_list *fl = UNITTEST_ASSERT(forward_list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize forward list", 
        arg->logger, "Successfully initialized forward list\n");
    struct timeval start, end;
    int total = __FORWARD_LIST_STRESS_TEST_SIZE;
    double elapsed;
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        forward_list_push_front(fl, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Time taken to insert %d elements: %.6f seconds\n", total, elapsed);

    gettimeofday(&start, NULL);
    int search_target = total / 2;
    forward_list_find(fl, &search_target);
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    forward_list_sort(fl);
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "sort", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        forward_list_pop_front(fl);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "delete", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Time taken to pop %d elements: %.6f seconds\n", total, elapsed);

    forward_list_free(fl);
    return SUCCESSFUL_TEST_RES;
}

test forward_list_tests[] = {
    // forward_list_init
    test_forward_list_init,

    // forward_list_push_front
    test_forward_list_push_front,
    test_forward_list_push_front_deepcopy,

    // forward_list_pop_front
    test_forward_list_pop_front,
    test_forward_list_pop_front_deepfree,

    // forward_list_find
    test_forward_list_find,
    test_forward_list_find_no_comp,

    // forward_list_sort
    test_forward_list_sort,

    // forward_list_swap
    test_forward_list_swap,

    // forward_list_clear
    test_forward_list_clear,
    test_forward_list_clear_deepfree,

    // forward_list_print
    test_forward_list_print,

    // forward_list_empty
    test_forward_list_empty,

    // forward_list_size
    test_forward_list_size,

    // Stress test with time
    test_forward_list_stress_time,
};