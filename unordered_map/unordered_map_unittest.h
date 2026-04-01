#include <cs/unordered_map.h>

#include <unittest.h>
#include <string.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_map_init_basic() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);

    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered_map",
            .return_code = rc
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_null_pointer() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    cs_codes rc = unordered_map_init(NULL, key_attr, value_attr, hash_int, 16);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL pointer",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_invalid_capacity() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 0);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero capacity",
            .return_code = CS_UNKNOWN
        };
    }

    rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, -5);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for negative capacity",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_invalid_key_size() {
    elem_attr_t key_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero key size",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_invalid_value_size() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero value size",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_null_hash() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, NULL, 16);

    // Should succeed with default hash
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Init with NULL hash should work with default hash",
            .return_code = rc
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              ADD ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_map_add_entry_basic() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value = 100;
    rc = unordered_map_add_entry(&umap, &key, &value);

    if (rc != CS_SUCCESS) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry",
            .return_code = rc
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_multiple() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(&umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(&umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = rc
            };
        }
    }

    if (unordered_map_size(umap) != 100) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after multiple adds",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_duplicate_key() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value1 = 100;
    int value2 = 200;

    rc = unordered_map_add_entry(&umap, &key, &value1);
    if (rc != CS_SUCCESS) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed first add",
            .return_code = rc
        };
    }

    // Add duplicate key - should fail with CS_ELEM
    rc = unordered_map_add_entry(&umap, &key, &value2);
    if (rc != CS_ELEM) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_ELEM for duplicate key",
            .return_code = CS_UNKNOWN
        };
    }

    // Size should still be 1
    if (unordered_map_size(umap) != 1) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should remain 1 after duplicate add",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_map() {
    int key = 42;
    int value = 100;
    cs_codes rc = unordered_map_add_entry(NULL, &key, &value);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL map",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_key() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int value = 100;
    rc = unordered_map_add_entry(&umap, NULL, &value);

    if (rc != CS_NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_value() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    rc = unordered_map_add_entry(&umap, &key, NULL);

    if (rc != CS_NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL value",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              GET ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_map_get_entry_existing() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value = 100;
    unordered_map_add_entry(&umap, &key, &value);

    void *found = unordered_map_get_entry(umap, &key);

    if (found == NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to get existing entry",
            .return_code = CS_UNKNOWN
        };
    }

    if (*(int *)found != 100) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_nonexistent() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value = 100;
    unordered_map_add_entry(&umap, &key, &value);

    int search_key = 99;
    void *found = unordered_map_get_entry(umap, &search_key);

    if (found != NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should not find nonexistent key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_null_key() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    void *found = unordered_map_get_entry(umap, NULL);

    if (found != NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should return NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_after_many_adds() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap, &i, &value);
    }

    // Verify all entries
    for (int i = 0; i < 100; i++) {
        void *found = unordered_map_get_entry(umap, &i);
        if (found == NULL || *(int *)found != i * 10) {
            unordered_map_free(&umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to get entry or value mismatch",
                .return_code = CS_UNKNOWN
            };
        }
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              REMOVE ENTRY TESTS                            */
/******************************************************************************/

test_res test_unordered_map_remove_entry_existing() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value = 100;
    unordered_map_add_entry(&umap, &key, &value);

    rc = unordered_map_remove_entry(&umap, &key);

    if (rc != CS_SUCCESS) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to remove entry",
            .return_code = rc
        };
    }

    // Verify entry is gone
    if (unordered_map_get_entry(umap, &key) != NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Entry still exists after remove",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_nonexistent() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    rc = unordered_map_remove_entry(&umap, &key);

    // Should fail for nonexistent key
    if (rc == CS_SUCCESS) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Remove should fail for nonexistent key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_null_map() {
    int key = 42;
    cs_codes rc = unordered_map_remove_entry(NULL, &key);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL map",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_null_key() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = unordered_map_remove_entry(&umap, NULL);

    if (rc != CS_NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_all() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap, &i, &value);
    }

    // Remove all entries
    for (int i = 0; i < 50; i++) {
        rc = unordered_map_remove_entry(&umap, &i);
        if (rc != CS_SUCCESS) {
            unordered_map_free(&umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to remove entry",
                .return_code = rc
            };
        }
    }

    if (!unordered_map_empty(umap)) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map should be empty after removing all",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              COUNT TESTS                                   */
/******************************************************************************/

test_res test_unordered_map_count_existing() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value = 100;
    unordered_map_add_entry(&umap, &key, &value);

    int count = unordered_map_count(umap, &key);

    if (count != 1) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 1 for existing key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_count_nonexistent() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int count = unordered_map_count(umap, &key);

    if (count != 0) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 0 for nonexistent key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_count_null_key() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int count = unordered_map_count(umap, NULL);

    if (count != 0) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should return 0 for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_unordered_map_size_empty() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (unordered_map_size(umap) != 0) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 0 for empty map",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_size_after_adds() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 25; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap, &i, &value);
    }

    if (unordered_map_size(umap) != 25) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after adds",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_size_after_removes() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap, &i, &value);
    }

    for (int i = 0; i < 5; i++) {
        unordered_map_remove_entry(&umap, &i);
    }

    if (unordered_map_size(umap) != 5) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 5 after removing 5 entries",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_true() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (!unordered_map_empty(umap)) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Newly created map should be empty",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_false() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value = 100;
    unordered_map_add_entry(&umap, &key, &value);

    if (unordered_map_empty(umap)) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map with entry should not be empty",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_after_remove_all() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap, &i, &value);
    }

    for (int i = 0; i < 10; i++) {
        unordered_map_remove_entry(&umap, &i);
    }

    if (!unordered_map_empty(umap)) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map should be empty after removing all entries",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SWAP TESTS                                    */
/******************************************************************************/

test_res test_unordered_map_swap_basic() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap1, umap2;
    cs_codes rc = unordered_map_init(&umap1, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize umap1",
            .return_code = rc
        };
    }

    rc = unordered_map_init(&umap2, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        unordered_map_free(&umap1);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize umap2",
            .return_code = rc
        };
    }

    // Add entries to umap1
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap1, &i, &value);
    }

    // Add entries to umap2
    for (int i = 100; i < 108; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap2, &i, &value);
    }

    int size1_before = unordered_map_size(umap1);
    int size2_before = unordered_map_size(umap2);

    unordered_map_swap(&umap1, &umap2);

    if (unordered_map_size(umap1) != size2_before) {
        unordered_map_free(&umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap1 size mismatch after swap",
            .return_code = CS_UNKNOWN
        };
    }

    if (unordered_map_size(umap2) != size1_before) {
        unordered_map_free(&umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap2 size mismatch after swap",
            .return_code = CS_UNKNOWN
        };
    }

    // Verify entries were swapped
    int key1 = 0;
    int key2 = 100;
    if (unordered_map_get_entry(umap1, &key1) != NULL) {
        unordered_map_free(&umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap1 should not have key 0 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    if (unordered_map_get_entry(umap1, &key2) == NULL) {
        unordered_map_free(&umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap1 should have key 100 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap1);
    unordered_map_free(&umap2);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_swap_null() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Swap with NULL should not crash
    unordered_map_swap(&umap, NULL);
    unordered_map_swap(NULL, &umap);
    unordered_map_swap(NULL, NULL);

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_unordered_map_clear_basic() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap, &i, &value);
    }

    unordered_map_clear(&umap);

    // After clear, map should be in cleared state
    // Note: based on implementation, clear sets ht to NULL

    unordered_map_free(&umap);

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

test_res test_unordered_map_free_null() {
    // Free NULL should not crash
    unordered_map_free(NULL);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_free_empty() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_free_with_entries() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        unordered_map_add_entry(&umap, &i, &value);
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_unordered_map_large_capacity() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 1000);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with large capacity",
            .return_code = rc
        };
    }

    for (int i = 0; i < 5000; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(&umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(&umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry during stress test",
                .return_code = rc
            };
        }
    }

    if (unordered_map_size(umap) != 5000) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch in stress test",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_small_capacity_many_adds() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 2);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with small capacity",
            .return_code = rc
        };
    }

    // Add many more entries than initial capacity
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(&umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(&umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add beyond initial capacity",
                .return_code = rc
            };
        }
    }

    if (unordered_map_size(umap) != 100) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after resize",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_remove_cycle() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Multiple cycles of add and remove
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            int value = i * 10;
            unordered_map_add_entry(&umap, &i, &value);
        }
        for (int i = 0; i < 20; i++) {
            unordered_map_remove_entry(&umap, &i);
        }
    }

    if (!unordered_map_empty(umap)) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map should be empty after cycles",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_negative_keys() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = -50; i < 50; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(&umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(&umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry with negative key",
                .return_code = rc
            };
        }
    }

    if (unordered_map_size(umap) != 100) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch with negative keys",
            .return_code = CS_UNKNOWN
        };
    }

    // Verify negative key can be found
    int neg_key = -25;
    void *found = unordered_map_get_entry(umap, &neg_key);
    if (found == NULL || *(int *)found != -250) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find entry with negative key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_readd_after_remove() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    int value1 = 100;
    int value2 = 200;

    unordered_map_add_entry(&umap, &key, &value1);
    unordered_map_remove_entry(&umap, &key);

    // Re-add with different value
    rc = unordered_map_add_entry(&umap, &key, &value2);
    if (rc != CS_SUCCESS) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to re-add after remove",
            .return_code = rc
        };
    }

    void *found = unordered_map_get_entry(umap, &key);
    if (found == NULL || *(int *)found != 200) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Re-added value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_different_value_types() {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    elem_attr_t value_attr = {
        .size = sizeof(double),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL
    };

    unordered_map umap;
    cs_codes rc = unordered_map_init(&umap, key_attr, value_attr, hash_int, 16);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int key = 42;
    double value = 3.14159;
    rc = unordered_map_add_entry(&umap, &key, &value);
    if (rc != CS_SUCCESS) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry with double value",
            .return_code = rc
        };
    }

    void *found = unordered_map_get_entry(umap, &key);
    if (found == NULL) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to get entry with double value",
            .return_code = CS_UNKNOWN
        };
    }

    double epsilon = 0.0001;
    if (*(double *)found - 3.14159 > epsilon || *(double *)found - 3.14159 < -epsilon) {
        unordered_map_free(&umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Double value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(&umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test unordered_map_tests[] = {
    // Init tests
    test_unordered_map_init_basic,
    test_unordered_map_init_null_pointer,
    test_unordered_map_init_invalid_capacity,
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
};