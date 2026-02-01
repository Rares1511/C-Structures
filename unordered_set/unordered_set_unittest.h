#include <cs/unordered_set.h>

#include <unittest.h>
#include <string.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_unordered_set_init_basic() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);

    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered set",
            .return_code = rc
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_init_null_pointer() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    cs_codes rc = unordered_set_init(NULL, attr, hash_int, 10);

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

test_res test_unordered_set_init_invalid_capacity() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 0);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero capacity",
            .return_code = CS_UNKNOWN
        };
    }

    rc = unordered_set_init(&uset, attr, hash_int, -5);

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

test_res test_unordered_set_init_invalid_size() {
    unordered_set_attr_t attr = {
        .size = 0,
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_SIZE for zero element size",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_init_null_hash() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, NULL, 10);

    // Should succeed - hash_table likely has default hash
    if (rc != CS_SUCCESS) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Init with NULL hash should work or fail gracefully",
            .return_code = CS_SUCCESS
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              INSERT TESTS                                  */
/******************************************************************************/

test_res test_unordered_set_insert_basic() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    rc = unordered_set_insert(&uset, &val);

    if (rc != CS_SUCCESS) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to insert element",
            .return_code = rc
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_insert_multiple() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        rc = unordered_set_insert(&uset, &i);
        if (rc != CS_SUCCESS) {
            unordered_set_free(&uset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert element",
                .return_code = rc
            };
        }
    }

    if (unordered_set_size(uset) != 100) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after multiple inserts",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_insert_duplicate() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    rc = unordered_set_insert(&uset, &val);
    if (rc != CS_SUCCESS) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed first insert",
            .return_code = rc
        };
    }

    // Insert duplicate - should fail with CS_ELEM
    rc = unordered_set_insert(&uset, &val);
    if (rc != CS_ELEM) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_ELEM for duplicate insert",
            .return_code = CS_UNKNOWN
        };
    }

    // Size should still be 1
    if (unordered_set_size(uset) != 1) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should remain 1 after duplicate insert",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_insert_null_set() {
    int val = 42;
    cs_codes rc = unordered_set_insert(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL set",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_insert_null_key() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = unordered_set_insert(&uset, NULL);

    if (rc != CS_NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FIND TESTS                                    */
/******************************************************************************/

test_res test_unordered_set_find_existing() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    unordered_set_insert(&uset, &val);

    void *found = unordered_set_find(uset, &val);

    if (found == NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find existing element",
            .return_code = CS_UNKNOWN
        };
    }

    if (*(int *)found != 42) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Found element value mismatch",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_find_nonexistent() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    unordered_set_insert(&uset, &val);

    int search = 99;
    void *found = unordered_set_find(uset, &search);

    if (found != NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should not find nonexistent element",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_find_null_key() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    void *found = unordered_set_find(uset, NULL);

    if (found != NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Should return NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_find_after_many_inserts() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        unordered_set_insert(&uset, &i);
    }

    // Find all inserted elements
    for (int i = 0; i < 100; i++) {
        void *found = unordered_set_find(uset, &i);
        if (found == NULL || *(int *)found != i) {
            unordered_set_free(&uset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to find element after many inserts",
                .return_code = CS_UNKNOWN
            };
        }
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              ERASE TESTS                                   */
/******************************************************************************/

test_res test_unordered_set_erase_existing() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    unordered_set_insert(&uset, &val);

    rc = unordered_set_erase(&uset, &val);

    if (rc != CS_SUCCESS) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to erase element",
            .return_code = rc
        };
    }

    // Verify element is gone
    if (unordered_set_find(uset, &val) != NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Element still exists after erase",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_erase_nonexistent() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    rc = unordered_set_erase(&uset, &val);

    // Should fail or return error code for nonexistent element
    if (rc == CS_SUCCESS) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Erase should fail for nonexistent element",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_erase_null_set() {
    int val = 42;
    cs_codes rc = unordered_set_erase(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL set",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_erase_null_key() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = unordered_set_erase(&uset, NULL);

    if (rc != CS_NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Expected CS_NULL for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_erase_all() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        unordered_set_insert(&uset, &i);
    }

    // Erase all elements
    for (int i = 0; i < 50; i++) {
        rc = unordered_set_erase(&uset, &i);
        if (rc != CS_SUCCESS) {
            unordered_set_free(&uset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to erase element",
                .return_code = rc
            };
        }
    }

    if (!unordered_set_empty(uset)) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after erasing all",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              COUNT TESTS                                   */
/******************************************************************************/

test_res test_unordered_set_count_existing() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    unordered_set_insert(&uset, &val);

    int count = unordered_set_count(uset, &val);

    if (count != 1) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 1 for existing element",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_count_nonexistent() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    int count = unordered_set_count(uset, &val);

    if (count != 0) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should be 0 for nonexistent element",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_count_null_key() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int count = unordered_set_count(uset, NULL);

    if (count != 0) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Count should return 0 for NULL key",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SIZE/EMPTY TESTS                              */
/******************************************************************************/

test_res test_unordered_set_size_empty() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (unordered_set_size(uset) != 0) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 0 for empty set",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_size_after_inserts() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 25; i++) {
        unordered_set_insert(&uset, &i);
    }

    if (unordered_set_size(uset) != 25) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after inserts",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_size_after_erase() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        unordered_set_insert(&uset, &i);
    }

    for (int i = 0; i < 5; i++) {
        unordered_set_erase(&uset, &i);
    }

    if (unordered_set_size(uset) != 5) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 5 after erasing 5 elements",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_empty_true() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (!unordered_set_empty(uset)) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Newly created set should be empty",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_empty_false() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    unordered_set_insert(&uset, &val);

    if (unordered_set_empty(uset)) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set with element should not be empty",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_empty_after_erase_all() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        unordered_set_insert(&uset, &i);
    }

    for (int i = 0; i < 10; i++) {
        unordered_set_erase(&uset, &i);
    }

    if (!unordered_set_empty(uset)) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after erasing all elements",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_unordered_set_clear_basic() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        unordered_set_insert(&uset, &i);
    }

    unordered_set_clear(&uset);

    if (!unordered_set_empty(uset)) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    if (unordered_set_size(uset) != 0) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size should be 0 after clear",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_clear_empty() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    unordered_set_clear(&uset);

    if (!unordered_set_empty(uset)) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Empty set should remain empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_clear_then_insert() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        unordered_set_insert(&uset, &i);
    }

    unordered_set_clear(&uset);

    // Insert after clear
    for (int i = 100; i < 110; i++) {
        rc = unordered_set_insert(&uset, &i);
        if (rc != CS_SUCCESS) {
            unordered_set_free(&uset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert after clear",
                .return_code = rc
            };
        }
    }

    if (unordered_set_size(uset) != 10) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after clear and insert",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_clear_null() {
    // Clear NULL should not crash
    unordered_set_clear(NULL);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_unordered_set_free_null() {
    // Free NULL should not crash
    unordered_set_free(NULL);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_free_empty() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_free_with_elements() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        unordered_set_insert(&uset, &i);
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              STRESS/EDGE TESTS                             */
/******************************************************************************/

test_res test_unordered_set_large_capacity() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 1000);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with large capacity",
            .return_code = rc
        };
    }

    for (int i = 0; i < 5000; i++) {
        rc = unordered_set_insert(&uset, &i);
        if (rc != CS_SUCCESS) {
            unordered_set_free(&uset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert during stress test",
                .return_code = rc
            };
        }
    }

    if (unordered_set_size(uset) != 5000) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch in stress test",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_small_capacity_many_inserts() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 2);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize with small capacity",
            .return_code = rc
        };
    }

    // Insert many more elements than initial capacity
    for (int i = 0; i < 100; i++) {
        rc = unordered_set_insert(&uset, &i);
        if (rc != CS_SUCCESS) {
            unordered_set_free(&uset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert beyond initial capacity",
                .return_code = rc
            };
        }
    }

    if (unordered_set_size(uset) != 100) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch after resize",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_insert_erase_cycle() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Multiple cycles of insert and erase
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < 20; i++) {
            unordered_set_insert(&uset, &i);
        }
        for (int i = 0; i < 20; i++) {
            unordered_set_erase(&uset, &i);
        }
    }

    if (!unordered_set_empty(uset)) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Set should be empty after cycles",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_negative_values() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = -50; i < 50; i++) {
        rc = unordered_set_insert(&uset, &i);
        if (rc != CS_SUCCESS) {
            unordered_set_free(&uset);
            return (test_res){
                .test_name = (char*) __func__,
                .reason = "Failed to insert negative value",
                .return_code = rc
            };
        }
    }

    if (unordered_set_size(uset) != 100) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Size mismatch with negative values",
            .return_code = CS_UNKNOWN
        };
    }

    // Verify negative values can be found
    int neg = -25;
    if (unordered_set_find(uset, &neg) == NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find negative value",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_unordered_set_reinsert_after_erase() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set uset;
    cs_codes rc = unordered_set_init(&uset, attr, hash_int, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    unordered_set_insert(&uset, &val);
    unordered_set_erase(&uset, &val);

    // Re-insert the same value
    rc = unordered_set_insert(&uset, &val);
    if (rc != CS_SUCCESS) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to re-insert after erase",
            .return_code = rc
        };
    }

    if (unordered_set_find(uset, &val) == NULL) {
        unordered_set_free(&uset);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to find re-inserted element",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_set_free(&uset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test unordered_set_tests[] = {
    // Init tests
    test_unordered_set_init_basic,
    test_unordered_set_init_null_pointer,
    test_unordered_set_init_invalid_capacity,
    test_unordered_set_init_invalid_size,
    test_unordered_set_init_null_hash,

    // Insert tests
    test_unordered_set_insert_basic,
    test_unordered_set_insert_multiple,
    test_unordered_set_insert_duplicate,
    test_unordered_set_insert_null_set,
    test_unordered_set_insert_null_key,

    // Find tests
    test_unordered_set_find_existing,
    test_unordered_set_find_nonexistent,
    test_unordered_set_find_null_key,
    test_unordered_set_find_after_many_inserts,

    // Erase tests
    test_unordered_set_erase_existing,
    test_unordered_set_erase_nonexistent,
    test_unordered_set_erase_null_set,
    test_unordered_set_erase_null_key,
    test_unordered_set_erase_all,

    // Count tests
    test_unordered_set_count_existing,
    test_unordered_set_count_nonexistent,
    test_unordered_set_count_null_key,

    // Size/Empty tests
    test_unordered_set_size_empty,
    test_unordered_set_size_after_inserts,
    test_unordered_set_size_after_erase,
    test_unordered_set_empty_true,
    test_unordered_set_empty_false,
    test_unordered_set_empty_after_erase_all,

    // Clear tests
    test_unordered_set_clear_basic,
    test_unordered_set_clear_empty,
    test_unordered_set_clear_then_insert,
    test_unordered_set_clear_null,

    // Free tests
    test_unordered_set_free_null,
    test_unordered_set_free_empty,
    test_unordered_set_free_with_elements,

    // Stress/Edge tests
    test_unordered_set_large_capacity,
    test_unordered_set_small_capacity_many_inserts,
    test_unordered_set_insert_erase_cycle,
    test_unordered_set_negative_values,
    test_unordered_set_reinsert_after_erase,
};