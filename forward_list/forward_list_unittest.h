#include <cs/forward_list.h>
#include <unittest.h>

// ============================================================================
// forward_list_init
// ============================================================================

test_res test_forward_list_init() {
    forward_list_attr_t attr = get_test_struct_attr();
    forward_list *fl = forward_list_init(attr);

    if (!fl) return (test_res){(char*)__func__, "Init returned NULL", CS_MEM};
    if (forward_list_size(*fl) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    if (!forward_list_empty(*fl)) return (test_res){(char*)__func__, "List not empty after init", CS_UNKNOWN};
    if (fl->head != NULL) return (test_res){(char*)__func__, "Head should be NULL", CS_UNKNOWN};

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// forward_list_push_front
// ============================================================================

test_res test_forward_list_push_front_single() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "FrontItem", 42.0);

    cs_codes result = forward_list_push_front(fl, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Push front returned error", CS_MEM};
    }

    test_struct *front = (test_struct*)forward_list_front(*fl);
    if (!front || front->id != 42 || strcmp(front->name, "FrontItem") != 0) {
        free_test_struct(&ts);
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Push front value mismatch", CS_ELEM};
    }

    free_test_struct(&ts);
    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_push_front_multiple() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "FrontMultiple", (double)i);
        if (forward_list_push_front(fl, &ts) != CS_SUCCESS) {
            free_test_struct(&ts);
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Push front failed", CS_MEM};
        }
        free_test_struct(&ts);
    }

    if (forward_list_size(*fl) != total) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Size mismatch after push front", CS_UNKNOWN};
    }

    // Front should be the last pushed element
    test_struct *front = (test_struct*)forward_list_front(*fl);
    if (!front || front->id != total - 1) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Front element mismatch", CS_ELEM};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_push_front_order() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    // Push 0, 1, 2, 3, 4 to front - should result in 4, 3, 2, 1, 0
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "OrderTest", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    // Traverse and verify order
    forward_list_node *curr = fl->head;
    for (int i = 4; i >= 0; i--) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != i) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Push front order incorrect", CS_ELEM};
        }
        curr = curr->next;
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// forward_list_pop_front
// ============================================================================

test_res test_forward_list_pop_front_single() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "PopFrontTest", 42.0);
    forward_list_push_front(fl, &ts);
    free_test_struct(&ts);

    cs_codes result = forward_list_pop_front(fl);
    if (result != CS_SUCCESS) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Pop front failed", CS_EMPTY};
    }

    if (!forward_list_empty(*fl)) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "List not empty after pop", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_pop_front_multiple() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    
    // Push in reverse order so we pop in ascending order
    for (int i = 9; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "PopFrontMulti", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    for (int i = 0; i < 10; i++) {
        test_struct *front = (test_struct*)forward_list_front(*fl);
        if (!front || front->id != i) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Front value mismatch before pop", CS_ELEM};
        }
        forward_list_pop_front(fl);
    }

    if (!forward_list_empty(*fl)) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "List not empty after all pops", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_pop_front_empty() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    cs_codes result = forward_list_pop_front(fl);
    if (result == CS_SUCCESS) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Pop front on empty should fail", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// forward_list_front
// ============================================================================

test_res test_forward_list_front() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "FrontTest", (double)i);
        forward_list_push_front(fl, &ts);
        test_struct *front = (test_struct*)forward_list_front(*fl);
        if (!front || front->id != i) {
            free_test_struct(&ts);
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Front mismatch after push", CS_ELEM};
        }
        free_test_struct(&ts);
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_front_empty() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    void *front = forward_list_front(*fl);
    if (front != NULL) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Front on empty should return NULL", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// forward_list_empty
// ============================================================================

test_res test_forward_list_empty_initial() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    if (!forward_list_empty(*fl)) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "New list should be empty", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_empty_after_ops() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "EmptyOpsTest", 42.0);

    forward_list_push_front(fl, &ts);
    if (forward_list_empty(*fl)) {
        free_test_struct(&ts);
        forward_list_free(fl);
        return (test_res){(char*)__func__, "List should not be empty after push", CS_UNKNOWN};
    }

    forward_list_pop_front(fl);
    if (!forward_list_empty(*fl)) {
        free_test_struct(&ts);
        forward_list_free(fl);
        return (test_res){(char*)__func__, "List should be empty after pop", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// forward_list_size
// ============================================================================

test_res test_forward_list_size_initial() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    if (forward_list_size(*fl) != 0) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "New list size should be 0", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_size_after_ops() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "SizeOpsTest", (double)i);
        forward_list_push_front(fl, &ts);
        if (forward_list_size(*fl) != i + 1) {
            free_test_struct(&ts);
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Size mismatch during push", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    for (int i = 99; i >= 0; i--) {
        forward_list_pop_front(fl);
        if (forward_list_size(*fl) != i) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Size mismatch during pop", CS_UNKNOWN};
        }
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// forward_list_swap
// ============================================================================

test_res test_forward_list_swap() {
    forward_list *fl1 = forward_list_init(get_test_struct_attr());
    forward_list *fl2 = forward_list_init(get_test_struct_attr());

    // Push to fl1: 4, 3, 2, 1, 0 (front is 4)
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "Swap1", (double)i);
        forward_list_push_front(fl1, &ts);
        free_test_struct(&ts);
    }
    // Push to fl2: 14, 13, 12, 11, 10 (front is 14)
    for (int i = 10; i < 15; i++) {
        test_struct ts = create_test_struct(i, "Swap2", (double)i);
        forward_list_push_front(fl2, &ts);
        free_test_struct(&ts);
    }

    forward_list_swap(fl1, fl2);

    test_struct *front1 = (test_struct*)forward_list_front(*fl1);
    if (!front1 || front1->id != 14) {
        forward_list_free(fl1);
        forward_list_free(fl2);
        return (test_res){(char*)__func__, "Swap fl1 front mismatch", CS_ELEM};
    }

    test_struct *front2 = (test_struct*)forward_list_front(*fl2);
    if (!front2 || front2->id != 4) {
        forward_list_free(fl1);
        forward_list_free(fl2);
        return (test_res){(char*)__func__, "Swap fl2 front mismatch", CS_ELEM};
    }

    forward_list_free(fl1);
    forward_list_free(fl2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_swap_empty() {
    forward_list *fl1 = forward_list_init(get_test_struct_attr());
    forward_list *fl2 = forward_list_init(get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "SwapEmpty", (double)i);
        forward_list_push_front(fl1, &ts);
        free_test_struct(&ts);
    }

    forward_list_swap(fl1, fl2);

    if (!forward_list_empty(*fl1)) {
        forward_list_free(fl1);
        forward_list_free(fl2);
        return (test_res){(char*)__func__, "fl1 should be empty after swap", CS_UNKNOWN};
    }

    if (forward_list_size(*fl2) != 5) {
        forward_list_free(fl1);
        forward_list_free(fl2);
        return (test_res){(char*)__func__, "fl2 should have 5 elements", CS_UNKNOWN};
    }

    forward_list_free(fl1);
    forward_list_free(fl2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// forward_list_clear
// ============================================================================

test_res test_forward_list_clear() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    forward_list_clear(fl);

    if (forward_list_size(*fl) != 0 || !forward_list_empty(*fl)) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Clear did not reset list", CS_UNKNOWN};
    }

    if (fl->head != NULL) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Head should be NULL after clear", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_clear_reuse() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearReuse", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    forward_list_clear(fl);

    // Verify can reuse after clear
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    test_struct *front = (test_struct*)forward_list_front(*fl);
    if (forward_list_size(*fl) != 50 || !front || front->id != 149) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_ELEM};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Traversal tests
// ============================================================================

test_res test_forward_list_traverse() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    // Push in reverse so traversal gives ascending order
    for (int i = 9; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "TraverseTest", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    // Traverse forward
    forward_list_node *curr = fl->head;
    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != i) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Forward traverse failed", CS_ELEM};
        }
        curr = curr->next;
    }

    // Should be at end
    if (curr != NULL) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Traverse did not end at NULL", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_forward_list_nested_data_integrity() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    // Verify all nested structures are intact (in reverse order since push_front)
    forward_list_node *curr = fl->head;
    for (int i = 49; i >= 0; i--) {
        test_struct *val = (test_struct*)curr->data;
        if (!val) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Element is NULL", CS_ELEM};
        }

        // Check address
        if (!val->address || val->address->zip_code != 10000 + i) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Address data corrupted", CS_ELEM};
        }

        // Check contacts array
        int expected_contacts = 2 + (i % 3);
        if (val->contact_count != expected_contacts || !val->contacts) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Contacts array corrupted", CS_ELEM};
        }

        // Check tags array
        int expected_tags = 3 + (i % 3);
        if (val->tag_count != expected_tags || !val->tags) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Tags array corrupted", CS_ELEM};
        }

        // Check flags
        unsigned char expected_flags = TEST_FLAG_ACTIVE;
        if (i % 2 == 0) expected_flags |= TEST_FLAG_VERIFIED;
        if (i % 5 == 0) expected_flags |= TEST_FLAG_PREMIUM;
        if (i % 10 == 0) expected_flags |= TEST_FLAG_ADMIN;
        if (val->flags != expected_flags) {
            forward_list_free(fl);
            return (test_res){(char*)__func__, "Flags corrupted", CS_ELEM};
        }

        curr = curr->next;
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_deep_copy_verification() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    test_struct original = create_test_struct(42, "DeepCopyTest", 42.42);
    forward_list_push_front(fl, &original);

    // Modify original after push - should not affect list content
    original.id = 999;
    free(original.name);
    original.name = strdup("Modified");
    original.address->zip_code = 99999;

    test_struct *stored = (test_struct*)forward_list_front(*fl);
    if (!stored || stored->id != 42) {
        free_test_struct(&original);
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopyTest") != 0) {
        free_test_struct(&original);
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10042) {
        free_test_struct(&original);
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    free_test_struct(&original);
    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests
// ============================================================================

test_res test_forward_list_large_dataset() {
    forward_list *fl = forward_list_init(get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "LargeDataset", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    if (forward_list_size(*fl) != total) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Large dataset size mismatch", CS_UNKNOWN};
    }

    // Front should be last pushed
    test_struct *front = (test_struct*)forward_list_front(*fl);
    if (!front || front->id != total - 1) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Large dataset front mismatch", CS_ELEM};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_push_pop_cycle() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    // Push and pop in cycles
    for (int cycle = 0; cycle < 10; cycle++) {
        // Push 10 elements
        for (int i = 0; i < 10; i++) {
            test_struct ts = create_test_struct(cycle * 10 + i, "Cycle", (double)i);
            forward_list_push_front(fl, &ts);
            free_test_struct(&ts);
        }

        // Pop 5 elements
        for (int i = 0; i < 5; i++) {
            forward_list_pop_front(fl);
        }
    }

    // Should have 50 elements left (10 cycles * 5 remaining per cycle)
    if (forward_list_size(*fl) != 50) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Push/pop cycle size mismatch", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_pop_all() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "PopAll", (double)i);
        forward_list_push_front(fl, &ts);
        free_test_struct(&ts);
    }

    // Pop all elements
    while (!forward_list_empty(*fl)) {
        forward_list_pop_front(fl);
    }

    if (!forward_list_empty(*fl) || fl->head != NULL) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "List not empty after popping all", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_forward_list_alternating_ops() {
    forward_list *fl = forward_list_init(get_test_struct_attr());

    // Alternating push and pop
    for (int i = 0; i < 50; i++) {
        test_struct ts1 = create_test_struct(i * 2, "Alt1", (double)i);
        test_struct ts2 = create_test_struct(i * 2 + 1, "Alt2", (double)i);
        forward_list_push_front(fl, &ts1);
        forward_list_push_front(fl, &ts2);
        free_test_struct(&ts1);
        free_test_struct(&ts2);

        if (i % 3 == 0) {
            forward_list_pop_front(fl);
        }
    }

    // 100 pushes - 17 pops = 83 elements
    if (forward_list_size(*fl) != 83) {
        forward_list_free(fl);
        return (test_res){(char*)__func__, "Alternating ops size mismatch", CS_UNKNOWN};
    }

    forward_list_free(fl);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test forward_list_tests[] = {
    // forward_list_init
    test_forward_list_init,
    
    // forward_list_push_front
    test_forward_list_push_front_single,
    test_forward_list_push_front_multiple,
    test_forward_list_push_front_order,
    
    // forward_list_pop_front
    test_forward_list_pop_front_single,
    test_forward_list_pop_front_multiple,
    test_forward_list_pop_front_empty,
    
    // forward_list_front
    test_forward_list_front,
    test_forward_list_front_empty,
    
    // forward_list_empty
    test_forward_list_empty_initial,
    test_forward_list_empty_after_ops,
    
    // forward_list_size
    test_forward_list_size_initial,
    test_forward_list_size_after_ops,
    
    // forward_list_swap
    test_forward_list_swap,
    test_forward_list_swap_empty,
    
    // forward_list_clear
    test_forward_list_clear,
    test_forward_list_clear_reuse,
    
    // Traversal
    test_forward_list_traverse,
    
    // Complex struct integrity
    test_forward_list_nested_data_integrity,
    test_forward_list_deep_copy_verification,
    
    // Stress tests
    test_forward_list_large_dataset,
    test_forward_list_push_pop_cycle,
    test_forward_list_pop_all,
    test_forward_list_alternating_ops
};