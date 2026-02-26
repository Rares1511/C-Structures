#include <cs/vector.h>
#include <unittest.h>

// ============================================================================
// vector_init
// ============================================================================

test_res test_vector_init(test_arg *arg) {
    vector_attr_t attr = get_test_struct_attr();
    vector vec;
    cs_codes rc = vector_init(&vec, attr);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initialized vector with element size: %zu\n", (size_t)attr.size);
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Vector initialized successfully\n");
    if (vector_size(vec) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initial size is 0 as expected\n");
    if (!vector_empty(vec)) return (test_res){(char*)__func__, "Vector not empty after init", CS_UNKNOWN};
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Vector is empty after init as expected\n");

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_push_back
// ============================================================================

test_res test_vector_push_back_single(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "TestItem42", 42.5);

    cs_codes result = vector_push_back(&vec, &ts);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Pushed back element with id: %d\n", ts.id);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Push back returned error", CS_MEM};
    }

    test_struct *back = (test_struct*)vector_at(vec, 0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Accessed back element %p with id: %d\n", back, back ? back->id : -1);
    if (!back || back->id != 42 || strcmp(back->name, "TestItem42") != 0) {
        free_test_struct(&ts);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Push back value mismatch", CS_ELEM};
    }

    free_test_struct(&ts);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_push_back_multiple(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    int total = __TEST_SIZE;

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Pushing back %d elements\n", total);
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "Item", (double)i * 1.5);
        if (vector_push_back(&vec, &ts) != CS_SUCCESS) {
            free_test_struct(&ts);
            vector_free(&vec);
            return (test_res){(char*)__func__, "Push back failed", CS_MEM};
        }
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Finished pushing back %d elements\n", total);
    if (vector_size(vec) != total) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Size mismatch after push back", CS_UNKNOWN};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size after push back is correct: %d\n", vector_size(vec));

    test_struct *last = (test_struct*)vector_at(vec, total - 1);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Accessed last element %p with id: %d\n", last, last ? last->id : -1);
    if (!last || last->id != total - 1) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Last element mismatch", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_push_back_growth(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    int total = 100; // Force multiple reallocations
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing growth with %d elements to force reallocations\n", total);

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "GrowthTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    // Verify all elements are accessible and have correct data
    for (int i = 0; i < total; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->id != i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Data corruption after growth", CS_ELEM};
        }
        // Verify nested data integrity
        if (!val->address || val->address->zip_code != 10000 + i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Nested address corruption", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_insert_at
// ============================================================================

test_res test_vector_insert_at_front(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 1; i <= 5; i++) {
        test_struct ts = create_test_struct(i, "InsertTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct ts0 = create_test_struct(0, "InsertedFront", 0.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserting element at front position 0\n");
    cs_codes result = vector_insert_at(&vec, &ts0, 0);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts0);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Insert at front failed", CS_MEM};
    }

    test_struct *at0 = (test_struct*)vector_at(vec, 0);
    if (!at0 || at0->id != 0 || vector_size(vec) != 6) {
        free_test_struct(&ts0);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Insert at front value mismatch", CS_POS};
    }

    free_test_struct(&ts0);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_insert_at_middle(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "MiddleTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct ts99 = create_test_struct(99, "InsertedMiddle", 99.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserting element at middle position 5\n");
    cs_codes result = vector_insert_at(&vec, &ts99, 5);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts99);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Insert at middle failed", CS_MEM};
    }

    test_struct *at5 = (test_struct*)vector_at(vec, 5);
    if (!at5 || at5->id != 99 || vector_size(vec) != 11) {
        free_test_struct(&ts99);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Insert at middle value mismatch", CS_POS};
    }

    // Verify elements shifted correctly
    test_struct *at6 = (test_struct*)vector_at(vec, 6);
    if (!at6 || at6->id != 5) {
        free_test_struct(&ts99);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Elements not shifted correctly", CS_ELEM};
    }

    free_test_struct(&ts99);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_insert_at_back(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "BackInsertTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct ts99 = create_test_struct(99, "InsertedBack", 99.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserting element at back position 5\n");
    cs_codes result = vector_insert_at(&vec, &ts99, 5);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts99);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Insert at back failed", CS_MEM};
    }

    test_struct *at5 = (test_struct*)vector_at(vec, 5);
    if (!at5 || at5->id != 99 || vector_size(vec) != 6) {
        free_test_struct(&ts99);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Insert at back value mismatch", CS_POS};
    }

    free_test_struct(&ts99);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_pop_back
// ============================================================================

test_res test_vector_pop_back_single(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "PopBackTest", 42.0);
    vector_push_back(&vec, &ts);
    free_test_struct(&ts);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Popping single element from vector\n");

    cs_codes result = vector_pop_back(&vec);
    if (result != CS_SUCCESS) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Pop back failed", CS_EMPTY};
    }

    if (!vector_empty(vec)) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Vector not empty after pop", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_pop_back_multiple(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "PopBackMulti", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Popping all 10 elements in reverse order\n");

    for (int i = 9; i >= 0; i--) {
        test_struct *last = (test_struct*)vector_at(vec, i);
        if (!last || last->id != i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Last value mismatch before pop", CS_ELEM};
        }
        vector_pop_back(&vec);
    }

    if (!vector_empty(vec)) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Vector not empty after all pops", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_pop_back_empty(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pop_back on empty vector (should fail)\n");

    cs_codes result = vector_pop_back(&vec);
    if (result == CS_SUCCESS) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Pop back on empty should fail", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_erase
// ============================================================================

test_res test_vector_erase_front(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing element at front position 0\n");

    cs_codes result = vector_erase(&vec, 0);
    if (result != CS_SUCCESS) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Erase front failed", CS_POS};
    }

    test_struct *front = (test_struct*)vector_at(vec, 0);
    if (vector_size(vec) != 4 || !front || front->id != 1) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Erase front value mismatch", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_erase_middle(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseMiddle", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing element at middle position 2\n");

    cs_codes result = vector_erase(&vec, 2); // Erase id=2
    if (result != CS_SUCCESS) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Erase middle failed", CS_POS};
    }

    test_struct *at2 = (test_struct*)vector_at(vec, 2);
    if (vector_size(vec) != 4 || !at2 || at2->id != 3) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Erase middle shift failed", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_erase_back(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseBack", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing element at back position 4\n");

    cs_codes result = vector_erase(&vec, 4); // Erase last
    if (result != CS_SUCCESS) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Erase back failed", CS_POS};
    }

    test_struct *last = (test_struct*)vector_at(vec, 3);
    if (vector_size(vec) != 4 || !last || last->id != 3) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Erase back value mismatch", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_replace
// ============================================================================

test_res test_vector_replace_single(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "Original", 42.0);
    vector_push_back(&vec, &ts);
    free_test_struct(&ts);

    test_struct ts_new = create_test_struct(99, "Replaced", 99.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Replacing element at position 0 (id 42 -> 99)\n");
    cs_codes result = vector_replace(&vec, &ts_new, 0);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts_new);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Replace failed", CS_POS};
    }

    test_struct *at0 = (test_struct*)vector_at(vec, 0);
    if (!at0 || at0->id != 99 || strcmp(at0->name, "Replaced") != 0) {
        free_test_struct(&ts_new);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Replace value mismatch", CS_ELEM};
    }

    free_test_struct(&ts_new);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_replace_middle(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "ReplaceTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct ts_new = create_test_struct(99, "ReplacedMiddle", 99.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Replacing element at middle position 2\n");
    cs_codes result = vector_replace(&vec, &ts_new, 2);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts_new);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Replace middle failed", CS_POS};
    }

    test_struct *at2 = (test_struct*)vector_at(vec, 2);
    if (!at2 || at2->id != 99) {
        free_test_struct(&ts_new);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Replace middle value mismatch", CS_ELEM};
    }

    // Verify other elements unchanged
    test_struct *at1 = (test_struct*)vector_at(vec, 1);
    test_struct *at3 = (test_struct*)vector_at(vec, 3);
    if (!at1 || at1->id != 1 || !at3 || at3->id != 3) {
        free_test_struct(&ts_new);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Replace affected other elements", CS_ELEM};
    }

    free_test_struct(&ts_new);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_replace_invalid_pos(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "Test", 42.0);
    vector_push_back(&vec, &ts);
    free_test_struct(&ts);

    test_struct ts_new = create_test_struct(99, "New", 99.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing replace at invalid position 5 (should fail)\n");
    cs_codes result = vector_replace(&vec, &ts_new, 5);
    if (result == CS_SUCCESS) {
        free_test_struct(&ts_new);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Replace at invalid pos should fail", CS_POS};
    }

    free_test_struct(&ts_new);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_at
// ============================================================================

test_res test_vector_at_valid(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "AtValidTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing vector_at for all 100 positions\n");

    for (int i = 0; i < 100; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->id != i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "At value mismatch", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_at_out_of_bounds(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "OOBTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing out of bounds access at positions 10, -1, 1000\n");

    if (vector_at(vec, 10) != NULL) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "At index 10 should return NULL", CS_POS};
    }

    if (vector_at(vec, -1) != NULL) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "At index -1 should return NULL", CS_POS};
    }

    if (vector_at(vec, 1000) != NULL) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "At index 1000 should return NULL", CS_POS};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_find
// ============================================================================

test_res test_vector_find_existing(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "FindTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct search = create_test_struct(5, "FindTest", 5.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Searching for element with id 5\n");
    int pos = vector_find(vec, &search);
    free_test_struct(&search);

    if (pos != 5) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Find returned wrong position", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_find_not_existing(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "FindTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct search = create_test_struct(99, "NotFound", 99.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Searching for non-existing element with id 99\n");
    int pos = vector_find(vec, &search);
    free_test_struct(&search);

    if (pos != CS_ELEM) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Find should return CS_ELEM for non-existing", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_find_first_occurrence(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    // Insert duplicates
    for (int i = 0; i < 3; i++) {
        test_struct ts = create_test_struct(42, "Duplicate", 42.0);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct search = create_test_struct(42, "Duplicate", 42.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Finding first occurrence of duplicate element (id 42)\n");
    int pos = vector_find(vec, &search);
    free_test_struct(&search);

    if (pos != 0) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Find should return first occurrence", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_count
// ============================================================================

test_res test_vector_count_none(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "CountTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct search = create_test_struct(99, "NotFound", 99.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Counting non-existing element (id 99)\n");
    int count = vector_count(vec, &search);
    free_test_struct(&search);

    if (count != 0) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Count should be 0", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_count_single(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "CountTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct search = create_test_struct(5, "CountTest", 5.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Counting single occurrence (id 5)\n");
    int count = vector_count(vec, &search);
    free_test_struct(&search);

    if (count != 1) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Count should be 1", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_count_multiple(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(42, "Duplicate", 42.0);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    for (int i = 0; i < 3; i++) {
        test_struct ts = create_test_struct(99, "Other", 99.0);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct search = create_test_struct(42, "Duplicate", 42.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Counting 5 duplicates (id 42)\n");
    int count = vector_count(vec, &search);
    free_test_struct(&search);

    if (count != 5) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Count should be 5", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_empty
// ============================================================================

test_res test_vector_empty_initial(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Checking if newly initialized vector is empty\n");

    if (!vector_empty(vec)) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "New vector should be empty", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_empty_after_ops(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "EmptyOpsTest", 42.0);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing empty state after push/pop operations\n");

    vector_push_back(&vec, &ts);
    if (vector_empty(vec)) {
        free_test_struct(&ts);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Vector should not be empty after push", CS_UNKNOWN};
    }

    vector_pop_back(&vec);
    if (!vector_empty(vec)) {
        free_test_struct(&ts);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Vector should be empty after pop", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_size
// ============================================================================

test_res test_vector_size_initial(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Checking initial size is 0\n");

    if (vector_size(vec) != 0) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "New vector size should be 0", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_size_after_ops(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing size changes during 100 push/pop operations\n");

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "SizeOpsTest", (double)i);
        vector_push_back(&vec, &ts);
        if (vector_size(vec) != i + 1) {
            free_test_struct(&ts);
            vector_free(&vec);
            return (test_res){(char*)__func__, "Size mismatch during push", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    for (int i = 99; i >= 0; i--) {
        vector_pop_back(&vec);
        if (vector_size(vec) != i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Size mismatch during pop", CS_UNKNOWN};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_swap
// ============================================================================

test_res test_vector_swap(test_arg *arg) {
    vector vec1, vec2;
    vector_init(&vec1, get_test_struct_attr());
    vector_init(&vec2, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Swapping two vectors with 5 elements each\n");

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "Swap1", (double)i);
        vector_push_back(&vec1, &ts);
        free_test_struct(&ts);
    }
    for (int i = 10; i < 15; i++) {
        test_struct ts = create_test_struct(i, "Swap2", (double)i);
        vector_push_back(&vec2, &ts);
        free_test_struct(&ts);
    }

    vector_swap(&vec1, &vec2);

    test_struct *first1 = (test_struct*)vector_at(vec1, 0);
    test_struct *last1 = (test_struct*)vector_at(vec1, 4);
    if (!first1 || first1->id != 10 || !last1 || last1->id != 14) {
        vector_free(&vec1);
        vector_free(&vec2);
        return (test_res){(char*)__func__, "Swap vec1 content mismatch", CS_ELEM};
    }

    test_struct *first2 = (test_struct*)vector_at(vec2, 0);
    test_struct *last2 = (test_struct*)vector_at(vec2, 4);
    if (!first2 || first2->id != 0 || !last2 || last2->id != 4) {
        vector_free(&vec1);
        vector_free(&vec2);
        return (test_res){(char*)__func__, "Swap vec2 content mismatch", CS_ELEM};
    }

    vector_free(&vec1);
    vector_free(&vec2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_swap_empty(test_arg *arg) {
    vector vec1, vec2;
    vector_init(&vec1, get_test_struct_attr());
    vector_init(&vec2, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Swapping non-empty vector with empty vector\n");

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "SwapEmpty", (double)i);
        vector_push_back(&vec1, &ts);
        free_test_struct(&ts);
    }

    vector_swap(&vec1, &vec2);

    if (!vector_empty(vec1)) {
        vector_free(&vec1);
        vector_free(&vec2);
        return (test_res){(char*)__func__, "vec1 should be empty after swap", CS_UNKNOWN};
    }

    if (vector_size(vec2) != 5) {
        vector_free(&vec1);
        vector_free(&vec2);
        return (test_res){(char*)__func__, "vec2 should have 5 elements", CS_UNKNOWN};
    }

    vector_free(&vec1);
    vector_free(&vec2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_clear
// ============================================================================

test_res test_vector_clear(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Clearing vector with 100 elements\n");

    vector_clear(&vec);

    if (vector_size(vec) != 0 || !vector_empty(vec)) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Clear did not reset vector", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_clear_reuse(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearReuse", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Clearing then reusing vector\n");

    vector_clear(&vec);

    // Verify can reuse after clear
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    test_struct *first = (test_struct*)vector_at(vec, 0);
    if (vector_size(vec) != 50 || !first || first->id != 100) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_sort
// ============================================================================

test_res test_vector_sort_ascending(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    int ids[] = {50, 10, 40, 20, 30};
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting unsorted array [50,10,40,20,30]\n");

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(ids[i], "SortTest", (double)ids[i]);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    vector_sort(&vec);

    // Should be sorted by id: 10, 20, 30, 40, 50
    int expected[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->id != expected[i]) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Sort order incorrect", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_sort_already_sorted(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting already sorted array\n");

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "AlreadySorted", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    vector_sort(&vec);

    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->id != i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Sort corrupted already sorted data", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_sort_reverse(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting reverse-ordered array\n");

    for (int i = 9; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "ReverseSort", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    vector_sort(&vec);

    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->id != i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Sort from reverse failed", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_sort_by_score(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr_by_score());
    double scores[] = {50.5, 10.1, 40.4, 20.2, 30.3};
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting by score [50.5,10.1,40.4,20.2,30.3]\n");

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "ScoreSort", scores[i]);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    vector_sort(&vec);

    // Should be sorted by score: 10.1, 20.2, 30.3, 40.4, 50.5
    double expected[] = {10.1, 20.2, 30.3, 40.4, 50.5};
    for (int i = 0; i < 5; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->score != expected[i]) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Sort by score order incorrect", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// vector_set_* functions
// ============================================================================

test_res test_vector_set_attr(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Changing vector attributes to sort by score\n");

    // Change to sort by score
    vector_attr_t new_attr = get_test_struct_attr_by_score();
    vector_set_attr(&vec, new_attr);

    // Add elements
    test_struct ts1 = create_test_struct(1, "SetAttr1", 50.0);
    test_struct ts2 = create_test_struct(2, "SetAttr2", 10.0);
    vector_push_back(&vec, &ts1);
    vector_push_back(&vec, &ts2);

    vector_sort(&vec);

    // Should be sorted by score now
    test_struct *first = (test_struct*)vector_at(vec, 0);
    if (!first || first->score != 10.0) {
        free_test_struct(&ts1);
        free_test_struct(&ts2);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Set attr did not change comparator", CS_ELEM};
    }

    free_test_struct(&ts1);
    free_test_struct(&ts2);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_set_comp(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Changing comparator function at runtime\n");

    // Add elements
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "SetComp", (double)(10 - i));
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    // Change comparator to sort by score
    vector_set_comp(&vec, comp_test_struct_by_score);
    vector_sort(&vec);

    // First element should have lowest score (6.0)
    test_struct *first = (test_struct*)vector_at(vec, 0);
    if (!first || first->score != 6.0) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Set comp did not change sort order", CS_ELEM};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_vector_nested_data_integrity(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing nested data integrity for 50 complex structs\n");

    // Push elements with complex nested data
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    // Verify all nested structures are intact
    for (int i = 0; i < 50; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Element is NULL", CS_ELEM};
        }

        // Check address
        if (!val->address || val->address->zip_code != 10000 + i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Address data corrupted", CS_ELEM};
        }

        // Check contacts array
        int expected_contacts = 2 + (i % 3);
        if (val->contact_count != expected_contacts || !val->contacts) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Contacts array corrupted", CS_ELEM};
        }

        // Check tags array
        int expected_tags = 3 + (i % 3);
        if (val->tag_count != expected_tags || !val->tags) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Tags array corrupted", CS_ELEM};
        }

        // Check flags
        unsigned char expected_flags = TEST_FLAG_ACTIVE;
        if (i % 2 == 0) expected_flags |= TEST_FLAG_VERIFIED;
        if (i % 5 == 0) expected_flags |= TEST_FLAG_PREMIUM;
        if (i % 10 == 0) expected_flags |= TEST_FLAG_ADMIN;
        if (val->flags != expected_flags) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Flags corrupted", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_deep_copy_verification(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());

    test_struct original = create_test_struct(42, "DeepCopyTest", 42.42);
    vector_push_back(&vec, &original);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verifying deep copy (modifying original should not affect stored)\n");

    // Modify original after push - should not affect vector content
    original.id = 999;
    free(original.name);
    original.name = strdup("Modified");
    original.address->zip_code = 99999;

    test_struct *stored = (test_struct*)vector_at(vec, 0);
    if (!stored || stored->id != 42) {
        free_test_struct(&original);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopyTest") != 0) {
        free_test_struct(&original);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10042) {
        free_test_struct(&original);
        vector_free(&vec);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    free_test_struct(&original);
    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests
// ============================================================================

test_res test_vector_large_dataset(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    int total = __TEST_SIZE;
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing large dataset with %d elements\n", total);

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "LargeDataset", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }

    // Random access pattern
    for (int i = 0; i < total; i += 7) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->id != i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Large random access failed", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_interleaved_ops(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing interleaved push/pop operations\n");

    // Interleave pushes and pops
    for (int i = 0; i < 50; i++) {
        test_struct ts1 = create_test_struct(i * 2, "Interleaved", (double)i);
        test_struct ts2 = create_test_struct(i * 2 + 1, "Interleaved", (double)i);
        vector_push_back(&vec, &ts1);
        vector_push_back(&vec, &ts2);
        free_test_struct(&ts1);
        free_test_struct(&ts2);

        if (i % 3 == 0) {
            vector_pop_back(&vec);
        }
    }

    // Verify size is correct
    int expected_size = 100 - 17; // 50 pairs - 17 pops (i=0,3,6,...,48)
    if (vector_size(vec) != expected_size) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Interleaved ops size mismatch", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_erase_all(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "EraseAll", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing all 50 elements from front\n");

    // Erase from front until empty
    while (!vector_empty(vec)) {
        vector_erase(&vec, 0);
    }

    if (!vector_empty(vec)) {
        vector_free(&vec);
        return (test_res){(char*)__func__, "Vector not empty after erasing all", CS_UNKNOWN};
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_vector_replace_all(test_arg *arg) {
    vector vec;
    vector_init(&vec, get_test_struct_attr());

    for (int i = 0; i < 20; i++) {
        test_struct ts = create_test_struct(i, "ReplaceAll", (double)i);
        vector_push_back(&vec, &ts);
        free_test_struct(&ts);
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Replacing all 20 elements\n");

    // Replace all elements
    for (int i = 0; i < 20; i++) {
        test_struct ts_new = create_test_struct(100 + i, "Replaced", (double)(100 + i));
        vector_replace(&vec, &ts_new, i);
        free_test_struct(&ts_new);
    }

    // Verify all replaced
    for (int i = 0; i < 20; i++) {
        test_struct *val = (test_struct*)vector_at(vec, i);
        if (!val || val->id != 100 + i) {
            vector_free(&vec);
            return (test_res){(char*)__func__, "Replace all failed", CS_ELEM};
        }
    }

    vector_free(&vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test vector_tests[] = {
    // vector_init
    test_vector_init,
    
    // vector_push_back
    test_vector_push_back_single,
    test_vector_push_back_multiple,
    test_vector_push_back_growth,
    
    // vector_insert_at
    test_vector_insert_at_front,
    test_vector_insert_at_middle,
    test_vector_insert_at_back,
    
    // vector_pop_back
    test_vector_pop_back_single,
    test_vector_pop_back_multiple,
    test_vector_pop_back_empty,
    
    // vector_erase
    test_vector_erase_front,
    test_vector_erase_middle,
    test_vector_erase_back,
    
    // vector_replace
    test_vector_replace_single,
    test_vector_replace_middle,
    test_vector_replace_invalid_pos,
    
    // vector_at
    test_vector_at_valid,
    test_vector_at_out_of_bounds,
    
    // vector_find
    test_vector_find_existing,
    test_vector_find_not_existing,
    test_vector_find_first_occurrence,
    
    // vector_count
    test_vector_count_none,
    test_vector_count_single,
    test_vector_count_multiple,
    
    // vector_empty
    test_vector_empty_initial,
    test_vector_empty_after_ops,
    
    // vector_size
    test_vector_size_initial,
    test_vector_size_after_ops,
    
    // vector_swap
    test_vector_swap,
    test_vector_swap_empty,
    
    // vector_clear
    test_vector_clear,
    test_vector_clear_reuse,
    
    // vector_sort
    test_vector_sort_ascending,
    test_vector_sort_already_sorted,
    test_vector_sort_reverse,
    test_vector_sort_by_score,
    
    // vector_set_* functions
    test_vector_set_attr,
    test_vector_set_comp,
    
    // Complex struct integrity
    test_vector_nested_data_integrity,
    test_vector_deep_copy_verification,
    
    // Stress tests
    test_vector_large_dataset,
    test_vector_interleaved_ops,
    test_vector_erase_all,
    test_vector_replace_all
};