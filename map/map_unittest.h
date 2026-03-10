#include <cs/map.h>
#include <cs/rbt.h>
#include <cs/pair.h>

#include <unittest.h>
#include <benchmark.h>

#include <sys/time.h>
#include <valgrind/valgrind.h>

// ============================================================================
// map_init
// ============================================================================
test_res test_map_init(test_arg *arg) {
    map_attr_t key_attr = get_test_struct_attr();
    map_attr_t val_attr = get_test_struct_attr();

    map m;
    cs_codes init_result = map_init(&m, key_attr, val_attr);

    if (init_result != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", CS_MEM};
    if (map_size(m) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};
    if (!map_empty(m)) return (test_res){(char*)__func__, "Map not empty after init", CS_UNKNOWN};

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map initialized with key size %d and value size %d\n", key_attr.size, val_attr.size);

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated after init", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Empty map initialized successfully with valid RBT structure.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_insert
// ============================================================================
test_res test_map_insert_single(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "Key42", 42.0);
    test_struct val = create_test_struct(100, "Val100", 100.0);

    cs_codes result = map_insert(&m, &key, &val);
    if (result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Insert returned error", result};
    }

    if (map_size(m) != 1) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Size not 1 after insert", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted key with id %d and value with id %d\n", key.id, val.id);

    if (!rbt_is_valid(m.t)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated after insert", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after single insert.\n");

    free_test_struct(&key);
    free_test_struct(&val);
    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_multiple(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "KeyMulti", (double)i);
        test_struct val = create_test_struct(i * 10, "ValMulti", (double)(i * 10));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);

        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert returned error", result};
        }

        if (!rbt_is_valid(m.t)) {
            map_free(&m);
            return (test_res){(char*)__func__, "RBT integrity violated during inserts", CS_UNKNOWN};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted %d key-value pairs successfully.\n", total);

    if (map_size(m) != total) {
        map_free(&m);
        return (test_res){(char*)__func__, "Size mismatch after inserts", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after multiple inserts.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_duplicate_key(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "DupKey", 42.0);
    test_struct val1 = create_test_struct(100, "Val1", 100.0);
    test_struct val2 = create_test_struct(200, "Val2", 200.0);

    cs_codes first_result = map_insert(&m, &key, &val1);
    if (first_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(&m);
        return (test_res){(char*)__func__, "First insert failed", first_result};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted initial key with id %d and value with id %d\n", key.id, val1.id);

    cs_codes dup_result = map_insert(&m, &key, &val2);
    // Maps should not allow duplicate keys
    if (dup_result == CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(&m);
        return (test_res){(char*)__func__, "Duplicate key insert should not return SUCCESS", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Attempted duplicate key insert and received expected error code.\n");

    if (map_size(m) != 1) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(&m);
        return (test_res){(char*)__func__, "Size should be 1 after duplicate attempt", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after duplicate key insert attempt.\n");

    if (!rbt_is_valid(m.t)) {
        free_test_struct(&key);
        free_test_struct(&val1);
        free_test_struct(&val2);
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after duplicate key insert attempt.\n");

    free_test_struct(&key);
    free_test_struct(&val1);
    free_test_struct(&val2);
    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_ascending(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "AscKey", (double)i);
        test_struct val = create_test_struct(i * 2, "AscVal", (double)(i * 2));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 100 key-value pairs in ascending order.\n");

    if (map_size(m) != 100) {
        map_free(&m);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after ascending inserts.\n");

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated after ascending inserts", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after ascending inserts.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_insert_descending(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 99; i >= 0; i--) {
        test_struct key = create_test_struct(i, "DescKey", (double)i);
        test_struct val = create_test_struct(i * 3, "DescVal", (double)(i * 3));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 100 key-value pairs in descending order.\n");

    if (map_size(m) != 100) {
        map_free(&m);
        return (test_res){(char*)__func__, "Size mismatch", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after descending inserts.\n");

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated after descending inserts", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after descending inserts.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_delete
// ============================================================================
test_res test_map_delete_single(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "DelKey", 42.0);
    test_struct val = create_test_struct(100, "DelVal", 100.0);

    cs_codes ins_result = map_insert(&m, &key, &val);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted key with id %d and value with id %d for delete test\n", key.id, val.id);

    cs_codes del_result = map_delete(&m, &key);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Deleted key with id %d and value with id %d for delete test\n", key.id, val.id);

    if (!map_empty(m)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Map not empty after delete", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map is empty after delete.\n");

    if (!rbt_is_valid(m.t)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated after delete", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after delete.\n");

    free_test_struct(&key);
    free_test_struct(&val);
    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_multiple(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    int total = 50;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "DelMultiKey", (double)i);
        test_struct val = create_test_struct(i * 5, "DelMultiVal", (double)(i * 5));
        cs_codes ins_result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted %d key-value pairs for delete test.\n", total);

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "DelMultiKey", (double)i);
        cs_codes del_result = map_delete(&m, &key);
        free_test_struct(&key);

        if (del_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(m.t)) {
            map_free(&m);
            return (test_res){(char*)__func__, "RBT integrity violated during deletes", CS_UNKNOWN};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Deleted all key-value pairs successfully.\n");

    if (!map_empty(m)) {
        map_free(&m);
        return (test_res){(char*)__func__, "Map not empty after all deletes", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map is empty after all deletes.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_nonexistent(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 10; i++) {
        test_struct key = create_test_struct(i, "NonExistKey", (double)i);
        test_struct val = create_test_struct(i * 2, "NonExistVal", (double)(i * 2));
        cs_codes ins_result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 10 key-value pairs for non-existent delete test.\n");

    test_struct key_not_in = create_test_struct(999, "NotInMap", 999.0);
    cs_codes del_result = map_delete(&m, &key_not_in);
    free_test_struct(&key_not_in);

    if (del_result == CS_SUCCESS) {
        map_free(&m);
        return (test_res){(char*)__func__, "Delete of non-existent should not return SUCCESS", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Attempted delete of non-existent key and received expected error code.\n");

    if (map_size(m) != 10) {
        map_free(&m);
        return (test_res){(char*)__func__, "Size changed after deleting non-existent", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after attempting to delete non-existent key.\n");

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after attempting to delete non-existent key.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_random_order(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    int total = 100;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "RandDelKey", (double)i);
        test_struct val = create_test_struct(i * 7, "RandDelVal", (double)(i * 7));
        cs_codes ins_result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted %d key-value pairs for random delete test.\n", total);

    int order[] = {50, 25, 75, 10, 40, 60, 90, 5, 15, 30, 45, 55, 70, 80, 95};
    for (int i = 0; i < 15; i++) {
        test_struct key = create_test_struct(order[i], "RandDelKey", (double)order[i]);
        cs_codes del_result = map_delete(&m, &key);
        free_test_struct(&key);
        if (del_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(m.t)) {
            map_free(&m);
            return (test_res){(char*)__func__, "RBT integrity violated during random deletes", CS_UNKNOWN};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Deleted 15 key-value pairs in random order successfully.\n");

    if (map_size(m) != total - 15) {
        map_free(&m);
        return (test_res){(char*)__func__, "Size mismatch after random deletes", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after random deletes.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_find
// ============================================================================
test_res test_map_find_existing(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 20; i++) {
        test_struct key = create_test_struct(i, "FindKey", (double)i);
        test_struct val = create_test_struct(i * 100, "FindVal", (double)(i * 100));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 20 key-value pairs for find test.\n");

    test_struct search_key = create_test_struct(10, "FindKey", 10.0);
    void *found = map_find(m, &search_key);
    free_test_struct(&search_key);

    if (found == NULL) {
        map_free(&m);
        return (test_res){(char*)__func__, "Find returned NULL for existing key", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Find returned non-NULL for existing key.\n");

    test_struct *found_val = (test_struct*)found;
    if (found_val->id != 1000) {  // 10 * 100
        map_free(&m);
        return (test_res){(char*)__func__, "Find returned wrong value", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Find returned correct value for existing key.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_find_nonexistent(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 20; i++) {
        test_struct key = create_test_struct(i, "FindNonKey", (double)i);
        test_struct val = create_test_struct(i * 50, "FindNonVal", (double)(i * 50));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 20 key-value pairs for non-existent find test.\n");

    test_struct search_key = create_test_struct(999, "NotInMap", 999.0);
    void *found = map_find(m, &search_key);
    free_test_struct(&search_key);

    if (found != NULL) {
        map_free(&m);
        return (test_res){(char*)__func__, "Find should return NULL for non-existent key", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Find returned NULL for non-existent key as expected.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_find_all(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    int total = 100;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "FindAllKey", (double)i);
        test_struct val = create_test_struct(i * 3, "FindAllVal", (double)(i * 3));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted %d key-value pairs for find all test.\n", total);

    for (int i = 0; i < total; i++) {
        test_struct search_key = create_test_struct(i, "FindAllKey", (double)i);
        void *found = map_find(m, &search_key);
        free_test_struct(&search_key);

        if (found == NULL) {
            map_free(&m);
            return (test_res){(char*)__func__, "Failed to find existing key", CS_ELEM};
        }

        test_struct *found_val = (test_struct*)found;
        if (found_val->id != i * 3) {
            map_free(&m);
            return (test_res){(char*)__func__, "Found value has wrong id", CS_ELEM};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Successfully found all existing keys with correct values.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_empty
// ============================================================================
test_res test_map_empty_initial(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    if (!map_empty(m)) {
        map_free(&m);
        return (test_res){(char*)__func__, "New map should be empty", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "New map is empty as expected.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_empty_after_ops(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    test_struct key = create_test_struct(42, "EmptyKey", 42.0);
    test_struct val = create_test_struct(100, "EmptyVal", 100.0);

    cs_codes ins_result = map_insert(&m, &key, &val);
    if (ins_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Insert failed", ins_result};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted key-value pair successfully.\n");

    if (map_empty(m)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Map should not be empty after insert", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map is not empty after insert as expected.\n");

    cs_codes del_result = map_delete(&m, &key);
    if (del_result != CS_SUCCESS) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Delete failed", del_result};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Deleted key-value pair successfully.\n");

    if (!map_empty(m)) {
        free_test_struct(&key);
        free_test_struct(&val);
        map_free(&m);
        return (test_res){(char*)__func__, "Map should be empty after delete", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map is empty after delete as expected.\n");

    free_test_struct(&key);
    free_test_struct(&val);
    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_size
// ============================================================================
test_res test_map_size_initial(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    if (map_size(m) != 0) {
        map_free(&m);
        return (test_res){(char*)__func__, "New map size should be 0", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "New map size is 0 as expected.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_size_after_ops(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "SizeKey", (double)i);
        test_struct val = create_test_struct(i * 4, "SizeVal", (double)(i * 4));
        cs_codes ins_result = map_insert(&m, &key, &val);
        if (ins_result != CS_SUCCESS) {
            free_test_struct(&key);
            free_test_struct(&val);
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
        if (map_size(m) != i + 1) {
            free_test_struct(&key);
            free_test_struct(&val);
            map_free(&m);
            return (test_res){(char*)__func__, "Size mismatch during insert", CS_UNKNOWN};
        }
        free_test_struct(&key);
        free_test_struct(&val);
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 50 key-value pairs and verified size after each insert.\n");

    for (int i = 49; i >= 0; i--) {
        test_struct key = create_test_struct(i, "SizeKey", (double)i);
        cs_codes del_result = map_delete(&m, &key);
        if (del_result != CS_SUCCESS) {
            free_test_struct(&key);
            map_free(&m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
        if (map_size(m) != i) {
            free_test_struct(&key);
            map_free(&m);
            return (test_res){(char*)__func__, "Size mismatch during delete", CS_UNKNOWN};
        }
        free_test_struct(&key);
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Deleted all key-value pairs and verified size after each delete.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_swap
// ============================================================================
test_res test_map_swap(test_arg *arg) {
    map m1;
    map_init(&m1, get_test_struct_attr(), get_test_struct_attr());
    map m2;
    map_init(&m2, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct key = create_test_struct(i, "Swap1Key", (double)i);
        test_struct val = create_test_struct(i * 10, "Swap1Val", (double)(i * 10));
        cs_codes result = map_insert(&m1, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m1);
            map_free(&m2);
            return (test_res){(char*)__func__, "Insert to m1 failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 5 key-value pairs into m1 for swap test.\n");

    for (int i = 10; i < 15; i++) {
        test_struct key = create_test_struct(i, "Swap2Key", (double)i);
        test_struct val = create_test_struct(i * 20, "Swap2Val", (double)(i * 20));
        cs_codes result = map_insert(&m2, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m1);
            map_free(&m2);
            return (test_res){(char*)__func__, "Insert to m2 failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 5 key-value pairs into m2 for swap test.\n");

    map_swap(&m1, &m2);

    // m1 should now have keys 10-14
    test_struct search1 = create_test_struct(10, "Swap2Key", 10.0);
    if (map_find(m1, &search1) == NULL) {
        free_test_struct(&search1);
        map_free(&m1);
        map_free(&m2);
        return (test_res){(char*)__func__, "Swap m1 content mismatch", CS_ELEM};
    }
    free_test_struct(&search1);

    clogger_log((*arg->logger), CLOGGER_DEBUG, "m1 contains expected keys after swap.\n");

    // m2 should now have keys 0-4
    test_struct search2 = create_test_struct(0, "Swap1Key", 0.0);
    if (map_find(m2, &search2) == NULL) {
        free_test_struct(&search2);
        map_free(&m1);
        map_free(&m2);
        return (test_res){(char*)__func__, "Swap m2 content mismatch", CS_ELEM};
    }
    free_test_struct(&search2);

    clogger_log((*arg->logger), CLOGGER_DEBUG, "m2 contains expected keys after swap.\n");

    if (!rbt_is_valid(m1.t) || !rbt_is_valid(m2.t)) {
        map_free(&m1);
        map_free(&m2);
        return (test_res){(char*)__func__, "RBT integrity violated after swap", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structures valid after swap.\n");

    map_free(&m1);
    map_free(&m2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_swap_empty(test_arg *arg) {
    map m1;
    map_init(&m1, get_test_struct_attr(), get_test_struct_attr());
    map m2;
    map_init(&m2, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 5; i++) {
        test_struct key = create_test_struct(i, "SwapEmptyKey", (double)i);
        test_struct val = create_test_struct(i * 8, "SwapEmptyVal", (double)(i * 8));
        cs_codes result = map_insert(&m1, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m1);
            map_free(&m2);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 5 key-value pairs into m1 for empty swap test.\n");

    map_swap(&m1, &m2);

    if (!map_empty(m1)) {
        map_free(&m1);
        map_free(&m2);
        return (test_res){(char*)__func__, "m1 should be empty after swap", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "m1 is empty after swap as expected.\n");

    if (map_size(m2) != 5) {
        map_free(&m1);
        map_free(&m2);
        return (test_res){(char*)__func__, "m2 should have 5 elements", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "m2 has correct size after swap.\n");

    map_free(&m1);
    map_free(&m2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// map_clear
// ============================================================================
test_res test_map_clear(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "ClearKey", (double)i);
        test_struct val = create_test_struct(i * 6, "ClearVal", (double)(i * 6));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 100 key-value pairs for clear test.\n");

    map_clear(&m);

    if (map_size(m) != 0 || !map_empty(m)) {
        map_free(&m);
        return (test_res){(char*)__func__, "Clear did not reset map", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map is empty after clear as expected.\n");

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated after clear", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after clear.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_clear_reuse(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "ClearReuseKey", (double)i);
        test_struct val = create_test_struct(i * 9, "ClearReuseVal", (double)(i * 9));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert before clear failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 50 key-value pairs before clear for reuse test.\n");

    map_clear(&m);

    for (int i = 100; i < 150; i++) {
        test_struct key = create_test_struct(i, "AfterClearKey", (double)i);
        test_struct val = create_test_struct(i * 11, "AfterClearVal", (double)(i * 11));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert after clear failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map cleared, now inserting 50 new key-value pairs.\n");

    if (map_size(m) != 50) {
        map_free(&m);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after inserting new pairs post-clear.\n");

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT integrity violated after reuse", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map structure valid after inserting new pairs post-clear.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Complex struct integrity tests
// ============================================================================
test_res test_map_nested_data_integrity(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 50; i++) {
        test_struct key = create_test_struct(i, "NestedKey", (double)i * 1.5);
        test_struct val = create_test_struct(i * 100, "NestedVal", (double)(i * 100) + 0.5);
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 50 key-value pairs with nested data for integrity test.\n");

    for (int i = 0; i < 50; i++) {
        test_struct search_key = create_test_struct(i, "NestedKey", (double)i * 1.5);
        test_struct *found_val = (test_struct*)map_find(m, &search_key);
        free_test_struct(&search_key);

        if (!found_val) {
            map_free(&m);
            return (test_res){(char*)__func__, "Value not found", CS_ELEM};
        }

        // Check value's address
        if (!found_val->address || found_val->address->zip_code != 10000 + (i * 100)) {
            map_free(&m);
            return (test_res){(char*)__func__, "Value address data corrupted", CS_ELEM};
        }

        // Check value's contacts array
        int expected_contacts = 2 + ((i * 100) % 3);
        if (found_val->contact_count != expected_contacts || !found_val->contacts) {
            map_free(&m);
            return (test_res){(char*)__func__, "Value contacts array corrupted", CS_ELEM};
        }

        // Check value's tags array
        int expected_tags = 3 + ((i * 100) % 3);
        if (found_val->tag_count != expected_tags || !found_val->tags) {
            map_free(&m);
            return (test_res){(char*)__func__, "Value tags array corrupted", CS_ELEM};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "All nested data verified for integrity.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_deep_copy_verification(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    test_struct original_key = create_test_struct(42, "OrigKey", 42.42);
    test_struct original_val = create_test_struct(100, "OrigVal", 100.0);
    cs_codes result = map_insert(&m, &original_key, &original_val);
    if (result != CS_SUCCESS) {
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(&m);
        return (test_res){(char*)__func__, "Insert failed", result};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted original key-value pair for deep copy test.\n");

    // Modify originals after insert
    original_key.id = 999;
    free(original_key.name);
    original_key.name = strdup("ModifiedKey");
    original_val.id = 888;
    original_val.address->zip_code = 99999;

    // Search with original key id
    test_struct search_key = create_test_struct(42, "OrigKey", 42.42);
    test_struct *stored_val = (test_struct*)map_find(m, &search_key);
    free_test_struct(&search_key);

    if (!stored_val || stored_val->id != 100) {
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(&m);
        return (test_res){(char*)__func__, "Deep copy failed - value id changed", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Value id is correct, indicating deep copy of value.\n");

    if (strcmp(stored_val->name, "OrigVal") != 0) {
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(&m);
        return (test_res){(char*)__func__, "Deep copy failed - value name changed", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Value name is correct, indicating deep copy of value.\n");

    if (stored_val->address->zip_code != 10100) {  // 10000 + 100
        free_test_struct(&original_key);
        free_test_struct(&original_val);
        map_free(&m);
        return (test_res){(char*)__func__, "Deep copy failed - value address changed", CS_ELEM};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Value address zip code is correct, indicating deep copy of nested data.\n");

    free_test_struct(&original_key);
    free_test_struct(&original_val);
    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress tests with RBT integrity
// ============================================================================
test_res test_map_stress_insert_delete(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());
    int total = __TEST_SIZE;

    for (int i = 0; i < total; i++) {
        test_struct key = create_test_struct(i, "StressKey", (double)i);
        test_struct val = create_test_struct(i * 2, "StressVal", (double)(i * 2));
        cs_codes ins_result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted %d key-value pairs for stress test.\n", total);

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT invalid after stress inserts", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "RBT structure valid after stress inserts.\n");

    for (int i = 0; i < total / 2; i++) {
        test_struct key = create_test_struct(i, "StressKey", (double)i);
        cs_codes del_result = map_delete(&m, &key);
        free_test_struct(&key);
        if (del_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Deleted %d key-value pairs for stress test.\n", total / 2);

    if (!rbt_is_valid(m.t)) {
        map_free(&m);
        return (test_res){(char*)__func__, "RBT invalid after stress deletes", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "RBT structure valid after stress deletes.\n");

    if (map_size(m) != total - total / 2) {
        map_free(&m);
        return (test_res){(char*)__func__, "Size mismatch after stress test", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map size correct after stress test.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_interleaved_insert_delete(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "InterleavedKey", (double)i);
        test_struct val = create_test_struct(i * 5, "InterleavedVal", (double)(i * 5));
        cs_codes ins_result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }

        if (i % 3 == 0 && i > 0) {
            test_struct del_key = create_test_struct(i - 1, "InterleavedKey", (double)(i - 1));
            cs_codes del_result = map_delete(&m, &del_key);
            free_test_struct(&del_key);
            if (del_result != CS_SUCCESS) {
                map_free(&m);
                return (test_res){(char*)__func__, "Delete failed", del_result};
            }
        }

        if (!rbt_is_valid(m.t)) {
            map_free(&m);
            return (test_res){(char*)__func__, "RBT invalid during interleaved ops", CS_UNKNOWN};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Completed interleaved inserts and deletes.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_map_delete_all_verify_rbt(test_arg *arg) {
    map m;
    map_init(&m, get_test_struct_attr(), get_test_struct_attr());

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "DeleteAllKey", (double)i);
        test_struct val = create_test_struct(i * 12, "DeleteAllVal", (double)(i * 12));
        cs_codes ins_result = map_insert(&m, &key, &val);
        free_test_struct(&key);
        free_test_struct(&val);
        if (ins_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Insert failed", ins_result};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Inserted 100 key-value pairs for delete all test.\n");

    for (int i = 0; i < 100; i++) {
        test_struct key = create_test_struct(i, "DeleteAllKey", (double)i);
        cs_codes del_result = map_delete(&m, &key);
        free_test_struct(&key);
        if (del_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Delete failed", del_result};
        }

        if (!rbt_is_valid(m.t)) {
            map_free(&m);
            return (test_res){(char*)__func__, "RBT invalid during sequential deletes", CS_UNKNOWN};
        }
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Deleted all key-value pairs and verified RBT integrity after each delete.\n");

    if (!map_empty(m)) {
        map_free(&m);
        return (test_res){(char*)__func__, "Map not empty after deleting all", CS_UNKNOWN};
    }

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Map is empty after deleting all pairs as expected.\n");

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stress test with timing
// ============================================================================
test_res test_map_stress_time(test_arg *arg) {
    if (RUNNING_ON_VALGRIND) {
        return (test_res){(char*)__func__, "Valgrind active - skipping stress test", CS_SUCCESS};
    }

    if (arg->op_time_count != 3) {
        return (test_res){(char*)__func__, "Expected 3 timing slots for insert, find, delete", CS_UNKNOWN};
    }

    map m;
    struct timeval start, end;
    double elapsed;

    if (map_init(&m, get_int_attr(), get_test_struct_attr()) != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Map initialization failed", CS_UNKNOWN};
    }
    int total = __MAP_STRESS_TEST_SIZE;

    /* INSERT timing */
    gettimeofday(&start, NULL);
    for (int i = 0; i < total; i++) {
        int key = i;
        test_struct val = create_test_struct(i, "StressVal", (double)(i * 3));
        cs_codes result = map_insert(&m, &key, &val);
        free_test_struct(&val);

        if (result != CS_SUCCESS) {
            map_free(&m);
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
        void *found = map_find(m, &key);

        if (found == NULL) {
            map_free(&m);
            return (test_res){(char*)__func__, "Find failed during stress test", CS_ELEM};
        }

        test_struct *found_val = (test_struct*)found;
        if (found_val->id != i) {
            map_free(&m);
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
        cs_codes del_result = map_delete(&m, &key);

        if (del_result != CS_SUCCESS) {
            map_free(&m);
            return (test_res){(char*)__func__, "Delete failed during stress test", del_result};
        }
    }
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    post_operation_time(arg, "delete", elapsed);

    clogger_log((*arg->logger), CLOGGER_DEBUG, "Stress test completed: Total Delete Time = %.9f sec\n", elapsed);

    map_free(&m);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test map_tests[] = {
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
    test_map_delete_all_verify_rbt,

    // Stress test with timing
    test_map_stress_time,
};