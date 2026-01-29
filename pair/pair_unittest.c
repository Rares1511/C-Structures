#include <cs/pair.h>
#include "../include/unittest.h"

// Required by unittest.h
FILE *__DEBUG_OUT = NULL;

// ============================================================================
// pair_init
// ============================================================================

test_res test_pair_init() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    if (p.first != NULL || p.second != NULL) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Initial values should be NULL", CS_UNKNOWN};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_init_different_types() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = {
        .size = sizeof(int),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init with different types failed", rc};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// pair_set
// ============================================================================

test_res test_pair_set_single() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "FirstValue", 42.0);
    test_struct second = create_test_struct(100, "SecondValue", 100.0);

    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }

    free_test_struct(&first);
    free_test_struct(&second);
    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_set_overwrite() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    // First set
    test_struct first1 = create_test_struct(1, "First1", 1.0);
    test_struct second1 = create_test_struct(2, "Second1", 2.0);
    rc = pair_set(&p, &first1, &second1);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first1);
        free_test_struct(&second1);
        pair_free(&p);
        return (test_res){(char*)__func__, "First pair_set failed", rc};
    }
    free_test_struct(&first1);
    free_test_struct(&second1);

    // Overwrite with new values
    test_struct first2 = create_test_struct(10, "First2", 10.0);
    test_struct second2 = create_test_struct(20, "Second2", 20.0);
    rc = pair_set(&p, &first2, &second2);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first2);
        free_test_struct(&second2);
        pair_free(&p);
        return (test_res){(char*)__func__, "Overwrite pair_set failed", rc};
    }
    free_test_struct(&first2);
    free_test_struct(&second2);

    // Verify new values
    test_struct *stored_first = (test_struct*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    if (!stored_first || stored_first->id != 10) {
        pair_free(&p);
        return (test_res){(char*)__func__, "First value not overwritten correctly", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 20) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Second value not overwritten correctly", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_set_multiple_overwrites() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    // Overwrite many times to test for memory leaks
    for (int i = 0; i < 100; i++) {
        test_struct first = create_test_struct(i, "OverwriteFirst", (double)i);
        test_struct second = create_test_struct(i * 2, "OverwriteSecond", (double)(i * 2));
        rc = pair_set(&p, &first, &second);
        free_test_struct(&first);
        free_test_struct(&second);

        if (rc != CS_SUCCESS) {
            pair_free(&p);
            return (test_res){(char*)__func__, "pair_set failed during overwrites", rc};
        }
    }

    // Verify final values
    test_struct *stored_first = (test_struct*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    if (!stored_first || stored_first->id != 99) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Final first value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 198) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Final second value incorrect", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// pair_first / pair_second
// ============================================================================

test_res test_pair_first() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "GetFirst", 42.42);
    test_struct second = create_test_struct(100, "GetSecond", 100.0);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&first);
    free_test_struct(&second);

    test_struct *retrieved = (test_struct*)pair_first(p);
    if (!retrieved) {
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_first returned NULL", CS_ELEM};
    }

    if (retrieved->id != 42 || strcmp(retrieved->name, "GetFirst") != 0) {
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_first returned wrong value", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_second() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "GetFirst", 42.0);
    test_struct second = create_test_struct(100, "GetSecond", 100.5);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&first);
    free_test_struct(&second);

    test_struct *retrieved = (test_struct*)pair_second(p);
    if (!retrieved) {
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_second returned NULL", CS_ELEM};
    }

    if (retrieved->id != 100 || strcmp(retrieved->name, "GetSecond") != 0) {
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_second returned wrong value", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_first_empty() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    // pair_first on empty pair should return NULL
    void *retrieved = pair_first(p);
    if (retrieved != NULL) {
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_first should return NULL for empty pair", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_second_empty() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    // pair_second on empty pair should return NULL
    void *retrieved = pair_second(p);
    if (retrieved != NULL) {
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_second should return NULL for empty pair", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Deep copy verification
// ============================================================================

test_res test_pair_deep_copy_first() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "DeepCopyFirst", 42.0);
    test_struct second = create_test_struct(100, "DeepCopySecond", 100.0);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }

    // Modify original after set
    first.id = 999;
    free(first.name);
    first.name = strdup("Modified");
    first.address->zip_code = 99999;

    // Stored value should be unchanged
    test_struct *stored = (test_struct*)pair_first(p);
    if (!stored || stored->id != 42) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopyFirst") != 0) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10042) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    free_test_struct(&first);
    free_test_struct(&second);
    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_deep_copy_second() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "First", 42.0);
    test_struct second = create_test_struct(100, "DeepCopySecond", 100.0);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }

    // Modify second original after set
    second.id = 888;
    free(second.name);
    second.name = strdup("Modified");
    second.address->zip_code = 88888;

    // Stored second value should be unchanged
    test_struct *stored = (test_struct*)pair_second(p);
    if (!stored || stored->id != 100) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopySecond") != 0) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10100) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    free_test_struct(&first);
    free_test_struct(&second);
    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Nested data integrity
// ============================================================================

test_res test_pair_nested_data_integrity() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(50, "NestedFirst", 50.5);
    test_struct second = create_test_struct(75, "NestedSecond", 75.5);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&first);
    free_test_struct(&second);

    // Check first's nested structures
    test_struct *stored_first = (test_struct*)pair_first(p);
    if (!stored_first->address || stored_first->address->zip_code != 10050) {
        pair_free(&p);
        return (test_res){(char*)__func__, "First address corrupted", CS_ELEM};
    }

    int expected_first_contacts = 2 + (50 % 3);
    if (stored_first->contact_count != expected_first_contacts || !stored_first->contacts) {
        pair_free(&p);
        return (test_res){(char*)__func__, "First contacts corrupted", CS_ELEM};
    }

    int expected_first_tags = 3 + (50 % 3);
    if (stored_first->tag_count != expected_first_tags || !stored_first->tags) {
        pair_free(&p);
        return (test_res){(char*)__func__, "First tags corrupted", CS_ELEM};
    }

    // Check second's nested structures
    test_struct *stored_second = (test_struct*)pair_second(p);
    if (!stored_second->address || stored_second->address->zip_code != 10075) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Second address corrupted", CS_ELEM};
    }

    int expected_second_contacts = 2 + (75 % 3);
    if (stored_second->contact_count != expected_second_contacts || !stored_second->contacts) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Second contacts corrupted", CS_ELEM};
    }

    int expected_second_tags = 3 + (75 % 3);
    if (stored_second->tag_count != expected_second_tags || !stored_second->tags) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Second tags corrupted", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Different type combinations
// ============================================================================

test_res test_pair_int_and_struct() {
    pair p;
    pair_attr_t first_attr = {
        .size = sizeof(int),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    int first = 42;
    test_struct second = create_test_struct(100, "MixedSecond", 100.0);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&second);

    int *stored_first = (int*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    if (!stored_first || *stored_first != 42) {
        pair_free(&p);
        return (test_res){(char*)__func__, "First int value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 100) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Second struct value incorrect", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_struct_and_double() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = {
        .size = sizeof(double),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL
    };

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(50, "MixedFirst", 50.0);
    double second = 3.14159;
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&first);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&first);

    test_struct *stored_first = (test_struct*)pair_first(p);
    double *stored_second = (double*)pair_second(p);

    if (!stored_first || stored_first->id != 50) {
        pair_free(&p);
        return (test_res){(char*)__func__, "First struct value incorrect", CS_ELEM};
    }

    if (!stored_second || *stored_second != 3.14159) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Second double value incorrect", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_string_and_struct() {
    pair p;
    pair_attr_t first_attr = {
        .size = 64,  // capacity for the string buffer
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL
    };
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    char first[] = "Hello, Pair!";
    test_struct second = create_test_struct(77, "WithString", 77.7);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&second);

    char *stored_first = (char*)pair_first(p);
    printf("Stored first: %s\n", stored_first);
    test_struct *stored_second = (test_struct*)pair_second(p);

    if (!stored_first || strcmp(stored_first, "Hello, Pair!") != 0) {
        pair_free(&p);
        return (test_res){(char*)__func__, "First string value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 77) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Second struct value incorrect", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests
// ============================================================================

test_res test_pair_stress_set() {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    // Set many times with complex structures
    for (int i = 0; i < __TEST_SIZE; i++) {
        test_struct first = create_test_struct(i, "StressFirst", (double)i * 1.1);
        test_struct second = create_test_struct(i * 2, "StressSecond", (double)i * 2.2);
        rc = pair_set(&p, &first, &second);
        free_test_struct(&first);
        free_test_struct(&second);

        if (rc != CS_SUCCESS) {
            pair_free(&p);
            return (test_res){(char*)__func__, "pair_set failed during stress", rc};
        }
    }

    // Verify final values
    test_struct *stored_first = (test_struct*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    int expected_first_id = __TEST_SIZE - 1;
    int expected_second_id = (__TEST_SIZE - 1) * 2;

    if (!stored_first || stored_first->id != expected_first_id) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Final first value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != expected_second_id) {
        pair_free(&p);
        return (test_res){(char*)__func__, "Final second value incorrect", CS_ELEM};
    }

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_multiple_pairs() {
    pair pairs[50];
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    // Initialize all pairs
    for (int i = 0; i < 50; i++) {
        cs_codes rc = pair_init(&pairs[i], &first_attr, &second_attr);
        if (rc != CS_SUCCESS) {
            // Free already initialized
            for (int j = 0; j < i; j++) {
                pair_free(&pairs[j]);
            }
            return (test_res){(char*)__func__, "pair_init failed", rc};
        }

        test_struct first = create_test_struct(i, "MultiFirst", (double)i);
        test_struct second = create_test_struct(i * 3, "MultiSecond", (double)(i * 3));
        rc = pair_set(&pairs[i], &first, &second);
        free_test_struct(&first);
        free_test_struct(&second);

        if (rc != CS_SUCCESS) {
            for (int j = 0; j <= i; j++) {
                pair_free(&pairs[j]);
            }
            return (test_res){(char*)__func__, "pair_set failed", rc};
        }
    }

    // Verify all pairs
    for (int i = 0; i < 50; i++) {
        test_struct *stored_first = (test_struct*)pair_first(pairs[i]);
        test_struct *stored_second = (test_struct*)pair_second(pairs[i]);

        if (!stored_first || stored_first->id != i) {
            for (int j = 0; j < 50; j++) pair_free(&pairs[j]);
            return (test_res){(char*)__func__, "First value mismatch", CS_ELEM};
        }

        if (!stored_second || stored_second->id != i * 3) {
            for (int j = 0; j < 50; j++) pair_free(&pairs[j]);
            return (test_res){(char*)__func__, "Second value mismatch", CS_ELEM};
        }
    }

    // Free all pairs
    for (int i = 0; i < 50; i++) {
        pair_free(&pairs[i]);
    }

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// MAIN RUNNER
int main(int argc, char** argv) {
    test tests[] = {
        // pair_init
        test_pair_init,
        test_pair_init_different_types,

        // pair_set
        test_pair_set_single,
        test_pair_set_overwrite,
        test_pair_set_multiple_overwrites,

        // pair_first / pair_second
        test_pair_first,
        test_pair_second,
        test_pair_first_empty,
        test_pair_second_empty,

        // Deep copy verification
        test_pair_deep_copy_first,
        test_pair_deep_copy_second,

        // Nested data integrity
        test_pair_nested_data_integrity,

        // Different type combinations
        test_pair_int_and_struct,
        test_pair_struct_and_double,
        test_pair_string_and_struct,

        // Stress tests
        test_pair_stress_set,
        test_pair_multiple_pairs
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}