#include <cs/unordered_map.h>

#include <unittest.h>
#include <string.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_map_init_basic(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_invalid_key_size(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    UNITTEST_ASSERT(unordered_map_init(key_attr, get_int_attr(), hash_int), ==, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map with invalid key size\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_invalid_value_size(test_arg *arg) {
    elem_attr_t value_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    UNITTEST_ASSERT(unordered_map_init(get_int_attr(), value_attr, hash_int), ==, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map with invalid value size\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_null_hash(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), NULL), !=, NULL, "Failed to initialize unordered_map with NULL hash",
        arg->logger, "Initialized unordered_map with NULL hash function (using default)\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              ADD ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_map_add_entry_basic(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for add_entry test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value), ==, CS_SUCCESS, "Failed to add entry",
        arg->logger, "Added entry with key %d and value %d\n", key, value);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_multiple(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for multiple add_entry test\n");

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map\n");

    UNITTEST_ASSERT(unordered_map_size(umap), ==, 100, "Size mismatch after multiple adds",
        arg->logger, "Verified size is 100 after adding entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_duplicate_key(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for duplicate key test\n");

    int key = 42;
    int value1 = 100;
    int value2 = 200;

    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value1), ==, CS_SUCCESS, "Failed to add entry",
        arg->logger, "Added first entry with key %d and value %d\n", key, value1);

    // Add duplicate key - should fail with CS_ELEM
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value2), ==, CS_ELEM, "Expected CS_ELEM for duplicate key",
        arg->logger, "Attempted to add duplicate key %d\n", key);

    // Size should still be 1
    UNITTEST_ASSERT(unordered_map_size(umap), ==, 1, "Size should remain 1 after duplicate add",
        arg->logger, "Verified size is still 1 after attempting to add duplicate key\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_map(test_arg *arg) {
    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(NULL, &key, &value), ==, CS_NULL, "Expected CS_NULL for NULL map",
        arg->logger, "Attempted to add entry to NULL map\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_key(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for null key test\n");

    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, NULL, &value), ==, CS_NULL, "Expected CS_NULL for NULL key",
        arg->logger, "Attempted to add entry with NULL key\n");

    clogger_log(arg->logger, CLOGGER_DEBUG, "Correctly handled NULL key for add_entry\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_value(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for null value test\n");

    int key = 42;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, NULL), ==, CS_NULL, "Expected CS_NULL for NULL value",
        arg->logger, "Attempted to add entry with NULL value\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              GET ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_map_get_entry_existing(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for get_entry existing key test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value), ==, CS_SUCCESS, "Failed to add entry",
        arg->logger, "Added entry with key %d and value %d for get_entry test\n", key, value);

    void *found = UNITTEST_ASSERT(unordered_map_get_entry(umap, &key), !=, NULL, "Failed to get existing entry",
        arg->logger, "Retrieved entry for key %d\n", key);

    UNITTEST_ASSERT(*(int *)found, ==, 100, "Value mismatch for existing entry",
        arg->logger, "Verified value for key %d is correct\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_nonexistent(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for get_entry nonexistent key test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value), ==, CS_SUCCESS, "Failed to add entry",
        arg->logger, "Added entry with key %d for get_entry nonexistent key test\n", key);

    int search_key = 99;
    UNITTEST_ASSERT(unordered_map_get_entry(umap, &search_key), ==, NULL, "Expected NULL for nonexistent key",
        arg->logger, "Correctly returned NULL for nonexistent key %d\n", search_key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_null_key(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for get_entry null key test\n");

    UNITTEST_ASSERT(unordered_map_get_entry(umap, NULL), ==, NULL, "Expected NULL for NULL key",
        arg->logger, "Attempted to get entry with NULL key\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_after_many_adds(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for get_entry after many adds test\n");

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map for get_entry test\n");

    // Verify all entries
    for (int i = 0; i < 100; i++) {
        void *found = UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &i), !=, NULL, "Failed to get entry");
        UNITTEST_ASSERT_SILENT(*(int *)found, ==, i * 10, "Value mismatch for entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully retrieved and verified all entries after many adds\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              REMOVE ENTRY TESTS                            */
/******************************************************************************/

test_res test_unordered_map_remove_entry_existing(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for remove_entry existing key test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value), ==, CS_SUCCESS, "Failed to add entry",
        arg->logger, "Added entry with key %d for remove_entry test\n", key);

    UNITTEST_ASSERT(unordered_map_remove_entry(umap, &key), ==, CS_SUCCESS, "Failed to remove entry",
        arg->logger, "Removed entry with key %d\n", key);

    UNITTEST_ASSERT(unordered_map_get_entry(umap, &key), ==, NULL, "Expected NULL after removal",
        arg->logger, "Verified entry with key %d is removed\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_nonexistent(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for remove_entry nonexistent key test\n");

    int key = 42;
    UNITTEST_ASSERT(unordered_map_remove_entry(umap, &key), ==, CS_ELEM, "Expected CS_ELEM for nonexistent key",
        arg->logger, "Attempted to remove nonexistent key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_null_map(test_arg *arg) {
    int key = 42;
    UNITTEST_ASSERT(unordered_map_remove_entry(NULL, &key), ==, CS_NULL, "Expected CS_NULL for NULL map",
        arg->logger, "Attempted to remove entry from NULL map\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_null_key(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for remove_entry null key test\n");

    UNITTEST_ASSERT(unordered_map_remove_entry(umap, NULL), ==, CS_NULL, "Expected CS_NULL for NULL key",
        arg->logger, "Attempted to remove entry with NULL key\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_all(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for remove_entry all test\n");

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 50 entries to unordered_map for remove_entry all test\n");

    // Remove all entries
    for (int i = 0; i < 50; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Removed all entries from unordered_map\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 1, "Expected map to be empty after removing all entries",
        arg->logger, "Verified unordered_map is empty after removing all entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              COUNT TESTS                                   */
/******************************************************************************/

test_res test_unordered_map_count_existing(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for count existing key test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value), ==, CS_SUCCESS, "Failed to add entry",
        arg->logger, "Added entry with key %d for count test\n", key);

    UNITTEST_ASSERT(unordered_map_count(umap, &key), ==, 1, "Expected count to be 1 for existing key",
        arg->logger, "Verified count is 1 for existing key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_count_nonexistent(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for count nonexistent key test\n");

    int key = 42;
    UNITTEST_ASSERT(unordered_map_count(umap, &key), ==, 0, "Expected count to be 0 for nonexistent key",
        arg->logger, "Verified count is 0 for nonexistent key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_count_null_key(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for count null key test\n");

    UNITTEST_ASSERT(unordered_map_count(umap, NULL), ==, 0, "Expected count to be 0 for NULL key",
        arg->logger, "Attempted to count with NULL key\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_unordered_map_size_empty(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for size empty test\n");
    

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 1, "Expected map to be empty", arg->logger, "Verified unordered_map is empty\n");
    UNITTEST_ASSERT(unordered_map_size(umap), ==, 0, "Expected size to be 0 for empty map", arg->logger, "Verified size is 0 for empty unordered_map\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_size_after_adds(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for size after adds test\n");

    for (int i = 0; i < 25; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 25 entries to unordered_map for size test\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 0, "Expected map to not be empty after adds",
        arg->logger, "Verified unordered_map is not empty after adding entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_size_after_removes(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for size after removes test\n");

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 10 entries to unordered_map for size after removes test\n");

    for (int i = 0; i < 5; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Removed 5 entries from unordered_map for size after removes test\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 0, "Expected map to not be empty after removing some entries",
        arg->logger, "Verified unordered_map is not empty after removing some entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_true(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for empty true test\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 1, "Expected map to be empty", arg->logger, "Verified newly initialized unordered_map is empty\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_false(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for empty false test\n");

    int key = 42;
    int value = 100;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value), ==, CS_SUCCESS, "Failed to add entry",
        arg->logger, "Added entry with key %d for empty false test\n", key);
    
    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 0, "Expected map to not be empty", arg->logger, "Verified unordered_map with entry is not empty\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_after_remove_all(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for empty after remove all test\n");

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 10 entries to unordered_map for empty after remove all test\n");

    for (int i = 0; i < 10; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Removed all entries from unordered_map for empty after remove all test\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 1, "Expected map to be empty after removing all entries",
        arg->logger, "Verified unordered_map is empty after removing all entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SWAP TESTS                                    */
/******************************************************************************/

test_res test_unordered_map_swap_basic(test_arg *arg) {
    unordered_map *umap1 = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize first unordered_map",
        arg->logger, "Initialized first unordered_map for swap test\n");
    unordered_map *umap2 = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize second unordered_map",
        arg->logger, "Initialized second unordered_map for swap test\n");
    

    // Add entries to umap1
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap1, &i, &value), ==, CS_SUCCESS, "Failed to add entry to umap1");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 5 entries to umap1 for swap test\n");

    // Add entries to umap2
    for (int i = 100; i < 108; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap2, &i, &value), ==, CS_SUCCESS, "Failed to add entry to umap2");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 8 entries to umap2 for swap test\n");

    size_t size1_before = unordered_map_size(umap1);
    size_t size2_before = unordered_map_size(umap2);

    unordered_map_swap(umap1, umap2);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Swapped umap1 and umap2\n");

    UNITTEST_ASSERT(unordered_map_size(umap1), ==, size2_before, "Expected umap1 size to match umap2 size before swap",
        arg->logger, "Verified umap1 size matches umap2 size before swap\n");

    UNITTEST_ASSERT(unordered_map_size(umap2), ==, size1_before, "Expected umap2 size to match umap1 size before swap",
        arg->logger, "Verified umap2 size matches umap1 size before swap\n");

    // Verify entries were swapped
    int key1 = 0;
    UNITTEST_ASSERT(unordered_map_get_entry(umap1, &key1), ==, NULL, "Expected key1 to not be in umap1 after swap",
        arg->logger, "Verified key %d is not in umap1 after swap\n", key1);
    UNITTEST_ASSERT(unordered_map_get_entry(umap2, &key1), !=, NULL, "Expected key1 to be in umap2 after swap",
        arg->logger, "Verified key %d is in umap2 after swap\n", key1);

    int key2 = 100;
    UNITTEST_ASSERT(unordered_map_get_entry(umap1, &key2), !=, NULL, "Expected key2 to be in umap1 after swap",
        arg->logger, "Verified key %d is in umap1 after swap\n", key2);
    UNITTEST_ASSERT(unordered_map_get_entry(umap2, &key2), ==, NULL, "Expected key2 to not be in umap2 after swap",
        arg->logger, "Verified key %d is not in umap2 after swap\n", key2);

    unordered_map_free(umap1);
    unordered_map_free(umap2);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_swap_null(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for swap null test\n");

    // Swap with NULL should not crash
    unordered_map_swap(umap, NULL);
    unordered_map_swap(NULL, umap);
    unordered_map_swap(NULL, NULL);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully called swap with NULL maps\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_unordered_map_clear_basic(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for clear basic test\n");

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 50 entries to unordered_map for clear test\n");

    unordered_map_clear(umap);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_clear_null() {
    // Clear NULL should not crash
    unordered_map_clear(NULL);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_unordered_map_free_null(test_arg *arg) {
    // Free NULL should not crash
    unordered_map_free(NULL);

    clogger_log(arg->logger, CLOGGER_DEBUG, "Successfully called free on NULL unordered_map\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_free_empty(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for free empty test\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_free_with_entries(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for free with entries test\n");

    
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map for free with entries test\n");
    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_unordered_map_large_capacity(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for large capacity test\n");

    for (int i = 0; i < 5000; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry during stress test");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 5000 entries to unordered_map for large capacity test\n");

    UNITTEST_ASSERT(unordered_map_size(umap), ==, 5000, "Expected size to be 5000 after adding 5000 entries",
        arg->logger, "Verified size is 5000 after adding 5000 entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_small_capacity_many_adds(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for small capacity many adds test\n");

    // Add many more entries than initial capacity
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry beyond initial capacity");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map with small initial capacity\n");

    UNITTEST_ASSERT(unordered_map_size(umap), ==, 100, "Expected size to be 100 after adding 100 entries",
        arg->logger, "Verified size is 100 after adding 100 entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_remove_cycle(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for add/remove cycle test\n");

    // Multiple cycles of add and remove
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            int value = i * 10;
            UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry in cycle");
        }
        for (int i = 0; i < 20; i++) {
            UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry in cycle");
        }
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Completed 5 cycles of adding and removing 20 entries\n");

    UNITTEST_ASSERT(unordered_map_size(umap), ==, 0, "Expected size to be 0 after add/remove cycles",
        arg->logger, "Verified size is 0 after add/remove cycles\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_negative_keys(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for negative keys test\n");

    for (int i = -50; i < 50; i++) {
        int value = i * 10;
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry with negative key");
    }

    clogger_log(arg->logger, CLOGGER_DEBUG, "Added 100 entries with keys from -50 to 49 for negative keys test\n");

    UNITTEST_ASSERT(unordered_map_empty(umap), ==, 0, "Expected map to not be empty after adding entries with negative keys",
        arg->logger, "Verified unordered_map is not empty after adding entries with negative keys\n");

    // Verify negative key can be found
    int neg_key = -25;
    void *found = UNITTEST_ASSERT(unordered_map_get_entry(umap, &neg_key), !=, NULL, "Failed to get entry with negative key",
        arg->logger, "Attempted to get entry with negative key %d\n", neg_key);
    UNITTEST_ASSERT(*(int *)found, ==, neg_key * 10, "Value mismatch for entry with negative key",
        arg->logger, "Verified value for entry with negative key %d is correct\n", neg_key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_readd_after_remove(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_int_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for re-add after remove test\n");

    int key = 42;
    int value1 = 100;
    int value2 = 200;

    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value1), ==, CS_SUCCESS, "Failed to add entry for re-add after remove test",
        arg->logger, "Added entry with key %d and value %d for re-add after remove test\n", key, value1);

    UNITTEST_ASSERT(unordered_map_remove_entry(umap, &key), ==, CS_SUCCESS, "Failed to remove entry",
        arg->logger, "Removed entry with key %d for re-add after remove test\n", key);


    // Re-add with different value
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value2), ==, CS_SUCCESS, "Failed to re-add entry after remove",
        arg->logger, "Re-added entry with key %d and value %d for re-add after remove test\n", key, value2);

    void *found = UNITTEST_ASSERT(unordered_map_get_entry(umap, &key), !=, NULL, "Failed to get re-added entry",
        arg->logger, "Attempted to get re-added entry with key %d\n", key);
    UNITTEST_ASSERT(*(int *)found, ==, value2, "Re-added value mismatch",
        arg->logger, "Verified re-added entry with key %d has correct value %d\n", key, value2);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_different_value_types(test_arg *arg) {
    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_double_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Retrieved unordered_map from test_arg for different value types test\n");

    int key = 42;
    double value = 3.14159;
    UNITTEST_ASSERT(unordered_map_add_entry(umap, &key, &value), ==, CS_SUCCESS, "Failed to add entry with double value",
        arg->logger, "Added entry with key %d and double value %f for different value types test\n", key, value);

    void *found = UNITTEST_ASSERT(unordered_map_get_entry(umap, &key), !=, NULL, "Failed to get entry with double value",
        arg->logger, "Attempted to get entry with key %d for different value types test\n", key);

    double epsilon = 0.0001;
    UNITTEST_ASSERT(*(double *)found - 3.14159, <, epsilon, "Double value mismatch",
        arg->logger, "Verified retrieved double value is correct for key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Skipping time-based stress test on Valgrind\n");
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Skipping time-based stress test on Valgrind",
            .return_code = CS_SUCCESS
        };
    }

    unordered_map *umap = UNITTEST_ASSERT(unordered_map_init(get_int_attr(), get_string_attr(), hash_int), !=, NULL, "Failed to initialize unordered_map",
        arg->logger, "Initialized unordered_map for stress time test\n");
    struct timeval start, end;
    double elapsed;

    int total = __UNORDERED_MAP_STRESS_TEST_SIZE;
    char value[25] = "StressValGeneric";

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_add_entry(umap, &i, &value), ==, CS_SUCCESS, "Failed to add entry during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_get_entry(umap, &i), !=, NULL, "Failed to get entry during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test get completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        UNITTEST_ASSERT_SILENT(unordered_map_remove_entry(umap, &i), ==, CS_SUCCESS, "Failed to remove entry during stress test");
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(arg->logger, CLOGGER_DEBUG, "Stress test remove completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "delete", elapsed);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = "",
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test unordered_map_tests[] = {
    // Init tests
    test_unordered_map_init_basic,
    test_unordered_map_init_invalid_key_size,
    test_unordered_map_init_invalid_value_size,
    test_unordered_map_init_null_hash,

    // Add entry tests
    test_unordered_map_add_entry_basic,
    test_unordered_map_add_entry_multiple,
    test_unordered_map_add_entry_duplicate_key,
    test_unordered_map_add_entry_null_map,
    test_unordered_map_add_entry_null_key,
    test_unordered_map_add_entry_null_value,

    // Get entry tests
    test_unordered_map_get_entry_existing,
    test_unordered_map_get_entry_nonexistent,
    test_unordered_map_get_entry_null_key,
    test_unordered_map_get_entry_after_many_adds,

    // Remove entry tests
    test_unordered_map_remove_entry_existing,
    test_unordered_map_remove_entry_nonexistent,
    test_unordered_map_remove_entry_null_map,
    test_unordered_map_remove_entry_null_key,
    test_unordered_map_remove_entry_all,

    // Count tests
    test_unordered_map_count_existing,
    test_unordered_map_count_nonexistent,
    test_unordered_map_count_null_key,

    // Size/Empty tests
    test_unordered_map_size_empty,
    test_unordered_map_size_after_adds,
    test_unordered_map_size_after_removes,
    test_unordered_map_empty_true,
    test_unordered_map_empty_false,
    test_unordered_map_empty_after_remove_all,

    // Swap tests
    test_unordered_map_swap_basic,
    test_unordered_map_swap_null,

    // Clear tests
    test_unordered_map_clear_basic,
    test_unordered_map_clear_null,

    // Free tests
    test_unordered_map_free_null,
    test_unordered_map_free_empty,
    test_unordered_map_free_with_entries,

    // Stress/Edge tests
    test_unordered_map_large_capacity,
    test_unordered_map_small_capacity_many_adds,
    test_unordered_map_add_remove_cycle,
    test_unordered_map_negative_keys,
    test_unordered_map_readd_after_remove,
    test_unordered_map_different_value_types,
    test_unordered_map_stress_time,
};