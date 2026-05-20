#include <cs/flat_set.h>

#include <unittest.h>

/******************************************************************************/
/*                              INIT TESTS                                    */
/******************************************************************************/

test_res test_flat_set_init_vector() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize vector flat_set",
            .return_code = rc
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_init_deque() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque flat_set",
            .return_code = rc
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_init_default() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_DEFAULT);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize default flat_set",
            .return_code = rc
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_init_null_pointer() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    cs_codes rc = flat_set_init(NULL, attr, CS_FLAT_SET_VECTOR);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL pointer",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_init_invalid_size() {
    elem_attr_t attr = {
        .size = 0,
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);

    if (rc != CS_SIZE) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_SIZE for zero element size",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              INSERT TESTS                                  */
/******************************************************************************/

test_res test_flat_set_insert_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    rc = flat_set_insert(&fs, &val);
    if (rc != CS_SUCCESS) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to insert element",
            .return_code = rc
        };
    }

    if (flat_set_size(fs) != 1) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 1 after insert",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_insert_multiple() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        rc = flat_set_insert(&fs, &i);
        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to insert element",
                .return_code = rc
            };
        }
    }

    if (flat_set_size(fs) != 100) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_insert_duplicate() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);
    rc = flat_set_insert(&fs, &val);

    // Set should reject duplicates
    if (flat_set_size(fs) != 1) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Set should not allow duplicates",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_insert_sorted_order() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Insert in random order
    int vals[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs, &vals[i]);
    }

    // Verify sorted order (ascending)
    for (int i = 0; i < 10; i++) {
        int *val = (int *)flat_set_at(fs, i);
        if (val == NULL || *val != i) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Elements not in sorted order",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_insert_null_set() {
    int val = 42;
    cs_codes rc = flat_set_insert(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL set",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_insert_null_data() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    rc = flat_set_insert(&fs, NULL);
    flat_set_free(&fs);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL data",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              ERASE TESTS                                   */
/******************************************************************************/

test_res test_flat_set_erase_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);

    rc = flat_set_erase(&fs, &val);
    if (rc != CS_SUCCESS) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to erase element",
            .return_code = rc
        };
    }

    if (flat_set_size(fs) != 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 after erase",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_erase_multiple() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        flat_set_insert(&fs, &i);
    }

    for (int i = 0; i < 25; i++) {
        rc = flat_set_erase(&fs, &i);
        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to erase element",
                .return_code = rc
            };
        }
    }

    if (flat_set_size(fs) != 25) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 25 after erasing half",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_erase_nonexistent() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);

    int nonexistent = 99;
    rc = flat_set_erase(&fs, &nonexistent);

    // Should return error for nonexistent element
    if (rc == CS_SUCCESS) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should fail to erase nonexistent element",
            .return_code = CS_UNKNOWN
        };
    }

    // Size should be unchanged
    if (flat_set_size(fs) != 1) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be unchanged after failed erase",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_erase_null_set() {
    int val = 42;
    cs_codes rc = flat_set_erase(NULL, &val);

    if (rc != CS_NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Expected CS_NULL for NULL set",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_erase_maintains_order() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Insert 0-9
    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs, &i);
    }

    // Erase 5
    int to_erase = 5;
    flat_set_erase(&fs, &to_erase);

    // Verify order: 0,1,2,3,4,6,7,8,9
    int expected[] = {0, 1, 2, 3, 4, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
        int *val = (int *)flat_set_at(fs, i);
        if (val == NULL || *val != expected[i]) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Order not maintained after erase",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FIND TESTS                                    */
/******************************************************************************/

test_res test_flat_set_find_existing() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);

    int idx = flat_set_find(fs, &val);
    if (idx < 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should find existing element",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_find_nonexistent() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);

    int nonexistent = 99;
    int idx = flat_set_find(fs, &nonexistent);
    if (idx >= 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should not find nonexistent element",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_find_all() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        flat_set_insert(&fs, &i);
    }

    for (int i = 0; i < 100; i++) {
        int idx = flat_set_find(fs, &i);
        if (idx < 0) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to find inserted element",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_find_after_erase() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs, &i);
    }

    int to_erase = 5;
    flat_set_erase(&fs, &to_erase);

    // Should not find erased element
    int idx = flat_set_find(fs, &to_erase);
    if (idx >= 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should not find erased element",
            .return_code = CS_UNKNOWN
        };
    }

    // Should still find others
    int still_exists = 3;
    idx = flat_set_find(fs, &still_exists);
    if (idx < 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should find remaining elements",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              AT TESTS                                      */
/******************************************************************************/

test_res test_flat_set_at_valid() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs, &i);
    }

    for (int i = 0; i < 10; i++) {
        int *val = (int *)flat_set_at(fs, i);
        if (val == NULL || *val != i) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "at() returned wrong value",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_at_out_of_bounds() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);

    void *result = flat_set_at(fs, 10);
    if (result != NULL) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "at() should return NULL for out of bounds",
            .return_code = CS_UNKNOWN
        };
    }

    result = flat_set_at(fs, -1);
    if (result != NULL) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "at() should return NULL for negative index",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          SIZE/EMPTY TESTS                                  */
/******************************************************************************/

test_res test_flat_set_size_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (flat_set_size(fs) != 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 for empty set",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_size_after_insert() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 1; i <= 10; i++) {
        flat_set_insert(&fs, &i);
        if (flat_set_size(fs) != i) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Size mismatch after insert",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_size_after_erase() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs, &i);
    }

    for (int i = 9; i >= 0; i--) {
        flat_set_erase(&fs, &i);
        if (flat_set_size(fs) != i) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Size mismatch after erase",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_empty_true() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    if (!flat_set_empty(fs)) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty should return true for new set",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_empty_false() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);

    if (flat_set_empty(fs)) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty should return false for non-empty set",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_empty_after_erase_all() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs, &i);
    }

    for (int i = 0; i < 10; i++) {
        flat_set_erase(&fs, &i);
    }

    if (!flat_set_empty(fs)) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Empty should return true after erasing all",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              CLEAR TESTS                                   */
/******************************************************************************/

test_res test_flat_set_clear_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        flat_set_insert(&fs, &i);
    }

    flat_set_clear(&fs);

    if (!flat_set_empty(fs)) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Set should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    if (flat_set_size(fs) != 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 0 after clear",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_clear_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Clear an already empty set - should not crash
    flat_set_clear(&fs);

    if (!flat_set_empty(fs)) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Set should still be empty after clearing empty set",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_clear_null() {
    // Clear on NULL should not crash
    flat_set_clear(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_clear_and_reuse() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 50; i++) {
        flat_set_insert(&fs, &i);
    }

    flat_set_clear(&fs);

    // Insert new values
    for (int i = 100; i < 150; i++) {
        rc = flat_set_insert(&fs, &i);
        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to insert after clear",
                .return_code = rc
            };
        }
    }

    if (flat_set_size(fs) != 50) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 50 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              SWAP TESTS                                    */
/******************************************************************************/

test_res test_flat_set_swap_basic() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs1, fs2;
    cs_codes rc1 = flat_set_init(&fs1, attr, CS_FLAT_SET_VECTOR);
    cs_codes rc2 = flat_set_init(&fs2, attr, CS_FLAT_SET_VECTOR);
    if (rc1 != CS_SUCCESS || rc2 != CS_SUCCESS) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize sets",
            .return_code = CS_UNKNOWN
        };
    }

    // Insert 10 elements to fs1
    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs1, &i);
    }

    // Insert 5 elements to fs2
    for (int i = 100; i < 105; i++) {
        flat_set_insert(&fs2, &i);
    }

    flat_set_swap(&fs1, &fs2);

    // fs1 should now have 5 elements starting with 100
    if (flat_set_size(fs1) != 5) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs1 should have 5 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    int *first = (int *)flat_set_at(fs1, 0);
    if (first == NULL || *first != 100) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs1 first element should be 100 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    // fs2 should now have 10 elements starting with 0
    if (flat_set_size(fs2) != 10) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    first = (int *)flat_set_at(fs2, 0);
    if (first == NULL || *first != 0) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs2 first element should be 0 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs1);
    flat_set_free(&fs2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_swap_with_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs1, fs2;
    cs_codes rc1 = flat_set_init(&fs1, attr, CS_FLAT_SET_VECTOR);
    cs_codes rc2 = flat_set_init(&fs2, attr, CS_FLAT_SET_VECTOR);
    if (rc1 != CS_SUCCESS || rc2 != CS_SUCCESS) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize sets",
            .return_code = CS_UNKNOWN
        };
    }

    // Insert to fs1 only
    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs1, &i);
    }

    flat_set_swap(&fs1, &fs2);

    if (!flat_set_empty(fs1)) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs1 should be empty after swap with empty set",
            .return_code = CS_UNKNOWN
        };
    }

    if (flat_set_size(fs2) != 10) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs1);
    flat_set_free(&fs2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              FREE TESTS                                    */
/******************************************************************************/

test_res test_flat_set_free_null() {
    // Free on NULL should not crash
    flat_set_free(NULL);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_free_empty() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_free_with_elements() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        flat_set_insert(&fs, &i);
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          CONTAINER TYPE TESTS                              */
/******************************************************************************/

test_res test_flat_set_deque_operations() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque flat_set",
            .return_code = rc
        };
    }

    // Insert in random order
    int vals[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        flat_set_insert(&fs, &vals[i]);
    }

    // Verify sorted order
    for (int i = 0; i < 10; i++) {
        int *val = (int *)flat_set_at(fs, i);
        if (val == NULL || *val != i) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Deque container: elements not sorted",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          STRESS/EDGE TESTS                                 */
/******************************************************************************/

test_res test_flat_set_large_capacity() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10000; i++) {
        rc = flat_set_insert(&fs, &i);
        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to insert during large capacity test",
                .return_code = rc
            };
        }
    }

    if (flat_set_size(fs) != 10000) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 10000",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_insert_erase_cycle() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Insert 100, erase 50, insert 100
    for (int i = 0; i < 100; i++) {
        flat_set_insert(&fs, &i);
    }

    for (int i = 0; i < 50; i++) {
        flat_set_erase(&fs, &i);
    }

    for (int i = 100; i < 200; i++) {
        flat_set_insert(&fs, &i);
    }

    if (flat_set_size(fs) != 150) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 150 after cycle",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_negative_values() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Insert negative values
    for (int i = -50; i <= 50; i++) {
        flat_set_insert(&fs, &i);
    }

    // First should be -50
    int *first = (int *)flat_set_at(fs, 0);
    if (first == NULL || *first != -50) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "First element should be -50",
            .return_code = CS_UNKNOWN
        };
    }

    // Last should be 50
    int *last = (int *)flat_set_at(fs, flat_set_size(fs) - 1);
    if (last == NULL || *last != 50) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Last element should be 50",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_reinsert_after_erase() {
    elem_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    int val = 42;
    flat_set_insert(&fs, &val);
    flat_set_erase(&fs, &val);

    rc = flat_set_insert(&fs, &val);
    if (rc != CS_SUCCESS) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should be able to reinsert after erase",
            .return_code = rc
        };
    }

    if (flat_set_size(fs) != 1) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 1 after reinsert",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                          TEST STRUCT TESTS                                 */
/******************************************************************************/

test_res test_flat_set_struct_basic() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    test_struct ts = create_test_struct(1, "TestFlatSet", 10.5);
    rc = flat_set_insert(&fs, &ts);
    free_test_struct(&ts);

    if (rc != CS_SUCCESS) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to insert struct",
            .return_code = rc
        };
    }

    if (flat_set_size(fs) != 1) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 1",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_multiple() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "MultiTest", (double)i * 1.5);
        rc = flat_set_insert(&fs, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to insert struct",
                .return_code = rc
            };
        }
    }

    if (flat_set_size(fs) != 100) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_sorted_order() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Insert in random order
    int ids[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(ids[i], "SortTest", (double)ids[i]);
        flat_set_insert(&fs, &ts);
        free_test_struct(&ts);
    }

    // Verify sorted order by ID (ascending based on comp_test_struct_max)
    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct *)flat_set_at(fs, i);
        if (val == NULL || val->id != i) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Structs not in sorted order",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_duplicate() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    test_struct ts1 = create_test_struct(42, "Duplicate", 10.0);
    test_struct ts2 = create_test_struct(42, "DifferentName", 20.0); // Same ID

    flat_set_insert(&fs, &ts1);
    flat_set_insert(&fs, &ts2);

    free_test_struct(&ts1);
    free_test_struct(&ts2);

    // Set should reject duplicates based on comparison (ID)
    if (flat_set_size(fs) != 1) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Set should not allow duplicate structs",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_find() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "FindTest", (double)i);
        flat_set_insert(&fs, &ts);
        free_test_struct(&ts);
    }

    // Find existing
    test_struct search = create_test_struct(5, "SearchKey", 0.0);
    int idx = flat_set_find(fs, &search);
    free_test_struct(&search);

    if (idx < 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should find existing struct",
            .return_code = CS_UNKNOWN
        };
    }

    // Find nonexistent
    test_struct not_found = create_test_struct(99, "NotFound", 0.0);
    idx = flat_set_find(fs, &not_found);
    free_test_struct(&not_found);

    if (idx >= 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should not find nonexistent struct",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_erase() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "EraseTest", (double)i);
        flat_set_insert(&fs, &ts);
        free_test_struct(&ts);
    }

    // Erase element with id=5
    test_struct to_erase = create_test_struct(5, "ToErase", 0.0);
    rc = flat_set_erase(&fs, &to_erase);
    free_test_struct(&to_erase);

    if (rc != CS_SUCCESS) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to erase struct",
            .return_code = rc
        };
    }

    if (flat_set_size(fs) != 9) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 9 after erase",
            .return_code = CS_UNKNOWN
        };
    }

    // Verify erased element is gone
    test_struct search = create_test_struct(5, "Search", 0.0);
    int idx = flat_set_find(fs, &search);
    free_test_struct(&search);

    if (idx >= 0) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Should not find erased struct",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_simple() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Test with simple structs (no nested allocations)
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct_simple(i);
        rc = flat_set_insert(&fs, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to insert simple struct",
                .return_code = rc
            };
        }
    }

    if (flat_set_size(fs) != 100) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 100",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_deque_container() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_DEQUE);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize deque flat_set",
            .return_code = rc
        };
    }

    // Insert in random order
    int ids[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 0};
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(ids[i], "DequeTest", (double)ids[i] * 2.5);
        rc = flat_set_insert(&fs, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to insert struct to deque flat_set",
                .return_code = rc
            };
        }
    }

    // Verify sorted order
    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct *)flat_set_at(fs, i);
        if (val == NULL || val->id != i) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Deque container: structs not sorted",
                .return_code = CS_UNKNOWN
            };
        }
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_clear() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Insert complex structs
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        flat_set_insert(&fs, &ts);
        free_test_struct(&ts);
    }

    // Clear should free all struct memory
    flat_set_clear(&fs);

    if (!flat_set_empty(fs)) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Flat set should be empty after clear",
            .return_code = CS_UNKNOWN
        };
    }

    // Insert again to verify clear worked
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        rc = flat_set_insert(&fs, &ts);
        free_test_struct(&ts);

        if (rc != CS_SUCCESS) {
            flat_set_free(&fs);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "Failed to insert after clear",
                .return_code = rc
            };
        }
    }

    if (flat_set_size(fs) != 50) {
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Size should be 50 after clear and reuse",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_swap() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs1, fs2;
    cs_codes rc1 = flat_set_init(&fs1, attr, CS_FLAT_SET_VECTOR);
    cs_codes rc2 = flat_set_init(&fs2, attr, CS_FLAT_SET_VECTOR);
    if (rc1 != CS_SUCCESS || rc2 != CS_SUCCESS) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize flat sets",
            .return_code = CS_UNKNOWN
        };
    }

    // Insert to fs1
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "Set1", (double)i);
        flat_set_insert(&fs1, &ts);
        free_test_struct(&ts);
    }

    // Insert to fs2
    for (int i = 100; i < 105; i++) {
        test_struct ts = create_test_struct(i, "Set2", (double)i);
        flat_set_insert(&fs2, &ts);
        free_test_struct(&ts);
    }

    flat_set_swap(&fs1, &fs2);

    // fs1 should now have 5 elements with id starting at 100
    if (flat_set_size(fs1) != 5) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs1 should have 5 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    test_struct *first1 = (test_struct *)flat_set_at(fs1, 0);
    if (first1 == NULL || first1->id != 100) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs1 first element should have id=100 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    // fs2 should now have 10 elements with id starting at 0
    if (flat_set_size(fs2) != 10) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs2 should have 10 elements after swap",
            .return_code = CS_UNKNOWN
        };
    }

    test_struct *first2 = (test_struct *)flat_set_at(fs2, 0);
    if (first2 == NULL || first2->id != 0) {
        flat_set_free(&fs1);
        flat_set_free(&fs2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "fs2 first element should have id=0 after swap",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(&fs1);
    flat_set_free(&fs2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_flat_set_struct_deep_copy_verify() {
    elem_attr_t attr = {
        .size = sizeof(test_struct),
        .comp = comp_test_struct_max,
        .copy = copy_test_struct,
        .fr = free_test_struct,
        .print = print_test_struct_compact
    };

    flat_set fs;
    cs_codes rc = flat_set_init(&fs, attr, CS_FLAT_SET_VECTOR);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize",
            .return_code = rc
        };
    }

    // Create a complex struct
    test_struct ts = create_test_struct(42, "DeepCopyTest", 99.99);
    char *original_name = ts.name;

    flat_set_insert(&fs, &ts);

    // The flat_set should have deep copied the struct
    test_struct *stored = (test_struct *)flat_set_at(fs, 0);
    if (stored == NULL) {
        free_test_struct(&ts);
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "at() returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    // The name pointers should be different (deep copy)
    if (stored->name == original_name) {
        free_test_struct(&ts);
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Name was not deep copied",
            .return_code = CS_UNKNOWN
        };
    }

    // But the values should be equal
    if (stored->id != 42 || strcmp(stored->name, "DeepCopyTest") != 0) {
        free_test_struct(&ts);
        flat_set_free(&fs);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Deep copied values don't match",
            .return_code = CS_UNKNOWN
        };
    }

    free_test_struct(&ts);
    flat_set_free(&fs);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

/******************************************************************************/
/*                              TEST REGISTRY                                 */
/******************************************************************************/

test flat_set_tests[] = {
    // Init tests
    test_flat_set_init_vector,
    test_flat_set_init_deque,
    test_flat_set_init_default,
    test_flat_set_init_null_pointer,
    test_flat_set_init_invalid_size,

    // Insert tests
    test_flat_set_insert_basic,
    test_flat_set_insert_multiple,
    test_flat_set_insert_duplicate,
    test_flat_set_insert_sorted_order,
    test_flat_set_insert_null_set,
    test_flat_set_insert_null_data,

    // Erase tests
    test_flat_set_erase_basic,
    test_flat_set_erase_multiple,
    test_flat_set_erase_nonexistent,
    test_flat_set_erase_null_set,
    test_flat_set_erase_maintains_order,

    // Find tests
    test_flat_set_find_existing,
    test_flat_set_find_nonexistent,
    test_flat_set_find_all,
    test_flat_set_find_after_erase,

    // At tests
    test_flat_set_at_valid,
    test_flat_set_at_out_of_bounds,

    // Size/Empty tests
    test_flat_set_size_empty,
    test_flat_set_size_after_insert,
    test_flat_set_size_after_erase,
    test_flat_set_empty_true,
    test_flat_set_empty_false,
    test_flat_set_empty_after_erase_all,

    // Clear tests
    test_flat_set_clear_basic,
    test_flat_set_clear_empty,
    test_flat_set_clear_null,
    test_flat_set_clear_and_reuse,

    // Swap tests
    test_flat_set_swap_basic,
    test_flat_set_swap_with_empty,

    // Free tests
    test_flat_set_free_null,
    test_flat_set_free_empty,
    test_flat_set_free_with_elements,

    // Container type tests
    test_flat_set_deque_operations,

    // Stress/Edge tests
    test_flat_set_large_capacity,
    test_flat_set_insert_erase_cycle,
    test_flat_set_negative_values,
    test_flat_set_reinsert_after_erase,

    // Struct tests
    test_flat_set_struct_basic,
    test_flat_set_struct_multiple,
    test_flat_set_struct_sorted_order,
    test_flat_set_struct_duplicate,
    test_flat_set_struct_find,
    test_flat_set_struct_erase,
    test_flat_set_struct_simple,
    test_flat_set_struct_deque_container,
    test_flat_set_struct_clear,
    test_flat_set_struct_swap,
    test_flat_set_struct_deep_copy_verify,
}; 