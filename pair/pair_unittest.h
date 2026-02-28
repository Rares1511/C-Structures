#include <cs/pair.h>
#include <unittest.h>   

// ============================================================================
// pair_init
// ============================================================================

test_res test_pair_init(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing pair with test_struct attributes\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_init succeeded\n");

    if (p.first != NULL || p.second != NULL) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Initial values should be NULL but got first=%p, second=%p\n", p.first, p.second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Initial values should be NULL", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified initial values are NULL\n");

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_init_different_types(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = {
        .size = sizeof(int),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing pair with different types (test_struct, int)\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init with different types failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init with different types failed", rc};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Pair initialized successfully with different types\n");

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_init_null_pair(test_arg *arg) {
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_init with NULL pair pointer\n");
    cs_codes rc = pair_init(NULL, &first_attr, &second_attr);
    if (rc != CS_ELEM) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected CS_ELEM but got %d\n", rc);
        return (test_res){(char*)__func__, "pair_init with NULL pair should return CS_ELEM", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_ELEM for NULL pair\n");

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_init_null_first_attr(test_arg *arg) {
    pair p;
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_init with NULL first_attr\n");
    cs_codes rc = pair_init(&p, NULL, &second_attr);
    if (rc != CS_ELEM) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected CS_ELEM but got %d\n", rc);
        return (test_res){(char*)__func__, "pair_init with NULL first_attr should return CS_ELEM", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_ELEM for NULL first_attr\n");

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_init_null_second_attr(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_init with NULL second_attr\n");
    cs_codes rc = pair_init(&p, &first_attr, NULL);
    if (rc != CS_ELEM) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected CS_ELEM but got %d\n", rc);
        return (test_res){(char*)__func__, "pair_init with NULL second_attr should return CS_ELEM", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_ELEM for NULL second_attr\n");

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_init_zero_first_size(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    first_attr.size = 0;
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_init with zero first size\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SIZE) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected CS_SIZE but got %d\n", rc);
        return (test_res){(char*)__func__, "pair_init with zero first size should return CS_SIZE", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_SIZE for zero first size\n");

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_init_zero_second_size(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();
    second_attr.size = 0;

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_init with zero second size\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SIZE) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected CS_SIZE but got %d\n", rc);
        return (test_res){(char*)__func__, "pair_init with zero second size should return CS_SIZE", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_SIZE for zero second size\n");

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// pair_set
// ============================================================================

test_res test_pair_set_null_pair(test_arg *arg) {
    int first = 42;
    int second = 100;

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_set with NULL pair\n");
    cs_codes rc = pair_set(NULL, &first, &second);
    if (rc != CS_ELEM) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Expected CS_ELEM but got %d\n", rc);
        return (test_res){(char*)__func__, "pair_set with NULL pair should return CS_ELEM", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Correctly returned CS_ELEM for NULL pair\n");

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_set_first_only(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_int_attr();
    pair_attr_t second_attr = get_int_attr();
    pair_init(&p, &first_attr, &second_attr);

    int first = 42;
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting only first value to %d\n", first);
    cs_codes rc = pair_set(&p, &first, NULL);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set with first only failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_set with first only should succeed", rc};
    }
    if (p.first == NULL) {
        pair_free(&p);
        clogger_log(*arg->logger, CLOGGER_ERROR, "first should be allocated but is NULL\n");
        return (test_res){(char*)__func__, "first should be allocated", CS_UNKNOWN};
    }
    if (p.second != NULL) {
        pair_free(&p);
        clogger_log(*arg->logger, CLOGGER_ERROR, "second should remain NULL but is %p\n", p.second);
        return (test_res){(char*)__func__, "second should remain NULL", CS_UNKNOWN};
    }
    if (*(int*)p.first != 42) {
        pair_free(&p);
        clogger_log(*arg->logger, CLOGGER_ERROR, "first value incorrect: expected 42, got %d\n", *(int*)p.first);
        return (test_res){(char*)__func__, "first value incorrect", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Successfully set first value only\n");

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_set_second_only(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_int_attr();
    pair_attr_t second_attr = get_int_attr();
    pair_init(&p, &first_attr, &second_attr);

    int second = 100;
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting only second value to %d\n", second);
    cs_codes rc = pair_set(&p, NULL, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set with second only failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_set with second only should succeed", rc};
    }
    if (p.first != NULL) {
        pair_free(&p);
        clogger_log(*arg->logger, CLOGGER_ERROR, "first should remain NULL but is %p\n", p.first);
        return (test_res){(char*)__func__, "first should remain NULL", CS_UNKNOWN};
    }
    if (p.second == NULL) {
        pair_free(&p);
        clogger_log(*arg->logger, CLOGGER_ERROR, "second should be allocated but is NULL\n");
        return (test_res){(char*)__func__, "second should be allocated", CS_UNKNOWN};
    }
    if (*(int*)p.second != 100) {
        pair_free(&p);
        clogger_log(*arg->logger, CLOGGER_ERROR, "second value incorrect: expected 100, got %d\n", *(int*)p.second);
        return (test_res){(char*)__func__, "second value incorrect", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Successfully set second value only\n");

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_set_single(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing pair for single set test\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "FirstValue", 42.0);
    test_struct second = create_test_struct(100, "SecondValue", 100.0);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting pair with first.id=%d, second.id=%d\n", first.id, second.id);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_set succeeded\n");

    free_test_struct(&first);
    free_test_struct(&second);
    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_set_overwrite(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    // First set
    test_struct first1 = create_test_struct(1, "First1", 1.0);
    test_struct second1 = create_test_struct(2, "Second1", 2.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "First set: first.id=%d, second.id=%d\n", first1.id, second1.id);
    rc = pair_set(&p, &first1, &second1);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "First pair_set failed with code %d\n", rc);
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
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Overwrite: first.id=%d, second.id=%d\n", first2.id, second2.id);
    rc = pair_set(&p, &first2, &second2);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Overwrite pair_set failed with code %d\n", rc);
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
        clogger_log(*arg->logger, CLOGGER_ERROR, "First value not overwritten correctly: expected 10, got %d\n", 
                    stored_first ? stored_first->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "First value not overwritten correctly", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 20) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Second value not overwritten correctly: expected 20, got %d\n", 
                    stored_second ? stored_second->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Second value not overwritten correctly", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verified overwritten values: first.id=%d, second.id=%d\n", 
                stored_first->id, stored_second->id);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_set_multiple_overwrites(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Starting 100 overwrites to test for memory leaks\n");
    // Overwrite many times to test for memory leaks
    for (int i = 0; i < 100; i++) {
        test_struct first = create_test_struct(i, "OverwriteFirst", (double)i);
        test_struct second = create_test_struct(i * 2, "OverwriteSecond", (double)(i * 2));
        rc = pair_set(&p, &first, &second);
        free_test_struct(&first);
        free_test_struct(&second);

        if (rc != CS_SUCCESS) {
            clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed at iteration %d with code %d\n", i, rc);
            pair_free(&p);
            return (test_res){(char*)__func__, "pair_set failed during overwrites", rc};
        }
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Completed 100 overwrites successfully\n");

    // Verify final values
    test_struct *stored_first = (test_struct*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    if (!stored_first || stored_first->id != 99) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Final first value incorrect: expected 99, got %d\n", 
                    stored_first ? stored_first->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Final first value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 198) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Final second value incorrect: expected 198, got %d\n", 
                    stored_second ? stored_second->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Final second value incorrect", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Final values verified: first.id=%d, second.id=%d\n", 
                stored_first->id, stored_second->id);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// pair_first / pair_second
// ============================================================================

test_res test_pair_first(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "GetFirst", 42.42);
    test_struct second = create_test_struct(100, "GetSecond", 100.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting pair with first.id=%d\n", first.id);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&first);
    free_test_struct(&second);

    test_struct *retrieved = (test_struct*)pair_first(p);
    if (!retrieved) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_first returned NULL\n");
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_first returned NULL", CS_ELEM};
    }

    if (retrieved->id != 42 || strcmp(retrieved->name, "GetFirst") != 0) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_first returned wrong value: id=%d, name=%s\n", 
                    retrieved->id, retrieved->name);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_first returned wrong value", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_first returned correct value: id=%d, name=%s\n", 
                retrieved->id, retrieved->name);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_second(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "GetFirst", 42.0);
    test_struct second = create_test_struct(100, "GetSecond", 100.5);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting pair with second.id=%d\n", second.id);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&first);
    free_test_struct(&second);

    test_struct *retrieved = (test_struct*)pair_second(p);
    if (!retrieved) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_second returned NULL\n");
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_second returned NULL", CS_ELEM};
    }

    if (retrieved->id != 100 || strcmp(retrieved->name, "GetSecond") != 0) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_second returned wrong value: id=%d, name=%s\n", 
                    retrieved->id, retrieved->name);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_second returned wrong value", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_second returned correct value: id=%d, name=%s\n", 
                retrieved->id, retrieved->name);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_first_empty(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_first on empty pair\n");
    // pair_first on empty pair should return NULL
    void *retrieved = pair_first(p);
    if (retrieved != NULL) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_first should return NULL for empty pair but got %p\n", retrieved);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_first should return NULL for empty pair", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_first correctly returned NULL for empty pair\n");

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_second_empty(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_second on empty pair\n");
    // pair_second on empty pair should return NULL
    void *retrieved = pair_second(p);
    if (retrieved != NULL) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_second should return NULL for empty pair but got %p\n", retrieved);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_second should return NULL for empty pair", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_second correctly returned NULL for empty pair\n");

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Deep copy verification
// ============================================================================

test_res test_pair_deep_copy_first(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "DeepCopyFirst", 42.0);
    test_struct second = create_test_struct(100, "DeepCopySecond", 100.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting pair and testing deep copy of first\n");
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }

    // Modify original after set
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Modifying original first struct after set\n");
    first.id = 999;
    free(first.name);
    first.name = strdup("Modified");
    first.address->zip_code = 99999;

    // Stored value should be unchanged
    test_struct *stored = (test_struct*)pair_first(p);
    if (!stored || stored->id != 42) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Deep copy failed - id changed: expected 42, got %d\n", 
                    stored ? stored->id : -1);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopyFirst") != 0) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Deep copy failed - name changed: expected 'DeepCopyFirst', got '%s'\n", 
                    stored->name);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10042) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Deep copy failed - address changed: expected 10042, got %d\n", 
                    stored->address->zip_code);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deep copy verified - stored values unchanged after modifying original\n");

    free_test_struct(&first);
    free_test_struct(&second);
    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_deep_copy_second(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(42, "First", 42.0);
    test_struct second = create_test_struct(100, "DeepCopySecond", 100.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting pair and testing deep copy of second\n");
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }

    // Modify second original after set
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Modifying original second struct after set\n");
    second.id = 888;
    free(second.name);
    second.name = strdup("Modified");
    second.address->zip_code = 88888;

    // Stored second value should be unchanged
    test_struct *stored = (test_struct*)pair_second(p);
    if (!stored || stored->id != 100) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Deep copy failed - id changed: expected 100, got %d\n", 
                    stored ? stored->id : -1);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopySecond") != 0) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Deep copy failed - name changed: expected 'DeepCopySecond', got '%s'\n", 
                    stored->name);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10100) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Deep copy failed - address changed: expected 10100, got %d\n", 
                    stored->address->zip_code);
        free_test_struct(&first);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deep copy verified - stored values unchanged after modifying original\n");

    free_test_struct(&first);
    free_test_struct(&second);
    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Nested data integrity
// ============================================================================

test_res test_pair_nested_data_integrity(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(50, "NestedFirst", 50.5);
    test_struct second = create_test_struct(75, "NestedSecond", 75.5);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Setting pair with nested structures for integrity test\n");
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
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
        clogger_log(*arg->logger, CLOGGER_ERROR, "First address corrupted: expected zip 10050, got %d\n", 
                    stored_first->address ? stored_first->address->zip_code : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "First address corrupted", CS_ELEM};
    }

    int expected_first_contacts = 2 + (50 % 3);
    if (stored_first->contact_count != expected_first_contacts || !stored_first->contacts) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "First contacts corrupted: expected %d, got %d\n", 
                    expected_first_contacts, stored_first->contact_count);
        pair_free(&p);
        return (test_res){(char*)__func__, "First contacts corrupted", CS_ELEM};
    }

    int expected_first_tags = 3 + (50 % 3);
    if (stored_first->tag_count != expected_first_tags || !stored_first->tags) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "First tags corrupted: expected %d, got %d\n", 
                    expected_first_tags, stored_first->tag_count);
        pair_free(&p);
        return (test_res){(char*)__func__, "First tags corrupted", CS_ELEM};
    }

    // Check second's nested structures
    test_struct *stored_second = (test_struct*)pair_second(p);
    if (!stored_second->address || stored_second->address->zip_code != 10075) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Second address corrupted: expected zip 10075, got %d\n", 
                    stored_second->address ? stored_second->address->zip_code : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Second address corrupted", CS_ELEM};
    }

    int expected_second_contacts = 2 + (75 % 3);
    if (stored_second->contact_count != expected_second_contacts || !stored_second->contacts) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Second contacts corrupted: expected %d, got %d\n", 
                    expected_second_contacts, stored_second->contact_count);
        pair_free(&p);
        return (test_res){(char*)__func__, "Second contacts corrupted", CS_ELEM};
    }

    int expected_second_tags = 3 + (75 % 3);
    if (stored_second->tag_count != expected_second_tags || !stored_second->tags) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Second tags corrupted: expected %d, got %d\n", 
                    expected_second_tags, stored_second->tag_count);
        pair_free(&p);
        return (test_res){(char*)__func__, "Second tags corrupted", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "All nested data integrity checks passed\n");

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Different type combinations
// ============================================================================

test_res test_pair_int_and_struct(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = {
        .size = sizeof(int),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair with int first and struct second\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    int first = 42;
    test_struct second = create_test_struct(100, "MixedSecond", 100.0);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&second);

    int *stored_first = (int*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    if (!stored_first || *stored_first != 42) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "First int value incorrect: expected 42, got %d\n", 
                    stored_first ? *stored_first : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "First int value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 100) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Second struct value incorrect: expected id 100, got %d\n", 
                    stored_second ? stored_second->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Second struct value incorrect", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Mixed types verified: first=%d, second.id=%d\n", 
                *stored_first, stored_second->id);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_struct_and_double(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = {
        .size = sizeof(double),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL
    };

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair with struct first and double second\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    test_struct first = create_test_struct(50, "MixedFirst", 50.0);
    double second = 3.14159;
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&first);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&first);

    test_struct *stored_first = (test_struct*)pair_first(p);
    double *stored_second = (double*)pair_second(p);

    if (!stored_first || stored_first->id != 50) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "First struct value incorrect: expected id 50, got %d\n", 
                    stored_first ? stored_first->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "First struct value incorrect", CS_ELEM};
    }

    if (!stored_second || *stored_second != 3.14159) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Second double value incorrect: expected 3.14159, got %f\n", 
                    stored_second ? *stored_second : 0.0);
        pair_free(&p);
        return (test_res){(char*)__func__, "Second double value incorrect", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Mixed types verified: first.id=%d, second=%f\n", 
                stored_first->id, *stored_second);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_string_and_struct(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = {
        .size = 64,  // capacity for the string buffer
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL
    };
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair with string first and struct second\n");
    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    char first[] = "Hello, Pair!";
    test_struct second = create_test_struct(77, "WithString", 77.7);
    rc = pair_set(&p, &first, &second);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed with code %d\n", rc);
        free_test_struct(&second);
        pair_free(&p);
        return (test_res){(char*)__func__, "pair_set failed", rc};
    }
    free_test_struct(&second);

    char *stored_first = (char*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    if (!stored_first || strcmp(stored_first, "Hello, Pair!") != 0) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "First string value incorrect: expected 'Hello, Pair!', got '%s'\n", 
                    stored_first ? stored_first : "(null)");
        pair_free(&p);
        return (test_res){(char*)__func__, "First string value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != 77) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Second struct value incorrect: expected id 77, got %d\n", 
                    stored_second ? stored_second->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Second struct value incorrect", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Mixed types verified: first='%s', second.id=%d\n", 
                stored_first, stored_second->id);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// pair_free edge cases
// ============================================================================

test_res test_pair_free_null(test_arg *arg) {
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_free with NULL pointer\n");
    // Should not crash when freeing NULL
    pair_free(NULL);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_free(NULL) completed without crash\n");
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_free_unset_values(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_int_attr();
    pair_attr_t second_attr = get_int_attr();
    pair_init(&p, &first_attr, &second_attr);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_free on pair with unset values\n");
    // Don't set any values - first and second are NULL
    // Should not crash
    pair_free(&p);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_free completed without crash on unset pair\n");
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_free_partial_first(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();
    pair_init(&p, &first_attr, &second_attr);

    test_struct ts = create_test_struct(1, "First", 1.5);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_free with only first set\n");
    pair_set(&p, &ts, NULL);
    free_test_struct(&ts);

    // Only first is set, second is NULL
    pair_free(&p);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_free completed without crash on partial (first only) pair\n");
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_free_partial_second(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();
    pair_init(&p, &first_attr, &second_attr);

    test_struct ts = create_test_struct(2, "Second", 2.5);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pair_free with only second set\n");
    pair_set(&p, NULL, &ts);
    free_test_struct(&ts);

    // Only second is set, first is NULL
    pair_free(&p);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "pair_free completed without crash on partial (second only) pair\n");
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests
// ============================================================================

test_res test_pair_stress_set(test_arg *arg) {
    pair p;
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    cs_codes rc = pair_init(&p, &first_attr, &second_attr);
    if (rc != CS_SUCCESS) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed with code %d\n", rc);
        return (test_res){(char*)__func__, "pair_init failed", rc};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Starting stress test with %d iterations\n", __TEST_SIZE);
    // Set many times with complex structures
    for (int i = 0; i < __TEST_SIZE; i++) {
        test_struct first = create_test_struct(i, "StressFirst", (double)i * 1.1);
        test_struct second = create_test_struct(i * 2, "StressSecond", (double)i * 2.2);
        rc = pair_set(&p, &first, &second);
        free_test_struct(&first);
        free_test_struct(&second);

        if (rc != CS_SUCCESS) {
            clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed at iteration %d with code %d\n", i, rc);
            pair_free(&p);
            return (test_res){(char*)__func__, "pair_set failed during stress", rc};
        }
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Completed %d stress iterations\n", __TEST_SIZE);

    // Verify final values
    test_struct *stored_first = (test_struct*)pair_first(p);
    test_struct *stored_second = (test_struct*)pair_second(p);

    int expected_first_id = __TEST_SIZE - 1;
    int expected_second_id = (__TEST_SIZE - 1) * 2;

    if (!stored_first || stored_first->id != expected_first_id) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Final first value incorrect: expected %d, got %d\n", 
                    expected_first_id, stored_first ? stored_first->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Final first value incorrect", CS_ELEM};
    }

    if (!stored_second || stored_second->id != expected_second_id) {
        clogger_log(*arg->logger, CLOGGER_ERROR, "Final second value incorrect: expected %d, got %d\n", 
                    expected_second_id, stored_second ? stored_second->id : -1);
        pair_free(&p);
        return (test_res){(char*)__func__, "Final second value incorrect", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Stress test verified: final first.id=%d, second.id=%d\n", 
                stored_first->id, stored_second->id);

    pair_free(&p);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_pair_multiple_pairs(test_arg *arg) {
    pair pairs[50];
    pair_attr_t first_attr = get_test_struct_attr();
    pair_attr_t second_attr = get_test_struct_attr();

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing multiple pairs: initializing 50 pairs\n");
    // Initialize all pairs
    for (int i = 0; i < 50; i++) {
        cs_codes rc = pair_init(&pairs[i], &first_attr, &second_attr);
        if (rc != CS_SUCCESS) {
            clogger_log(*arg->logger, CLOGGER_ERROR, "pair_init failed at index %d with code %d\n", i, rc);
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
            clogger_log(*arg->logger, CLOGGER_ERROR, "pair_set failed at index %d with code %d\n", i, rc);
            for (int j = 0; j <= i; j++) {
                pair_free(&pairs[j]);
            }
            return (test_res){(char*)__func__, "pair_set failed", rc};
        }
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "All 50 pairs initialized and set successfully\n");

    // Verify all pairs
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verifying all 50 pairs\n");
    for (int i = 0; i < 50; i++) {
        test_struct *stored_first = (test_struct*)pair_first(pairs[i]);
        test_struct *stored_second = (test_struct*)pair_second(pairs[i]);

        if (!stored_first || stored_first->id != i) {
            clogger_log(*arg->logger, CLOGGER_ERROR, "First value mismatch at index %d: expected %d, got %d\n", 
                        i, i, stored_first ? stored_first->id : -1);
            for (int j = 0; j < 50; j++) pair_free(&pairs[j]);
            return (test_res){(char*)__func__, "First value mismatch", CS_ELEM};
        }

        if (!stored_second || stored_second->id != i * 3) {
            clogger_log(*arg->logger, CLOGGER_ERROR, "Second value mismatch at index %d: expected %d, got %d\n", 
                        i, i * 3, stored_second ? stored_second->id : -1);
            for (int j = 0; j < 50; j++) pair_free(&pairs[j]);
            return (test_res){(char*)__func__, "Second value mismatch", CS_ELEM};
        }
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "All 50 pairs verified successfully\n");

    // Free all pairs
    for (int i = 0; i < 50; i++) {
        pair_free(&pairs[i]);
    }

    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test pair_tests[] = {
    // pair_init
    test_pair_init,
    test_pair_init_different_types,
    test_pair_init_null_pair,
    test_pair_init_null_first_attr,
    test_pair_init_null_second_attr,
    test_pair_init_zero_first_size,
    test_pair_init_zero_second_size,

    // pair_set
    test_pair_set_null_pair,
    test_pair_set_first_only,
    test_pair_set_second_only,
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

    // pair_free edge cases
    test_pair_free_null,
    test_pair_free_unset_values,
    test_pair_free_partial_first,
    test_pair_free_partial_second,

    // Stress tests
    test_pair_stress_set,
    test_pair_multiple_pairs
};
