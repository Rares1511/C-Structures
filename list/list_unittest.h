#include <cs/list.h>
#include <unittest.h>

#include <sys/time.h>
#include <valgrind/valgrind.h>

// ============================================================================
// list_init
// ============================================================================

test_res test_list_init(test_arg *arg) {
    elem_attr_t attr = get_test_struct_attr();
    list l;
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing list with element size: %zu\n", (size_t)attr.size);
    cs_codes init_result = list_init(&l, attr);

    if (init_result != CS_SUCCESS) return (test_res){(char*)__func__, "Init failed", CS_MEM};
    clogger_log(*arg->logger, CLOGGER_DEBUG, "List initialized, size: %d, empty: %d\n", list_size(l), list_empty(l));
    if (list_size(l) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    if (!list_empty(l)) return (test_res){(char*)__func__, "List not empty after init", CS_UNKNOWN};
    if (l.front != NULL) return (test_res){(char*)__func__, "Front should be NULL", CS_UNKNOWN};

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_push_front
// ============================================================================

test_res test_list_push_front_single(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "FrontItem", 42.0);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Pushing front single element with id: %d\n", ts.id);
    cs_codes result = list_push_front(&l, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        list_free(&l);
        return (test_res){(char*)__func__, "Push front returned error", CS_MEM};
    }

    test_struct *front = (test_struct*)list_front(l);
    if (!front || front->id != 42 || strcmp(front->name, "FrontItem") != 0) {
        free_test_struct(&ts);
        list_free(&l);
        return (test_res){(char*)__func__, "Push front value mismatch", CS_ELEM};
    }

    free_test_struct(&ts);
    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_push_front_multiple(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    int total = __TEST_SIZE;

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Pushing front %d elements\n", total);
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "FrontMultiple", (double)i);
        if (list_push_front(&l, &ts) != CS_SUCCESS) {
            free_test_struct(&ts);
            list_free(&l);
            return (test_res){(char*)__func__, "Push front failed", CS_MEM};
        }
        free_test_struct(&ts);
    }

    if (list_size(l) != total) {
        list_free(&l);
        return (test_res){(char*)__func__, "Size mismatch after push front", CS_UNKNOWN};
    }

    // Front should be the last pushed element
    test_struct *front = (test_struct*)list_front(l);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Front element after %d push_fronts: id=%d\n", total, front ? front->id : -1);
    if (!front || front->id != total - 1) {
        list_free(&l);
        return (test_res){(char*)__func__, "Front element mismatch", CS_ELEM};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_push_back
// ============================================================================

test_res test_list_push_back_single(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "BackItem", 42.5);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Pushing back single element with id: %d\n", ts.id);
    cs_codes result = list_push_back(&l, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        list_free(&l);
        return (test_res){(char*)__func__, "Push back returned error", CS_MEM};
    }

    test_struct *back = (test_struct*)list_back(l);
    if (!back || back->id != 42 || strcmp(back->name, "BackItem") != 0) {
        free_test_struct(&ts);
        list_free(&l);
        return (test_res){(char*)__func__, "Push back value mismatch", CS_ELEM};
    }

    free_test_struct(&ts);
    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_push_back_multiple(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    int total = __TEST_SIZE;

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Pushing back %d elements\n", total);
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "BackMultiple", (double)i * 1.5);
        if (list_push_back(&l, &ts) != CS_SUCCESS) {
            free_test_struct(&ts);
            list_free(&l);
            return (test_res){(char*)__func__, "Push back failed", CS_MEM};
        }
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Finished pushing, size: %d\n", list_size(l));
    if (list_size(l) != total) {
        list_free(&l);
        return (test_res){(char*)__func__, "Size mismatch after push back", CS_UNKNOWN};
    }

    test_struct *back = (test_struct*)list_back(l);
    if (!back || back->id != total - 1) {
        list_free(&l);
        return (test_res){(char*)__func__, "Back element mismatch", CS_ELEM};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_pop_front
// ============================================================================

test_res test_list_pop_front_single(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "PopFrontTest", 42.0);
    list_push_back(&l, &ts);
    free_test_struct(&ts);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Popping single element from front\n");
    cs_codes result = list_pop_front(&l);
    if (result != CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Pop front failed", CS_EMPTY};
    }

    if (!list_empty(l)) {
        list_free(&l);
        return (test_res){(char*)__func__, "List not empty after pop", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_pop_front_multiple(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "PopFrontMulti", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Popping all 10 elements from front\n");
    for (int i = 0; i < 10; i++) {
        test_struct *front = (test_struct*)list_front(l);
        if (!front || front->id != i) {
            list_free(&l);
            return (test_res){(char*)__func__, "Front value mismatch before pop", CS_ELEM};
        }
        list_pop_front(&l);
    }

    if (!list_empty(l)) {
        list_free(&l);
        return (test_res){(char*)__func__, "List not empty after all pops", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_pop_front_empty(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pop_front on empty list (should fail)\n");
    cs_codes result = list_pop_front(&l);
    if (result == CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Pop front on empty should fail", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_pop_back
// ============================================================================

test_res test_list_pop_back_single(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "PopBackTest", 42.0);
    list_push_back(&l, &ts);
    free_test_struct(&ts);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Popping single element from back\n");
    cs_codes result = list_pop_back(&l);
    if (result != CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Pop back failed", CS_EMPTY};
    }

    if (!list_empty(l)) {
        list_free(&l);
        return (test_res){(char*)__func__, "List not empty after pop", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_pop_back_multiple(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "PopBackMulti", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Popping all 10 elements from back\n");
    for (int i = 9; i >= 0; i--) {
        test_struct *back = (test_struct*)list_back(l);
        if (!back || back->id != i) {
            list_free(&l);
            return (test_res){(char*)__func__, "Back value mismatch before pop", CS_ELEM};
        }
        list_pop_back(&l);
    }

    if (!list_empty(l)) {
        list_free(&l);
        return (test_res){(char*)__func__, "List not empty after all pops", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_pop_back_empty(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing pop_back on empty list (should fail)\n");
    cs_codes result = list_pop_back(&l);
    if (result == CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Pop back on empty should fail", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_erase
// ============================================================================

test_res test_list_erase_front(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseTest", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing element at front position 0\n");
    cs_codes result = list_erase(&l, 0);
    if (result != CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Erase front failed", CS_POS};
    }

    test_struct *front = (test_struct*)list_front(l);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "After erase: size=%d, front id=%d\n", list_size(l), front ? front->id : -1);
    if (list_size(l) != 4 || !front || front->id != 1) {
        list_free(&l);
        return (test_res){(char*)__func__, "Erase front value mismatch", CS_ELEM};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_erase_middle(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseMiddle", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing element at middle position 2\n");
    cs_codes result = list_erase(&l, 2); // Erase id=2
    if (result != CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Erase middle failed", CS_POS};
    }

    // Traverse to check order: 0, 1, 3, 4
    list_node *curr = l.front;
    int expected[] = {0, 1, 3, 4};
    for (int i = 0; i < 4; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != expected[i]) {
            list_free(&l);
            return (test_res){(char*)__func__, "Erase middle order incorrect", CS_ELEM};
        }
        curr = curr->next;
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_erase_back(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseBack", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing element at back position 4\n");
    cs_codes result = list_erase(&l, 4); // Erase last
    if (result != CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Erase back failed", CS_POS};
    }

    test_struct *back = (test_struct*)list_back(l);
    if (list_size(l) != 4 || !back || back->id != 3) {
        list_free(&l);
        return (test_res){(char*)__func__, "Erase back value mismatch", CS_ELEM};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_erase_invalid(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "EraseInvalid", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Testing erase at invalid position 10 (should fail)\n");
    cs_codes result = list_erase(&l, 10);
    if (result == CS_SUCCESS) {
        list_free(&l);
        return (test_res){(char*)__func__, "Erase at invalid pos should fail", CS_POS};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_front
// ============================================================================

test_res test_list_front(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verifying list_front after each of 10 push_fronts\n");
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "FrontTest", (double)i);
        list_push_front(&l, &ts);
        test_struct *front = (test_struct*)list_front(l);
        if (!front || front->id != i) {
            free_test_struct(&ts);
            list_free(&l);
            return (test_res){(char*)__func__, "Front mismatch after push", CS_ELEM};
        }
        free_test_struct(&ts);
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_back
// ============================================================================

test_res test_list_back(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verifying list_back after each of 10 push_backs\n");
    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "BackTest", (double)i);
        list_push_back(&l, &ts);
        test_struct *back = (test_struct*)list_back(l);
        if (!back || back->id != i) {
            free_test_struct(&ts);
            list_free(&l);
            return (test_res){(char*)__func__, "Back mismatch after push", CS_ELEM};
        }
        free_test_struct(&ts);
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_empty
// ============================================================================

test_res test_list_empty_initial(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Checking empty on freshly initialized list\n");
    if (!list_empty(l)) {
        list_free(&l);
        return (test_res){(char*)__func__, "New list should be empty", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_empty_after_ops(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "EmptyOpsTest", 42.0);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Checking empty transitions: push then pop\n");
    list_push_back(&l, &ts);
    if (list_empty(l)) {
        free_test_struct(&ts);
        list_free(&l);
        return (test_res){(char*)__func__, "List should not be empty after push", CS_UNKNOWN};
    }

    list_pop_back(&l);
    if (!list_empty(l)) {
        free_test_struct(&ts);
        list_free(&l);
        return (test_res){(char*)__func__, "List should be empty after pop", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_size
// ============================================================================

test_res test_list_size_initial(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Checking size on freshly initialized list: %d\n", list_size(l));
    if (list_size(l) != 0) {
        list_free(&l);
        return (test_res){(char*)__func__, "New list size should be 0", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_size_after_ops(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verifying size consistency during 100 pushes and 100 pops\n");
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "SizeOpsTest", (double)i);
        list_push_back(&l, &ts);
        if (list_size(l) != i + 1) {
            free_test_struct(&ts);
            list_free(&l);
            return (test_res){(char*)__func__, "Size mismatch during push", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    for (int i = 99; i >= 0; i--) {
        list_pop_back(&l);
        if (list_size(l) != i) {
            list_free(&l);
            return (test_res){(char*)__func__, "Size mismatch during pop", CS_UNKNOWN};
        }
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_swap
// ============================================================================

test_res test_list_swap(test_arg *arg) {
    list l1;
    list_init(&l1, get_test_struct_attr());
    list l2;
    list_init(&l2, get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "Swap1", (double)i);
        list_push_back(&l1, &ts);
        free_test_struct(&ts);
    }
    for (int i = 10; i < 15; i++) {
        test_struct ts = create_test_struct(i, "Swap2", (double)i);
        list_push_back(&l2, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Swapping l1 (size=%d) with l2 (size=%d)\n", list_size(l1), list_size(l2));
    list_swap(&l1, &l2);

    test_struct *front1 = (test_struct*)list_front(l1);
    test_struct *back1 = (test_struct*)list_back(l1);
    if (!front1 || front1->id != 10 || !back1 || back1->id != 14) {
        list_free(&l1);
        list_free(&l2);
        return (test_res){(char*)__func__, "Swap l1 content mismatch", CS_ELEM};
    }

    test_struct *front2 = (test_struct*)list_front(l2);
    test_struct *back2 = (test_struct*)list_back(l2);
    if (!front2 || front2->id != 0 || !back2 || back2->id != 4) {
        list_free(&l1);
        list_free(&l2);
        return (test_res){(char*)__func__, "Swap l2 content mismatch", CS_ELEM};
    }

    list_free(&l1);
    list_free(&l2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_swap_empty(test_arg *arg) {
    list l1;
    list_init(&l1, get_test_struct_attr());
    list l2;
    list_init(&l2, get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "SwapEmpty", (double)i);
        list_push_back(&l1, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Swapping populated l1 (size=%d) with empty l2\n", list_size(l1));
    list_swap(&l1, &l2);

    if (!list_empty(l1)) {
        list_free(&l1);
        list_free(&l2);
        return (test_res){(char*)__func__, "l1 should be empty after swap", CS_UNKNOWN};
    }

    if (list_size(l2) != 5) {
        list_free(&l1);
        list_free(&l2);
        return (test_res){(char*)__func__, "l2 should have 5 elements", CS_UNKNOWN};
    }

    list_free(&l1);
    list_free(&l2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_clear
// ============================================================================

test_res test_list_clear(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Clearing list with %d elements\n", list_size(l));
    list_clear(&l);

    if (list_size(l) != 0 || !list_empty(l)) {
        list_free(&l);
        return (test_res){(char*)__func__, "Clear did not reset list", CS_UNKNOWN};
    }

    if (l.front != NULL) {
        list_free(&l);
        return (test_res){(char*)__func__, "Front should be NULL after clear", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_clear_reuse(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearReuse", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Clearing list with %d elements then reusing\n", list_size(l));
    list_clear(&l);

    // Verify can reuse after clear
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    test_struct *front = (test_struct*)list_front(l);
    if (list_size(l) != 50 || !front || front->id != 100) {
        list_free(&l);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_ELEM};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// list_sort
// ============================================================================

test_res test_list_sort_ascending(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    int ids[] = {50, 10, 40, 20, 30};

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(ids[i], "SortTest", (double)ids[i]);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting 5 elements in ascending order\n");
    list_sort(&l);

    // Should be sorted by id: 10, 20, 30, 40, 50
    int expected[] = {10, 20, 30, 40, 50};
    list_node *curr = l.front;
    for (int i = 0; i < 5; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != expected[i]) {
            list_free(&l);
            return (test_res){(char*)__func__, "Sort order incorrect", CS_ELEM};
        }
        curr = curr->next;
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_sort_already_sorted(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "AlreadySorted", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting already-sorted list of 10 elements\n");
    list_sort(&l);

    list_node *curr = l.front;
    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != i) {
            list_free(&l);
            return (test_res){(char*)__func__, "Sort corrupted already sorted data", CS_ELEM};
        }
        curr = curr->next;
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_sort_reverse(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    for (int i = 9; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "ReverseSort", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting reverse-ordered list of 10 elements\n");
    list_sort(&l);

    list_node *curr = l.front;
    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != i) {
            list_free(&l);
            return (test_res){(char*)__func__, "Sort from reverse failed", CS_ELEM};
        }
        curr = curr->next;
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_sort_by_score(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr_by_score());
    double scores[] = {50.5, 10.1, 40.4, 20.2, 30.3};

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "ScoreSort", scores[i]);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Sorting 5 elements by score\n");
    list_sort(&l);

    // Should be sorted by score: 10.1, 20.2, 30.3, 40.4, 50.5
    double expected[] = {10.1, 20.2, 30.3, 40.4, 50.5};
    list_node *curr = l.front;
    for (int i = 0; i < 5; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->score != expected[i]) {
            list_free(&l);
            return (test_res){(char*)__func__, "Sort by score order incorrect", CS_ELEM};
        }
        curr = curr->next;
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Circularity tests (doubly linked circular list)
// ============================================================================

test_res test_list_circularity(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "CircularTest", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verifying circular links on 5-element list\n");
    // Check circular links: front->prev should be back, back->next should be front
    if (l.front->prev == NULL || l.front->prev->next != l.front) {
        list_free(&l);
        return (test_res){(char*)__func__, "Front->prev circular link broken", CS_UNKNOWN};
    }

    // back is front->prev in circular list
    list_node *back = l.front->prev;
    if (back->next != l.front) {
        list_free(&l);
        return (test_res){(char*)__func__, "Back->next circular link broken", CS_UNKNOWN};
    }

    test_struct *back_data = (test_struct*)back->data;
    if (!back_data || back_data->id != 4) {
        list_free(&l);
        return (test_res){(char*)__func__, "Back element data mismatch", CS_ELEM};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_traverse_forward(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "TraverseForward", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Traversing forward through 10 elements\n");
    // Traverse forward
    list_node *curr = l.front;
    for (int i = 0; i < 10; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != i) {
            list_free(&l);
            return (test_res){(char*)__func__, "Forward traverse failed", CS_ELEM};
        }
        curr = curr->next;
    }

    // Should wrap back to front
    if (curr != l.front) {
        list_free(&l);
        return (test_res){(char*)__func__, "Forward traverse did not wrap", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_traverse_backward(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "TraverseBackward", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Traversing backward through 10 elements\n");
    // Traverse backward from back (front->prev)
    list_node *curr = l.front->prev;
    for (int i = 9; i >= 0; i--) {
        test_struct *val = (test_struct*)curr->data;
        if (!val || val->id != i) {
            list_free(&l);
            return (test_res){(char*)__func__, "Backward traverse failed", CS_ELEM};
        }
        curr = curr->prev;
    }

    // Should wrap back to back (front->prev)
    if (curr != l.front->prev) {
        list_free(&l);
        return (test_res){(char*)__func__, "Backward traverse did not wrap", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_list_nested_data_integrity(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Verifying nested data integrity for 50 complex structs\n");
    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    // Verify all nested structures are intact
    list_node *curr = l.front;
    for (int i = 0; i < 50; i++) {
        test_struct *val = (test_struct*)curr->data;
        if (!val) {
            list_free(&l);
            return (test_res){(char*)__func__, "Element is NULL", CS_ELEM};
        }

        // Check address
        if (!val->address || val->address->zip_code != 10000 + i) {
            clogger_log(*arg->logger, CLOGGER_DEBUG, "Address corruption at index %d: addr=%p, zip=%d\n", i, (void*)val->address, val->address ? val->address->zip_code : -1);
            list_free(&l);
            return (test_res){(char*)__func__, "Address data corrupted", CS_ELEM};
        }

        // Check contacts array
        int expected_contacts = 2 + (i % 3);
        if (val->contact_count != expected_contacts || !val->contacts) {
            list_free(&l);
            return (test_res){(char*)__func__, "Contacts array corrupted", CS_ELEM};
        }

        // Check tags array
        int expected_tags = 3 + (i % 3);
        if (val->tag_count != expected_tags || !val->tags) {
            list_free(&l);
            return (test_res){(char*)__func__, "Tags array corrupted", CS_ELEM};
        }

        // Check flags
        unsigned char expected_flags = TEST_FLAG_ACTIVE;
        if (i % 2 == 0) expected_flags |= TEST_FLAG_VERIFIED;
        if (i % 5 == 0) expected_flags |= TEST_FLAG_PREMIUM;
        if (i % 10 == 0) expected_flags |= TEST_FLAG_ADMIN;
        if (val->flags != expected_flags) {
            list_free(&l);
            return (test_res){(char*)__func__, "Flags corrupted", CS_ELEM};
        }

        curr = curr->next;
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_deep_copy_verification(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    test_struct original = create_test_struct(42, "DeepCopyTest", 42.42);
    list_push_back(&l, &original);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Modifying original after push to verify deep copy isolation\n");
    // Modify original after push - should not affect list content
    original.id = 999;
    free(original.name);
    original.name = strdup("Modified");
    original.address->zip_code = 99999;

    test_struct *stored = (test_struct*)list_front(l);
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Stored id=%d (expected 42), original id=%d (modified to 999)\n", stored ? stored->id : -1, original.id);
    if (!stored || stored->id != 42) {
        free_test_struct(&original);
        list_free(&l);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopyTest") != 0) {
        free_test_struct(&original);
        list_free(&l);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10042) {
        free_test_struct(&original);
        list_free(&l);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    free_test_struct(&original);
    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests
// ============================================================================

test_res test_list_alternating_push_pop(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Running alternating push/pop operations\n");
    // Alternate push front/back and pop
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Alternating", (double)i);
        if (i % 2 == 0) {
            list_push_front(&l, &ts);
        } else {
            list_push_back(&l, &ts);
        }
        free_test_struct(&ts);
    }

    for (int i = 0; i < 50; i++) {
        if (i % 2 == 0) {
            list_pop_front(&l);
        } else {
            list_pop_back(&l);
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "After alternating ops: size=%d (expected 50)\n", list_size(l));
    if (list_size(l) != 50) {
        list_free(&l);
        return (test_res){(char*)__func__, "Size mismatch after alternating ops", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_large_dataset(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());
    int total = __TEST_SIZE;

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Large dataset test with %d elements\n", total);
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "LargeDataset", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    if (list_size(l) != total) {
        list_free(&l);
        return (test_res){(char*)__func__, "Large dataset size mismatch", CS_UNKNOWN};
    }

    // Verify front and back
    test_struct *front = (test_struct*)list_front(l);
    test_struct *back = (test_struct*)list_back(l);
    if (!front || front->id != 0 || !back || back->id != total - 1) {
        list_free(&l);
        return (test_res){(char*)__func__, "Large dataset front/back mismatch", CS_ELEM};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_erase_all(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "EraseAll", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Erasing all %d elements from front\n", list_size(l));
    // Erase from front until empty
    while (!list_empty(l)) {
        list_erase(&l, 0);
    }

    if (!list_empty(l)) {
        list_free(&l);
        return (test_res){(char*)__func__, "List not empty after erasing all", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_list_mixed_operations(test_arg *arg) {
    list l;
    list_init(&l, get_test_struct_attr());

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Running mixed operations: push, pop front/back, erase\n");
    // Mixed operations
    for (int i = 0; i < 20; i++) {
        test_struct ts = create_test_struct(i, "Mixed", (double)i);
        list_push_back(&l, &ts);
        free_test_struct(&ts);
    }

    // Pop some from front
    for (int i = 0; i < 5; i++) {
        list_pop_front(&l);
    }

    // Pop some from back
    for (int i = 0; i < 5; i++) {
        list_pop_back(&l);
    }

    // Erase from middle
    list_erase(&l, 3);
    list_erase(&l, 5);

    // Size should be 20 - 5 - 5 - 2 = 8
    clogger_log(*arg->logger, CLOGGER_DEBUG, "After mixed ops: size=%d (expected 8)\n", list_size(l));
    if (list_size(l) != 8) {
        list_free(&l);
        return (test_res){(char*)__func__, "Mixed operations size mismatch", CS_UNKNOWN};
    }

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress test with time measurement
// ============================================================================
test_res test_list_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        clogger_log(*arg->logger, CLOGGER_DEBUG, "Skipping time-based stress test on Valgrind\n");
        return (test_res){(char*)__func__, "Skipped on Valgrind", CS_SUCCESS};
    }

    list l;
    struct timeval start, end;
    double elapsed;
    int total = __LIST_STRESS_TEST_SIZE, rc;
    
    rc = list_init(&l, get_int_attr());
    if (rc != 0) {
        return (test_res){(char*)__func__, "Failed to initialize list", CS_UNKNOWN};
    }

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        rc = list_push_back(&l, &i);
        if (rc != 0) {
            list_free(&l);
            return (test_res){(char*)__func__, "Failed to push element", CS_UNKNOWN};
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "insert", elapsed);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted %d elements in %.6f seconds\n", total, elapsed);

    gettimeofday(&start, NULL);
    int search_target = total / 2;
    if (list_find(l, &search_target) == -1) {
        list_free(&l);
        return (test_res){(char*)__func__, "Failed to find existing element", CS_UNKNOWN};
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "find", elapsed);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Found element %d in %.6f seconds\n", search_target, elapsed);

    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        rc = list_pop_back(&l);
        if (rc != 0) {
            list_free(&l);
            return (test_res){(char*)__func__, "Failed to pop element", CS_UNKNOWN};
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    post_operation_time(arg, "delete", elapsed);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Popped %d elements in %.6f seconds\n", total, elapsed);

    list_free(&l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test list_tests[] = {
    // list_init
    test_list_init,
    
    // list_push_front
    test_list_push_front_single,
    test_list_push_front_multiple,
    
    // list_push_back
    test_list_push_back_single,
    test_list_push_back_multiple,
    
    // list_pop_front
    test_list_pop_front_single,
    test_list_pop_front_multiple,
    test_list_pop_front_empty,
    
    // list_pop_back
    test_list_pop_back_single,
    test_list_pop_back_multiple,
    test_list_pop_back_empty,
    
    // list_erase
    test_list_erase_front,
    test_list_erase_middle,
    test_list_erase_back,
    test_list_erase_invalid,
    
    // list_front
    test_list_front,
    
    // list_back
    test_list_back,
    
    // list_empty
    test_list_empty_initial,
    test_list_empty_after_ops,
    
    // list_size
    test_list_size_initial,
    test_list_size_after_ops,
    
    // list_swap
    test_list_swap,
    test_list_swap_empty,
    
    // list_clear
    test_list_clear,
    test_list_clear_reuse,
    
    // list_sort
    test_list_sort_ascending,
    test_list_sort_already_sorted,
    test_list_sort_reverse,
    test_list_sort_by_score,
    
    // Circularity tests
    test_list_circularity,
    test_list_traverse_forward,
    test_list_traverse_backward,
    
    // Complex struct integrity
    test_list_nested_data_integrity,
    test_list_deep_copy_verification,
    
    // Stress tests
    test_list_alternating_push_pop,
    test_list_large_dataset,
    test_list_erase_all,
    test_list_mixed_operations,

    // Stress test with time
    test_list_stress_time,
};