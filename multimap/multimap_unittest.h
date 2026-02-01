#include <cs/multimap.h>
#include <cs/rbt.h>
#include <cs/pair.h>
#include <cs/vector.h>
#include <unittest.h>

// ============================================================================
// multimap_init
// ============================================================================

test_res test_multimap_init() {
    multimap_attr_t key_attr = get_test_struct_attr();
    multimap_attr_t value_attr = get_test_struct_attr();
    multimap mm;
    cs_codes init_result = multimap_init(&mm, key_attr, value_attr);

    if (init_result != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", init_result};
    if (mm.t == NULL) return (test_res){(char*)__func__, "RBT is NULL", CS_MEM};
    if (rbt_size(*(mm.t)) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated after init", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_init_invalid_key_size() {
    multimap_attr_t key_attr = get_test_struct_attr();
    key_attr.size = 0;
    multimap_attr_t value_attr = get_test_struct_attr();
    multimap mm;
    cs_codes init_result = multimap_init(&mm, key_attr, value_attr);

    if (init_result != CS_SIZE) {
        return (test_res){(char*)__func__, "Init should return CS_SIZE for invalid key size", CS_UNKNOWN};
    }

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_init_invalid_value_size() {
    multimap_attr_t key_attr = get_test_struct_attr();
    multimap_attr_t value_attr = get_test_struct_attr();
    value_attr.size = 0;
    multimap mm;
    cs_codes init_result = multimap_init(&mm, key_attr, value_attr);

    if (init_result != CS_SIZE) {
        return (test_res){(char*)__func__, "Init should return CS_SIZE for invalid value size", CS_UNKNOWN};
    }

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// multimap_insert
// ============================================================================

test_res test_multimap_insert_single() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(1, "Key1", 1.0);
    test_struct value = create_test_struct(100, "Value100", 100.0);

    cs_codes result = multimap_insert(&mm, &key, &value);
    if (result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Insert returned error", result};
    }

    if (rbt_size(*(mm.t)) != 1) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT size should be 1", CS_UNKNOWN};
    }

    vector *values = multimap_get(&mm, &key);
    if (NULL == values || vector_size(*values) != 1) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Value not found or wrong count", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&value);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_insert_multiple_keys() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "Key", (double)i);
        test_struct value = create_test_struct(i * 10, "Value", (double)(i * 10));
        cs_codes result = multimap_insert(&mm, &key, &value);
        if (result != CS_SUCCESS) {
            free_test_struct(&key);
            free_test_struct(&value);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Insert returned error", result};
        }
        free_test_struct(&key);
        free_test_struct(&value);
    }

    if (rbt_size(*(mm.t)) != total) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT size mismatch", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_insert_duplicate_keys() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "SameKey", 42.0);

    // Insert 5 values for the same key
    for (int i = 0; i < 5; i++) {
        test_struct value = create_test_struct(i, "Value", (double)i);
        cs_codes result = multimap_insert(&mm, &key, &value);
        if (result != CS_SUCCESS) {
            free_test_struct(&key);
            free_test_struct(&value);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Insert returned error", result};
        }
        free_test_struct(&value);
    }

    // RBT should have only 1 node (same key)
    if (rbt_size(*(mm.t)) != 1) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should have 1 node for duplicate keys", CS_UNKNOWN};
    }

    // Value vector should have 5 elements
    vector *values = multimap_get(&mm, &key);
    if (!values || vector_size(*values) != 5) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Value vector should have 5 elements", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    free_test_struct(&key);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_insert_many_duplicates() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    int num_keys = 10;
    int values_per_key = 100;

    for (int i = 0; i < num_keys; i++) {
        test_struct key = create_test_struct(i, "Key", (double)i);
        for (int j = 0; j < values_per_key; j++) {
            test_struct value = create_test_struct(j, "Value", (double)j);
            cs_codes result = multimap_insert(&mm, &key, &value);
            if (result != CS_SUCCESS) {
                free_test_struct(&key);
                free_test_struct(&value);
                multimap_free(&mm);
                return (test_res){(char*)__func__, "Insert returned error", result};
            }
            free_test_struct(&value);
        }
        free_test_struct(&key);
    }

    // Verify each key has correct number of values
    for (int i = 0; i < num_keys; i++) {
        test_struct key = create_test_struct(i, "Key", (double)i);
        vector *values = multimap_get(&mm, &key);
        if (!values || vector_size(*values) != values_per_key) {
            free_test_struct(&key);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Value count mismatch", CS_UNKNOWN};
        }
        free_test_struct(&key);
    }

    if (rbt_size(*(mm.t)) != num_keys) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT size should equal num_keys", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_insert_null() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(1, "Key", 1.0);
    test_struct value = create_test_struct(1, "Value", 1.0);

    cs_codes result = multimap_insert(&mm, NULL, &value);
    if (result != CS_NULL) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Insert NULL key should return CS_NULL", CS_UNKNOWN};
    }

    result = multimap_insert(&mm, &key, NULL);
    if (result != CS_NULL) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Insert NULL value should return CS_NULL", CS_UNKNOWN};
    }

    result = multimap_insert(NULL, &key, &value);
    if (result != CS_NULL) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Insert with NULL multimap should return CS_NULL", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&value);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// multimap_delete
// ============================================================================

test_res test_multimap_delete_single_value() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "Key", 42.0);
    test_struct value = create_test_struct(100, "Value", 100.0);

    multimap_insert(&mm, &key, &value);
    cs_codes result = multimap_delete(&mm, &key);

    if (result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Delete returned error", result};
    }

    if (rbt_size(*(mm.t)) != 0) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should be empty after deleting only value", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        free_test_struct(&key);
        free_test_struct(&value);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&value);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_delete_one_of_many() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "Key", 42.0);

    // Insert 5 values
    for (int i = 0; i < 5; i++) {
        test_struct value = create_test_struct(i, "Value", (double)i);
        multimap_insert(&mm, &key, &value);
        free_test_struct(&value);
    }

    // Delete once - should remove one value from vector
    cs_codes result = multimap_delete(&mm, &key);
    if (result != CS_SUCCESS) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Delete returned error", result};
    }

    // Should still have the key with 4 values
    vector *values = multimap_get(&mm, &key);
    if (!values || vector_size(*values) != 4) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Should have 4 values after one delete", CS_UNKNOWN};
    }

    if (rbt_size(*(mm.t)) != 1) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should still have 1 node", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    free_test_struct(&key);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_delete_all_values() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "Key", 42.0);

    // Insert 5 values
    for (int i = 0; i < 5; i++) {
        test_struct value = create_test_struct(i, "Value", (double)i);
        multimap_insert(&mm, &key, &value);
        free_test_struct(&value);
    }

    // Delete 5 times
    for (int i = 0; i < 5; i++) {
        cs_codes result = multimap_delete(&mm, &key);
        if (result != CS_SUCCESS) {
            free_test_struct(&key);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Delete returned error", result};
        }

        if (!rbt_is_valid(mm.t)) {
            free_test_struct(&key);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "RBT integrity violated during deletes", CS_UNKNOWN};
        }
    }

    if (rbt_size(*(mm.t)) != 0) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should be empty", CS_UNKNOWN};
    }

    free_test_struct(&key);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_delete_nonexistent() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "NonExistent", 42.0);

    cs_codes result = multimap_delete(&mm, &key);
    if (result != CS_ELEM) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Delete nonexistent should return CS_ELEM", CS_UNKNOWN};
    }

    free_test_struct(&key);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_delete_multiple_keys() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert all
    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "Key", (double)i);
        test_struct value = create_test_struct(i * 10, "Value", (double)(i * 10));
        multimap_insert(&mm, &key, &value);
        free_test_struct(&key);
        free_test_struct(&value);
    }

    // Delete all in reverse order
    for (int i = total - 1; i >= 0; i--) {
        test_struct key = create_test_struct(i, "Key", (double)i);
        cs_codes result = multimap_delete(&mm, &key);
        if (result != CS_SUCCESS) {
            free_test_struct(&key);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Delete returned error", result};
        }
        free_test_struct(&key);

        if (!rbt_is_valid(mm.t)) {
            multimap_free(&mm);
            return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
        }
    }

    if (rbt_size(*(mm.t)) != 0) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should be empty", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_delete_random_order() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert all
    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "Key", (double)i);
        test_struct value = create_test_struct(i * 10, "Value", (double)(i * 10));
        multimap_insert(&mm, &key, &value);
        free_test_struct(&key);
        free_test_struct(&value);
    }

    // Create random order
    int *order = malloc(total * sizeof(int));
    for (int i = 0; i < total; i++) order[i] = i;
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    // Delete in random order
    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(order[i], "Key", (double)order[i]);
        cs_codes result = multimap_delete(&mm, &key);
        if (result != CS_SUCCESS) {
            free_test_struct(&key);
            free(order);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Delete returned error", result};
        }
        free_test_struct(&key);

        if (!rbt_is_valid(mm.t)) {
            free(order);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
        }
    }

    free(order);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// multimap_clear
// ============================================================================

test_res test_multimap_clear() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());

    // Insert elements with multiple values
    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "Key", (double)i);
        for (int j = 0; j < 3; j++) {
            test_struct value = create_test_struct(j, "Value", (double)j);
            multimap_insert(&mm, &key, &value);
            free_test_struct(&value);
        }
        free_test_struct(&key);
    }

    multimap_clear(&mm);

    if (rbt_size(*(mm.t)) != 0) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Size should be 0 after clear", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated after clear", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_clear_empty() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());

    multimap_clear(&mm);

    if (rbt_size(*(mm.t)) != 0) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Size should still be 0", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_clear_reuse() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());

    // Insert, clear, insert again
    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "Key1", (double)i);
        test_struct value = create_test_struct(i, "Value1", (double)i);
        multimap_insert(&mm, &key, &value);
        free_test_struct(&key);
        free_test_struct(&value);
    }

    multimap_clear(&mm);

    for (int i = 0; i < 30; i++) {
        test_struct key = create_test_struct(i + 100, "Key2", (double)(i + 100));
        test_struct value = create_test_struct(i + 100, "Value2", (double)(i + 100));
        multimap_insert(&mm, &key, &value);
        free_test_struct(&key);
        free_test_struct(&value);
    }

    if (rbt_size(*(mm.t)) != 30) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Size should be 30 after reuse", CS_UNKNOWN};
    }

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated after reuse", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_multimap_nested_data_integrity() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());

    // Insert complex structs
    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "NestedKey", (double)i * 1.5);
        for (int j = 0; j < 3; j++) {
            test_struct value = create_test_struct(i * 100 + j, "NestedValue", (double)(i * 100 + j));
            multimap_insert(&mm, &key, &value);
            free_test_struct(&value);
        }
        free_test_struct(&key);
    }

    // Verify all keys can be found with correct value counts
    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "NestedKey", (double)i * 1.5);
        vector *values = multimap_get(&mm, &key);
        if (!values || vector_size(*values) != 3) {
            free_test_struct(&key);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Value count mismatch", CS_UNKNOWN};
        }
        free_test_struct(&key);
    }

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_deep_copy_verification() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "DeepCopyKey", 42.0);
    test_struct value = create_test_struct(100, "DeepCopyValue", 100.0);

    multimap_insert(&mm, &key, &value);

    // Modify original key
    key.id = 999;
    free(key.name);
    key.name = strdup("ModifiedKey");

    // Original key should still find the value
    test_struct original_key = create_test_struct(42, "DeepCopyKey", 42.0);
    vector *values = multimap_get(&mm, &original_key);
    if (!values || vector_size(*values) != 1) {
        free_test_struct(&key);
        free_test_struct(&value);
        free_test_struct(&original_key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Deep copy failed - original key not found", CS_UNKNOWN};
    }

    // Modified key should not find anything
    values = multimap_get(&mm, &key);
    if (values != NULL) {
        free_test_struct(&key);
        free_test_struct(&value);
        free_test_struct(&original_key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Modified key should not be in multimap", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&value);
    free_test_struct(&original_key);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests
// ============================================================================

test_res test_multimap_stress_insert_delete() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert with some keys having multiple values
    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i % 100, "StressKey", (double)(i % 100));
        test_struct value = create_test_struct(i, "StressValue", (double)i);
        multimap_insert(&mm, &key, &value);
        free_test_struct(&key);
        free_test_struct(&value);
    }

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated after stress inserts", CS_UNKNOWN};
    }

    // Each of 100 keys should have total/100 values
    int expected_count = total / 100;
    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "StressKey", (double)i);
        vector *values = multimap_get(&mm, &key);
        if (!values || vector_size(*values) != expected_count) {
            free_test_struct(&key);
            multimap_free(&mm);
            return (test_res){(char*)__func__, "Value count mismatch", CS_UNKNOWN};
        }
        free_test_struct(&key);
    }

    // Delete half of each key's values
    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "StressKey", (double)i);
        for (int j = 0; j < expected_count / 2; j++) {
            multimap_delete(&mm, &key);
        }
        free_test_struct(&key);
    }

    if (!rbt_is_valid(mm.t)) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT integrity violated after stress deletes", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_interleaved_insert_delete() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());

    for (int round = 0; round < 10; round++) {
        // Insert 100 key-value pairs
        for (int i = 0; i < 100; i++) {
            test_struct key = create_test_struct(i, "InterleavedKey", (double)i);
            test_struct value = create_test_struct(round * 100 + i, "InterleavedValue", (double)(round * 100 + i));
            multimap_insert(&mm, &key, &value);
            free_test_struct(&key);
            free_test_struct(&value);
        }

        // Delete 50 of them
        for (int i = 0; i < 50; i++) {
            test_struct key = create_test_struct(i * 2, "InterleavedKey", (double)(i * 2));
            multimap_delete(&mm, &key);
            free_test_struct(&key);
        }

        if (!rbt_is_valid(mm.t)) {
            multimap_free(&mm);
            return (test_res){(char*)__func__, "RBT integrity violated during interleaved ops", CS_UNKNOWN};
        }
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_delete_all_verify_rbt() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert with multiple values per key
    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "DeleteAllKey", (double)i);
        int num_values = (i % 5) + 1; // 1 to 5 values per key
        for (int j = 0; j < num_values; j++) {
            test_struct value = create_test_struct(j, "DeleteAllValue", (double)j);
            multimap_insert(&mm, &key, &value);
            free_test_struct(&value);
        }
        free_test_struct(&key);
    }

    // Delete all values for all keys
    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "DeleteAllKey", (double)i);
        vector *values;
        while ((values = multimap_get(&mm, &key)) != NULL && vector_size(*values) > 0) {
            cs_codes result = multimap_delete(&mm, &key);
            if (result != CS_SUCCESS) {
                free_test_struct(&key);
                multimap_free(&mm);
                return (test_res){(char*)__func__, "Delete failed", result};
            }
        }
        free_test_struct(&key);

        if (!rbt_is_valid(mm.t)) {
            multimap_free(&mm);
            return (test_res){(char*)__func__, "RBT integrity violated during delete all", CS_UNKNOWN};
        }
    }

    if (rbt_size(*(mm.t)) != 0) {
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should be empty", CS_UNKNOWN};
    }

    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multimap_large_value_counts() {
    multimap mm;
    multimap_init(&mm, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "LargeCountKey", 42.0);
    int large_count = 10000;

    // Insert many values for one key
    for (int i = 0; i < large_count; i++) {
        test_struct value = create_test_struct(i, "LargeCountValue", (double)i);
        multimap_insert(&mm, &key, &value);
        free_test_struct(&value);
    }

    vector *values = multimap_get(&mm, &key);
    if (!values || vector_size(*values) != large_count) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "Value count mismatch", CS_UNKNOWN};
    }

    // RBT should still have only 1 node
    if (rbt_size(*(mm.t)) != 1) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should have 1 node", CS_UNKNOWN};
    }

    // Delete all
    for (int i = 0; i < large_count; i++) {
        multimap_delete(&mm, &key);
    }

    if (rbt_size(*(mm.t)) != 0) {
        free_test_struct(&key);
        multimap_free(&mm);
        return (test_res){(char*)__func__, "RBT should be empty", CS_UNKNOWN};
    }

    free_test_struct(&key);
    multimap_free(&mm);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Test array
// ============================================================================

test multimap_tests[] = {
    // multimap_init
    test_multimap_init,
    test_multimap_init_invalid_key_size,
    test_multimap_init_invalid_value_size,

    // multimap_insert
    test_multimap_insert_single,
    test_multimap_insert_multiple_keys,
    test_multimap_insert_duplicate_keys,
    test_multimap_insert_many_duplicates,
    test_multimap_insert_null,

    // multimap_delete
    test_multimap_delete_single_value,
    test_multimap_delete_one_of_many,
    test_multimap_delete_all_values,
    test_multimap_delete_nonexistent,
    test_multimap_delete_multiple_keys,
    test_multimap_delete_random_order,

    // multimap_clear
    test_multimap_clear,
    test_multimap_clear_empty,
    test_multimap_clear_reuse,

    // Complex struct integrity
    test_multimap_nested_data_integrity,
    test_multimap_deep_copy_verification,

    // Stress tests
    test_multimap_stress_insert_delete,
    test_multimap_interleaved_insert_delete,
    test_multimap_delete_all_verify_rbt,
    test_multimap_large_value_counts,
};