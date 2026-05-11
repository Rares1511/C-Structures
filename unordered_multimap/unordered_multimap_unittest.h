#include <cs/unordered_multimap.h>

#include <unittest.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_multimap_init_basic(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize unordered_multimap",
            .return_code = rc,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_multimap with int keys and values\n");

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_init_null_pointer(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    cs_codes rc = unordered_multimap_init(NULL, key_attr, value_attr, hash_int);
    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL pointer",
            .return_code = CS_UNKNOWN,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_NULL for NULL pointer\n");

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_init_invalid_key_size(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_SIZE for zero key size",
            .return_code = CS_UNKNOWN,
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_init_invalid_value_size(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = 0,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_SIZE for zero value size",
            .return_code = CS_UNKNOWN,
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_init_null_hash(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, NULL);

    // Should succeed with default hash
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Init with NULL hash should work",
            .return_code = rc,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              ADD ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_multimap_add_entry_basic(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int value = 100;
    rc = unordered_multimap_add_entry(ummap, &key, &value);

    if (rc != CS_SUCCESS) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to add entry",
            .return_code = rc,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_add_entry_multiple(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        rc = unordered_multimap_add_entry(ummap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to add entry",
                .return_code = rc,
            };
        }
    }

    size_t size = unordered_multimap_size(ummap);

    if (size != 100) {
        unordered_multimap_free(ummap);
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected size 100, got %zu\n", size);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after multiple adds",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_add_entry_duplicate_keys(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    // Add same key with different values - multimap should allow this
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        rc = unordered_multimap_add_entry(ummap, &key, &value);
        if (rc != CS_SUCCESS) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to add duplicate key",
                .return_code = rc,
            };
        }
    }

    // Size should be 5 (multimap allows duplicate keys)
    if (unordered_multimap_size(ummap) != 5) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 5 after adding 5 entries with same key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_add_entry_null_map(test_arg *arg) {
    int key = 42;
    int value = 100;
    cs_codes rc = unordered_multimap_add_entry(NULL, &key, &value);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL map",
            .return_code = CS_UNKNOWN,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_NULL for NULL map\n");

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_add_entry_null_key(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int value = 100;
    rc = unordered_multimap_add_entry(ummap, NULL, &value);

    if (rc != CS_NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_add_entry_null_value(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    rc = unordered_multimap_add_entry(ummap, &key, NULL);

    if (rc != CS_NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL value",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              GET ENTRY TESTS                               */
/******************************************************************************/

test_res test_unordered_multimap_get_entry_existing(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int value = 100;
    unordered_multimap_add_entry(ummap, &key, &value);

    void *found = unordered_multimap_get_entry(ummap, &key);

    if (found == NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to get existing entry",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_get_entry_nonexistent(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int value = 100;
    unordered_multimap_add_entry(ummap, &key, &value);

    int search = 99;
    void *found = unordered_multimap_get_entry(ummap, &search);

    if (found != NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should not find nonexistent entry",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_get_entry_null_key(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    void *found = unordered_multimap_get_entry(ummap, NULL);

    if (found != NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should return NULL for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_get_entry_after_many_adds(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    // Find all entries
    for (int i = 0; i < 100; i++) {
        void *found = unordered_multimap_get_entry(ummap, &i);
        if (found == NULL) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to get entry after many adds",
                .return_code = CS_UNKNOWN,
            };
        }
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              REMOVE ENTRY TESTS                            */
/******************************************************************************/

test_res test_unordered_multimap_remove_entry_existing(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int value = 100;
    unordered_multimap_add_entry(ummap, &key, &value);

    rc = unordered_multimap_remove_entry(ummap, &key);

    if (rc != CS_SUCCESS) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to remove entry",
            .return_code = rc,
        };
    }

    // Verify entry is gone
    if (unordered_multimap_get_entry(ummap, &key) != NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Entry still exists after remove",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_remove_entry_one_of_many(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    // Add 5 entries with same key
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &key, &value);
    }

    // Remove one
    rc = unordered_multimap_remove_entry(ummap, &key);
    if (rc != CS_SUCCESS) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to remove entry",
            .return_code = rc,
        };
    }

    // Should still have 4 entries
    if (unordered_multimap_size(ummap) != 4) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 4 after removing one of 5 duplicates",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_remove_entry_nonexistent(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_multimap for remove nonexistent entry test\n");

    int key = 42;
    rc = unordered_multimap_remove_entry(ummap, &key);

    // Should fail for nonexistent entry
    if (rc == CS_SUCCESS) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Remove should fail for nonexistent entry",
            .return_code = CS_UNKNOWN,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly failed to remove nonexistent entry\n");

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_remove_entry_null_map(test_arg *arg) {
    int key = 42;
    cs_codes rc = unordered_multimap_remove_entry(NULL, &key);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL map",
            .return_code = CS_UNKNOWN,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_NULL for NULL map\n");

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_remove_entry_null_key(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    rc = unordered_multimap_remove_entry(ummap, NULL);

    if (rc != CS_NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_remove_entry_all(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    // Remove all entries
    for (int i = 0; i < 50; i++) {
        rc = unordered_multimap_remove_entry(ummap, &i);
        if (rc != CS_SUCCESS) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to remove entry",
                .return_code = rc,
            };
        }
    }

    if (!unordered_multimap_empty(ummap)) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Map should be empty after removing all",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              COUNT TESTS                                   */
/******************************************************************************/

test_res test_unordered_multimap_count_zero(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int count = unordered_multimap_count(ummap, &key);

    if (count != 0) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Count should be 0 for nonexistent key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_count_single(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int value = 100;
    unordered_multimap_add_entry(ummap, &key, &value);

    int count = unordered_multimap_count(ummap, &key);

    if (count != 1) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Count should be 1 for single entry",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_count_multiple(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    for (int i = 0; i < 7; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &key, &value);
    }

    int count = unordered_multimap_count(ummap, &key);

    if (count != 7) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Count should be 7 after adding 7 entries with same key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_count_null_key(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int count = unordered_multimap_count(ummap, NULL);

    if (count != 0) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Count should return 0 for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_count_after_remove(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &key, &value);
    }

    // Remove all entries with the same key
    unordered_multimap_remove_entry(ummap, &key);

    int count = unordered_multimap_count(ummap, &key);

    if (count != 0) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Count should be 0 after removing all entries with the same key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_unordered_multimap_size_empty(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    if (unordered_multimap_size(ummap) != 0) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 for empty map",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_size_after_adds(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 25; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    if (unordered_multimap_size(ummap) != 25) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after adds",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_size_after_remove(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    for (int i = 0; i < 5; i++) {
        unordered_multimap_remove_entry(ummap, &i);
    }

    if (unordered_multimap_size(ummap) != 5) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 5 after removing 5 entries",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_empty_true(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    if (!unordered_multimap_empty(ummap)) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Newly created map should be empty",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_empty_false(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int value = 100;
    unordered_multimap_add_entry(ummap, &key, &value);

    if (unordered_multimap_empty(ummap)) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Map with entry should not be empty",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_empty_after_remove_all(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    for (int i = 0; i < 10; i++) {
        unordered_multimap_remove_entry(ummap, &i);
    }

    if (!unordered_multimap_empty(ummap)) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Map should be empty after removing all entries",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_unordered_multimap_clear_basic(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    unordered_multimap_clear(ummap);

    if (!unordered_multimap_empty(ummap)) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Map should be empty after clear",
            .return_code = CS_UNKNOWN,
        };
    }

    if (unordered_multimap_size(ummap) != 0) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 after clear",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_clear_empty(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_multimap for clear empty test\n");

    unordered_multimap_clear(ummap);

    if (!unordered_multimap_empty(ummap)) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty map should remain empty after clear",
            .return_code = CS_UNKNOWN,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly handled clear on empty map\n");

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              SWAP TESTS                                    */
/******************************************************************************/

test_res test_unordered_multimap_swap_basic(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap1 = (unordered_multimap *)arg->data_structure;
    unordered_multimap ummap2;
    cs_codes rc = unordered_multimap_init(ummap1, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize ummap1",
            .return_code = rc,
        };
    }

    rc = unordered_multimap_init(&ummap2, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        unordered_multimap_free(ummap1);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize ummap2",
            .return_code = rc,
        };
    }

    // Add entries to ummap1
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap1, &i, &value);
    }

    // Add entries to ummap2
    for (int i = 100; i < 103; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(&ummap2, &i, &value);
    }

    size_t size1_before = unordered_multimap_size(ummap1);
    size_t size2_before = unordered_multimap_size(&ummap2);

    unordered_multimap_swap(ummap1, &ummap2);

    if (unordered_multimap_size(ummap1) != size2_before) {
        unordered_multimap_free(ummap1);
        unordered_multimap_free(&ummap2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "ummap1 size should match ummap2's old size",
            .return_code = CS_UNKNOWN,
        };
    }

    if (unordered_multimap_size(&ummap2) != size1_before) {
        unordered_multimap_free(ummap1);
        unordered_multimap_free(&ummap2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "ummap2 size should match ummap1's old size",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap1);
    unordered_multimap_free(&ummap2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_swap_with_empty(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap1 = (unordered_multimap *)arg->data_structure;
    unordered_multimap ummap2;
    cs_codes rc = unordered_multimap_init(ummap1, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize ummap1",
            .return_code = rc,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized ummap1 for swap with empty test\n");

    rc = unordered_multimap_init(&ummap2, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        unordered_multimap_free(ummap1);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize ummap2",
            .return_code = rc,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized ummap2 for swap with empty test\n");

    // Add entries only to ummap1
    for (int i = 0; i < 10; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap1, &i, &value);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Added entries to ummap1 for swap with empty test\n");

    unordered_multimap_swap(ummap1, &ummap2);

    if (!unordered_multimap_empty(ummap1)) {
        unordered_multimap_free(ummap1);
        unordered_multimap_free(&ummap2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "ummap1 should be empty after swap",
            .return_code = CS_UNKNOWN,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "ummap1 is empty after swap as expected\n");

    if (unordered_multimap_size(&ummap2) != 10) {
        unordered_multimap_free(ummap1);
        unordered_multimap_free(&ummap2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "ummap2 should have 10 entries after swap",
            .return_code = CS_UNKNOWN,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "ummap2 has 10 entries after swap as expected\n");

    unordered_multimap_free(ummap1);
    unordered_multimap_free(&ummap2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_unordered_multimap_free_empty(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized unordered_multimap for free empty test\n");

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_free_with_entries(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    unordered_multimap_free(ummap);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Successfully freed unordered_multimap with entries\n");

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_unordered_multimap_large_capacity(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize with large capacity",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 5000; i++) {
        int value = i * 10;
        rc = unordered_multimap_add_entry(ummap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to add during stress test",
                .return_code = rc,
            };
        }
    }

    if (unordered_multimap_size(ummap) != 5000) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch in stress test",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_small_capacity_many_adds(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize with small capacity",
            .return_code = rc,
        };
    }

    // Add many more entries than initial capacity
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        rc = unordered_multimap_add_entry(ummap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to add beyond initial capacity",
                .return_code = rc,
            };
        }
    }

    if (unordered_multimap_size(ummap) != 100) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch after resize",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_add_remove_cycle(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    // Multiple cycles of add and remove
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            int value = i * 10;
            unordered_multimap_add_entry(ummap, &i, &value);
        }
        for (int i = 0; i < 20; i++) {
            unordered_multimap_remove_entry(ummap, &i);
        }
    }

    if (!unordered_multimap_empty(ummap)) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Map should be empty after cycles",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_negative_keys(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = -50; i < 50; i++) {
        int value = i * 10;
        rc = unordered_multimap_add_entry(ummap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to add with negative key",
                .return_code = rc,
            };
        }
    }

    if (unordered_multimap_size(ummap) != 100) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size mismatch with negative keys",
            .return_code = CS_UNKNOWN,
        };
    }

    // Verify negative keys can be found
    int neg = -25;
    if (unordered_multimap_get_entry(ummap, &neg) == NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to find negative key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_readd_after_remove(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    int value = 100;
    unordered_multimap_add_entry(ummap, &key, &value);
    unordered_multimap_remove_entry(ummap, &key);

    // Re-add the same key
    value = 200;
    rc = unordered_multimap_add_entry(ummap, &key, &value);
    if (rc != CS_SUCCESS) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to re-add after remove",
            .return_code = rc,
        };
    }

    if (unordered_multimap_get_entry(ummap, &key) == NULL) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to find re-added entry",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_many_duplicates(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int key = 42;
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &key, &value);
    }

    int count = unordered_multimap_count(ummap, &key);
    if (count != 100) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected count of 100 for duplicates, got %d", count);
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Count should be 100 for 100 duplicates",
            .return_code = CS_UNKNOWN,
        };
    }

    if (unordered_multimap_size(ummap) != 100) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100 for 100 duplicates",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_clear_and_reuse(test_arg *arg) {
    elem_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    elem_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL,
    };

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    cs_codes rc = unordered_multimap_init(ummap, key_attr, value_attr, hash_int);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    // Add, clear, and add again
    for (int i = 0; i < 50; i++) {
        int value = i * 10;
        unordered_multimap_add_entry(ummap, &i, &value);
    }

    unordered_multimap_clear(ummap);

    for (int i = 100; i < 150; i++) {
        int value = i * 10;
        rc = unordered_multimap_add_entry(ummap, &i, &value);
        if (rc != CS_SUCCESS) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to add after clear",
                .return_code = rc,
            };
        }
    }

    if (unordered_multimap_size(ummap) != 50) {
        unordered_multimap_free(ummap);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 50 after clear and reuse",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(ummap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multimap_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        clogger_log(*arg->logger, CLOGGER_DEBUG, "Skipping time stress test on Valgrind");
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Skipping time stress test on Valgrind",
            .return_code = CS_SUCCESS,
        };
    }

    unordered_multimap *ummap = (unordered_multimap *)arg->data_structure;
    struct timeval start, end;
    double elapsed;

    if (CS_SUCCESS != unordered_multimap_init(ummap, get_int_attr(), get_int_attr(), hash_int)) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = CS_UNKNOWN,
        };
    }

    int total = __UNORDERED_MULTIMAP_STRESS_TEST_SIZE;
    
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        if (CS_SUCCESS != unordered_multimap_add_entry(ummap, &i, &i)) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert during stress test",
                .return_code = CS_UNKNOWN,
            };
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(*arg->logger, CLOGGER_INFO, "Inserted %d elements in %.4f seconds", total, elapsed);
    post_operation_time(arg, "insert", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        vector *found = unordered_multimap_get_entry(ummap, &i);
        if (found == NULL || vector_size(found) == 0) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to find element during stress test",
                .return_code = CS_UNKNOWN,
            };
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(*arg->logger, CLOGGER_INFO, "Found %d elements in %.4f seconds", total, elapsed);
    post_operation_time(arg, "find", elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        if (CS_SUCCESS != unordered_multimap_remove_entry(ummap, &i)) {
            unordered_multimap_free(ummap);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to erase element during stress test",
                .return_code = CS_UNKNOWN,
            };
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    clogger_log(*arg->logger, CLOGGER_INFO, "Erased %d elements in %.4f seconds", total, elapsed);
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

test unordered_multimap_tests[] = {
    // Init tests
    test_unordered_multimap_init_basic,
    test_unordered_multimap_init_null_pointer,
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
    test_unordered_multimap_negative_keys,
    test_unordered_multimap_readd_after_remove,
    test_unordered_multimap_many_duplicates,
    test_unordered_multimap_clear_and_reuse,
    test_unordered_multimap_stress_time,
};