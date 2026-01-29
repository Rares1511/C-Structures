#include <cs/deque.h>
#include <unittest.h>

// ============================================================================
// deque_init
// ============================================================================

test_res test_deque_init() {
    deque_attr_t attr = get_test_struct_attr();
    deque *dq = deque_init(attr);

    if (!dq) return (test_res){(char*)__func__, "Init returned NULL", CS_MEM};
    if (deque_size(*dq) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    if (!deque_empty(*dq)) return (test_res){(char*)__func__, "Deque not empty after init", CS_UNKNOWN};

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_push_back
// ============================================================================

test_res test_deque_push_back_single() {
    deque *dq = deque_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "TestItem42", 42.5);

    cs_codes result = deque_push_back(dq, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        deque_free(dq);
        return (test_res){(char*)__func__, "Push back returned error", CS_MEM};
    }

    test_struct *back = (test_struct*)deque_back(*dq);
    if (!back || back->id != 42 || strcmp(back->name, "TestItem42") != 0) {
        free_test_struct(&ts);
        deque_free(dq);
        return (test_res){(char*)__func__, "Push back value mismatch", CS_ELEM};
    }

    free_test_struct(&ts);
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_push_back_multiple() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "Item", (double)i * 1.5);
        if (deque_push_back(dq, &ts) != CS_SUCCESS) {
            free_test_struct(&ts);
            deque_free(dq);
            return (test_res){(char*)__func__, "Push back failed", CS_MEM};
        }
        free_test_struct(&ts);
    }

    if (deque_size(*dq) != total) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Size mismatch after push back", CS_UNKNOWN};
    }

    test_struct *back = (test_struct*)deque_back(*dq);
    if (!back || back->id != total - 1) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Back element mismatch", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_push_back_growth() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = DEQUE_BLOCK_SIZE * 3; // Force multiple block allocations

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "GrowthTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    // Verify all elements are accessible and have correct data
    for (int i = 0; i < total; i++) {
        test_struct *val = (test_struct*)deque_at(*dq, i);
        if (!val || val->id != i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Data corruption after growth", CS_ELEM};
        }
        // Verify nested data integrity
        if (!val->address || val->address->zip_code != 10000 + i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Nested address corruption", CS_ELEM};
        }
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_push_front
// ============================================================================

test_res test_deque_push_front_single() {
    deque *dq = deque_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "FrontItem", 42.0);

    cs_codes result = deque_push_front(dq, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        deque_free(dq);
        return (test_res){(char*)__func__, "Push front returned error", CS_MEM};
    }

    test_struct *front = (test_struct*)deque_front(*dq);
    if (!front || front->id != 42) {
        free_test_struct(&ts);
        deque_free(dq);
        return (test_res){(char*)__func__, "Push front value mismatch", CS_ELEM};
    }

    free_test_struct(&ts);
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_push_front_multiple() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "FrontMultiple", (double)i);
        if (deque_push_front(dq, &ts) != CS_SUCCESS) {
            free_test_struct(&ts);
            deque_free(dq);
            return (test_res){(char*)__func__, "Push front failed", CS_MEM};
        }
        free_test_struct(&ts);
    }

    if (deque_size(*dq) != total) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Size mismatch after push front", CS_UNKNOWN};
    }

    // Front should be the last pushed element
    test_struct *front = (test_struct*)deque_front(*dq);
    if (!front || front->id != total - 1) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Front element mismatch", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_push_front_growth() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = DEQUE_BLOCK_SIZE * 3;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "FrontGrowth", (double)i);
        deque_push_front(dq, &ts);
        free_test_struct(&ts);
    }

    // Values should be in reverse order
    for (int i = 0; i < total; i++) {
        test_struct *val = (test_struct*)deque_at(*dq, i);
        if (!val || val->id != total - 1 - i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Data corruption after front growth", CS_ELEM};
        }
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_insert_at
// ============================================================================

test_res test_deque_insert_at_front() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 1; i <= 5; i++) {
        test_struct ts = create_test_struct(i, "InsertTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    test_struct ts0 = create_test_struct(0, "InsertedFront", 0.0);
    cs_codes result = deque_insert_at(dq, &ts0, 0);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts0);
        deque_free(dq);
        return (test_res){(char*)__func__, "Insert at front failed", CS_MEM};
    }

    test_struct *at0 = (test_struct*)deque_at(*dq, 0);
    if (!at0 || at0->id != 0 || deque_size(*dq) != 6) {
        free_test_struct(&ts0);
        deque_free(dq);
        return (test_res){(char*)__func__, "Insert at front value mismatch", CS_POS};
    }

    free_test_struct(&ts0);
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_insert_at_middle() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "MiddleTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    test_struct ts99 = create_test_struct(99, "InsertedMiddle", 99.0);
    cs_codes result = deque_insert_at(dq, &ts99, 5);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts99);
        deque_free(dq);
        return (test_res){(char*)__func__, "Insert at middle failed", CS_MEM};
    }

    test_struct *at5 = (test_struct*)deque_at(*dq, 5);
    if (!at5 || at5->id != 99 || deque_size(*dq) != 11) {
        free_test_struct(&ts99);
        deque_free(dq);
        return (test_res){(char*)__func__, "Insert at middle value mismatch", CS_POS};
    }

    // Verify elements shifted correctly
    test_struct *at6 = (test_struct*)deque_at(*dq, 6);
    if (!at6 || at6->id != 5) {
        free_test_struct(&ts99);
        deque_free(dq);
        return (test_res){(char*)__func__, "Elements not shifted correctly", CS_ELEM};
    }

    free_test_struct(&ts99);
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_insert_at_back() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "BackInsertTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    test_struct ts99 = create_test_struct(99, "InsertedBack", 99.0);
    cs_codes result = deque_insert_at(dq, &ts99, 5);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts99);
        deque_free(dq);
        return (test_res){(char*)__func__, "Insert at back failed", CS_MEM};
    }

    test_struct *at5 = (test_struct*)deque_at(*dq, 5);
    if (!at5 || at5->id != 99 || deque_size(*dq) != 6) {
        free_test_struct(&ts99);
        deque_free(dq);
        return (test_res){(char*)__func__, "Insert at back value mismatch", CS_POS};
    }

    free_test_struct(&ts99);
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_pop_back
// ============================================================================

test_res test_deque_pop_back_single() {
    deque *dq = deque_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "PopBackTest", 42.0);
    deque_push_back(dq, &ts);
    free_test_struct(&ts);

    cs_codes result = deque_pop_back(dq);
    if (result != CS_SUCCESS) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Pop back failed", CS_EMPTY};
    }

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque not empty after pop", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_pop_back_multiple() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "PopBackMulti", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    for (int i = 9; i >= 0; i--) {
        test_struct *back = (test_struct*)deque_back(*dq);
        if (!back || back->id != i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Back value mismatch before pop", CS_ELEM};
        }
        deque_pop_back(dq);
    }

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque not empty after all pops", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_pop_back_empty() {
    deque *dq = deque_init(get_test_struct_attr());

    cs_codes result = deque_pop_back(dq);
    if (result == CS_SUCCESS) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Pop back on empty should fail", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_pop_front
// ============================================================================

test_res test_deque_pop_front_single() {
    deque *dq = deque_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "PopFrontTest", 42.0);
    deque_push_back(dq, &ts);
    free_test_struct(&ts);

    cs_codes result = deque_pop_front(dq);
    if (result != CS_SUCCESS) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Pop front failed", CS_EMPTY};
    }

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque not empty after pop", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_pop_front_multiple() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "PopFrontMulti", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    for (int i = 0; i < 10; i++) {
        test_struct *front = (test_struct*)deque_front(*dq);
        if (!front || front->id != i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Front value mismatch before pop", CS_ELEM};
        }
        deque_pop_front(dq);
    }

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque not empty after all pops", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_pop_front_empty() {
    deque *dq = deque_init(get_test_struct_attr());

    cs_codes result = deque_pop_front(dq);
    if (result == CS_SUCCESS) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Pop front on empty should fail", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_erase
// ============================================================================

test_res test_deque_erase_front() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    cs_codes result = deque_erase(dq, 0);
    if (result != CS_SUCCESS) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Erase front failed", CS_POS};
    }

    test_struct *front = (test_struct*)deque_front(*dq);
    if (deque_size(*dq) != 4 || !front || front->id != 1) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Erase front value mismatch", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_erase_middle() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseMiddle", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    cs_codes result = deque_erase(dq, 2); // Erase id=2
    if (result != CS_SUCCESS) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Erase middle failed", CS_POS};
    }

    test_struct *at2 = (test_struct*)deque_at(*dq, 2);
    if (deque_size(*dq) != 4 || !at2 || at2->id != 3) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Erase middle shift failed", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_erase_back() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseBack", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    cs_codes result = deque_erase(dq, 4); // Erase last
    if (result != CS_SUCCESS) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Erase back failed", CS_POS};
    }

    test_struct *back = (test_struct*)deque_back(*dq);
    if (deque_size(*dq) != 4 || !back || back->id != 3) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Erase back value mismatch", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_back
// ============================================================================

test_res test_deque_back() {
    deque *dq = deque_init(get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "BackTest", (double)i);
        deque_push_back(dq, &ts);
        test_struct *back = (test_struct*)deque_back(*dq);
        if (!back || back->id != i) {
            free_test_struct(&ts);
            deque_free(dq);
            return (test_res){(char*)__func__, "Back mismatch after push", CS_ELEM};
        }
        free_test_struct(&ts);
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_front
// ============================================================================

test_res test_deque_front() {
    deque *dq = deque_init(get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "FrontTest", (double)i);
        deque_push_front(dq, &ts);
        test_struct *front = (test_struct*)deque_front(*dq);
        if (!front || front->id != i) {
            free_test_struct(&ts);
            deque_free(dq);
            return (test_res){(char*)__func__, "Front mismatch after push", CS_ELEM};
        }
        free_test_struct(&ts);
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_at
// ============================================================================

test_res test_deque_at_valid() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "AtValidTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    for (int i = 0; i < 100; i++) {
        test_struct *val = (test_struct*)deque_at(*dq, i);
        if (!val || val->id != i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "At value mismatch", CS_ELEM};
        }
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_at_out_of_bounds() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "OOBTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    if (deque_at(*dq, 10) != NULL) {
        deque_free(dq);
        return (test_res){(char*)__func__, "At index 10 should return NULL", CS_POS};
    }

    if (deque_at(*dq, -1) != NULL) {
        deque_free(dq);
        return (test_res){(char*)__func__, "At index -1 should return NULL", CS_POS};
    }

    if (deque_at(*dq, 1000) != NULL) {
        deque_free(dq);
        return (test_res){(char*)__func__, "At index 1000 should return NULL", CS_POS};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_at_cross_block() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = DEQUE_BLOCK_SIZE * 3;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "CrossBlockTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    // Test at block boundaries
    test_struct *val1 = (test_struct*)deque_at(*dq, DEQUE_BLOCK_SIZE - 1);
    test_struct *val2 = (test_struct*)deque_at(*dq, DEQUE_BLOCK_SIZE);
    test_struct *val3 = (test_struct*)deque_at(*dq, DEQUE_BLOCK_SIZE * 2);

    if (!val1 || val1->id != DEQUE_BLOCK_SIZE - 1) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Cross-block access failed at boundary-1", CS_ELEM};
    }
    if (!val2 || val2->id != DEQUE_BLOCK_SIZE) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Cross-block access failed at boundary", CS_ELEM};
    }
    if (!val3 || val3->id != DEQUE_BLOCK_SIZE * 2) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Cross-block access failed at 2x boundary", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_empty
// ============================================================================

test_res test_deque_empty_initial() {
    deque *dq = deque_init(get_test_struct_attr());

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "New deque should be empty", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_empty_after_ops() {
    deque *dq = deque_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "EmptyOpsTest", 42.0);

    deque_push_back(dq, &ts);
    if (deque_empty(*dq)) {
        free_test_struct(&ts);
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque should not be empty after push", CS_UNKNOWN};
    }

    deque_pop_back(dq);
    if (!deque_empty(*dq)) {
        free_test_struct(&ts);
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque should be empty after pop", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_size
// ============================================================================

test_res test_deque_size_initial() {
    deque *dq = deque_init(get_test_struct_attr());

    if (deque_size(*dq) != 0) {
        deque_free(dq);
        return (test_res){(char*)__func__, "New deque size should be 0", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_size_after_ops() {
    deque *dq = deque_init(get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "SizeOpsTest", (double)i);
        deque_push_back(dq, &ts);
        if (deque_size(*dq) != i + 1) {
            free_test_struct(&ts);
            deque_free(dq);
            return (test_res){(char*)__func__, "Size mismatch during push", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    for (int i = 99; i >= 0; i--) {
        deque_pop_back(dq);
        if (deque_size(*dq) != i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Size mismatch during pop", CS_UNKNOWN};
        }
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_swap
// ============================================================================

test_res test_deque_swap() {
    deque *dq1 = deque_init(get_test_struct_attr());
    deque *dq2 = deque_init(get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "Swap1", (double)i);
        deque_push_back(dq1, &ts);
        free_test_struct(&ts);
    }
    for (int i = 10; i < 15; i++) {
        test_struct ts = create_test_struct(i, "Swap2", (double)i);
        deque_push_back(dq2, &ts);
        free_test_struct(&ts);
    }

    deque_swap(dq1, dq2);

    test_struct *front1 = (test_struct*)deque_front(*dq1);
    test_struct *back1 = (test_struct*)deque_back(*dq1);
    if (!front1 || front1->id != 10 || !back1 || back1->id != 14) {
        deque_free(dq1);
        deque_free(dq2);
        return (test_res){(char*)__func__, "Swap dq1 content mismatch", CS_ELEM};
    }

    test_struct *front2 = (test_struct*)deque_front(*dq2);
    test_struct *back2 = (test_struct*)deque_back(*dq2);
    if (!front2 || front2->id != 0 || !back2 || back2->id != 4) {
        deque_free(dq1);
        deque_free(dq2);
        return (test_res){(char*)__func__, "Swap dq2 content mismatch", CS_ELEM};
    }

    deque_free(dq1);
    deque_free(dq2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_swap_empty() {
    deque *dq1 = deque_init(get_test_struct_attr());
    deque *dq2 = deque_init(get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "SwapEmpty", (double)i);
        deque_push_back(dq1, &ts);
        free_test_struct(&ts);
    }

    deque_swap(dq1, dq2);

    if (!deque_empty(*dq1)) {
        deque_free(dq1);
        deque_free(dq2);
        return (test_res){(char*)__func__, "dq1 should be empty after swap", CS_UNKNOWN};
    }

    if (deque_size(*dq2) != 5) {
        deque_free(dq1);
        deque_free(dq2);
        return (test_res){(char*)__func__, "dq2 should have 5 elements", CS_UNKNOWN};
    }

    deque_free(dq1);
    deque_free(dq2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// deque_clear
// ============================================================================

test_res test_deque_clear() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    deque_clear(dq);

    if (deque_size(*dq) != 0 || !deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Clear did not reset deque", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_clear_reuse() {
    deque *dq = deque_init(get_test_struct_attr());
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearReuse", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    deque_clear(dq);

    // Verify can reuse after clear
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    test_struct *front = (test_struct*)deque_front(*dq);
    if (deque_size(*dq) != 50 || !front || front->id != 100) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_deque_nested_data_integrity() {
    deque *dq = deque_init(get_test_struct_attr());

    // Push elements with complex nested data
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    // Verify all nested structures are intact
    for (int i = 0; i < 50; i++) {
        test_struct *val = (test_struct*)deque_at(*dq, i);
        if (!val) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Element is NULL", CS_ELEM};
        }

        // Check address
        if (!val->address || val->address->zip_code != 10000 + i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Address data corrupted", CS_ELEM};
        }

        // Check contacts array
        int expected_contacts = 2 + (i % 3);
        if (val->contact_count != expected_contacts || !val->contacts) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Contacts array corrupted", CS_ELEM};
        }

        // Check tags array
        int expected_tags = 3 + (i % 3);
        if (val->tag_count != expected_tags || !val->tags) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Tags array corrupted", CS_ELEM};
        }

        // Check flags
        unsigned char expected_flags = TEST_FLAG_ACTIVE;
        if (i % 2 == 0) expected_flags |= TEST_FLAG_VERIFIED;
        if (i % 5 == 0) expected_flags |= TEST_FLAG_PREMIUM;
        if (i % 10 == 0) expected_flags |= TEST_FLAG_ADMIN;
        if (val->flags != expected_flags) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Flags corrupted", CS_ELEM};
        }
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_deep_copy_verification() {
    deque *dq = deque_init(get_test_struct_attr());

    test_struct original = create_test_struct(42, "DeepCopyTest", 42.42);
    deque_push_back(dq, &original);

    // Modify original after push - should not affect deque content
    original.id = 999;
    free(original.name);
    original.name = strdup("Modified");
    original.address->zip_code = 99999;

    test_struct *stored = (test_struct*)deque_at(*dq, 0);
    if (!stored || stored->id != 42) {
        free_test_struct(&original);
        deque_free(dq);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopyTest") != 0) {
        free_test_struct(&original);
        deque_free(dq);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10042) {
        free_test_struct(&original);
        deque_free(dq);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    free_test_struct(&original);
    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Mixed operations / stress tests
// ============================================================================

test_res test_deque_push_both_sides() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = DEQUE_BLOCK_SIZE * 2;

    // Push to back
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "BothSidesBack", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }
    // Push to front with negative ids
    for (int i = 1; i <= total; i++) {
        test_struct ts = create_test_struct(-i, "BothSidesFront", (double)-i);
        deque_push_front(dq, &ts);
        free_test_struct(&ts);
    }

    if (deque_size(*dq) != total * 2) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Size mismatch after bidirectional push", CS_UNKNOWN};
    }

    test_struct *front = (test_struct*)deque_front(*dq);
    test_struct *back = (test_struct*)deque_back(*dq);

    if (!front || front->id != -total || !back || back->id != total - 1) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Data corruption at deque ends", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_alternating_ops() {
    deque *dq = deque_init(get_test_struct_attr());

    // Alternate push/pop operations
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Alternating", (double)i);
        deque_push_back(dq, &ts);
        deque_push_front(dq, &ts);
        free_test_struct(&ts);
    }

    for (int i = 0; i < 50; i++) {
        deque_pop_front(dq);
        deque_pop_back(dq);
    }

    if (deque_size(*dq) != 100) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Size mismatch after alternating ops", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_pop_all_front() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = DEQUE_BLOCK_SIZE * 2;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "PopAllFront", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    for (int i = 0; i < total; i++) {
        if (deque_pop_front(dq) != CS_SUCCESS) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Pop front failed", CS_EMPTY};
        }
    }

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque not empty after all pops", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_pop_all_back() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = DEQUE_BLOCK_SIZE * 2;
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "PopAllBack", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    for (int i = 0; i < total; i++) {
        if (deque_pop_back(dq) != CS_SUCCESS) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Pop back failed", CS_EMPTY};
        }
    }

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque not empty after all pops", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_deque_large_random_access() {
    deque *dq = deque_init(get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "LargeRandom", (double)i);
        deque_push_back(dq, &ts);
        free_test_struct(&ts);
    }

    // Random access pattern
    for (int i = 0; i < total; i += 7) {
        test_struct *val = (test_struct*)deque_at(*dq, i);
        if (!val || val->id != i) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Large random access failed", CS_ELEM};
        }
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test deque_tests[] = {
    // deque_init
    test_deque_init,
    
    // deque_push_back
    test_deque_push_back_single,
    test_deque_push_back_multiple,
    test_deque_push_back_growth,
    
    // deque_push_front
    test_deque_push_front_single,
    test_deque_push_front_multiple,
    test_deque_push_front_growth,
    
    // deque_insert_at
    test_deque_insert_at_front,
    test_deque_insert_at_middle,
    test_deque_insert_at_back,
    
    // deque_pop_back
    test_deque_pop_back_single,
    test_deque_pop_back_multiple,
    test_deque_pop_back_empty,
    
    // deque_pop_front
    test_deque_pop_front_single,
    test_deque_pop_front_multiple,
    test_deque_pop_front_empty,
    
    // deque_erase
    test_deque_erase_front,
    test_deque_erase_middle,
    test_deque_erase_back,
    
    // deque_back
    test_deque_back,
    
    // deque_front
    test_deque_front,
    
    // deque_at
    test_deque_at_valid,
    test_deque_at_out_of_bounds,
    test_deque_at_cross_block,
    
    // deque_empty
    test_deque_empty_initial,
    test_deque_empty_after_ops,
    
    // deque_size
    test_deque_size_initial,
    test_deque_size_after_ops,
    
    // deque_swap
    test_deque_swap,
    test_deque_swap_empty,
    
    // deque_clear
    test_deque_clear,
    test_deque_clear_reuse,
    
    // Complex struct integrity
    test_deque_nested_data_integrity,
    test_deque_deep_copy_verification,
    
    // Mixed/stress tests
    test_deque_push_both_sides,
    test_deque_alternating_ops,
    test_deque_pop_all_front,
    test_deque_pop_all_back,
    test_deque_large_random_access
};
