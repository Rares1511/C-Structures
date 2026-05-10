#include <cs/unordered_map.h>

#include <unittest.h>
#include <string.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_map_init_basic(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);

    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered_map",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map with key size %ld and value size %ld\n",
            key_attr.size, value_attr.size);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_null_pointer(test_arg *arg) {
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

    cs_codes rc = unordered_map_init(NULL, key_attr, value_attr, hash_int);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL pointer",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled NULL pointer for unordered_map_init\n");

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

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero key size",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled zero key size for unordered_map_init\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_invalid_value_size(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero value size",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled zero value size for unordered_map_init\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_init_null_hash(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, NULL);

    // Should succeed with default hash
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Init with NULL hash should work with default hash",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map with NULL hash function (using default)\n");

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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for add_entry test\n");

    int key = 42;
    int value = 100;
    rc = unordered_map_add_entry(umap, &key, &value);

    if (rc != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entry with key %d and value %d\n", key, value);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_multiple(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for multiple add_entry test\n");

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = rc
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map\n");

    if (unordered_map_size(umap) != 100) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after multiple adds",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is 100 after adding entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_duplicate_key(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for duplicate key test\n");

    int key = 42;
    int value1 = 100;
    int value2 = 200;

    rc = unordered_map_add_entry(umap, &key, &value1);
    if (rc != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed first add",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added first entry with key %d and value %d\n", key, value1);

    // Add duplicate key - should fail with CS_ELEM
    rc = unordered_map_add_entry(umap, &key, &value2);
    if (rc != CS_ELEM) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_ELEM for duplicate key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled duplicate key addition\n");

    // Size should still be 1
    if (unordered_map_size(umap) != 1) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should remain 1 after duplicate add",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is still 1 after attempting to add duplicate key\n");

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
    cs_codes rc = unordered_map_add_entry(NULL, &key, &value);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL map",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled NULL map for add_entry\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_key(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for null key test\n");

    int value = 100;
    rc = unordered_map_add_entry(umap, NULL, &value);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Attempted to add entry with NULL key\n");

    if (rc != CS_NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled NULL key for add_entry\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_entry_null_value(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for null value test\n");

    int key = 42;
    rc = unordered_map_add_entry(umap, &key, NULL);

    if (rc != CS_NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL value",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled NULL value for add_entry\n");

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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for get_entry existing key test\n");

    int key = 42;
    int value = 100;
    unordered_map_add_entry(umap, &key, &value);

    void *found = unordered_map_get_entry(umap, &key);

    if (found == NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to get existing entry",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Retrieved entry for key %d\n", key);

    if (*(int *)found != 100) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified value for key %d is correct\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_nonexistent(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for get_entry nonexistent key test\n");

    int key = 42;
    int value = 100;
    if (unordered_map_add_entry(umap, &key, &value) != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entry with key %d for get_entry nonexistent key test\n", key);

    int search_key = 99;
    void *found = unordered_map_get_entry(umap, &search_key);

    if (found != NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should not find nonexistent key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned NULL for nonexistent key %d\n", search_key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_null_key(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for get_entry null key test\n");

    void *found = unordered_map_get_entry(umap, NULL);

    if (found != NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should return NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned NULL for NULL key\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_get_entry_after_many_adds(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for get_entry after many adds test\n");

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        unordered_map_add_entry(umap, &i, &value);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map for get_entry test\n");

    // Verify all entries
    for (int i = 0; i < 100; i++) {
        void *found = unordered_map_get_entry(umap, &i);
        if (found == NULL || *(int *)found != i * 10) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to get entry or value mismatch",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Successfully retrieved and verified all entries after many adds\n");

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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for remove_entry existing key test\n");

    int key = 42;
    int value = 100;
    if (unordered_map_add_entry(umap, &key, &value) != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entry with key %d for remove_entry test\n", key);

    if (unordered_map_remove_entry(umap, &key) != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to remove entry",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Removed entry with key %d\n", key);

    // Verify entry is gone
    if (unordered_map_get_entry(umap, &key) != NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Entry still exists after remove",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified entry with key %d is removed\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_nonexistent(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for remove_entry nonexistent key test\n");

    int key = 42;
    rc = unordered_map_remove_entry(umap, &key);
    if (rc == CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Remove should fail for nonexistent key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled remove of nonexistent key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_null_map(test_arg *arg) {
    int key = 42;
    cs_codes rc = unordered_map_remove_entry(NULL, &key);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL map",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled NULL map for remove_entry\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_null_key(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for remove_entry null key test\n");

    rc = unordered_map_remove_entry(umap, NULL);
    if (rc != CS_NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled NULL key for remove_entry\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_remove_entry_all(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for remove_entry all test\n");

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(umap, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 50 entries to unordered_map for remove_entry all test\n");

    // Remove all entries
    for (int i = 0; i < 50; i++) {
        rc = unordered_map_remove_entry(umap, &i);
        if (rc != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to remove entry",
                .return_code = rc
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Removed all entries from unordered_map\n");

    if (!unordered_map_empty(umap)) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map should be empty after removing all",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified unordered_map is empty after removing all entries\n");

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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for count existing key test\n");

    int key = 42;
    int value = 100;
    if (unordered_map_add_entry(umap, &key, &value) != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entry with key %d for count test\n", key);

    int count = unordered_map_count(umap, &key);

    if (count != 1) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 1 for existing key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified count is 1 for existing key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_count_nonexistent(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for count nonexistent key test\n");

    int key = 42;
    int count = unordered_map_count(umap, &key);

    if (count != 0) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 0 for nonexistent key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified count is 0 for nonexistent key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_count_null_key(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for count null key test\n");

    int count = unordered_map_count(umap, NULL);

    if (count != 0) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should return 0 for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified count returns 0 for NULL key\n");

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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for size empty test\n");

    if (unordered_map_size(umap) != 0) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 0 for empty map",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is 0 for empty unordered_map\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_size_after_adds(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for size after adds test\n");

    for (int i = 0; i < 25; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(umap, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 25 entries to unordered_map for size test\n");

    if (unordered_map_size(umap) != 25) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after adds",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is 25 after adding 25 entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_size_after_removes(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for size after removes test\n");

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(umap, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 10 entries to unordered_map for size after removes test\n");

    for (int i = 0; i < 5; i++) {
        if (unordered_map_remove_entry(umap, &i) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to remove entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Removed 5 entries from unordered_map for size after removes test\n");

    if (unordered_map_size(umap) != 5) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 5 after removing 5 entries",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is 5 after removing 5 entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_true(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for empty true test\n");

    if (!unordered_map_empty(umap)) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Newly created map should be empty",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified newly initialized unordered_map is empty\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_false(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for empty false test\n");

    int key = 42;
    int value = 100;
    if (unordered_map_add_entry(umap, &key, &value) != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entry with key %d for empty false test\n", key);

    if (unordered_map_empty(umap)) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map with entry should not be empty",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified unordered_map with entries is not empty\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_empty_after_remove_all(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for empty after remove all test\n");

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(umap, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 10 entries to unordered_map for empty after remove all test\n");

    for (int i = 0; i < 10; i++) {
        if (unordered_map_remove_entry(umap, &i) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to remove entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Removed all entries from unordered_map for empty after remove all test\n");

    if (!unordered_map_empty(umap)) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map should be empty after removing all entries",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified unordered_map is empty after removing all entries\n");

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

    unordered_map *umap1 = (unordered_map *)arg->data_structure;
    unordered_map umap2;
    cs_codes rc = unordered_map_init(umap1, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize umap1",
            .return_code = rc
        };
    }

    rc = unordered_map_init(&umap2, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        unordered_map_free(umap1);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize umap2",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized two unordered_maps for swap test\n");

    // Add entries to umap1
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(umap1, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap1);
            unordered_map_free(&umap2);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry to umap1",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 5 entries to umap1 for swap test\n");

    // Add entries to umap2
    for (int i = 100; i < 108; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(&umap2, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap1);
            unordered_map_free(&umap2);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry to umap2",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 8 entries to umap2 for swap test\n");

    size_t size1_before = unordered_map_size(umap1);
    size_t size2_before = unordered_map_size(&umap2);

    unordered_map_swap(umap1, &umap2);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Swapped umap1 and umap2\n");

    if (unordered_map_size(umap1) != size2_before) {
        unordered_map_free(umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap1 size mismatch after swap",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified umap1 size matches umap2 size before swap\n");

    if (unordered_map_size(&umap2) != size1_before) {
        unordered_map_free(umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap2 size mismatch after swap",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified umap2 size matches umap1 size before swap\n");

    // Verify entries were swapped
    int key1 = 0;
    int key2 = 100;
    if (unordered_map_get_entry(umap1, &key1) != NULL) {
        unordered_map_free(umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap1 should not have key 0 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified umap1 does not have key %d after swap\n", key1);

    if (unordered_map_get_entry(umap1, &key2) == NULL) {
        unordered_map_free(umap1);
        unordered_map_free(&umap2);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "umap1 should have key 100 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified umap1 has key %d after swap\n", key2);

    unordered_map_free(umap1);
    unordered_map_free(&umap2);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_swap_null(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for swap null test\n");

    // Swap with NULL should not crash
    unordered_map_swap(umap, NULL);
    unordered_map_swap(NULL, umap);
    unordered_map_swap(NULL, NULL);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Successfully called swap with NULL maps\n");

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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for clear basic test\n");

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(umap, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 50 entries to unordered_map for clear test\n");

    unordered_map_clear(umap);

    // After clear, map should be in cleared state
    // Note: based on implementation, clear sets ht to NULL

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

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Successfully called free on NULL unordered_map\n");

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_free_empty(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized empty unordered_map for free test\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_free_with_entries(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for free with entries test\n");

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        if (unordered_map_add_entry(umap, &i, &value) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry",
                .return_code = CS_UNKNOWN
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map for free with entries test\n");
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with large capacity",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for large capacity test\n");

    for (int i = 0; i < 5000; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry during stress test",
                .return_code = rc
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 5000 entries to unordered_map for large capacity test\n");

    if (unordered_map_size(umap) != 5000) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch in stress test",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is 5000 after adding 5000 entries\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_small_capacity_many_adds(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with small capacity",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for small capacity many adds test\n");

    // Add many more entries than initial capacity
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add beyond initial capacity",
                .return_code = rc
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 100 entries to unordered_map with small initial capacity\n");

    if (unordered_map_size(umap) != 100) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after resize",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is 100 after adding 100 entries to small capacity unordered_map\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_add_remove_cycle(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for add/remove cycle test\n");

    // Multiple cycles of add and remove
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            int value = i * 10;
            if (unordered_map_add_entry(umap, &i, &value) != CS_SUCCESS) {
                unordered_map_free(umap);
                return (test_res){
                    .test_name = (char*) __func__,
                    .reason = "Failed to add entry in cycle",
                    .return_code = CS_UNKNOWN
                };
            }
        }
        for (int i = 0; i < 20; i++) {
            if (unordered_map_remove_entry(umap, &i) != CS_SUCCESS) {
                unordered_map_free(umap);
                return (test_res){
                    .test_name = (char*) __func__,
                    .reason = "Failed to remove entry in cycle",
                    .return_code = CS_UNKNOWN
                };
            }
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Completed 5 cycles of adding and removing 20 entries\n");

    if (!unordered_map_empty(umap)) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Map should be empty after cycles",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified unordered_map is empty after add/remove cycles\n");

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_negative_keys(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for negative keys test\n");

    for (int i = -50; i < 50; i++) {
        int value = i * 10;
        rc = unordered_map_add_entry(umap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry with negative key",
                .return_code = rc
            };
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added 100 entries with keys from -50 to 49 for negative keys test\n");

    if (unordered_map_size(umap) != 100) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch with negative keys",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified size is 100 after adding entries with negative keys\n");

    // Verify negative key can be found
    int neg_key = -25;
    void *found = unordered_map_get_entry(umap, &neg_key);
    if (found == NULL || *(int *)found != -250) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find entry with negative key",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified entry with negative key %d can be found\n", neg_key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_readd_after_remove(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for re-add after remove test\n");

    int key = 42;
    int value1 = 100;
    int value2 = 200;

    if (unordered_map_add_entry(umap, &key, &value1) != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add first entry",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entry with key %d and value %d for re-add after remove test\n", key, value1);

    if (unordered_map_remove_entry(umap, &key) != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to remove entry",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Removed entry with key %d for re-add after remove test\n", key);

    // Re-add with different value
    rc = unordered_map_add_entry(umap, &key, &value2);
    if (rc != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to re-add after remove",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Re-added entry with key %d and value %d for re-add after remove test\n", key, value2);

    void *found = unordered_map_get_entry(umap, &key);
    if (found == NULL || *(int *)found != 200) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Re-added value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified re-added entry with key %d has correct value %d\n", key, value2);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_different_value_types(test_arg *arg) {
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

    unordered_map *umap = (unordered_map *)arg->data_structure;
    cs_codes rc = unordered_map_init(umap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_map for different value types test\n");

    int key = 42;
    double value = 3.14159;
    rc = unordered_map_add_entry(umap, &key, &value);
    if (rc != CS_SUCCESS) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to add entry with double value",
            .return_code = rc
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entry with key %d and double value %f for different value types test\n", key, value);

    void *found = unordered_map_get_entry(umap, &key);
    if (found == NULL) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to get entry with double value",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Retrieved entry with key %d for different value types test\n", key);

    double epsilon = 0.0001;
    if (*(double *)found - 3.14159 > epsilon || *(double *)found - 3.14159 < -epsilon) {
        unordered_map_free(umap);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Double value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified retrieved double value is correct for key %d\n", key);

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_map_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        clogger_log(*arg->logger, CLOGGER_DEBUG, "Skipping time-based stress test on Valgrind");
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Skipping time-based stress test on Valgrind",
            .return_code = CS_SUCCESS
        };
    }

    unordered_map *umap = (unordered_map *)arg->data_structure;
    struct timeval start, end;
    double elapsed;

    if (CS_SUCCESS != unordered_map_init(umap, get_int_attr(), get_string_attr(), hash_int)) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = CS_UNKNOWN
        };
    }

    int total = __UNORDERED_MAP_STRESS_TEST_SIZE;

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        char value[25];
        snprintf(value, sizeof(value), "StressVal_%d", i);
        if (CS_SUCCESS != unordered_map_add_entry(umap, &i, &value)) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to add entry during stress test",
                .return_code = CS_UNKNOWN
            };
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Stress test completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        if (unordered_map_get_entry(umap, &i) == NULL) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to get entry during stress test",
                .return_code = CS_UNKNOWN
            };
        };
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Stress test get completed in %.6f seconds\n", elapsed);
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        if (unordered_map_remove_entry(umap, &i) != CS_SUCCESS) {
            unordered_map_free(umap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to remove entry during stress test",
                .return_code = CS_UNKNOWN
            };
        };
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Stress test remove completed in %.6f seconds\n", elapsed);
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
    test_unordered_map_init_null_pointer,
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