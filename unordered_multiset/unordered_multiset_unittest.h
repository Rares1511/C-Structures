#include <cs/unordered_multiset.h>

#include <unittest.h>
#include <string.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_multiset_init_basic() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered_multiset",
            .return_code = rc,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_init_null_pointer() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    cs_codes rc = unordered_multiset_init(NULL, attr, hash_int, 10);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL pointer",
            .return_code = CS_UNKNOWN,
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_init_invalid_capacity() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 0);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero capacity",
            .return_code = CS_UNKNOWN,
        };
    }

    rc = unordered_multiset_init(&umset, attr, hash_int, -5);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for negative capacity",
            .return_code = CS_UNKNOWN,
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_init_invalid_size() {
    unordered_multiset_attr_t attr = {
        .size = 0,
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero element size",
            .return_code = CS_UNKNOWN,
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_init_null_hash() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, NULL, 10);

    // Should succeed with default hash
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Init with NULL hash should work with default hash",
            .return_code = rc,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              INSERT TESTS                                  */
/******************************************************************************/

test_res test_unordered_multiset_insert_basic() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    rc = unordered_multiset_insert(&umset, &val);

    if (rc != CS_SUCCESS) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to insert element",
            .return_code = rc,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_insert_multiple() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 100; i++) {
        rc = unordered_multiset_insert(&umset, &i);
        if (rc != CS_SUCCESS) {
            unordered_multiset_free(&umset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert element",
                .return_code = rc,
            };
        }
    }

    if (unordered_multiset_size(umset) != 100) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after multiple inserts",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_insert_duplicates() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    // Insert same value multiple times - multiset should allow this
    for (int i = 0; i < 5; i++) {
        rc = unordered_multiset_insert(&umset, &val);
        if (rc != CS_SUCCESS) {
            unordered_multiset_free(&umset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert duplicate",
                .return_code = rc,
            };
        }
    }

    // Size should be 5 (multiset allows duplicates)
    if (unordered_multiset_size(umset) != 5) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 5 after inserting 5 duplicates",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_insert_null_set() {
    int val = 42;
    cs_codes rc = unordered_multiset_insert(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL set",
            .return_code = CS_UNKNOWN,
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_insert_null_key() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    rc = unordered_multiset_insert(&umset, NULL);

    if (rc != CS_NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              FIND TESTS                                    */
/******************************************************************************/

test_res test_unordered_multiset_find_existing() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    unordered_multiset_insert(&umset, &val);

    void *found = unordered_multiset_find(umset, &val);

    if (found == NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find existing element",
            .return_code = CS_UNKNOWN,
        };
    }

    if (*(int *)found != 42) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Found element value mismatch",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_find_nonexistent() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    unordered_multiset_insert(&umset, &val);

    int search = 99;
    void *found = unordered_multiset_find(umset, &search);

    if (found != NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should not find nonexistent element",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_find_null_key() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    void *found = unordered_multiset_find(umset, NULL);

    if (found != NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should return NULL for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_find_after_many_inserts() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 100; i++) {
        unordered_multiset_insert(&umset, &i);
    }

    // Find all inserted elements
    for (int i = 0; i < 100; i++) {
        void *found = unordered_multiset_find(umset, &i);
        if (found == NULL || *(int *)found != i) {
            unordered_multiset_free(&umset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to find element after many inserts",
                .return_code = CS_UNKNOWN,
            };
        }
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              ERASE TESTS                                   */
/******************************************************************************/

test_res test_unordered_multiset_erase_existing() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    unordered_multiset_insert(&umset, &val);

    rc = unordered_multiset_erase(&umset, &val);

    if (rc != CS_SUCCESS) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to erase element",
            .return_code = rc,
        };
    }

    // Verify element is gone
    if (unordered_multiset_find(umset, &val) != NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Element still exists after erase",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_erase_one_of_many() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    // Insert 5 duplicates
    for (int i = 0; i < 5; i++) {
        unordered_multiset_insert(&umset, &val);
    }

    // Erase one
    rc = unordered_multiset_erase(&umset, &val);
    if (rc != CS_SUCCESS) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to erase element",
            .return_code = rc,
        };
    }

    // Should still have 4 elements
    if (unordered_multiset_size(umset) != 4) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 4 after erasing one duplicate",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_erase_nonexistent() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    rc = unordered_multiset_erase(&umset, &val);

    // Should fail for nonexistent element
    if (rc == CS_SUCCESS) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Erase should fail for nonexistent element",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_erase_null_set() {
    int val = 42;
    cs_codes rc = unordered_multiset_erase(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL set",
            .return_code = CS_UNKNOWN,
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_erase_null_key() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    rc = unordered_multiset_erase(&umset, NULL);

    if (rc != CS_NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_erase_all() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 50; i++) {
        unordered_multiset_insert(&umset, &i);
    }

    // Erase all elements
    for (int i = 0; i < 50; i++) {
        rc = unordered_multiset_erase(&umset, &i);
        if (rc != CS_SUCCESS) {
            unordered_multiset_free(&umset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to erase element",
                .return_code = rc,
            };
        }
    }

    if (!unordered_multiset_empty(umset)) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after erasing all",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              COUNT TESTS                                   */
/******************************************************************************/

test_res test_unordered_multiset_count_zero() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    int count = unordered_multiset_count(umset, &val);

    if (count != 0) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 0 for nonexistent element",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_count_single() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    unordered_multiset_insert(&umset, &val);

    int count = unordered_multiset_count(umset, &val);

    if (count != 1) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 1 for single element",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_count_multiple() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    for (int i = 0; i < 7; i++) {
        unordered_multiset_insert(&umset, &val);
    }

    int count = unordered_multiset_count(umset, &val);

    if (count != 7) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 7 after inserting 7 duplicates",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_count_null_key() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int count = unordered_multiset_count(umset, NULL);

    if (count != 0) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should return 0 for NULL key",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_count_after_erase() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    for (int i = 0; i < 5; i++) {
        unordered_multiset_insert(&umset, &val);
    }

    // Erase 2
    unordered_multiset_erase(&umset, &val);
    unordered_multiset_erase(&umset, &val);

    int count = unordered_multiset_count(umset, &val);

    if (count != 3) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 3 after erasing 2 from 5",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_unordered_multiset_size_empty() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    if (unordered_multiset_size(umset) != 0) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 0 for empty set",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_size_after_inserts() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 25; i++) {
        unordered_multiset_insert(&umset, &i);
    }

    if (unordered_multiset_size(umset) != 25) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after inserts",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_size_after_erase() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 10; i++) {
        unordered_multiset_insert(&umset, &i);
    }

    for (int i = 0; i < 5; i++) {
        unordered_multiset_erase(&umset, &i);
    }

    if (unordered_multiset_size(umset) != 5) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 5 after erasing 5 elements",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_empty_true() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    if (!unordered_multiset_empty(umset)) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Newly created set should be empty",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_empty_false() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    unordered_multiset_insert(&umset, &val);

    if (unordered_multiset_empty(umset)) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set with element should not be empty",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_empty_after_erase_all() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 10; i++) {
        unordered_multiset_insert(&umset, &i);
    }

    for (int i = 0; i < 10; i++) {
        unordered_multiset_erase(&umset, &i);
    }

    if (!unordered_multiset_empty(umset)) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after erasing all elements",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_unordered_multiset_clear_basic() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 50; i++) {
        unordered_multiset_insert(&umset, &i);
    }

    unordered_multiset_clear(&umset);

    if (!unordered_multiset_empty(umset)) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after clear",
            .return_code = CS_UNKNOWN,
        };
    }

    if (unordered_multiset_size(umset) != 0) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 0 after clear",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_clear_empty() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    unordered_multiset_clear(&umset);

    if (!unordered_multiset_empty(umset)) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Empty set should remain empty after clear",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_clear_null() {
    // Clear NULL should not crash
    unordered_multiset_clear(NULL);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_unordered_multiset_free_null() {
    // Free NULL should not crash
    unordered_multiset_free(NULL);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_free_empty() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_free_with_elements() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 100; i++) {
        unordered_multiset_insert(&umset, &i);
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_unordered_multiset_large_capacity() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 1000);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with large capacity",
            .return_code = rc,
        };
    }

    for (int i = 0; i < 5000; i++) {
        rc = unordered_multiset_insert(&umset, &i);
        if (rc != CS_SUCCESS) {
            unordered_multiset_free(&umset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert during stress test",
                .return_code = rc,
            };
        }
    }

    if (unordered_multiset_size(umset) != 5000) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch in stress test",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_small_capacity_many_inserts() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 2);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with small capacity",
            .return_code = rc,
        };
    }

    // Insert many more elements than initial capacity
    for (int i = 0; i < 100; i++) {
        rc = unordered_multiset_insert(&umset, &i);
        if (rc != CS_SUCCESS) {
            unordered_multiset_free(&umset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert beyond initial capacity",
                .return_code = rc,
            };
        }
    }

    if (unordered_multiset_size(umset) != 100) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after resize",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_insert_erase_cycle() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    // Multiple cycles of insert and erase
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            unordered_multiset_insert(&umset, &i);
        }
        for (int i = 0; i < 20; i++) {
            unordered_multiset_erase(&umset, &i);
        }
    }

    if (!unordered_multiset_empty(umset)) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after cycles",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_negative_values() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    for (int i = -50; i < 50; i++) {
        rc = unordered_multiset_insert(&umset, &i);
        if (rc != CS_SUCCESS) {
            unordered_multiset_free(&umset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert negative value",
                .return_code = rc,
            };
        }
    }

    if (unordered_multiset_size(umset) != 100) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch with negative values",
            .return_code = CS_UNKNOWN,
        };
    }

    // Verify negative values can be found
    int neg = -25;
    if (unordered_multiset_find(umset, &neg) == NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find negative value",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_reinsert_after_erase() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    unordered_multiset_insert(&umset, &val);
    unordered_multiset_erase(&umset, &val);

    // Re-insert the same value
    rc = unordered_multiset_insert(&umset, &val);
    if (rc != CS_SUCCESS) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to re-insert after erase",
            .return_code = rc,
        };
    }

    if (unordered_multiset_find(umset, &val) == NULL) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find re-inserted element",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_unordered_multiset_many_duplicates() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = print_int,
    };

    unordered_multiset umset;
    cs_codes rc = unordered_multiset_init(&umset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc,
        };
    }

    int val = 42;
    for (int i = 0; i < 100; i++) {
        unordered_multiset_insert(&umset, &val);
    }

    if (unordered_multiset_count(umset, &val) != 100) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 100 for 100 duplicates",
            .return_code = CS_UNKNOWN,
        };
    }

    if (unordered_multiset_size(umset) != 100) {
        unordered_multiset_free(&umset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 100 for 100 duplicates",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(&umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test unordered_multiset_tests[] = {
    // Init tests
    test_unordered_multiset_init_basic,
    test_unordered_multiset_init_null_pointer,
    test_unordered_multiset_init_invalid_capacity,
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
    test_unordered_multiset_negative_values,
    test_unordered_multiset_reinsert_after_erase,
    test_unordered_multiset_many_duplicates,
};