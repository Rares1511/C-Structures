#include <cs/unordered_multimap.h>

#include <unittest.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_multimap_init_basic(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap with int keys and values\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_init_invalid_key_size(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    UNITTEST_ASSERT(unordered_multimap_init(key_attr, get_int_attr(), hash_int), ==, NULL, 
        "Expected NULL for zero key size", arg->logger, "Correctly returned NULL for zero key size\n");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_init_invalid_value_size(test_arg *arg) {
    elem_attr_t value_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), value_attr, hash_int), ==, NULL, 
        "Expected NULL for zero value size", arg->logger, "Correctly returned NULL for zero value size\n");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_init_null_hash(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), NULL), !=, NULL, 
        "Failed to initialize unordered_multimap with NULL hash", arg->logger, "Initialized unordered_multimap with NULL hash (default hash should be used)\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              ADD ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_multimap_add_entry_basic(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for add entry test\n");
    
    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to add entry", arg->logger, "Added entry with key 42 and value 100\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_add_entry_multiple(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for multiple add entry test\n");
    
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_multimap\n");

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 100, 
        "Size should be 100 after adding 100 entries", arg->logger, "Verified size is 100 after adding 100 entries\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_add_entry_duplicate_keys(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for duplicate key add entry test\n");

    int key = 42;
    // Add same key with different values - multimap should allow this
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
            "Failed to add duplicate key");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 5 entries with duplicate key 42\n");

    // Size should be 5 (multimap allows duplicate keys)
    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 5, 
        "Size should be 5 after adding 5 entries with same key", arg->logger, "Verified size is 5 after adding 5 entries with duplicate key\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_add_entry_null_map(test_arg *arg) {
    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(NULL, &key, &value), ==, CS_NULL, 
        "Expected CS_NULL for NULL map", arg->logger, "Correctly returned CS_NULL for NULL map\n");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_add_entry_null_key(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for NULL key add entry test\n");

    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, NULL, &value), ==, CS_NULL, 
        "Expected CS_NULL for NULL key", arg->logger, "Correctly returned CS_NULL for NULL key\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_add_entry_null_value(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for NULL value add entry test\n");

    int key = 42;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, NULL), ==, CS_NULL, 
        "Expected CS_NULL for NULL value", arg->logger, "Correctly returned CS_NULL for NULL value\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              GET ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_multimap_get_entry_existing(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for get entry test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to add entry", arg->logger, "Added entry with key 42 and value 100\n");

    vector *found = UNITTEST_ASSERT(unordered_multimap_get_entry(ummap, &key), !=, NULL, 
        "Failed to get existing entry", arg->logger, "Successfully retrieved entry for key 42\n");

    UNITTEST_ASSERT(vector_size(found), ==, 1, 
        "Vector size should be 1 for single entry", arg->logger, "Verified vector size is 1 for single entry\n");
    
    void *el = UNITTEST_ASSERT(vector_at(found, 0), !=, NULL, 
        "Failed to get element from vector", arg->logger, "Successfully retrieved element from vector\n");
    UNITTEST_ASSERT(*(int *)el, ==, value, 
        "Retrieved value does not match expected", arg->logger, "Verified retrieved value matches expected\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_get_entry_nonexistent(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for get nonexistent entry test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to add entry", arg->logger, "Added entry with key 42 and value 100\n");

    int search = 99;
    UNITTEST_ASSERT(unordered_multimap_get_entry(ummap, &search), ==, NULL, 
        "Should not find nonexistent entry", arg->logger, "Correctly returned NULL for nonexistent entry\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_get_entry_null_key(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for NULL key get entry test\n");

    UNITTEST_ASSERT(unordered_multimap_get_entry(ummap, NULL), ==, NULL, 
        "Expected NULL for NULL key", arg->logger, "Correctly returned NULL for NULL key\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_get_entry_after_many_adds(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for get entry after many adds test\n");

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_multimap\n");

    // Find all entries
    for (int i = 0; i < 100; i++) {
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(ummap, &i), !=, NULL, 
            "Failed to get entry after many adds");
        UNITTEST_ASSERT_SILENT(vector_size(found), ==, 1, 
            "Vector size should be 1 for single entry");
        void *el = UNITTEST_ASSERT_SILENT(vector_at(found, 0), !=, NULL, 
            "Failed to get element from vector");
        UNITTEST_ASSERT_SILENT(*(int *)el, ==, i * 10, 
            "Retrieved value does not match expected after many adds");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully retrieved all entries after many adds\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              REMOVE ENTRY TESTS                            */
/******************************************************************************/

test_res test_unordered_multimap_remove_entry_existing(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for remove entry test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to add entry", arg->logger, "Added entry with key 42 and value 100\n");

    UNITTEST_ASSERT(unordered_multimap_remove_entry(ummap, &key), ==, CS_SUCCESS, 
        "Failed to remove entry", arg->logger, "Successfully removed entry with key 42\n");

    // Verify entry is gone
    UNITTEST_ASSERT(unordered_multimap_get_entry(ummap, &key), ==, NULL, 
        "Entry should not be found after remove", arg->logger, "Verified entry is not found after remove\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_remove_entry_one_of_many(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for remove one of many entries test\n");

    int key = 42;
    // Add 5 entries with same key
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 5 entries with duplicate key 42\n");

    // Remove one
    UNITTEST_ASSERT(unordered_multimap_remove_entry(ummap, &key), ==, CS_SUCCESS, 
        "Failed to remove entry", arg->logger, "Removed one entry with key 42\n");

    // Should still have 4 entries
    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 4, 
        "Size should be 4 after removing one of 5 duplicates", arg->logger, "Verified size is 4 after removing one of 5 duplicates\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_remove_entry_nonexistent(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for remove nonexistent entry test\n");

    int key = 42;
    UNITTEST_ASSERT(unordered_multimap_remove_entry(ummap, &key), !=, CS_SUCCESS, 
        "Should not succeed in removing nonexistent entry", arg->logger, "Correctly failed to remove nonexistent entry\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_remove_entry_null_map(test_arg *arg) {
    int key = 42;
    UNITTEST_ASSERT(unordered_multimap_remove_entry(NULL, &key), ==, CS_NULL, 
        "Expected CS_NULL for NULL map", arg->logger, "Correctly returned CS_NULL for NULL map\n");

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_remove_entry_null_key(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for NULL key remove entry test\n");
    
    UNITTEST_ASSERT(unordered_multimap_remove_entry(ummap, NULL), ==, CS_NULL, 
        "Expected CS_NULL for NULL key", arg->logger, "Correctly returned CS_NULL for NULL key\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_remove_entry_all(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for remove all entries test\n");

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 50 entries to unordered_multimap\n");

    // Remove all entries
    for (int i = 0; i < 50; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(ummap, &i), ==, CS_SUCCESS, 
            "Failed to remove entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Removed all entries from unordered_multimap\n");

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 0, 
        "Size should be 0 after removing all entries", arg->logger, "Verified size is 0 after removing all entries\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              COUNT TESTS                                   */
/******************************************************************************/

test_res test_unordered_multimap_count_zero(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for count test\n");

    int key = 42;
    UNITTEST_ASSERT(unordered_multimap_count(ummap, &key), ==, 0, 
        "Count should be 0 for nonexistent key", arg->logger, "Verified count is 0 for nonexistent key\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_count_single(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for single count test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to add entry", arg->logger, "Added entry with key 42 and value 100\n");
    
    UNITTEST_ASSERT(unordered_multimap_count(ummap, &key), ==, 1, 
        "Count should be 1 for single entry", arg->logger, "Verified count is 1 for single entry\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_count_multiple(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for multiple count test\n");

    int key = 42;
    for (int i = 0; i < 7; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 7 entries with duplicate key 42\n");

    UNITTEST_ASSERT(unordered_multimap_count(ummap, &key), ==, 7, 
        "Count should be 7 for 7 entries with same key", arg->logger, "Verified count is 7 for 7 entries with duplicate key\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_count_null_key(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for null key test\n");

    int count = unordered_multimap_count(ummap, NULL);

    UNITTEST_ASSERT(count, ==, 0, 
        "Count should be 0 for NULL key", arg->logger, "Verified count is 0 for NULL key\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_count_after_remove(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for count after remove test\n");

    int key = 42;
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    // Remove all entries with the same key
    UNITTEST_ASSERT(unordered_multimap_remove_entry(ummap, &key), ==, CS_SUCCESS, 
        "Failed to remove entry", arg->logger, "Removed entry with key 42\n");

    int count = unordered_multimap_count(ummap, &key);

    UNITTEST_ASSERT(count, ==, 0, 
        "Count should be 0 after removing all entries with the same key", arg->logger, "Verified count is 0 after removal\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_unordered_multimap_size_empty(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for size/empty test\n");

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 0, 
        "Size should be 0 for empty map", arg->logger, "Verified size is 0 for empty map\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_size_after_adds(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for size/empty test\n");

    for (int i = 0; i < 25; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 25, 
        "Size should be 25 after adding 25 entries", arg->logger, "Verified size is 25 after adds\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_size_after_remove(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for size/empty test\n");

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(ummap, &i), ==, CS_SUCCESS, 
            "Failed to remove entry");
    }

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 5, 
        "Size should be 5 after removing 5 entries", arg->logger, "Verified size is 5 after removal\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_empty_true(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for empty test\n");

    UNITTEST_ASSERT(unordered_multimap_empty(ummap), ==, 1, 
        "Newly created map should be empty", arg->logger, "Verified map is empty\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_empty_false(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for empty false test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to add entry", arg->logger, "Added entry with key 42 and value 100\n");

    UNITTEST_ASSERT(unordered_multimap_empty(ummap), ==, 0, 
        "Map with entry should not be empty", arg->logger, "Verified map is not empty after adding entry\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_empty_after_remove_all(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for empty after remove all test\n");

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 10 entries to unordered_multimap\n");

    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(ummap, &i), ==, CS_SUCCESS, 
            "Failed to remove entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Removed 10 entries from unordered_multimap\n");

    UNITTEST_ASSERT(unordered_multimap_empty(ummap), ==, 1, 
        "Map should be empty after removing all entries", arg->logger, "Verified map is empty after removing all entries\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_unordered_multimap_clear_basic(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for clear test\n");

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 50 entries to unordered_multimap\n");

    unordered_multimap_clear(ummap);

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 0, 
        "Size should be 0 after clear", arg->logger, "Verified size is 0 after clear\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_clear_empty(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for clear empty test\n");

    unordered_multimap_clear(ummap);

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 0, 
        "Size should be 0 after clear", arg->logger, "Verified size is 0 after clear\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              SWAP TESTS                                    */
/******************************************************************************/

test_res test_unordered_multimap_swap_basic(test_arg *arg) {
    unordered_multimap *ummap1 = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize ummap1", arg->logger, "Initialized ummap1 for swap test\n");
    unordered_multimap *ummap2 = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize ummap2", arg->logger, "Initialized ummap2 for swap test\n");

    // Add entries to ummap1
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap1, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry to ummap1");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 5 entries to ummap1 for swap test\n");

    // Add entries to ummap2
    for (int i = 100; i < 103; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap2, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry to ummap2");
    }

    size_t size1_before = UNITTEST_ASSERT(unordered_multimap_size(ummap1), ==, 5, 
        "ummap1 should have 5 entries before swap", arg->logger, "Verified ummap1 has 5 entries before swap\n");
    size_t size2_before = UNITTEST_ASSERT(unordered_multimap_size(ummap2), ==, 3, 
        "ummap2 should have 3 entries before swap", arg->logger, "Verified ummap2 has 3 entries before swap\n");

    unordered_multimap_swap(ummap1, ummap2);

    UNITTEST_ASSERT(unordered_multimap_size(ummap1), ==, size2_before, 
        "ummap1 size should match ummap2's old size after swap", arg->logger, "Verified ummap1 size matches ummap2's old size after swap\n");
    
    UNITTEST_ASSERT(unordered_multimap_size(ummap2), ==, size1_before, 
        "ummap2 size should match ummap1's old size after swap", arg->logger, "Verified ummap2 size matches ummap1's old size after swap\n");

    unordered_multimap_free(ummap1);
    unordered_multimap_free(ummap2);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_swap_with_empty(test_arg *arg) {
    unordered_multimap *ummap1 = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize ummap1", arg->logger, "Initialized ummap1 for swap with empty test\n");
    unordered_multimap *ummap2 = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize ummap2", arg->logger, "Initialized ummap2 for swap with empty test\n");   

    // Add entries only to ummap1
    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap1, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry to ummap1");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added entries to ummap1 for swap with empty test\n");

    unordered_multimap_swap(ummap1, ummap2);

    UNITTEST_ASSERT(unordered_multimap_empty(ummap1), ==, 1, 
        "ummap1 should be empty after swap", arg->logger, "Verified ummap1 is empty after swap\n");

    UNITTEST_ASSERT(unordered_multimap_size(ummap2), ==, 10, 
        "ummap2 should have 10 entries after swap", arg->logger, "Verified ummap2 has 10 entries after swap\n");

    unordered_multimap_free(ummap1);
    unordered_multimap_free(ummap2);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_unordered_multimap_free_empty(test_arg *arg) {
   unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for free empty test\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_free_with_entries(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for free with entries test\n");

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry to ummap");
    }

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_unordered_multimap_large_capacity(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for large capacity test\n");

    for (int i = 0; i < 5000; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry during stress test");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 5000 entries to unordered_multimap for stress test\n");

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 5000, 
        "Size should be 5000 after adding 5000 entries", arg->logger, "Verified size is 5000 after stress test adds\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_small_capacity_many_adds(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for small capacity test\n");

    // Add many more entries than initial capacity
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry during small capacity test");
    }

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 100, 
        "Size should be 100 after adding 100 entries", arg->logger, "Verified size is 100 after small capacity test adds\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_add_remove_cycle(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for add/remove cycle test\n");

    // Multiple cycles of add and remove
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            int value = i * 10;
            UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
                "Failed to add entry during add/remove cycle test");
        }
        for (int i = 0; i < 20; i++) {
            UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(ummap, &i), ==, CS_SUCCESS, 
                "Failed to remove entry during add/remove cycle test");
        }
    }

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 0, 
        "Size should be 0 after add/remove cycles", arg->logger, "Verified size is 0 after add/remove cycles\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_read_after_remove(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for read after remove test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to add entry", arg->logger, "Added entry with key 42 and value 100\n");
    UNITTEST_ASSERT(unordered_multimap_remove_entry(ummap, &key), ==, CS_SUCCESS, 
        "Failed to remove entry", arg->logger, "Removed entry with key 42\n");

    // Re-add the same key
    value = 200;
    UNITTEST_ASSERT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
        "Failed to re-add entry after remove", arg->logger, "Re-added entry with key 42 and value 200\n");
    
    UNITTEST_ASSERT(unordered_multimap_get_entry(ummap, &key), !=, NULL, 
        "Failed to get entry after re-adding", arg->logger, "Successfully retrieved entry after re-adding\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_many_duplicates(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for many duplicates test\n");

    int key = 42;
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &key, &value), ==, CS_SUCCESS, 
            "Failed to add entry during many duplicates test");
    }

    UNITTEST_ASSERT(unordered_multimap_count(ummap, &key), ==, 100, 
        "Count should be 100 for 100 duplicates", arg->logger, "Verified count is 100 for 100 duplicates\n");
    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 100, 
        "Size should be 100 after adding 100 duplicates", arg->logger, "Verified size is 100 after adding 100 duplicates\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_clear_and_reuse(test_arg *arg) {
    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for clear and reuse test\n");

    // Add, clear, and add again
    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 50 entries to unordered_multimap\n");

    unordered_multimap_clear(ummap);

    for (int i = 100; i < 150; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &value), ==, CS_SUCCESS, 
            "Failed to add entry after clear");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 50 entries after clear to unordered_multimap\n");

    UNITTEST_ASSERT(unordered_multimap_size(ummap), ==, 50, 
        "Size should be 50 after clear and reuse", arg->logger, "Verified size is 50 after clear and reuse\n");

    unordered_multimap_free(ummap);

    return SUCCESSFUL_TEST_RES;
}

test_res test_unordered_multimap_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time stress test on Valgrind");
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Skipping time stress test on Valgrind",
            .return_code = CS_SUCCESS,
        };
    }

    unordered_multimap *ummap = UNITTEST_ASSERT(unordered_multimap_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, 
        "Failed to initialize unordered_multimap", arg->logger, "Initialized unordered_multimap for stress time test\n");
    struct timeval start, end;
    double elapsed;

    int total = __UNORDERED_MULTIMAP_STRESS_TEST_SIZE;
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_add_entry(ummap, &i, &i), ==, CS_SUCCESS, 
            "Failed to add entry during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Inserted %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        vector *found = UNITTEST_ASSERT_SILENT(unordered_multimap_get_entry(ummap, &i), !=, NULL, 
            "Failed to get entry during stress test");
        UNITTEST_ASSERT_SILENT(vector_size(found), ==, 1, 
            "Expected exactly one value for each key in stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Found %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_multimap_remove_entry(ummap, &i), ==, CS_SUCCESS, 
            "Failed to erase element during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_INFO, "Erased %d elements in %.4f seconds\n", total, elapsed);
    post_operation_time(arg, "delete", elapsed);

    return SUCCESSFUL_TEST_RES;
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test unordered_multimap_tests[] = {
    // Init tests
    test_unordered_multimap_init_basic,
    test_unordered_multimap_init_invalid_key_size,
    test_unordered_multimap_init_invalid_value_size,
    test_unordered_multimap_init_null_hash,

    // Add entry tests
    test_unordered_multimap_add_entry_basic,
    test_unordered_multimap_add_entry_multiple,
    test_unordered_multimap_add_entry_duplicate_keys,
    test_unordered_multimap_add_entry_null_map,
    test_unordered_multimap_add_entry_null_key,
    test_unordered_multimap_add_entry_null_value,

    // Get entry tests
    test_unordered_multimap_get_entry_existing,
    test_unordered_multimap_get_entry_nonexistent,
    test_unordered_multimap_get_entry_null_key,
    test_unordered_multimap_get_entry_after_many_adds,

    // Remove entry tests
    test_unordered_multimap_remove_entry_existing,
    test_unordered_multimap_remove_entry_one_of_many,
    test_unordered_multimap_remove_entry_nonexistent,
    test_unordered_multimap_remove_entry_null_map,
    test_unordered_multimap_remove_entry_null_key,
    test_unordered_multimap_remove_entry_all,

    // Count tests
    test_unordered_multimap_count_zero,
    test_unordered_multimap_count_single,
    test_unordered_multimap_count_multiple,
    test_unordered_multimap_count_null_key,
    test_unordered_multimap_count_after_remove,

    // Size/Empty tests
    test_unordered_multimap_size_empty,
    test_unordered_multimap_size_after_adds,
    test_unordered_multimap_size_after_remove,
    test_unordered_multimap_empty_true,
    test_unordered_multimap_empty_false,
    test_unordered_multimap_empty_after_remove_all,

    // Clear tests
    test_unordered_multimap_clear_basic,
    test_unordered_multimap_clear_empty,

    // Swap tests
    test_unordered_multimap_swap_basic,
    test_unordered_multimap_swap_with_empty,

    // Free tests
    test_unordered_multimap_free_empty,
    test_unordered_multimap_free_with_entries,

    // Stress/Edge tests
    test_unordered_multimap_large_capacity,
    test_unordered_multimap_small_capacity_many_adds,
    test_unordered_multimap_add_remove_cycle,
    test_unordered_multimap_read_after_remove,
    test_unordered_multimap_many_duplicates,
    test_unordered_multimap_clear_and_reuse,
    test_unordered_multimap_stress_time,
};