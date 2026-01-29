#include <cs/map.h>
#include <cs/rbt.h>
#include <cs/pair.h>
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
// map_init
// ============================================================================

test_res test_map_init() {
    map_attr_t key_attr = get_test_struct_attr();
    map_attr_t val_attr = get_test_struct_attr();

    map *m = map_init(key_attr, val_attr);

    if (!m) return (test_res){(char*)__func__, "Init returned NULL", CS_MEM};
    if (map_size(*m) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    if (!map_empty(*m)) return (test_res){(char*)__func__, "Map not empty after init", CS_UNKNOWN};

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated after init", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_insert
// ============================================================================

test_res test_map_insert_single() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "Key42", 42.0);
    test_struct val = create_test_struct(100, "Val100", 100.0);

    cs_codes result = map_insert(m, &key, &val);
    if (result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Insert returned error", result};
    }

    if (map_size(*m) != 1) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Size not 1 after insert", CS_UNKNOWN};
    }

    if (!rbt_is_valid(m->t)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated after insert", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&val);
    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_multiple() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "KeyMulti", (double)i);
        test_struct val = create_test_struct(i * 10, "ValMulti", (double)(i * 10));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);

        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert returned error", result};
        }

        if (!rbt_is_valid(m->t)) {
            map_free(m);
            return (test_res){(char*)__func__, "RBT integrity violated during inserts", CS_UNKNOWN};
        }
    }

    if (map_size(*m) != total) {
        map_free(m);
        return (test_res){(char*)__func__, "Size mismatch after inserts", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_duplicate_key() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "DupKey", 42.0);
    test_struct val1 = create_test_struct(100, "Val1", 100.0);
    test_struct val2 = create_test_struct(200, "Val2", 200.0);

    cs_codes first_result = map_insert(m, &key, &val1);
    if (first_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(m);
        return (test_res){(char*)__func__, "First insert failed", first_result};
    }

    cs_codes dup_result = map_insert(m, &key, &val2);
    // Maps should not allow duplicate keys
    if (dup_result == CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(m);
        return (test_res){(char*)__func__, "Duplicate key insert should not return SUCCESS", CS_ELEM};
    }

    if (map_size(*m) != 1) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(m);
        return (test_res){(char*)__func__, "Size should be 1 after duplicate attempt", CS_UNKNOWN};
    }

    if (!rbt_is_valid(m->t)) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&val1);
    free_test_struct(&val2);
    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_ascending() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "AscKey", (double)i);
        test_struct val = create_test_struct(i * 2, "AscVal", (double)(i * 2));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated after ascending inserts", CS_UNKNOWN};
    }

    if (map_size(*m) != 100) {
        map_free(m);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_descending() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 99; i >= 0; i--) {
        test_struct key = create_test_struct(i, "DescKey", (double)i);
        test_struct val = create_test_struct(i * 3, "DescVal", (double)(i * 3));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated after descending inserts", CS_UNKNOWN};
    }

    if (map_size(*m) != 100) {
        map_free(m);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_delete
// ============================================================================

test_res test_map_delete_single() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "DelKey", 42.0);
    test_struct val = create_test_struct(100, "DelVal", 100.0);

    cs_codes ins_result = map_insert(m, &key, &val);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }

    cs_codes del_result = map_delete(m, &key);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }

    if (!map_empty(*m)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Map not empty after delete", CS_UNKNOWN};
    }

    if (!rbt_is_valid(m->t)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated after delete", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&val);
    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_multiple() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    int total = 50;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "DelMultiKey", (double)i);
        test_struct val = create_test_struct(i * 5, "DelMultiVal", (double)(i * 5));
        cs_codes ins_result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "DelMultiKey", (double)i);
        cs_codes del_result = map_delete(m, &key);
        free_test_struct(&key);

        if (del_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(m->t)) {
            map_free(m);
            return (test_res){(char*)__func__, "RBT integrity violated during deletes", CS_UNKNOWN};
        }
    }

    if (!map_empty(*m)) {
        map_free(m);
        return (test_res){(char*)__func__, "Map not empty after all deletes", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_nonexistent() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct key = create_test_struct(i, "NonExistKey", (double)i);
        test_struct val = create_test_struct(i * 2, "NonExistVal", (double)(i * 2));
        cs_codes ins_result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    test_struct key_not_in = create_test_struct(999, "NotInMap", 999.0);
    cs_codes del_result = map_delete(m, &key_not_in);
    free_test_struct(&key_not_in);

    if (del_result == CS_SUCCESS) {
        map_free(m);
        return (test_res){(char*)__func__, "Delete of non-existent should not return SUCCESS", CS_ELEM};
    }

    if (map_size(*m) != 10) {
        map_free(m);
        return (test_res){(char*)__func__, "Size changed after deleting non-existent", CS_UNKNOWN};
    }

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_random_order() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    int total = 100;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "RandDelKey", (double)i);
        test_struct val = create_test_struct(i * 7, "RandDelVal", (double)(i * 7));
        cs_codes ins_result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    int order[] = {50, 25, 75, 10, 40, 60, 90, 5, 15, 30, 45, 55, 70, 80, 95};
    for (int i = 0; i < 15; i++) {
        test_struct key = create_test_struct(order[i], "RandDelKey", (double)order[i]);
        cs_codes del_result = map_delete(m, &key);
        free_test_struct(&key);
        if (del_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(m->t)) {
            map_free(m);
            return (test_res){(char*)__func__, "RBT integrity violated during random deletes", CS_UNKNOWN};
        }
    }

    if (map_size(*m) != total - 15) {
        map_free(m);
        return (test_res){(char*)__func__, "Size mismatch after random deletes", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_find
// ============================================================================

test_res test_map_find_existing() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 20; i++) {
        test_struct key = create_test_struct(i, "FindKey", (double)i);
        test_struct val = create_test_struct(i * 100, "FindVal", (double)(i * 100));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    test_struct search_key = create_test_struct(10, "FindKey", 10.0);
    void *found = map_find(*m, &search_key);
    free_test_struct(&search_key);

    if (found == NULL) {
        map_free(m);
        return (test_res){(char*)__func__, "Find returned NULL for existing key", CS_ELEM};
    }

    test_struct *found_val = (test_struct*)found;
    if (found_val->id != 1000) {  // 10 * 100
        map_free(m);
        return (test_res){(char*)__func__, "Find returned wrong value", CS_ELEM};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_find_nonexistent() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 20; i++) {
        test_struct key = create_test_struct(i, "FindNonKey", (double)i);
        test_struct val = create_test_struct(i * 50, "FindNonVal", (double)(i * 50));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    test_struct search_key = create_test_struct(999, "NotInMap", 999.0);
    void *found = map_find(*m, &search_key);
    free_test_struct(&search_key);

    if (found != NULL) {
        map_free(m);
        return (test_res){(char*)__func__, "Find should return NULL for non-existent key", CS_ELEM};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_find_all() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    int total = 100;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "FindAllKey", (double)i);
        test_struct val = create_test_struct(i * 3, "FindAllVal", (double)(i * 3));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    for (int i = 0; i < total; i++) {
        test_struct search_key = create_test_struct(i, "FindAllKey", (double)i);
        void *found = map_find(*m, &search_key);
        free_test_struct(&search_key);

        if (found == NULL) {
            map_free(m);
            return (test_res){(char*)__func__, "Failed to find existing key", CS_ELEM};
        }

        test_struct *found_val = (test_struct*)found;
        if (found_val->id != i * 3) {
            map_free(m);
            return (test_res){(char*)__func__, "Found value has wrong id", CS_ELEM};
        }
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_empty
// ============================================================================

test_res test_map_empty_initial() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    if (!map_empty(*m)) {
        map_free(m);
        return (test_res){(char*)__func__, "New map should be empty", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_empty_after_ops() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "EmptyKey", 42.0);
    test_struct val = create_test_struct(100, "EmptyVal", 100.0);

    cs_codes ins_result = map_insert(m, &key, &val);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }

    if (map_empty(*m)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Map should not be empty after insert", CS_UNKNOWN};
    }

    cs_codes del_result = map_delete(m, &key);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }

    if (!map_empty(*m)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(m);
        return (test_res){(char*)__func__, "Map should be empty after delete", CS_UNKNOWN};
    }

    free_test_struct(&key);
    free_test_struct(&val);
    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_size
// ============================================================================

test_res test_map_size_initial() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    if (map_size(*m) != 0) {
        map_free(m);
        return (test_res){(char*)__func__, "New map size should be 0", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_size_after_ops() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "SizeKey", (double)i);
        test_struct val = create_test_struct(i * 4, "SizeVal", (double)(i * 4));
        cs_codes ins_result = map_insert(m, &key, &val);
        if (ins_result != CS_SUCCESS) {
            free_test_struct(&key);
            free_test_struct(&val);
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
        if (map_size(*m) != i + 1) {
            free_test_struct(&key);
            free_test_struct(&val);
            map_free(m);
            return (test_res){(char*)__func__, "Size mismatch during insert", CS_UNKNOWN};
        }
        free_test_struct(&key);
        free_test_struct(&val);
    }

    for (int i = 49; i >= 0; i--) {
        test_struct key = create_test_struct(i, "SizeKey", (double)i);
        cs_codes del_result = map_delete(m, &key);
        if (del_result != CS_SUCCESS) {
            free_test_struct(&key);
            map_free(m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
        if (map_size(*m) != i) {
            free_test_struct(&key);
            map_free(m);
            return (test_res){(char*)__func__, "Size mismatch during delete", CS_UNKNOWN};
        }
        free_test_struct(&key);
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_swap
// ============================================================================

test_res test_map_swap() {
    map *m1 = map_init(get_test_struct_attr(), get_test_struct_attr());
    map *m2 = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct key = create_test_struct(i, "Swap1Key", (double)i);
        test_struct val = create_test_struct(i * 10, "Swap1Val", (double)(i * 10));
        cs_codes result = map_insert(m1, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m1);
            map_free(m2);
            return (test_res){(char*)__func__, "Insert to m1 failed", result};
        }
    }
    for (int i = 10; i < 15; i++) {
        test_struct key = create_test_struct(i, "Swap2Key", (double)i);
        test_struct val = create_test_struct(i * 20, "Swap2Val", (double)(i * 20));
        cs_codes result = map_insert(m2, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m1);
            map_free(m2);
            return (test_res){(char*)__func__, "Insert to m2 failed", result};
        }
    }

    map_swap(m1, m2);

    // m1 should now have keys 10-14
    test_struct search1 = create_test_struct(10, "Swap2Key", 10.0);
    if (map_find(*m1, &search1) == NULL) {
        free_test_struct(&search1);
        map_free(m1);
        map_free(m2);
        return (test_res){(char*)__func__, "Swap m1 content mismatch", CS_ELEM};
    }
    free_test_struct(&search1);

    // m2 should now have keys 0-4
    test_struct search2 = create_test_struct(0, "Swap1Key", 0.0);
    if (map_find(*m2, &search2) == NULL) {
        free_test_struct(&search2);
        map_free(m1);
        map_free(m2);
        return (test_res){(char*)__func__, "Swap m2 content mismatch", CS_ELEM};
    }
    free_test_struct(&search2);

    if (!rbt_is_valid(m1->t) || !rbt_is_valid(m2->t)) {
        map_free(m1);
        map_free(m2);
        return (test_res){(char*)__func__, "RBT integrity violated after swap", CS_UNKNOWN};
    }

    map_free(m1);
    map_free(m2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_swap_empty() {
    map *m1 = map_init(get_test_struct_attr(), get_test_struct_attr());
    map *m2 = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct key = create_test_struct(i, "SwapEmptyKey", (double)i);
        test_struct val = create_test_struct(i * 8, "SwapEmptyVal", (double)(i * 8));
        cs_codes result = map_insert(m1, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m1);
            map_free(m2);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    map_swap(m1, m2);

    if (!map_empty(*m1)) {
        map_free(m1);
        map_free(m2);
        return (test_res){(char*)__func__, "m1 should be empty after swap", CS_UNKNOWN};
    }

    if (map_size(*m2) != 5) {
        map_free(m1);
        map_free(m2);
        return (test_res){(char*)__func__, "m2 should have 5 elements", CS_UNKNOWN};
    }

    map_free(m1);
    map_free(m2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_clear
// ============================================================================

test_res test_map_clear() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "ClearKey", (double)i);
        test_struct val = create_test_struct(i * 6, "ClearVal", (double)(i * 6));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    map_clear(m);

    if (map_size(*m) != 0 || !map_empty(*m)) {
        map_free(m);
        return (test_res){(char*)__func__, "Clear did not reset map", CS_UNKNOWN};
    }

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated after clear", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_clear_reuse() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "ClearReuseKey", (double)i);
        test_struct val = create_test_struct(i * 9, "ClearReuseVal", (double)(i * 9));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert before clear failed", result};
        }
    }

    map_clear(m);

    for (int i = 100; i < 150; i++) {
        test_struct key = create_test_struct(i, "AfterClearKey", (double)i);
        test_struct val = create_test_struct(i * 11, "AfterClearVal", (double)(i * 11));
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert after clear failed", result};
        }
    }

    if (map_size(*m) != 50) {
        map_free(m);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_UNKNOWN};
    }

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT integrity violated after reuse", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================

test_res test_map_nested_data_integrity() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "NestedKey", (double)i * 1.5);
        test_struct val = create_test_struct(i * 100, "NestedVal", (double)(i * 100) + 0.5);
        cs_codes result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    for (int i = 0; i < 50; i++) {
        test_struct search_key = create_test_struct(i, "NestedKey", (double)i * 1.5);
        test_struct *found_val = (test_struct*)map_find(*m, &search_key);
        free_test_struct(&search_key);

        if (!found_val) {
            map_free(m);
            return (test_res){(char*)__func__, "Value not found", CS_ELEM};
        }

        // Check value's address
        if (!found_val->address || found_val->address->zip_code != 10000 + (i * 100)) {
            map_free(m);
            return (test_res){(char*)__func__, "Value address data corrupted", CS_ELEM};
        }

        // Check value's contacts array
        int expected_contacts = 2 + ((i * 100) % 3);
        if (found_val->contact_count != expected_contacts || !found_val->contacts) {
            map_free(m);
            return (test_res){(char*)__func__, "Value contacts array corrupted", CS_ELEM};
        }

        // Check value's tags array
        int expected_tags = 3 + ((i * 100) % 3);
        if (found_val->tag_count != expected_tags || !found_val->tags) {
            map_free(m);
            return (test_res){(char*)__func__, "Value tags array corrupted", CS_ELEM};
        }
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_deep_copy_verification() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    test_struct original_key = create_test_struct(42, "OrigKey", 42.42);
    test_struct original_val = create_test_struct(100, "OrigVal", 100.0);
    cs_codes result = map_insert(m, &original_key, &original_val);
    if (result != CS_SUCCESS) {
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(m);
        return (test_res){(char*)__func__, "Insert failed", result};
    }

    // Modify originals after insert
    original_key.id = 999;
    free(original_key.name);
    original_key.name = strdup("ModifiedKey");
    original_val.id = 888;
    original_val.address->zip_code = 99999;

    // Search with original key id
    test_struct search_key = create_test_struct(42, "OrigKey", 42.42);
    test_struct *stored_val = (test_struct*)map_find(*m, &search_key);
    free_test_struct(&search_key);

    if (!stored_val || stored_val->id != 100) {
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(m);
        return (test_res){(char*)__func__, "Deep copy failed - value id changed", CS_ELEM};
    }

    if (strcmp(stored_val->name, "OrigVal") != 0) {
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(m);
        return (test_res){(char*)__func__, "Deep copy failed - value name changed", CS_ELEM};
    }

    if (stored_val->address->zip_code != 10100) {  // 10000 + 100
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(m);
        return (test_res){(char*)__func__, "Deep copy failed - value address changed", CS_ELEM};
    }

    free_test_struct(&original_key);
    free_test_struct(&original_val);
    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests with RBT integrity
// ============================================================================

test_res test_map_stress_insert_delete() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "StressKey", (double)i);
        test_struct val = create_test_struct(i * 2, "StressVal", (double)(i * 2));
        cs_codes ins_result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT invalid after stress inserts", CS_UNKNOWN};
    }

    for (int i = 0; i < total / 2; i++) {
        test_struct key = create_test_struct(i, "StressKey", (double)i);
        cs_codes del_result = map_delete(m, &key);
        free_test_struct(&key);
        if (del_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
    }

    if (!rbt_is_valid(m->t)) {
        map_free(m);
        return (test_res){(char*)__func__, "RBT invalid after stress deletes", CS_UNKNOWN};
    }

    if (map_size(*m) != total - total / 2) {
        map_free(m);
        return (test_res){(char*)__func__, "Size mismatch after stress test", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_interleaved_insert_delete() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "InterleavedKey", (double)i);
        test_struct val = create_test_struct(i * 5, "InterleavedVal", (double)(i * 5));
        cs_codes ins_result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }

        if (i % 3 == 0 && i > 0) {
            test_struct del_key = create_test_struct(i - 1, "InterleavedKey", (double)(i - 1));
            cs_codes del_result = map_delete(m, &del_key);
            free_test_struct(&del_key);
            if (del_result != CS_SUCCESS) {
                map_free(m);
                return (test_res){(char*)__func__, "Delete failed", del_result};
            }
        }

        if (!rbt_is_valid(m->t)) {
            map_free(m);
            return (test_res){(char*)__func__, "RBT invalid during interleaved ops", CS_UNKNOWN};
        }
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_all_verify_rbt() {
    map *m = map_init(get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "DeleteAllKey", (double)i);
        test_struct val = create_test_struct(i * 12, "DeleteAllVal", (double)(i * 12));
        cs_codes ins_result = map_insert(m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "DeleteAllKey", (double)i);
        cs_codes del_result = map_delete(m, &key);
        free_test_struct(&key);
        if (del_result != CS_SUCCESS) {
            map_free(m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(m->t)) {
            map_free(m);
            return (test_res){(char*)__func__, "RBT invalid during sequential deletes", CS_UNKNOWN};
        }
    }

    if (!map_empty(*m)) {
        map_free(m);
        return (test_res){(char*)__func__, "Map not empty after deleting all", CS_UNKNOWN};
    }

    map_free(m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// MAIN RUNNER
int main(int argc, char **argv) {
    test tests[] = {
        // map_init
        test_map_init,

        // map_insert
        test_map_insert_single,
        test_map_insert_multiple,
        test_map_insert_duplicate_key,
        test_map_insert_ascending,
        test_map_insert_descending,

        // map_delete
        test_map_delete_single,
        test_map_delete_multiple,
        test_map_delete_nonexistent,
        test_map_delete_random_order,

        // map_find
        test_map_find_existing,
        test_map_find_nonexistent,
        test_map_find_all,

        // map_empty
        test_map_empty_initial,
        test_map_empty_after_ops,

        // map_size
        test_map_size_initial,
        test_map_size_after_ops,

        // map_swap
        test_map_swap,
        test_map_swap_empty,

        // map_clear
        test_map_clear,
        test_map_clear_reuse,

        // Complex struct integrity
        test_map_nested_data_integrity,
        test_map_deep_copy_verification,

        // Stress tests with RBT integrity
        test_map_stress_insert_delete,
        test_map_interleaved_insert_delete,
        test_map_delete_all_verify_rbt
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}