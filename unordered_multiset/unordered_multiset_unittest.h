#include <cs/unordered_multiset.h>

#include <unittest.h>
#include <string.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_multiset_init_basic(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_init_invalid_size(test_arg *arg) {
    elem_attr_t attr = {
        .size = 0, // Invalid size
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };
    UNITTEST_ASSERT(unordered_multiset_init(attr, hash_int), ==, NULL, "Expected NULL for zero element size", 
        arg->logger, "Successfully handled zero element size");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_init_null_hash(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), NULL), !=, NULL, "Failed to initialize unordered_multiset with NULL hash", 
        arg->logger, "Successfully initialized unordered_multiset with NULL hash");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              INSERT TESTS                                  */
/******************************************************************************/

test_res test_unordered_multiset_insert_basic(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");

    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_insert_multiple(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 100; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted 100 elements into unordered_multiset");

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 100, "Size mismatch after multiple inserts", 
        arg->logger, "Size is correct after multiple inserts = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_insert_duplicates(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    
    int val = 42;
    // Insert same value multiple times - multiset should allow this
    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert duplicate element");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted 5 duplicates into unordered_multiset");

    // Size should be 5 (multiset allows duplicates)
    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 5, "Size mismatch after inserting duplicates", 
        arg->logger, "Size is correct after inserting duplicates = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_insert_null_set(test_arg *arg) {
    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(NULL, &val), ==, CS_NULL, "Expected CS_NULL for NULL set", 
        arg->logger, "Successfully handled NULL set on insert");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_insert_null_key(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
   
    UNITTEST_ASSERT(unordered_multiset_insert(umset, NULL), ==, CS_NULL, "Expected CS_NULL for NULL key", 
        arg->logger, "Successfully handled NULL key on insert");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              FIND TESTS                                    */
/******************************************************************************/

test_res test_unordered_multiset_find_existing(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");

    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");

    void *found = UNITTEST_ASSERT(unordered_multiset_find(umset, &val), !=, NULL, "Failed to find existing element", 
        arg->logger, "Successfully found existing element");

    UNITTEST_ASSERT(*(int *)found, ==, 42, "Found element value mismatch", 
        arg->logger, "Found element value is correct = %d", *(int *)found);

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_find_nonexistent(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");

    int search = 99;
    UNITTEST_ASSERT(unordered_multiset_find(umset, &search), ==, NULL, "Should not find nonexistent element", 
        arg->logger, "Correctly did not find nonexistent element");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_find_null_key(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    UNITTEST_ASSERT(unordered_multiset_find(umset, NULL), ==, NULL, "Expected NULL for NULL key", 
        arg->logger, "Successfully handled NULL key on find");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_find_after_many_inserts(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 100; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    // Find all inserted elements
    for (int i = 0; i < 100; i++) {
        void *found = UNITTEST_ASSERT_SILENT(unordered_multiset_find(umset, &i), !=, NULL, "Failed to find element after many inserts");
        UNITTEST_ASSERT_SILENT(*(int *)found, ==, i, "Found element value mismatch after many inserts");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully found all elements after many inserts");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              ERASE TESTS                                   */
/******************************************************************************/

test_res test_unordered_multiset_erase_existing(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");

    UNITTEST_ASSERT(unordered_multiset_erase(umset, &val), ==, CS_SUCCESS, "Failed to delete element", 
        arg->logger, "Successfully deleted element");

    // Verify element is gone
    UNITTEST_ASSERT(unordered_multiset_find(umset, &val), ==, NULL, "Element should not be found after erase", 
        arg->logger, "Successfully verified element is not found after erase");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_erase_one_of_many(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    
    int val = 42;
    // Insert 5 duplicates
    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element");
    }

    // Erase one
    UNITTEST_ASSERT(unordered_multiset_erase(umset, &val), ==, CS_SUCCESS, "Failed to erase element", 
        arg->logger, "Successfully erased one instance of duplicate element");

    // Should still have 4 elements
    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 4, "Size should be 4 after erasing one duplicate", 
        arg->logger, "Size is correct after erasing one duplicate = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_erase_nonexistent(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    
    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_erase(umset, &val), !=, CS_SUCCESS, "Failed to report nonexistent element", 
        arg->logger, "Successfully verified element is not found for erase");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_erase_null_set(test_arg *arg) {
    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_erase(NULL, &val), ==, CS_NULL, "Expected CS_NULL for NULL set", 
        arg->logger, "Successfully handled NULL set on erase");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_erase_null_key(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    
    UNITTEST_ASSERT(unordered_multiset_erase(umset, NULL), ==, CS_NULL, "Expected CS_NULL for NULL key", 
        arg->logger, "Successfully handled NULL key on erase");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_erase_all(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 50; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    // Erase all elements
    for (int i = 0; i < 50; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_erase(umset, &i), ==, CS_SUCCESS, "Failed to erase element");
    }

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 0, "Size should be 0 after erasing all elements", 
        arg->logger, "Size is correct after erasing all elements = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              COUNT TESTS                                   */
/******************************************************************************/

test_res test_unordered_multiset_count_zero(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_count(umset, &val), ==, 0, "Count should be 0 for nonexistent element", 
        arg->logger, "Successfully verified count is 0 for nonexistent element");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_count_single(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    
    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");
    
    UNITTEST_ASSERT(unordered_multiset_count(umset, &val), ==, 1, "Count should be 1 for single element", 
        arg->logger, "Successfully verified count is 1 for single element");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_count_multiple(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    

    int val = 42;
    for (int i = 0; i < 7; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element");
    }

    UNITTEST_ASSERT(unordered_multiset_count(umset, &val), ==, 7, "Count should be 7 for 7 duplicates", 
        arg->logger, "Successfully verified count is 7 for 7 duplicates");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_count_null_key(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    
    UNITTEST_ASSERT(unordered_multiset_count(umset, NULL), ==, 0, "Expected count of 0 for NULL key", 
        arg->logger, "Successfully handled NULL key on count");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_count_after_erase(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element");
    }

    // Erase 2
    UNITTEST_ASSERT(unordered_multiset_erase(umset, &val), ==, CS_SUCCESS, "Failed to erase element", 
        arg->logger, "Successfully erased element");
    UNITTEST_ASSERT(unordered_multiset_erase(umset, &val), ==, CS_SUCCESS, "Failed to erase element", 
        arg->logger, "Successfully erased element");

    UNITTEST_ASSERT(unordered_multiset_count(umset, &val), ==, 3, "Count should be 3 after erasing 2 from 5", 
        arg->logger, "Successfully verified count is correct after erasing duplicates");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_unordered_multiset_size_empty(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    
    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 0, "Size should be 0 for empty set", 
        arg->logger, "Size is correct for empty set = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_size_after_inserts(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 25; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 25, "Size should be 25 after 25 inserts", 
        arg->logger, "Size is correct after inserts = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_size_after_erase(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_erase(umset, &i), ==, CS_SUCCESS, "Failed to erase element");
    }

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 5, "Size should be 5 after erasing 5 elements", 
        arg->logger, "Size is correct after erasing elements = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_empty_true(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 1, "Newly created set should be empty", 
        arg->logger, "Successfully verified new set is empty");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_empty_false(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");

    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 0, "Set with element should not be empty", 
        arg->logger, "Successfully verified set with element is not empty");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_empty_after_erase_all(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_erase(umset, &i), ==, CS_SUCCESS, "Failed to erase element");
    }

    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 1, "Set should be empty after erasing all elements", 
        arg->logger, "Successfully verified set is empty after erasing all elements");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_unordered_multiset_clear_basic(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 50; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    unordered_multiset_clear(umset);

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 0, "Size should be 0 after clear", 
        arg->logger, "Size is correct after clear = %zu", unordered_multiset_size(umset));
    
    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 1, "Set should be empty after clear", 
        arg->logger, "Successfully verified set is empty after clear");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_clear_empty(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    unordered_multiset_clear(umset);

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 0, "Size should be 0 after clear", 
        arg->logger, "Size is correct after clear = %zu", unordered_multiset_size(umset));

    UNITTEST_ASSERT(unordered_multiset_empty(umset), ==, 1, "Set should be empty after clear", 
        arg->logger, "Successfully verified set is empty after clear");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_clear_null(test_arg *arg) {
    // Clear NULL should not crash
    unordered_multiset_clear(NULL);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully handled NULL set on clear");

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_unordered_multiset_free_null(test_arg *arg) {
    // Free NULL should not crash
    unordered_multiset_free(NULL);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully handled NULL set on free");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_free_empty(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_free_with_elements(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 100; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Inserted 100 elements into unordered_multiset before free");

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_unordered_multiset_large_capacity(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    for (int i = 0; i < 5000; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element during stress test");
    }

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 5000, "Size mismatch in stress test", 
        arg->logger, "Size is correct after stress test = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_small_capacity_many_inserts(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    // Insert many more elements than initial capacity
    for (int i = 0; i < 100; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element during resize test");
    }

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 100, "Size mismatch after resize", 
        arg->logger, "Size is correct after resize = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_insert_erase_cycle(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    // Multiple cycles of insert and erase
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element during cycle");
        }
        for (int i = 0; i < 20; i++) {
            UNITTEST_ASSERT_SILENT(unordered_multiset_erase(umset, &i), ==, CS_SUCCESS, "Failed to erase element during cycle");
        }
    }

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 0, "Size should be 0 after insert/erase cycles", 
        arg->logger, "Size is correct after insert/erase cycles = %zu", unordered_multiset_size(umset));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_reinsert_after_erase(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert element", 
        arg->logger, "Successfully inserted element");
    UNITTEST_ASSERT(unordered_multiset_erase(umset, &val), ==, CS_SUCCESS, "Failed to erase element", 
        arg->logger, "Successfully erased element");

    // Re-insert the same value
    UNITTEST_ASSERT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to re-insert after erase", 
        arg->logger, "Successfully re-inserted element after erase");

    void *found = UNITTEST_ASSERT(unordered_multiset_find(umset, &val), !=, NULL, "Failed to find element after re-insert", 
        arg->logger, "Successfully found element after re-insert");
    UNITTEST_ASSERT(*(int *)found, ==, 42, "Found element value mismatch after re-insert", 
        arg->logger, "Found element value is correct after re-insert = %d", *(int *)found);

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_many_duplicates(test_arg *arg) {
    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");

    int val = 42;
    for (int i = 0; i < 100; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &val), ==, CS_SUCCESS, "Failed to insert duplicate element");
    }

    UNITTEST_ASSERT(unordered_multiset_size(umset), ==, 100, "Size should be 100 for 100 duplicates", 
        arg->logger, "Size is correct for 100 duplicates = %zu", unordered_multiset_size(umset));

    UNITTEST_ASSERT(unordered_multiset_count(umset, &val), ==, 100, "Count should be 100 for 100 duplicates", 
        arg->logger, "Count is correct for 100 duplicates = %zu", unordered_multiset_count(umset, &val));

    unordered_multiset_free(umset);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multiset_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time stress test on Valgrind\n");
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Skipping time stress test on Valgrind",
            .return_code = CS_SUCCESS,
        };
    }

    unordered_multiset *umset = UNITTEST_ASSERT(unordered_multiset_init(get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_multiset", 
        arg->logger, "Successfully initialized unordered_multiset");
    struct timeval start, end;
    double elapsed;

    int total = __UNORDERED_MULTISET_STRESS_TEST_SIZE;
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_insert(umset, &i), ==, CS_SUCCESS, "Failed to insert element during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Inserted %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        void *found = UNITTEST_ASSERT_SILENT(unordered_multiset_find(umset, &i), !=, NULL, "Failed to find element during stress test");
        UNITTEST_ASSERT_SILENT(*(int *)found, ==, i, "Found element value mismatch during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Found %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multiset_erase(umset, &i), ==, CS_SUCCESS, "Failed to erase element during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Erased %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "delete", elapsed);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = "",
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test unordered_multiset_tests[] = {
    // Init tests
    test_unordered_multiset_init_basic,
    test_unordered_multiset_init_invalid_size,
    test_unordered_multiset_init_null_hash,

    // Insert tests
    test_unordered_multiset_insert_basic,
    test_unordered_multiset_insert_multiple,
    test_unordered_multiset_insert_duplicates,
    test_unordered_multiset_insert_null_set,
    test_unordered_multiset_insert_null_key,

    // Find tests
    test_unordered_multiset_find_existing,
    test_unordered_multiset_find_nonexistent,
    test_unordered_multiset_find_null_key,
    test_unordered_multiset_find_after_many_inserts,

    // Erase tests
    test_unordered_multiset_erase_existing,
    test_unordered_multiset_erase_one_of_many,
    test_unordered_multiset_erase_nonexistent,
    test_unordered_multiset_erase_null_set,
    test_unordered_multiset_erase_null_key,
    test_unordered_multiset_erase_all,

    // Count tests
    test_unordered_multiset_count_zero,
    test_unordered_multiset_count_single,
    test_unordered_multiset_count_multiple,
    test_unordered_multiset_count_null_key,
    test_unordered_multiset_count_after_erase,

    // Size/Empty tests
    test_unordered_multiset_size_empty,
    test_unordered_multiset_size_after_inserts,
    test_unordered_multiset_size_after_erase,
    test_unordered_multiset_empty_true,
    test_unordered_multiset_empty_false,
    test_unordered_multiset_empty_after_erase_all,

    // Clear tests
    test_unordered_multiset_clear_basic,
    test_unordered_multiset_clear_empty,
    test_unordered_multiset_clear_null,

    // Free tests
    test_unordered_multiset_free_null,
    test_unordered_multiset_free_empty,
    test_unordered_multiset_free_with_elements,

    // Stress/Edge tests
    test_unordered_multiset_large_capacity,
    test_unordered_multiset_small_capacity_many_inserts,
    test_unordered_multiset_insert_erase_cycle,
    test_unordered_multiset_reinsert_after_erase,
    test_unordered_multiset_many_duplicates,
    test_unordered_multiset_stress_time,
};