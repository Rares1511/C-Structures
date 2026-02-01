#include <cs/multiset.h>
#include <cs/rbt.h>
#include <cs/pair.h>
#include <unittest.h>

// ============================================================================
// multiset_init
// ============================================================================

test_res test_multiset_init() {
    multiset_attr_t attr = get_test_struct_attr();
    multiset ms;
    cs_codes init_result = multiset_init(&ms, attr);

    if (init_result != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", init_result};
    if (ms.t == NULL) return (test_res){(char*)__func__, "RBT is NULL", CS_MEM};
    if (rbt_size(*(ms.t)) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};

    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated after init", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_init_invalid_size() {
    multiset_attr_t attr = get_test_struct_attr();
    attr.size = 0;
    multiset ms;
    cs_codes init_result = multiset_init(&ms, attr);

    if (init_result != CS_SIZE) {
        return (test_res){(char*)__func__, "Init should return CS_SIZE for invalid size", CS_UNKNOWN};
    }

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// multiset_insert
// ============================================================================

test_res test_multiset_insert_single() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "InsertSingle", 42.0);

    cs_codes result = multiset_insert(&ms, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Insert returned error", result};
    }

    if (multiset_count(&ms, &ts) != 1) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count should be 1", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated after insert", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_insert_multiple_unique() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "InsertMultiple", (double)i);
        cs_codes result = multiset_insert(&ms, &ts);
        if (result != CS_SUCCESS) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Insert returned error", result};
        }
        free_test_struct(&ts);
    }

    // Verify RBT has total unique elements
    if (rbt_size(*(ms.t)) != total) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT size mismatch", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_insert_duplicates() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "Duplicate", 42.0);

    // Insert same element 5 times
    for (int i = 0; i < 5; i++) {
        cs_codes result = multiset_insert(&ms, &ts);
        if (result != CS_SUCCESS) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Insert returned error", result};
        }
    }

    // Count should be 5
    if (multiset_count(&ms, &ts) != 5) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count should be 5 after 5 inserts", CS_UNKNOWN};
    }

    // RBT should have only 1 node (element stored with count)
    if (rbt_size(*(ms.t)) != 1) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT should have 1 node for duplicates", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_insert_many_duplicates() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    int num_unique = 10;
    int duplicates_each = 100;

    for (int i = 0; i < num_unique; i++) {
        test_struct ts = create_test_struct(i, "ManyDuplicates", (double)i);
        for (int j = 0; j < duplicates_each; j++) {
            cs_codes result = multiset_insert(&ms, &ts);
            if (result != CS_SUCCESS) {
                free_test_struct(&ts);
                multiset_free(&ms);
                return (test_res){(char*)__func__, "Insert returned error", result};
            }
        }
        free_test_struct(&ts);
    }

    // Verify each element has correct count
    for (int i = 0; i < num_unique; i++) {
        test_struct ts = create_test_struct(i, "ManyDuplicates", (double)i);
        if (multiset_count(&ms, &ts) != duplicates_each) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Count mismatch for element", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    // RBT should have num_unique nodes
    if (rbt_size(*(ms.t)) != num_unique) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT size should equal num_unique", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_insert_null() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());

    cs_codes result = multiset_insert(&ms, NULL);
    if (result != CS_NULL) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Insert NULL should return CS_NULL", CS_UNKNOWN};
    }

    result = multiset_insert(NULL, NULL);
    if (result != CS_NULL) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Insert with NULL multiset should return CS_NULL", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// multiset_delete
// ============================================================================

test_res test_multiset_delete_single() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "DeleteSingle", 42.0);

    multiset_insert(&ms, &ts);
    cs_codes result = multiset_delete(&ms, &ts);

    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Delete returned error", result};
    }

    if (multiset_count(&ms, &ts) != 0) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count should be 0 after delete", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated after delete", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_delete_one_of_many() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "DeleteOneOfMany", 42.0);

    // Insert 5 times
    for (int i = 0; i < 5; i++) {
        multiset_insert(&ms, &ts);
    }

    // Delete once - should decrement count, not remove node
    cs_codes result = multiset_delete(&ms, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Delete returned error", result};
    }

    if (multiset_count(&ms, &ts) != 4) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count should be 4 after one delete", CS_UNKNOWN};
    }

    // RBT should still have 1 node
    if (rbt_size(*(ms.t)) != 1) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT should still have 1 node", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_delete_all_duplicates() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "DeleteAllDuplicates", 42.0);

    // Insert 5 times
    for (int i = 0; i < 5; i++) {
        multiset_insert(&ms, &ts);
    }

    // Delete 5 times
    for (int i = 0; i < 5; i++) {
        cs_codes result = multiset_delete(&ms, &ts);
        if (result != CS_SUCCESS) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Delete returned error", result};
        }

        if (!rbt_is_valid(ms.t)) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "RBT integrity violated during deletes", CS_UNKNOWN};
        }
    }

    if (multiset_count(&ms, &ts) != 0) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count should be 0 after all deletes", CS_UNKNOWN};
    }

    if (rbt_size(*(ms.t)) != 0) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT should be empty", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_delete_nonexistent() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "Nonexistent", 42.0);

    cs_codes result = multiset_delete(&ms, &ts);
    if (result != CS_ELEM) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Delete nonexistent should return CS_ELEM", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_delete_multiple() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert all
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteMultiple", (double)i);
        multiset_insert(&ms, &ts);
        free_test_struct(&ts);
    }

    // Delete all in reverse order
    for (int i = total - 1; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "DeleteMultiple", (double)i);
        cs_codes result = multiset_delete(&ms, &ts);
        if (result != CS_SUCCESS) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Delete returned error", result};
        }
        free_test_struct(&ts);

        if (!rbt_is_valid(ms.t)) {
            multiset_free(&ms);
            return (test_res){(char*)__func__, "RBT integrity violated during deletes", CS_UNKNOWN};
        }
    }

    if (rbt_size(*(ms.t)) != 0) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT should be empty after all deletes", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_delete_random_order() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert all
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteRandom", (double)i);
        multiset_insert(&ms, &ts);
        free_test_struct(&ts);
    }

    // Create random order array
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
        test_struct ts = create_test_struct(order[i], "DeleteRandom", (double)order[i]);
        cs_codes result = multiset_delete(&ms, &ts);
        if (result != CS_SUCCESS) {
            free_test_struct(&ts);
            free(order);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Delete returned error", result};
        }
        free_test_struct(&ts);

        if (!rbt_is_valid(ms.t)) {
            free(order);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "RBT integrity violated during random deletes", CS_UNKNOWN};
        }
    }

    free(order);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// multiset_count
// ============================================================================

test_res test_multiset_count_zero() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "CountZero", 42.0);

    if (multiset_count(&ms, &ts) != 0) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count should be 0 for non-existent", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_count_after_inserts() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "CountAfterInserts", 42.0);

    for (int i = 1; i <= 10; i++) {
        multiset_insert(&ms, &ts);
        if (multiset_count(&ms, &ts) != i) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Count mismatch after insert", CS_UNKNOWN};
        }
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_count_after_deletes() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "CountAfterDeletes", 42.0);

    // Insert 10 times
    for (int i = 0; i < 10; i++) {
        multiset_insert(&ms, &ts);
    }

    // Delete one by one and verify count
    for (int i = 9; i >= 0; i--) {
        multiset_delete(&ms, &ts);
        if (multiset_count(&ms, &ts) != i) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Count mismatch after delete", CS_UNKNOWN};
        }
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_count_multiple_elements() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());

    // Insert different elements with different counts
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "CountMultiple", (double)i);
        for (int j = 0; j <= i; j++) {
            multiset_insert(&ms, &ts);
        }
        free_test_struct(&ts);
    }

    // Verify counts
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "CountMultiple", (double)i);
        if (multiset_count(&ms, &ts) != i + 1) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Count mismatch for element", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// multiset_clear
// ============================================================================

test_res test_multiset_clear() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());

    // Insert elements
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Clear", (double)i);
        multiset_insert(&ms, &ts);
        multiset_insert(&ms, &ts); // Insert twice
        free_test_struct(&ts);
    }

    multiset_clear(&ms);

    if (rbt_size(*(ms.t)) != 0) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Size should be 0 after clear", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated after clear", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_clear_empty() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());

    multiset_clear(&ms);

    if (rbt_size(*(ms.t)) != 0) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Size should still be 0", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_clear_reuse() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());

    // Insert, clear, insert again
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearReuse1", (double)i);
        multiset_insert(&ms, &ts);
        free_test_struct(&ts);
    }

    multiset_clear(&ms);

    for (int i = 0; i < 30; i++) {
        test_struct ts = create_test_struct(i + 100, "ClearReuse2", (double)(i + 100));
        multiset_insert(&ms, &ts);
        free_test_struct(&ts);
    }

    if (rbt_size(*(ms.t)) != 30) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Size should be 30 after reuse", CS_UNKNOWN};
    }

    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated after reuse", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_multiset_nested_data_integrity() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());

    // Insert complex structs with duplicates
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 1.5);
        multiset_insert(&ms, &ts);
        multiset_insert(&ms, &ts); // Insert duplicate
        free_test_struct(&ts);
    }

    // Verify all elements can be found with correct count
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 1.5);
        if (multiset_count(&ms, &ts) != 2) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Count should be 2 for all elements", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_deep_copy_verification() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "DeepCopy", 42.0);

    multiset_insert(&ms, &ts);

    // Modify original struct
    ts.id = 999;
    free(ts.name);
    ts.name = strdup("ModifiedName");

    // The count for the original data should still work
    test_struct original = create_test_struct(42, "DeepCopy", 42.0);
    if (multiset_count(&ms, &original) != 1) {
        free_test_struct(&ts);
        free_test_struct(&original);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Deep copy failed - original data not found", CS_UNKNOWN};
    }

    // Modified data should not be found
    if (multiset_count(&ms, &ts) != 0) {
        free_test_struct(&ts);
        free_test_struct(&original);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Modified data should not be in multiset", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    free_test_struct(&original);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests
// ============================================================================

test_res test_multiset_stress_insert_delete() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert all with duplicates
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i % 100, "Stress", (double)(i % 100));
        multiset_insert(&ms, &ts);
        free_test_struct(&ts);
    }

    // Verify RBT integrity
    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated after stress inserts", CS_UNKNOWN};
    }

    // Each of 100 elements should have count of total/100
    int expected_count = total / 100;
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Stress", (double)i);
        if (multiset_count(&ms, &ts) != expected_count) {
            free_test_struct(&ts);
            multiset_free(&ms);
            return (test_res){(char*)__func__, "Count mismatch in stress test", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    // Delete half of each element's occurrences
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Stress", (double)i);
        for (int j = 0; j < expected_count / 2; j++) {
            multiset_delete(&ms, &ts);
        }
        free_test_struct(&ts);
    }

    if (!rbt_is_valid(ms.t)) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT integrity violated after stress deletes", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_interleaved_insert_delete() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());

    for (int round = 0; round < 10; round++) {
        // Insert 100 elements
        for (int i = 0; i < 100; i++) {
            test_struct ts = create_test_struct(i, "Interleaved", (double)i);
            multiset_insert(&ms, &ts);
            free_test_struct(&ts);
        }

        // Delete 50 elements
        for (int i = 0; i < 50; i++) {
            test_struct ts = create_test_struct(i * 2, "Interleaved", (double)(i * 2));
            multiset_delete(&ms, &ts);
            free_test_struct(&ts);
        }

        if (!rbt_is_valid(ms.t)) {
            multiset_free(&ms);
            return (test_res){(char*)__func__, "RBT integrity violated during interleaved ops", CS_UNKNOWN};
        }
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_delete_all_verify_rbt() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert with some duplicates
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteAllRBT", (double)i);
        multiset_insert(&ms, &ts);
        if (i % 3 == 0) {
            multiset_insert(&ms, &ts); // Add extra for every 3rd
        }
        free_test_struct(&ts);
    }

    // Delete all
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteAllRBT", (double)i);
        while (multiset_count(&ms, &ts) > 0) {
            cs_codes result = multiset_delete(&ms, &ts);
            if (result != CS_SUCCESS) {
                free_test_struct(&ts);
                multiset_free(&ms);
                return (test_res){(char*)__func__, "Delete failed", result};
            }
        }
        free_test_struct(&ts);

        if (!rbt_is_valid(ms.t)) {
            multiset_free(&ms);
            return (test_res){(char*)__func__, "RBT integrity violated during delete all", CS_UNKNOWN};
        }
    }

    if (rbt_size(*(ms.t)) != 0) {
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT should be empty", CS_UNKNOWN};
    }

    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_multiset_large_counts() {
    multiset ms;
    multiset_init(&ms, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "LargeCounts", 42.0);
    int large_count = 10000;

    // Insert many times
    for (int i = 0; i < large_count; i++) {
        multiset_insert(&ms, &ts);
    }

    if (multiset_count(&ms, &ts) != large_count) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count mismatch for large count", CS_UNKNOWN};
    }

    // RBT should still have only 1 node
    if (rbt_size(*(ms.t)) != 1) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT should have 1 node", CS_UNKNOWN};
    }

    // Delete all
    for (int i = 0; i < large_count; i++) {
        multiset_delete(&ms, &ts);
    }

    if (multiset_count(&ms, &ts) != 0) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "Count should be 0", CS_UNKNOWN};
    }

    if (rbt_size(*(ms.t)) != 0) {
        free_test_struct(&ts);
        multiset_free(&ms);
        return (test_res){(char*)__func__, "RBT should be empty", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    multiset_free(&ms);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Test array
// ============================================================================

test multiset_tests[] = {
    // multiset_init
    test_multiset_init,
    test_multiset_init_invalid_size,

    // multiset_insert
    test_multiset_insert_single,
    test_multiset_insert_multiple_unique,
    test_multiset_insert_duplicates,
    test_multiset_insert_many_duplicates,
    test_multiset_insert_null,

    // multiset_delete
    test_multiset_delete_single,
    test_multiset_delete_one_of_many,
    test_multiset_delete_all_duplicates,
    test_multiset_delete_nonexistent,
    test_multiset_delete_multiple,
    test_multiset_delete_random_order,

    // multiset_count
    test_multiset_count_zero,
    test_multiset_count_after_inserts,
    test_multiset_count_after_deletes,
    test_multiset_count_multiple_elements,

    // multiset_clear
    test_multiset_clear,
    test_multiset_clear_empty,
    test_multiset_clear_reuse,

    // Complex struct integrity
    test_multiset_nested_data_integrity,
    test_multiset_deep_copy_verification,

    // Stress tests
    test_multiset_stress_insert_delete,
    test_multiset_interleaved_insert_delete,
    test_multiset_delete_all_verify_rbt,
    test_multiset_large_counts,
};