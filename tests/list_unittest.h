#include <cs/list.h>
#include <unittest.h>

#include <sys/time.h>
#include <valgrind/valgrind.h>

// ============================================================================
// list_init
// ============================================================================
test_res test_list_init(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", 
        arg->logger, "Successfully initialized list\n");
    
    UNITTEST_ASSERT(l->size, ==, 0, "Newly initialized list should have size 0", arg->logger, "List size is 0 as expected\n");
    
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_push_front
// ============================================================================
test_res test_list_push_front(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(list_push_front(l, &val1), ==, CS_SUCCESS, "Failed to push element to front of list", arg->logger, "Successfully pushed element to front of list\n");
    UNITTEST_ASSERT(list_push_front(l, &val2), ==, CS_SUCCESS, "Failed to push element to front of list", arg->logger, "Successfully pushed element to front of list\n");
    UNITTEST_ASSERT(list_size(l), ==, 2, "List size should be 2 after pushing two elements", arg->logger, "List size is 2 as expected\n");
    int *retrieved = UNITTEST_ASSERT(list_front(l), !=, NULL, "Front element should not be NULL after pushing an element", arg->logger, "Front element is not NULL as expected\n");
    UNITTEST_ASSERT(*retrieved, ==, val2, "Front element should be the value that was pushed", arg->logger, "Front element is correct\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

test_res test_list_push_front_deepcopy(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    test_struct val = create_test_struct(1, "Test", 99.9);
    UNITTEST_ASSERT(list_push_front(l, &val), ==, CS_SUCCESS, "Failed to push element to front of list", arg->logger, "Successfully pushed element to front of list\n");
    free_test_struct(&val);
    UNITTEST_ASSERT(list_size(l), ==, 1, "List size should be 1 after pushing one element", arg->logger, "List size is 1 as expected\n");
    test_struct *front = UNITTEST_ASSERT(list_front(l), !=, NULL, "Front element should not be NULL", arg->logger, "Front element is not NULL\n");
    UNITTEST_ASSERT(comp_test_struct_max(front, &val), ==, 0, "Front element should be equal to the value that was pushed", arg->logger, "Front element is correct\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_push_back
// ============================================================================
test_res test_list_push_back(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_size(l), ==, 2, "List size should be 2 after pushing two elements", arg->logger, "List size is 2 as expected\n");
    int *retrieved = UNITTEST_ASSERT(list_back(l), !=, NULL, "Back element should not be NULL after pushing an element", arg->logger, "Back element is not NULL as expected\n");
    UNITTEST_ASSERT(*retrieved, ==, val2, "Back element should be the value that was pushed", arg->logger, "Back element is correct\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

test_res test_list_push_back_deepcopy(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    test_struct val = create_test_struct(1, "Test", 99.9);
    UNITTEST_ASSERT(list_push_back(l, &val), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    free_test_struct(&val);
    UNITTEST_ASSERT(list_size(l), ==, 1, "List size should be 1 after pushing one element", arg->logger, "List size is 1 as expected\n");
    test_struct *back = UNITTEST_ASSERT(list_back(l), !=, NULL, "Back element should not be NULL", arg->logger, "Back element is not NULL\n");
    UNITTEST_ASSERT(comp_test_struct_max(back, &val), ==, 0, "Back element should be equal to the value that was pushed", arg->logger, "Back element is correct\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_pop_front
// ============================================================================
test_res test_list_pop_front(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_pop_front(l), ==, CS_SUCCESS, "Failed to pop element from front of list", arg->logger, "Successfully popped element from front of list\n");
    UNITTEST_ASSERT(list_pop_front(l), ==, CS_SUCCESS, "Failed to pop element from front of list", arg->logger, "Successfully popped element from front of list\n");
    UNITTEST_ASSERT(list_size(l), ==, 0, "List size should be 0 after popping the only element", arg->logger, "List size is 0 as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

test_res test_list_pop_front_deepfree(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    test_struct val1 = create_test_struct(1, "Test", 99.9);
    test_struct val2 = create_test_struct(2, "Test2", 88.8);
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    free_test_struct(&val1);
    free_test_struct(&val2);
    UNITTEST_ASSERT(list_pop_front(l), ==, CS_SUCCESS, "Failed to pop element from front of list", arg->logger, "Successfully popped element from front of list\n");
    UNITTEST_ASSERT(list_pop_front(l), ==, CS_SUCCESS, "Failed to pop element from front of list", arg->logger, "Successfully popped element from front of list\n");
    UNITTEST_ASSERT(list_size(l), ==, 0, "List size should be 0 after popping the only element", arg->logger, "List size is 0 as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_pop_back
// ============================================================================
test_res test_list_pop_back(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_pop_back(l), ==, CS_SUCCESS, "Failed to pop element from back of list", arg->logger, "Successfully popped element from back of list\n");
    UNITTEST_ASSERT(list_pop_back(l), ==, CS_SUCCESS, "Failed to pop element from back of list", arg->logger, "Successfully popped element from back of list\n");
    UNITTEST_ASSERT(list_size(l), ==, 0, "List size should be 0 after popping the only element", arg->logger, "List size is 0 as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

test_res test_list_pop_back_deepfree(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    test_struct val1 = create_test_struct(1, "Test", 99.9);
    test_struct val2 = create_test_struct(2, "Test2", 88.8);
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    free_test_struct(&val1);
    free_test_struct(&val2);
    UNITTEST_ASSERT(list_pop_back(l), ==, CS_SUCCESS, "Failed to pop element from back of list", arg->logger, "Successfully popped element from back of list\n");
    UNITTEST_ASSERT(list_pop_back(l), ==, CS_SUCCESS, "Failed to pop element from back of list", arg->logger, "Successfully popped element from back of list\n");
    UNITTEST_ASSERT(list_size(l), ==, 0, "List size should be 0 after popping the only element", arg->logger, "List size is 0 as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_erase
// ============================================================================
test_res test_list_erase(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int total = 5;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(list_push_back(l, &i), ==, CS_SUCCESS, "Failed to push element to back of list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d elements to back of list\n", total);
    UNITTEST_ASSERT(list_erase(l, 2), ==, CS_SUCCESS, "Failed to erase element at position 2", arg->logger, "Successfully erased element at position 2\n");
    UNITTEST_ASSERT(list_erase(l, 0), ==, CS_SUCCESS, "Failed to erase element at position 0", arg->logger, "Successfully erased element at position 0\n");
    UNITTEST_ASSERT(list_erase(l, list_size(l) - 1), ==, CS_SUCCESS, "Failed to erase last element", arg->logger, "Successfully erased last element\n");

    UNITTEST_ASSERT(list_size(l), ==, total - 3, "List size should be decreased by 3 after erasing three elements", arg->logger, "List size is correct after erasing elements\n");

    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

test_res test_list_erase_deepfree(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int total = 5;
    test_struct vals[total];
    for (int i = 0; i < total; i++) {
        vals[i] = create_test_struct(i, "Test", 99.9 - i);
        UNITTEST_ASSERT_SILENT(list_push_back(l, &vals[i]), ==, CS_SUCCESS, "Failed to push element to back of list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d elements to back of list\n", total);
    for (int i = 0; i < total; i++) {
        free_test_struct(&vals[i]);
    }
    UNITTEST_ASSERT(list_erase(l, 2), ==, CS_SUCCESS, "Failed to erase element at position 2", arg->logger, "Successfully erased element at position 2\n");
    UNITTEST_ASSERT(list_erase(l, 0), ==, CS_SUCCESS, "Failed to erase element at position 0", arg->logger, "Successfully erased element at position 0\n");
    UNITTEST_ASSERT(list_erase(l, list_size(l) - 1), ==, CS_SUCCESS, "Failed to erase last element", arg->logger, "Successfully erased last element\n");

    UNITTEST_ASSERT(list_size(l), ==, total - 3, "List size should be decreased by 3 after erasing three elements", arg->logger, "List size is correct after erasing elements\n");

    list_free(l);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// list_find
// ============================================================================
test_res test_list_find(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int total = 5;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(list_push_back(l, &i), ==, CS_SUCCESS, "Failed to push element to back of list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d elements to back of list\n", total);
    for (int i = 0; i < total; i++) {
        int pos = UNITTEST_ASSERT(list_find(l, &i), >=, 0, "Failed to find element in list", arg->logger, "Successfully found element in list\n");
        UNITTEST_ASSERT(pos, ==, i, "Element found at incorrect position", arg->logger, "Element found at correct position\n");
    }
    int not_found = total + 1;
    UNITTEST_ASSERT(list_find(l, &not_found), ==, -1, "Finding an element that doesn't exist should return -1", arg->logger, "Finding non-existent element correctly returned -1\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

test_res test_list_find_no_comp(test_arg *arg) {
    elem_attr_t attr = get_int_attr();
    attr.comp = NULL; // Remove compare function
    list *l = UNITTEST_ASSERT(list_init(NULL, attr), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int val = 42;
    UNITTEST_ASSERT(list_push_back(l, &val), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_find(l, &val), ==, 0, "Finding an element without a compare function should return 0 since int values are compared by value", arg->logger, "Finding element without compare function correctly returned 0\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// list_sort
// ============================================================================
test_res test_list_sort(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int vals[] = {5, 3, 4, 1, 2};
    int sorted_vals[] = {1, 2, 3, 4, 5};
    int total = sizeof(vals) / sizeof(vals[0]);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(list_push_back(l, &vals[i]), ==, CS_SUCCESS, "Failed to push element to back of list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d elements to back of list\n", total);
    list_sort(l);
    for (int i = 0; i < total; i++) {
        int *front = UNITTEST_ASSERT(list_front(l), !=, NULL, "Front element should not be NULL", arg->logger, "Front element is not NULL\n");
        UNITTEST_ASSERT(*front, ==, sorted_vals[i], "List is not sorted correctly", arg->logger, "List is sorted correctly\n");
        UNITTEST_ASSERT(list_pop_front(l), ==, CS_SUCCESS, "Failed to pop element from front of list", arg->logger, "Successfully popped element from front of list\n");
    }
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_swap
// ============================================================================
test_res test_list_swap(test_arg *arg) {
    list *l1 = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list 1", arg->logger, "Successfully initialized list 1\n");
    list *l2 = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list 2", arg->logger, "Successfully initialized list 2\n");

    int vals1[] = {1, 2, 3};
    int vals2[] = {4, 5};

    for (int i = 0; i < 3; i++) {
        UNITTEST_ASSERT_SILENT(list_push_back(l1, &vals1[i]), ==, CS_SUCCESS, "Failed to push element to back of list 1");
    }

    for (int i = 0; i < 2; i++) {
        UNITTEST_ASSERT_SILENT(list_push_back(l2, &vals2[i]), ==, CS_SUCCESS, "Failed to push element to back of list 2");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed elements to back of both lists\n");
    list_swap(l1, l2);

    UNITTEST_ASSERT(list_size(l1), ==, 2, "List 1 should have size 2 after swap", arg->logger, "List 1 has correct size after swap\n");
    UNITTEST_ASSERT(list_size(l2), ==, 3, "List 2 should have size 3 after swap", arg->logger, "List 2 has correct size after swap\n");

    int *front1 = UNITTEST_ASSERT(list_front(l1), !=, NULL, "Front element of list 1 should not be NULL", arg->logger, "Front element of list 1 is not NULL\n");
    UNITTEST_ASSERT(*front1, ==, vals2[0], "Front element of list 1 should be the first element of original list 2", arg->logger, "Front element of list 1 is correct\n");
    int *front2 = UNITTEST_ASSERT(list_front(l2), !=, NULL, "Front element of list 2 should not be NULL", arg->logger, "Front element of list 2 is not NULL\n");
    UNITTEST_ASSERT(*front2, ==, vals1[0], "Front element of list 2 should be the first element of original list 1", arg->logger, "Front element of list 2 is correct\n");

    list_free(l1);
    list_free(l2);

    return SUCCESSFUL_TEST_RES;
}


// ============================================================================
// list_clear
// ============================================================================
test_res test_list_clear(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int total = 5;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(list_push_back(l, &i), ==, CS_SUCCESS, "Failed to push element to back of list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d elements to back of list\n", total);
    list_clear(l);
    UNITTEST_ASSERT(list_size(l), ==, 0, "List size should be 0 after clearing the list", arg->logger, "List size is 0 as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
};

test_res test_list_clear_deepfree(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int total = 5;
    test_struct vals[total];
    for (int i = 0; i < total; i++) {
        vals[i] = create_test_struct(i, "Test", 99.9 - i);
        UNITTEST_ASSERT_SILENT(list_push_back(l, &vals[i]), ==, CS_SUCCESS, "Failed to push element to back of list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d elements to back of list\n", total);
    for (int i = 0; i < total; i++) {
        free_test_struct(&vals[i]);
    }
    list_clear(l);
    UNITTEST_ASSERT(list_size(l), ==, 0, "List size should be 0 after clearing the list", arg->logger, "List size is 0 as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_print
// ============================================================================
test_res test_list_print(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    int total = 5;
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(list_push_back(l, &i), ==, CS_SUCCESS, "Failed to push element to back of list");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully pushed %d elements to back of list\n", total);
    char buffer[256];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    UNITTEST_ASSERT(stream, !=, NULL, "Failed to open memory stream for printing", arg->logger, "Successfully opened memory stream for printing\n");
    list_print(stream, l);
    fclose(stream);
    clogger_log(arg->logger, CLOGGER_DEBUG, "Printed list output:\n%s", buffer);

    for (int i = 0; i < total; i++) {
        char expected[16];
        snprintf(expected, sizeof(expected), "%d", i);
        UNITTEST_ASSERT_SILENT(strstr(buffer, expected), !=, NULL, "Printed output should contain the list elements");
    }
    clogger_log(arg->logger, CLOGGER_DEBUG, "Printed output contains all list elements as expected\n");

    list_free(l);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// list_empty
// ============================================================================
test_res test_list_empty(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    UNITTEST_ASSERT(list_empty(l), ==, 1, "Newly initialized list should be empty", arg->logger, "List is empty as expected\n");
    int val = 42;
    UNITTEST_ASSERT(list_push_back(l, &val), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_empty(l), ==, 0, "List should not be empty after pushing an element", arg->logger, "List is not empty as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_size
// ============================================================================
test_res test_list_size(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    UNITTEST_ASSERT(list_size(l), ==, 0, "Newly initialized list should have size 0", arg->logger, "List size is 0 as expected\n");
    int val = 42;
    UNITTEST_ASSERT(list_push_back(l, &val), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_size(l), ==, 1, "List size should be 1 after pushing one element", arg->logger, "List size is 1 as expected\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_front
// ============================================================================
test_res test_list_front(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    UNITTEST_ASSERT(list_front(l), ==, NULL, "Front element of an empty list should be NULL", arg->logger, "Front element is NULL as expected\n");
    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    int *retrieved = UNITTEST_ASSERT(list_front(l), !=, NULL, "Front element should not be NULL after pushing an element", arg->logger, "Front element is not NULL as expected\n");
    UNITTEST_ASSERT(*retrieved, ==, val1, "Front element should be the value that was pushed", arg->logger, "Front element is correct\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_back
// ============================================================================
test_res test_list_back(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    UNITTEST_ASSERT(list_back(l), ==, NULL, "Back element of an empty list should be NULL", arg->logger, "Back element is NULL as expected\n");
    int val1 = 42, val2 = 84;
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");

    int *retrieved = UNITTEST_ASSERT(list_back(l), !=, NULL, "Back element should not be NULL after pushing an element", arg->logger, "Back element is not NULL as expected\n");
    UNITTEST_ASSERT(*retrieved, ==, val2, "Back element should be the value that was pushed", arg->logger, "Back element is correct\n");
    list_free(l);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// list_free
// ============================================================================
test_res test_list_free_deepfree(test_arg *arg) {
    list *l = UNITTEST_ASSERT(list_init(NULL, get_test_struct_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    test_struct val1 = create_test_struct(1, "Test", 99.9);
    test_struct val2 = create_test_struct(2, "Test2", 88.8);
    UNITTEST_ASSERT(list_push_back(l, &val1), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    UNITTEST_ASSERT(list_push_back(l, &val2), ==, CS_SUCCESS, "Failed to push element to back of list", arg->logger, "Successfully pushed element to back of list\n");
    free_test_struct(&val1);
    free_test_struct(&val2);
    list_free(l);
    return SUCCESSFUL_TEST_RES;
};

// ============================================================================
// Stress test with time measurement
// ============================================================================
test_res test_list_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND || arg->op_time_count == 0) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time-based stress test on Valgrind\n");
        return (test_res){(char*)__func__, "Skipped on Valgrind", CS_SUCCESS};
    }

    list *l = UNITTEST_ASSERT(list_init(NULL, get_int_attr()), !=, NULL, "Failed to initialize list", arg->logger, "Successfully initialized list\n");
    if (l == NULL) {
        return (test_res){(char*)__func__, "Failed to initialize list", CS_UNKNOWN};
    }
    struct timeval start, end;
    double elapsed;
    int total = __LIST_STRESS_TEST_SIZE, rc;

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        list_push_back(l, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "insert_back", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements in %.6f seconds\n", total, elapsed);

    gettimeofday(&start, NULL);
    int search_target = total / 2;
    if (list_find(l, &search_target) == -1) {
        list_free(l);
        return (test_res){(char*)__func__, "Failed to find existing element", CS_UNKNOWN};
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "find", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Found element %d in %.6f seconds\n", search_target, elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        list_pop_back(l);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "delete_back", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped %d elements in %.6f seconds\n", total, elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        list_push_front(l, &i);
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "insert_front", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted %d elements in %.6f seconds\n", total, elapsed);

    gettimeofday(&start, NULL);
    list_sort(l);
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "sort", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        rc = list_pop_front(l);
        if (rc != 0) {
            list_free(l);
            return (test_res){(char*)__func__, "Failed to pop element", CS_UNKNOWN};
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "delete_front", elapsed);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Popped %d elements in %.6f seconds\n", total, elapsed);

    list_free(l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test list_tests[] = {
    // list_init
    test_list_init,

    // list_push_front
    test_list_push_front,
    test_list_push_front_deepcopy,

    // list_push_back
    test_list_push_back,
    test_list_push_back_deepcopy,

    // list_pop_front
    test_list_pop_front,
    test_list_pop_front_deepfree,

    // list_pop_back
    test_list_pop_back,
    test_list_pop_back_deepfree,

    // list_erase
    test_list_erase,
    test_list_erase_deepfree,

    // list_find
    test_list_find,
    test_list_find_no_comp,

    // list_sort
    test_list_sort,

    // list_swap
    test_list_swap,

    // list_clear
    test_list_clear,
    test_list_clear_deepfree,

    // list_print
    test_list_print,

    // list_empty
    test_list_empty,

    // list_size
    test_list_size,

    // list_front
    test_list_front,

    // list_back
    test_list_back,

    // list_free
    test_list_free_deepfree,

    // Stress test with time
    test_list_stress_time,
};