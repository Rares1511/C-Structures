#include <cs/set.h>
#include <cs/rbt.h>

#include <benchmark.h>
#include <unittest.h>

#include <sys/time.h>
#include <valgrind/valgrind.h>

// ============================================================================
// set_init
// ============================================================================
test_res test_set_init(test_arg *arg) {
    set_attr_t attr = get_test_struct_attr();
    set s;
    cs_codes init_result = set_init(&s, attr);

    if (init_result != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", CS_MEM};
    if (set_size(s) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    if (!set_empty(s)) return (test_res){(char*)__func__, "Set not empty after init", CS_UNKNOWN};

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Set initialized with custom struct attribute.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after init", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after init.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_insert
// ============================================================================
test_res test_set_insert_single(test_arg *arg) {
    set s;
    set_init(&s, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "InsertSingle", 42.0);

    cs_codes result = set_insert(&s, &ts);
    if (result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Insert returned error", CS_MEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted one element into set.\n");

    if (set_size(s) != 1) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Size not 1 after insert", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after insert.\n");

    if (!rbt_is_valid(s.t)) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after insert", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after insert.\n");

    free_test_struct(&ts);
    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_multiple(test_arg *arg) {
    set s;
    set_init(&s, get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "InsertMultiple", (double)i);
        cs_codes result = set_insert(&s, &ts);
        if (result != CS_SUCCESS) {
            free_test_struct(&ts);
            set_free(&s);
            return (test_res){(char*)__func__, "Insert returned error code", result};
        }
        free_test_struct(&ts);

        // Check RBT integrity after each insert
        if (!rbt_is_valid(s.t)) {
            set_free(&s);
            return (test_res){(char*)__func__, "RBT integrity violated during inserts", CS_UNKNOWN};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted %d elements into set.\n", total);

    if (set_size(s) != total) {
        set_free(&s);
        return (test_res){(char*)__func__, "Size mismatch after inserts", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after multiple inserts.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_duplicate(test_arg *arg) {
    set s;
    set_init(&s, get_test_struct_attr());
    test_struct ts = create_test_struct(42, "Duplicate", 42.0);

    if (set_insert(&s, &ts) != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Initial insert failed", CS_MEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted element into set for duplicate test.\n");

    if (set_insert(&s, &ts) == CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Duplicate insert was successful", CS_MEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Duplicate insert correctly returned error.\n");

    // Sets should not allow duplicates - size should still be 1
    if (set_size(s) != 1) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Duplicate was inserted", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after duplicate insert attempt.\n");

    if (!rbt_is_valid(s.t)) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after duplicate", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after duplicate insert attempt.\n");

    free_test_struct(&ts);
    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_ascending(test_arg *arg) {
    set s;
    set_init(&s, get_test_struct_attr());

    // Insert in ascending order (worst case for unbalanced BST)
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Ascending", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 100 elements in ascending order.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after ascending inserts", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after ascending inserts.\n");

    if (set_size(s) != 100) {
        set_free(&s);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after ascending inserts.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_insert_descending(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    // Insert in descending order (worst case for unbalanced BST)
    for (int i = 99; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "Descending", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 100 elements in descending order.\n");

    if (set_size(s) != 100) {
        set_free(&s);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after descending inserts.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after descending inserts", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after descending inserts.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_delete
// ============================================================================
test_res test_set_delete_single(test_arg *arg) {
    set s;
    test_struct ts = create_test_struct(42, "DeleteSingle", 42.0);

    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    
    cs_codes ins_result = set_insert(&s, &ts);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted element into set for delete test.\n");

    cs_codes del_result = set_delete(&s, &ts);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deleted element from set.\n");

    if (!set_empty(s)) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Set not empty after delete", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Set is empty after delete.\n");

    if (!rbt_is_valid(s.t)) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after delete", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after delete.\n");

    free_test_struct(&ts);
    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_multiple(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    int total = 50;

    // Insert elements
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteMulti", (double)i);
        cs_codes ins_result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted %d elements into set for delete test.\n", total);

    // Delete all elements
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "DeleteMulti", (double)i);
        cs_codes del_result = set_delete(&s, &ts);
        free_test_struct(&ts);

        if (del_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        // Check RBT integrity after each delete
        if (!rbt_is_valid(s.t)) {
            set_free(&s);
            return (test_res){(char*)__func__, "RBT integrity violated during deletes", CS_UNKNOWN};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deleted all elements from set.\n");

    if (!set_empty(s)) {
        set_free(&s);
        return (test_res){(char*)__func__, "Set not empty after all deletes", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Set is empty after all deletes.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_nonexistent(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 10; i++) {
        test_struct ts = create_test_struct(i, "DeleteNonExist", (double)i);
        set_insert(&s, &ts);
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 10 elements into set for non-existent delete test.\n");

    test_struct ts_not_in = create_test_struct(999, "NotInSet", 999.0);
    if(set_delete(&s, &ts_not_in) == CS_SUCCESS) {
        set_free(&s);
        free_test_struct(&ts_not_in);
        return (test_res){(char*)__func__, "Deleted non-existent element", CS_ELEM};
    }
    free_test_struct(&ts_not_in);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Attempted to delete non-existent element, correctly returned error.\n");

    // Delete of non-existent should return error or at least not crash
    if (set_size(s) != 10) {
        set_free(&s);
        return (test_res){(char*)__func__, "Size changed after deleting non-existent", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after attempting to delete non-existent element.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after attempting to delete non-existent element.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_random_order(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    int total = 100;

    // Insert in order
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "RandomDelete", (double)i);
        cs_codes ins_result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted %d elements into set for random delete test.\n", total);

    // Delete in "random" order (using a pattern)
    int order[] = {50, 25, 75, 10, 40, 60, 90, 5, 15, 30, 45, 55, 70, 80, 95};
    for (int i = 0; i < 15; i++) {
        test_struct ts = create_test_struct(order[i], "RandomDelete", (double)order[i]);
        cs_codes del_result = set_delete(&s, &ts);
        free_test_struct(&ts);
        if (del_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(s.t)) {
            set_free(&s);
            return (test_res){(char*)__func__, "RBT integrity violated during random deletes", CS_UNKNOWN};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deleted 15 elements in random order from set.\n");

    if (set_size(s) != total - 15) {
        set_free(&s);
        return (test_res){(char*)__func__, "Size mismatch after random deletes", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after random deletes.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_find
// ============================================================================
test_res test_set_find_existing(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 20; i++) {
        test_struct ts = create_test_struct(i, "FindExisting", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 20 elements into set for find test.\n");

    test_struct search = create_test_struct(10, "FindExisting", 10.0);
    void *found = set_find(s, &search);
    free_test_struct(&search);

    if (found == NULL) {
        set_free(&s);
        return (test_res){(char*)__func__, "Find returned NULL for existing element", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Find returned non-NULL for existing element.\n");

    test_struct *found_ts = (test_struct*)found;
    if (found_ts->id != 10) {
        set_free(&s);
        return (test_res){(char*)__func__, "Find returned wrong element", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Find returned correct element.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_find_nonexistent(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 20; i++) {
        test_struct ts = create_test_struct(i, "FindNonExist", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 20 elements into set for non-existent find test.\n");

    test_struct search = create_test_struct(999, "NotInSet", 999.0);
    void *found = set_find(s, &search);
    free_test_struct(&search);

    if (found != NULL) {
        set_free(&s);
        return (test_res){(char*)__func__, "Find should return NULL for non-existent", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Find correctly returned NULL for non-existent element.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_find_all(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    int total = 100;

    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "FindAll", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted %d elements into set for find all test.\n", total);

    // Find all elements
    for (int i = 0; i < total; i++) {
        test_struct search = create_test_struct(i, "FindAll", (double)i);
        void *found = set_find(s, &search);
        free_test_struct(&search);

        if (found == NULL) {
            set_free(&s);
            return (test_res){(char*)__func__, "Failed to find existing element", CS_ELEM};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Successfully found all existing elements.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_empty
// ============================================================================
test_res test_set_empty_initial(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    if (!set_empty(s)) {
        set_free(&s);
        return (test_res){(char*)__func__, "New set should be empty", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "New set is empty as expected.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_empty_after_ops(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    test_struct ts = create_test_struct(42, "EmptyOps", 42.0);

    cs_codes ins_result = set_insert(&s, &ts);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted element into set for empty test.\n");

    if (set_empty(s)) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Set should not be empty after insert", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Set is not empty after insert as expected.\n");

    cs_codes del_result = set_delete(&s, &ts);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deleted element from set.\n");

    if (!set_empty(s)) {
        free_test_struct(&ts);
        set_free(&s);
        return (test_res){(char*)__func__, "Set should be empty after delete", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Set is empty after delete as expected.\n");

    free_test_struct(&ts);
    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_size
// ============================================================================
test_res test_set_size_initial(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    if (set_size(s) != 0) {
        set_free(&s);
        return (test_res){(char*)__func__, "New set size should be 0", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "New set size is 0 as expected.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_size_after_ops(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "SizeOps", (double)i);
        cs_codes ins_result = set_insert(&s, &ts);
        if (ins_result != CS_SUCCESS) {
            free_test_struct(&ts);
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
        if (set_size(s) != i + 1) {
            free_test_struct(&ts);
            set_free(&s);
            return (test_res){(char*)__func__, "Size mismatch during insert", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 50 elements into set for size test.\n");

    for (int i = 49; i >= 0; i--) {
        test_struct ts = create_test_struct(i, "SizeOps", (double)i);
        cs_codes del_result = set_delete(&s, &ts);
        if (del_result != CS_SUCCESS) {
            free_test_struct(&ts);
            set_free(&s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
        if (set_size(s) != i) {
            free_test_struct(&ts);
            set_free(&s);
            return (test_res){(char*)__func__, "Size mismatch during delete", CS_UNKNOWN};
        }
        free_test_struct(&ts);
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deleted all elements from set for size test.\n");

     if (set_size(s) != 0) {
        set_free(&s);
        return (test_res){(char*)__func__, "Size should be 0 after deleting all", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is 0 after deleting all elements as expected.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_swap
// ============================================================================
test_res test_set_swap(test_arg *arg) {
    set s1;
    set s2;

    if (set_init(&s1, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set s1 init failed", CS_MEM};
    }
    if (set_init(&s2, get_test_struct_attr()) != CS_SUCCESS) {
        set_free(&s1);
        return (test_res){(char*)__func__, "Set s2 init failed", CS_MEM};
    }

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "Swap1", (double)i);
        cs_codes result = set_insert(&s1, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s1);
            set_free(&s2);
            return (test_res){(char*)__func__, "Insert to s1 failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 5 elements into s1 for swap test.\n");

    for (int i = 10; i < 15; i++) {
        test_struct ts = create_test_struct(i, "Swap2", (double)i);
        cs_codes result = set_insert(&s2, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s1);
            set_free(&s2);
            return (test_res){(char*)__func__, "Insert to s2 failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 5 elements into s2 for swap test.\n");

    set_swap(&s1, &s2);

    // s1 should now have elements 10-14
    test_struct search1 = create_test_struct(10, "Swap2", 10.0);
    if (set_find(s1, &search1) == NULL) {
        free_test_struct(&search1);
        set_free(&s1);
        set_free(&s2);
        return (test_res){(char*)__func__, "Swap s1 content mismatch", CS_ELEM};
    }
    free_test_struct(&search1);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "s1 has correct content after swap.\n");

    // s2 should now have elements 0-4
    test_struct search2 = create_test_struct(0, "Swap1", 0.0);
    if (set_find(s2, &search2) == NULL) {
        free_test_struct(&search2);
        set_free(&s1);
        set_free(&s2);
        return (test_res){(char*)__func__, "Swap s2 content mismatch", CS_ELEM};
    }
    free_test_struct(&search2);

    clogger_log(*arg->logger, CLOGGER_DEBUG, "s2 has correct content after swap.\n");

    if (!rbt_is_valid(s1.t) || !rbt_is_valid(s2.t)) {
        set_free(&s1);
        set_free(&s2);
        return (test_res){(char*)__func__, "RBT integrity violated after swap", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after swap.\n");

    set_free(&s1);
    set_free(&s2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_swap_empty(test_arg *arg) {
    set s1;
    
    set s2;

    if (set_init(&s1, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set s1 init failed", CS_MEM};
    }
    if (set_init(&s2, get_test_struct_attr()) != CS_SUCCESS) {
        set_free(&s1);
        return (test_res){(char*)__func__, "Set s2 init failed", CS_MEM};
    }

    for (int i = 0; i < 5; i++) {
        test_struct ts = create_test_struct(i, "SwapEmpty", (double)i);
        cs_codes result = set_insert(&s1, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s1);
            set_free(&s2);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 5 elements into s1 for empty swap test.\n");

    set_swap(&s1, &s2);

    if (!set_empty(s1)) {
        set_free(&s1);
        set_free(&s2);
        return (test_res){(char*)__func__, "s1 should be empty after swap", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "s1 is empty after swap as expected.\n");

    if (set_size(s2) != 5) {
        set_free(&s1);
        set_free(&s2);
        return (test_res){(char*)__func__, "s2 should have 5 elements", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "s2 has correct size after swap.\n");

    set_free(&s1);
    set_free(&s2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// set_clear
// ============================================================================
test_res test_set_clear(test_arg *arg) {
    set s;
    
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "ClearTest", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 100 elements into set for clear test.\n");

    set_clear(&s);

    if (set_size(s) != 0 || !set_empty(s)) {
        set_free(&s);
        return (test_res){(char*)__func__, "Clear did not reset set", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Set is empty and size is 0 after clear as expected.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after clear", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after clear.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_clear_reuse(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "ClearReuse", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert before clear failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 50 elements into set for clear reuse test.\n");

    set_clear(&s);

    // Verify can reuse after clear
    for (int i = 100; i < 150; i++) {
        test_struct ts = create_test_struct(i, "AfterClear", (double)i);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert after clear failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 50 new elements into set after clear.\n");

    if (set_size(s) != 50) {
        set_free(&s);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after inserting new elements post-clear.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT integrity violated after reuse", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after reuse post-clear.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================
test_res test_set_nested_data_integrity(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 50; i++) {
        test_struct ts = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        cs_codes result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 50 complex elements into set for nested integrity test.\n");

    // Find and verify nested structures
    for (int i = 0; i < 50; i++) {
        test_struct search = create_test_struct(i, "NestedIntegrity", (double)i * 2.5);
        test_struct *found = (test_struct*)set_find(s, &search);
        free_test_struct(&search);

        if (!found) {
            set_free(&s);
            return (test_res){(char*)__func__, "Element not found", CS_ELEM};
        }

        // Check address
        if (!found->address || found->address->zip_code != 10000 + i) {
            set_free(&s);
            return (test_res){(char*)__func__, "Address data corrupted", CS_ELEM};
        }

        // Check contacts array
        int expected_contacts = 2 + (i % 3);
        if (found->contact_count != expected_contacts || !found->contacts) {
            set_free(&s);
            return (test_res){(char*)__func__, "Contacts array corrupted", CS_ELEM};
        }

        // Check tags array
        int expected_tags = 3 + (i % 3);
        if (found->tag_count != expected_tags || !found->tags) {
            set_free(&s);
            return (test_res){(char*)__func__, "Tags array corrupted", CS_ELEM};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "All nested data verified successfully.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_deep_copy_verification(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    test_struct original = create_test_struct(42, "DeepCopyTest", 42.42);
    cs_codes result = set_insert(&s, &original);
    if (result != CS_SUCCESS) {
        free_test_struct(&original);
        set_free(&s);
        return (test_res){(char*)__func__, "Insert failed", result};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted element into set for deep copy test.\n");

    // Modify original after insert - should not affect set content
    original.id = 999;
    free(original.name);
    original.name = strdup("Modified");
    original.address->zip_code = 99999;

    // Search with original id
    test_struct search = create_test_struct(42, "DeepCopyTest", 42.42);
    test_struct *stored = (test_struct*)set_find(s, &search);
    free_test_struct(&search);

    if (!stored || stored->id != 42) {
        free_test_struct(&original);
        set_free(&s);
        return (test_res){(char*)__func__, "Deep copy failed - id changed", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "ID is correct in set after modifying original.\n");

    if (strcmp(stored->name, "DeepCopyTest") != 0) {
        free_test_struct(&original);
        set_free(&s);
        return (test_res){(char*)__func__, "Deep copy failed - name changed", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Name is correct in set after modifying original.\n");

    if (stored->address->zip_code != 10042) {
        free_test_struct(&original);
        set_free(&s);
        return (test_res){(char*)__func__, "Deep copy failed - address changed", CS_ELEM};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Address zip code is correct in set after modifying original.\n");

    free_test_struct(&original);
    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests with RBT integrity
// ============================================================================
test_res test_set_insert_delete(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    int total = __TEST_SIZE;

    // Insert all
    for (int i = 0; i < total; i++) {
        test_struct ts = create_test_struct(i, "Stress", (double)i);
        cs_codes ins_result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted %d elements into set for stress test.\n", total);

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT invalid after stress inserts", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after stress inserts.\n");

    // Delete half
    for (int i = 0; i < total / 2; i++) {
        test_struct ts = create_test_struct(i, "Stress", (double)i);
        cs_codes del_result = set_delete(&s, &ts);
        free_test_struct(&ts);
        if (del_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Deleted half of the elements from set for stress test.\n");

    if (set_size(s) != total - total / 2) {
        set_free(&s);
        return (test_res){(char*)__func__, "Size mismatch after stress test", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Stress test completed successfully.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT invalid after stress deletes", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid after stress deletes.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_interleaved_insert_delete(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    // Interleave inserts and deletes
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "Interleaved", (double)i);
        cs_codes ins_result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }

        if (i % 3 == 0 && i > 0) {
            test_struct del = create_test_struct(i - 1, "Interleaved", (double)(i - 1));
            cs_codes del_result = set_delete(&s, &del);
            free_test_struct(&del);
            if (del_result != CS_SUCCESS) {
                set_free(&s);
                return (test_res){(char*)__func__, "Delete failed", del_result};
            }
        }

        if (!rbt_is_valid(s.t)) {
            set_free(&s);
            return (test_res){(char*)__func__, "RBT invalid during interleaved ops", CS_UNKNOWN};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Completed interleaved insert/delete operations.\n");

     if (set_size(s) != 67) { // 100 inserts - 33 deletes
        set_free(&s);
        return (test_res){(char*)__func__, "Size mismatch after interleaved ops", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Size is correct after interleaved operations.\n");

     if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT invalid after interleaved ops", CS_UNKNOWN};
    }

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_set_delete_all_verify_rbt(test_arg *arg) {
    set s;
    if (set_init(&s, get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }

    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "DeleteAll", (double)i);
        cs_codes ins_result = set_insert(&s, &ts);
        free_test_struct(&ts);
        if (ins_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Inserted 100 elements into set for delete all test.\n");

    if (!rbt_is_valid(s.t)) {
        set_free(&s);
        return (test_res){(char*)__func__, "RBT invalid before deletes", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "RBT properties valid before deletes.\n");

    // Delete all, checking RBT after each
    for (int i = 0; i < 100; i++) {
        test_struct ts = create_test_struct(i, "DeleteAll", (double)i);
        cs_codes del_result = set_delete(&s, &ts);
        free_test_struct(&ts);
        if (del_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(s.t)) {
            set_free(&s);
            return (test_res){(char*)__func__, "RBT invalid during sequential deletes", CS_UNKNOWN};
        }
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Completed sequential delete operations.\n");

    if (!set_empty(s)) {
        set_free(&s);
        return (test_res){(char*)__func__, "Set not empty after deleting all", CS_UNKNOWN};
    }

    clogger_log(*arg->logger, CLOGGER_DEBUG, "Set is empty after deleting all elements as expected.\n");

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress test with timing
// ===========================================================================
test_res test_set_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        return (test_res){(char*)__func__, "Valgrind active - skipping stress test", CS_SUCCESS};
    }

    set s;
    struct timeval start, end;
    double elapsed;

    if (set_init(&s, get_int_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Set init failed", CS_MEM};
    }
    int total = __SET_STRESS_TEST_SIZE;

    /* INSERT timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int val = i;
        cs_codes result = set_insert(&s, &val);

        if (result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Insert failed during stress test", result};
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "insert", elapsed);

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Stress test completed: Total Insert Time = %.9f sec\n", elapsed);

    /* FIND timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        void *found = set_find(s, &key);

        if (found == NULL) {
            set_free(&s);
            return (test_res){(char*)__func__, "Find failed during stress test", CS_ELEM};
        }

        int *found_val = (int*)found;
        if (*found_val != i) {
            set_free(&s);
            return (test_res){(char*)__func__, "Find returned wrong value during stress test", CS_ELEM};
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "find", elapsed);

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Stress test completed: Total Find Time = %.9f sec\n", elapsed);

    /* DELETE timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        cs_codes del_result = set_delete(&s, &key);

        if (del_result != CS_SUCCESS) {
            set_free(&s);
            return (test_res){(char*)__func__, "Delete failed during stress test", del_result};
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "delete", elapsed);

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Stress test completed: Total Delete Time = %.9f sec\n", elapsed);

    set_free(&s);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test set_tests[] = {
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
    test_set_insert_delete,
    test_set_interleaved_insert_delete,
    test_set_delete_all_verify_rbt,

    // Stress tests with timing
    test_set_stress_time,
};