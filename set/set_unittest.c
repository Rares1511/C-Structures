#include <cs/set.h>
#include <cs/rbt.h>
#include "../include/unittest.h"

// Required by unittest.h
FILE *__DEBUG_OUT = NULL;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       START OF HELPER FUNCTIONS SECTION                                    ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

int rbt_check_black_height(rbt_node *node) {
    if (node == NULL) {
        return 1;
    }

    int left_bh  = rbt_check_black_height(node->left);
    int right_bh = rbt_check_black_height(node->right);

    if (left_bh == -1 || right_bh == -1)
        return -1;

    if (left_bh != right_bh)
        return -1;

    if (node->color == __RBT_NODE_RED_COLOR) {
        if ((node->left && node->left->color == __RBT_NODE_RED_COLOR) ||
            (node->right && node->right->color == __RBT_NODE_RED_COLOR)) {
            return -1;
        }
    }

    return left_bh + (node->color == __RBT_NODE_BLACK_COLOR ? 1 : 0);
}

int rbt_check_bst(rbt_node *node, rbt_attr_t attr, void *min_key, void *max_key) {
    if (!node) return 1;

    if (min_key) {
        if (attr.comp && attr.comp(node->data, min_key) <= 0)
            return 0;
    }

    if (max_key) {
        if (attr.comp && attr.comp(node->data, max_key) >= 0)
            return 0;
    }

    if (!rbt_check_bst(node->left, attr, min_key, node->data))
        return 0;

    if (!rbt_check_bst(node->right, attr, node->data, max_key))
        return 0;

    return 1;
}

int rbt_is_valid(rbt *t) {
    if (t->root && t->root->color != __RBT_NODE_BLACK_COLOR)
        return 0;

    if (!rbt_check_bst(t->root, t->attr, NULL, NULL))
        return 0;

    int bh = rbt_check_black_height(t->root);
    if (bh == -1)
        return 0;

    return 1;
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

// ============================================================================
// set_init
// ============================================================================

test_res test_set_init() {
    set_attr_t attr = get_test_struct_attr();
    set *s = set_init(attr);

    if (!s) return (test_res){(char*)__func__, "Init returned NULL", CS_MEM};
    if (set_size(*s) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    if (!set_empty(*s)) return (test_res){(char*)__func__, "Set not empty after init", CS_UNKNOWN};

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after init", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_insert
// ============================================================================

test_res test_set_insert_single() {
    set *s = set_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "InsertSingle", 42.0);

    cs_codes result = set_insert(s, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Insert returned error", CS_MEM};
    }

    if (set_size(*s) != 1) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Size not 1 after insert", CS_UNKNOWN};
    }

    if (!rbt_is_valid(s->t)) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after insert", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_multiple() {
    set *s = set_init(get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "InsertMultiple", (double)i);
        cs_codes result = set_insert(s, &ts);
        if (result != CS_SUCCESS) {
            free_test_struct(&ts);
            set_free(s);
            return (test_res){(char*)__func__, "Insert returned error code", result};
        }
        free_test_struct(&ts);

        // Check RBT integrity after each insert
        if (!rbt_is_valid(s->t)) {
            set_free(s);
            return (test_res){(char*)__func__, "RBT integrity violated during inserts", CS_UNKNOWN};
        }
    }

    if (set_size(*s) != total) {
        set_free(s);
        return (test_res){(char*)__func__, "Size mismatch after inserts", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_duplicate() {
    set *s = set_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "Duplicate", 42.0);

    if (set_insert(s, &ts) != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Initial insert failed", CS_MEM};
    }
    if (set_insert(s, &ts) == CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Duplicate insert was successful", CS_MEM};
    }

    // Sets should not allow duplicates - size should still be 1
    if (set_size(*s) != 1) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Duplicate was inserted", CS_ELEM};
    }

    if (!rbt_is_valid(s->t)) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after duplicate", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_ascending() {
    set *s = set_init(get_test_struct_attr());

    // Insert in ascending order (worst case for unbalanced BST)
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Ascending", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after ascending inserts", CS_UNKNOWN};
    }

    if (set_size(*s) != 100) {
        set_free(s);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_descending() {
    set *s = set_init(get_test_struct_attr());

    // Insert in descending order (worst case for unbalanced BST)
    for (int i = 99; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "Descending", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after descending inserts", CS_UNKNOWN};
    }

    if (set_size(*s) != 100) {
        set_free(s);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_delete
// ============================================================================

test_res test_set_delete_single() {
    set *s = set_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "DeleteSingle", 42.0);
    
    cs_codes ins_result = set_insert(s, &ts);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }

    cs_codes del_result = set_delete(s, &ts);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }

    if (!set_empty(*s)) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Set not empty after delete", CS_UNKNOWN};
    }

    if (!rbt_is_valid(s->t)) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after delete", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_multiple() {
    set *s = set_init(get_test_struct_attr());
    int total = 50;

    // Insert elements
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteMulti", (double)i);
        cs_codes ins_result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    // Delete all elements
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteMulti", (double)i);
        cs_codes del_result = set_delete(s, &ts);
        free_test_struct(&ts);

        if (del_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        // Check RBT integrity after each delete
        if (!rbt_is_valid(s->t)) {
            set_free(s);
            return (test_res){(char*)__func__, "RBT integrity violated during deletes", CS_UNKNOWN};
        }
    }

    if (!set_empty(*s)) {
        set_free(s);
        return (test_res){(char*)__func__, "Set not empty after all deletes", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_nonexistent() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "DeleteNonExist", (double)i);
        set_insert(s, &ts);
        free_test_struct(&ts);
    }

    test_struct ts_not_in = create_test_struct(999, "NotInSet", 999.0);
    if(set_delete(s, &ts_not_in) == CS_SUCCESS) {
        set_free(s);
        free_test_struct(&ts_not_in);
        return (test_res){(char*)__func__, "Deleted non-existent element", CS_ELEM};
    }
    free_test_struct(&ts_not_in);

    // Delete of non-existent should return error or at least not crash
    if (set_size(*s) != 10) {
        set_free(s);
        return (test_res){(char*)__func__, "Size changed after deleting non-existent", CS_UNKNOWN};
    }

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_random_order() {
    set *s = set_init(get_test_struct_attr());
    int total = 100;

    // Insert in order
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "RandomDelete", (double)i);
        cs_codes ins_result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    // Delete in "random" order (using a pattern)
    int order[] = {50, 25, 75, 10, 40, 60, 90, 5, 15, 30, 45, 55, 70, 80, 95};
    for (int i = 0; i < 15; i++) {
        test_struct ts = create_test_struct(order[i], "RandomDelete", (double)order[i]);
        cs_codes del_result = set_delete(s, &ts);
        free_test_struct(&ts);
        if (del_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(s->t)) {
            set_free(s);
            return (test_res){(char*)__func__, "RBT integrity violated during random deletes", CS_UNKNOWN};
        }
    }

    if (set_size(*s) != total - 15) {
        set_free(s);
        return (test_res){(char*)__func__, "Size mismatch after random deletes", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_find
// ============================================================================

test_res test_set_find_existing() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 20; i++) {
        test_struct ts = create_test_struct(i, "FindExisting", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    test_struct search = create_test_struct(10, "FindExisting", 10.0);
    void *found = set_find(*s, &search);
    free_test_struct(&search);

    if (found == NULL) {
        set_free(s);
        return (test_res){(char*)__func__, "Find returned NULL for existing element", CS_ELEM};
    }

    test_struct *found_ts = (test_struct*)found;
    if (found_ts->id != 10) {
        set_free(s);
        return (test_res){(char*)__func__, "Find returned wrong element", CS_ELEM};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_find_nonexistent() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 20; i++) {
        test_struct ts = create_test_struct(i, "FindNonExist", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    test_struct search = create_test_struct(999, "NotInSet", 999.0);
    void *found = set_find(*s, &search);
    free_test_struct(&search);

    if (found != NULL) {
        set_free(s);
        return (test_res){(char*)__func__, "Find should return NULL for non-existent", CS_ELEM};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_find_all() {
    set *s = set_init(get_test_struct_attr());
    int total = 100;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "FindAll", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    // Find all elements
    for (int i = 0; i < total; i++) {
        test_struct search = create_test_struct(i, "FindAll", (double)i);
        void *found = set_find(*s, &search);
        free_test_struct(&search);

        if (found == NULL) {
            set_free(s);
            return (test_res){(char*)__func__, "Failed to find existing element", CS_ELEM};
        }
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_empty
// ============================================================================

test_res test_set_empty_initial() {
    set *s = set_init(get_test_struct_attr());

    if (!set_empty(*s)) {
        set_free(s);
        return (test_res){(char*)__func__, "New set should be empty", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_empty_after_ops() {
    set *s = set_init(get_test_struct_attr());
    test_struct ts = create_test_struct(42, "EmptyOps", 42.0);

    cs_codes ins_result = set_insert(s, &ts);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }
    if (set_empty(*s)) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Set should not be empty after insert", CS_UNKNOWN};
    }

    cs_codes del_result = set_delete(s, &ts);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }
    if (!set_empty(*s)) {
        free_test_struct(&ts);
        set_free(s);
        return (test_res){(char*)__func__, "Set should be empty after delete", CS_UNKNOWN};
    }

    free_test_struct(&ts);
    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_size
// ============================================================================

test_res test_set_size_initial() {
    set *s = set_init(get_test_struct_attr());

    if (set_size(*s) != 0) {
        set_free(s);
        return (test_res){(char*)__func__, "New set size should be 0", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_size_after_ops() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "SizeOps", (double)i);
        cs_codes ins_result = set_insert(s, &ts);
        if (ins_result != CS_SUCCESS) {
            free_test_struct(&ts);
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
        if (set_size(*s) != i + 1) {
            free_test_struct(&ts);
            set_free(s);
            return (test_res){(char*)__func__, "Size mismatch during insert", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    for (int i = 49; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "SizeOps", (double)i);
        cs_codes del_result = set_delete(s, &ts);
        if (del_result != CS_SUCCESS) {
            free_test_struct(&ts);
            set_free(s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
        if (set_size(*s) != i) {
            free_test_struct(&ts);
            set_free(s);
            return (test_res){(char*)__func__, "Size mismatch during delete", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_swap
// ============================================================================

test_res test_set_swap() {
    set *s1 = set_init(get_test_struct_attr());
    set *s2 = set_init(get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "Swap1", (double)i);
        cs_codes result = set_insert(s1, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s1);
            set_free(s2);
            return (test_res){(char*)__func__, "Insert to s1 failed", result};
        }
    }
    for (int i = 10; i < 15; i++) {
        test_struct ts = create_test_struct(i, "Swap2", (double)i);
        cs_codes result = set_insert(s2, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s1);
            set_free(s2);
            return (test_res){(char*)__func__, "Insert to s2 failed", result};
        }
    }

    set_swap(s1, s2);

    // s1 should now have elements 10-14
    test_struct search1 = create_test_struct(10, "Swap2", 10.0);
    if (set_find(*s1, &search1) == NULL) {
        free_test_struct(&search1);
        set_free(s1);
        set_free(s2);
        return (test_res){(char*)__func__, "Swap s1 content mismatch", CS_ELEM};
    }
    free_test_struct(&search1);

    // s2 should now have elements 0-4
    test_struct search2 = create_test_struct(0, "Swap1", 0.0);
    if (set_find(*s2, &search2) == NULL) {
        free_test_struct(&search2);
        set_free(s1);
        set_free(s2);
        return (test_res){(char*)__func__, "Swap s2 content mismatch", CS_ELEM};
    }
    free_test_struct(&search2);

    if (!rbt_is_valid(s1->t) || !rbt_is_valid(s2->t)) {
        set_free(s1);
        set_free(s2);
        return (test_res){(char*)__func__, "RBT integrity violated after swap", CS_UNKNOWN};
    }

    set_free(s1);
    set_free(s2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_swap_empty() {
    set *s1 = set_init(get_test_struct_attr());
    set *s2 = set_init(get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "SwapEmpty", (double)i);
        cs_codes result = set_insert(s1, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s1);
            set_free(s2);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    set_swap(s1, s2);

    if (!set_empty(*s1)) {
        set_free(s1);
        set_free(s2);
        return (test_res){(char*)__func__, "s1 should be empty after swap", CS_UNKNOWN};
    }

    if (set_size(*s2) != 5) {
        set_free(s1);
        set_free(s2);
        return (test_res){(char*)__func__, "s2 should have 5 elements", CS_UNKNOWN};
    }

    set_free(s1);
    set_free(s2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_clear
// ============================================================================

test_res test_set_clear() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    set_clear(s);

    if (set_size(*s) != 0 || !set_empty(*s)) {
        set_free(s);
        return (test_res){(char*)__func__, "Clear did not reset set", CS_UNKNOWN};
    }

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after clear", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_clear_reuse() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearReuse", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert before clear failed", result};
        }
    }

    set_clear(s);

    // Verify can reuse after clear
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert after clear failed", result};
        }
    }

    if (set_size(*s) != 50) {
        set_free(s);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_UNKNOWN};
    }

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT integrity violated after reuse", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_set_nested_data_integrity() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        cs_codes result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    // Find and verify nested structures
    for (int i = 0; i < 50; i++) {
        test_struct search = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        test_struct *found = (test_struct*)set_find(*s, &search);
        free_test_struct(&search);

        if (!found) {
            set_free(s);
            return (test_res){(char*)__func__, "Element not found", CS_ELEM};
        }

        // Check address
        if (!found->address || found->address->zip_code != 10000 + i) {
            set_free(s);
            return (test_res){(char*)__func__, "Address data corrupted", CS_ELEM};
        }

        // Check contacts array
        int expected_contacts = 2 + (i % 3);
        if (found->contact_count != expected_contacts || !found->contacts) {
            set_free(s);
            return (test_res){(char*)__func__, "Contacts array corrupted", CS_ELEM};
        }

        // Check tags array
        int expected_tags = 3 + (i % 3);
        if (found->tag_count != expected_tags || !found->tags) {
            set_free(s);
            return (test_res){(char*)__func__, "Tags array corrupted", CS_ELEM};
        }
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_deep_copy_verification() {
    set *s = set_init(get_test_struct_attr());

    test_struct original = create_test_struct(42, "DeepCopyTest", 42.42);
    cs_codes result = set_insert(s, &original);
    if (result != CS_SUCCESS) {
        free_test_struct(&original);
        set_free(s);
        return (test_res){(char*)__func__, "Insert failed", result};
    }

    // Modify original after insert - should not affect set content
    original.id = 999;
    free(original.name);
    original.name = strdup("Modified");
    original.address->zip_code = 99999;

    // Search with original id
    test_struct search = create_test_struct(42, "DeepCopyTest", 42.42);
    test_struct *stored = (test_struct*)set_find(*s, &search);
    free_test_struct(&search);

    if (!stored || stored->id != 42) {
        free_test_struct(&original);
        set_free(s);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    if (strcmp(stored->name, "DeepCopyTest") != 0) {
        free_test_struct(&original);
        set_free(s);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    if (stored->address->zip_code != 10042) {
        free_test_struct(&original);
        set_free(s);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    free_test_struct(&original);
    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests with RBT integrity
// ============================================================================

test_res test_set_stress_insert_delete() {
    set *s = set_init(get_test_struct_attr());
    int total = __TEST_SIZE;

    // Insert all
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "Stress", (double)i);
        cs_codes ins_result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT invalid after stress inserts", CS_UNKNOWN};
    }

    // Delete half
    for (int i = 0; i < total / 2; i++) {
        test_struct ts = create_test_struct(i, "Stress", (double)i);
        cs_codes del_result = set_delete(s, &ts);
        free_test_struct(&ts);
        if (del_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
    }

    if (!rbt_is_valid(s->t)) {
        set_free(s);
        return (test_res){(char*)__func__, "RBT invalid after stress deletes", CS_UNKNOWN};
    }

    if (set_size(*s) != total - total / 2) {
        set_free(s);
        return (test_res){(char*)__func__, "Size mismatch after stress test", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_interleaved_insert_delete() {
    set *s = set_init(get_test_struct_attr());

    // Interleave inserts and deletes
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Interleaved", (double)i);
        cs_codes ins_result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }

        if (i % 3 == 0 && i > 0) {
            test_struct del = create_test_struct(i - 1, "Interleaved", (double)(i - 1));
            cs_codes del_result = set_delete(s, &del);
            free_test_struct(&del);
            if (del_result != CS_SUCCESS) {
                set_free(s);
                return (test_res){(char*)__func__, "Delete failed", del_result};
            }
        }

        if (!rbt_is_valid(s->t)) {
            set_free(s);
            return (test_res){(char*)__func__, "RBT invalid during interleaved ops", CS_UNKNOWN};
        }
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_all_verify_rbt() {
    set *s = set_init(get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "DeleteAll", (double)i);
        cs_codes ins_result = set_insert(s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    // Delete all, checking RBT after each
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "DeleteAll", (double)i);
        cs_codes del_result = set_delete(s, &ts);
        free_test_struct(&ts);
        if (del_result != CS_SUCCESS) {
            set_free(s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(s->t)) {
            set_free(s);
            return (test_res){(char*)__func__, "RBT invalid during sequential deletes", CS_UNKNOWN};
        }
    }

    if (!set_empty(*s)) {
        set_free(s);
        return (test_res){(char*)__func__, "Set not empty after deleting all", CS_UNKNOWN};
    }

    set_free(s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// MAIN RUNNER
int main(int argc, char **argv) {
    test tests[] = {
        // set_init
        test_set_init,
        
        // set_insert
        test_set_insert_single,
        test_set_insert_multiple,
        test_set_insert_duplicate,
        test_set_insert_ascending,
        test_set_insert_descending,
        
        // set_delete
        test_set_delete_single,
        test_set_delete_multiple,
        test_set_delete_nonexistent,
        test_set_delete_random_order,
        
        // set_find
        test_set_find_existing,
        test_set_find_nonexistent,
        test_set_find_all,
        
        // set_empty
        test_set_empty_initial,
        test_set_empty_after_ops,
        
        // set_size
        test_set_size_initial,
        test_set_size_after_ops,
        
        // set_swap
        test_set_swap,
        test_set_swap_empty,
        
        // set_clear
        test_set_clear,
        test_set_clear_reuse,
        
        // Complex struct integrity
        test_set_nested_data_integrity,
        test_set_deep_copy_verification,
        
        // Stress tests with RBT integrity
        test_set_stress_insert_delete,
        test_set_interleaved_insert_delete,
        test_set_delete_all_verify_rbt
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}