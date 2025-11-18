#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "../include/map.h"
#include "../include/unittest.h"

FILE *DEBUG_OUT = NULL;


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       START OF HELPER FUNCTIONS SECTION                                    ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


int rbt_check_black_height(map_node *node) {
    if (node == NULL) {
        // NULL leaves count as a black node in many definitions.
        // If you prefer not to, return 0 here and adjust logic accordingly.
        return 1;
    }

    int left_bh  = rbt_check_black_height(node->left_child);
    int right_bh = rbt_check_black_height(node->right_child);

    // If any subtree is already invalid, propagate error
    if (left_bh == -1 || right_bh == -1)
        return -1;

    // Property: both subtrees must have same black height
    if (left_bh != right_bh)
        return -1;

    // Property: red node cannot have red children
    if (node->color == RED) {
        if ((node->left_child && node->left_child->color == RED) ||
            (node->right_child && node->right_child->color == RED)) {
            return -1;
        }
    }

    // Add 1 if this node is black
    return left_bh + (node->color == BLACK ? 1 : 0);
}

int rbt_check_bst(map_node *node, map_attr_t key_attr, void *min_key, void *max_key) {
    if (!node) return 1;

    // If min_key != NULL, node->key must be > min_key
    if (min_key) {
        if (key_attr.comp && key_attr.comp(node->key, min_key) <= 0)
            return 0;
        else if (universal_compare(node->key, min_key, key_attr.size) <= 0)
            return 0;
    }

    // If max_key != NULL, node->key must be < max_key
    if (max_key) {
        if (key_attr.comp && key_attr.comp(node->key, max_key) >= 0)
            return 0;
        else if (universal_compare(node->key, max_key, key_attr.size) >= 0)
            return 0;
    }

    // Left: all keys < node->key
    if (!rbt_check_bst(node->left_child, key_attr, min_key, node->key))
        return 0;

    // Right: all keys > node->key
    if (!rbt_check_bst(node->right_child, key_attr, node->key, max_key))
        return 0;

    return 1;
}

int rbt_is_valid(map_node *root, map_attr_t key_attr) {
    // 1. Root must be black (if non-empty)
    if (root && root->color != BLACK)
        return 0;

    // 2. BST property
    if (!rbt_check_bst(root, key_attr, NULL, NULL))
        return 0;

    // 3. Red–Black properties + black height
    int bh = rbt_check_black_height(root);
    if (bh == -1)
        return 0;

    return 1;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


test_res test_map_init() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_init", .reason = "map_init failed", .return_code = rc};
    }

    map_free(&m);
    if (m.root != NULL) {
        return (test_res){.test_name = "test_map_init",
                          .reason = "map_free failed",
                          .return_code = CS_UNKNOWN};
    }

    if (rbt_is_valid(m.root, key_attr) == 0) {
        return (test_res){
            .test_name = "test_map_init", .reason = "RBT properties violated", .return_code = CS_UNKNOWN};
    }

    return (test_res){.test_name = "test_map_init", .reason = "none", .return_code = CS_SUCCESS};
};

test_res test_map_insert_1() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    int test_size = 100;

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_insert_1", .reason = "map_init failed", .return_code = rc};
    }

    for (int i = 0; i < test_size; i++) {
        int key = rand() % (test_size * 10);
        int value = rand() % 100000;
        rc = map_insert(&m, &key, &value);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_insert_1", .reason = "map_insert failed", .return_code = rc};
        }
    }

    if (rbt_is_valid(m.root, key_attr) == 0) {
        return (test_res){
            .test_name = "test_map_insert_1", .reason = "RBT properties violated", .return_code = CS_UNKNOWN};
    }

    map_free(&m);
    return (test_res){.test_name = "test_map_insert_1", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_map_insert_2() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    int test_size = 10000;

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_insert_2", .reason = "map_init failed", .return_code = rc};
    }

    for (int i = 0; i < test_size; i++) {
        int key = rand() % (test_size * 10);
        int value = rand() % 100000;
        rc = map_insert(&m, &key, &value);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_insert_2", .reason = "map_insert failed", .return_code = rc};
        }
    }

    if (rbt_is_valid(m.root, key_attr) == 0) {
        return (test_res){
            .test_name = "test_map_insert_2", .reason = "RBT properties violated", .return_code = CS_UNKNOWN};
    }

    map_free(&m);
    return (test_res){.test_name = "test_map_insert_2", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_map_insert_3() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    int test_size = 200000;

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_insert_3", .reason = "map_init failed", .return_code = rc};
    }

    for (int i = 0; i < test_size; i++) {
        int key = rand() % (test_size * 10);
        int value = rand() % 100000;
        rc = map_insert(&m, &key, &value);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_insert_3", .reason = "map_insert failed", .return_code = rc};
        }
    }

    if (rbt_is_valid(m.root, key_attr) == 0) {
        return (test_res){
            .test_name = "test_map_insert_3", .reason = "RBT properties violated", .return_code = CS_UNKNOWN};
    }

    map_free(&m);
    return (test_res){.test_name = "test_map_insert_3", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_map_get() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    int test_size = 100, key_save, value_save, retrieved_value, key_idx = rand() % test_size;

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_get", .reason = "map_init failed", .return_code = rc};
    }

    for (int i = 0; i < test_size; i++) {
        int key = rand() % (test_size * 10);
        int value = rand() % 100000;
        if (i == key_idx) {
            key_save = key;
            value_save = value;
        }
        rc = map_insert(&m, &key, &value);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_get", .reason = "map_insert failed", .return_code = rc};
        }
    }

    rc = map_get(m, &key_save, &retrieved_value);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_get", .reason = "map_get failed", .return_code = rc};
    }

    if (retrieved_value !=  value_save) {
        return (test_res){
            .test_name = "test_map_get", .reason = "retrieved value does not match inserted value", .return_code = CS_UNKNOWN};
    }

    if (rbt_is_valid(m.root, key_attr) == 0) {
        return (test_res){
            .test_name = "test_map_get", .reason = "RBT properties violated", .return_code = CS_UNKNOWN};
    }

    map_free(&m);
    return (test_res){.test_name = "test_map_get", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_map_delete() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    int test_size = 100, key_save, key_idx = rand() % test_size;

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_get", .reason = "map_init failed", .return_code = rc};
    }

    for (int i = 0; i < test_size; i++) {
        int key = rand() % (test_size * 10);
        int value = rand() % 100000;
        if (i == key_idx) {
            key_save = key;
        }
        rc = map_insert(&m, &key, &value);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_delete", .reason = "map_insert failed", .return_code = rc};
        }
    }

    rc = map_delete(&m, &key_save);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_delete", .reason = "map_delete failed", .return_code = rc};
    }

    rc = map_get(m, &key_save, NULL);
    if (rc != CS_ELEM) {
        return (test_res){
            .test_name = "test_map_delete", .reason = "deleted key still found in map", .return_code = CS_UNKNOWN};
    }

    if (rbt_is_valid(m.root, key_attr) == 0) {
        return (test_res){
            .test_name = "test_map_delete", .reason = "RBT properties violated", .return_code = CS_UNKNOWN};
    }

    map_free(&m);
    return (test_res){.test_name = "test_map_delete", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_map_clear() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    int test_size = 100;

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_clear", .reason = "map_init failed", .return_code = rc};
    }

    for (int i = 0; i < test_size; i++) {
        int key = rand() % (test_size * 10);
        int value = rand() % 100000;
        rc = map_insert(&m, &key, &value);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_clear", .reason = "map_insert failed", .return_code = rc};
        }
    }

    map_clear(&m);
    if (m.size != 0 || m.root != NULL) {
        return (test_res){
            .test_name = "test_map_clear", .reason = "map_clear failed", .return_code = CS_UNKNOWN};
    }

    map_free(&m);
    return (test_res){.test_name = "test_map_clear", .reason = "none", .return_code = CS_SUCCESS};
}

test_res test_map_swap() {
    map m1, m2;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL, DEBUG_OUT};
    int test_size = 10000;
    int keys1[test_size], values1[test_size];
    int keys2[test_size], values2[test_size];

    cs_codes rc = map_init(&m1, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_swap", .reason = "map_init m1 failed", .return_code = rc};
    }

    rc = map_init(&m2, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_swap", .reason = "map_init m2 failed", .return_code = rc};
    }

    for (int i = 0; i < test_size; i++) {
        int key1 = rand() % (test_size * 10);
        int value1 = rand() % 100000;
        rc = map_insert(&m1, &key1, &value1);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_swap", .reason = "map_insert m1 failed", .return_code = rc};
        }
        if (rc == CS_SUCCESS) {
            keys1[m1.size - 1] = key1;
            values1[m1.size - 1] = value1;
        }

        int key2 = rand() % (test_size * 10);
        int value2 = rand() % 100000;
        rc = map_insert(&m2, &key2, &value2);
        if (rc != CS_SUCCESS && rc != CS_ELEM) {
            return (test_res){
                .test_name = "test_map_swap", .reason = "map_insert m2 failed", .return_code = rc};
        }
        if (rc == CS_SUCCESS) {
            keys2[m2.size - 1] = key2;
            values2[m2.size - 1] = value2;
        }
    }

    map_swap(&m1, &m2);

    if (rbt_is_valid(m1.root, key_attr) == 0 || rbt_is_valid(m2.root, key_attr) == 0) {
        return (test_res){
            .test_name = "test_map_swap", .reason = "RBT properties violated after swap", .return_code = CS_UNKNOWN};
    }

    for (int i = 0; i < m1.size; i++) {
        int retrieved_value;
        rc = map_get(m1, &keys2[i], &retrieved_value);
        if (rc != CS_SUCCESS || retrieved_value != values2[i]) {
            return (test_res){
                .test_name = "test_map_swap", .reason = "m1 contents incorrect after swap", .return_code = CS_UNKNOWN};
        }
    }

    for (int i = 0; i < m2.size; i++) {
        int retrieved_value;
        rc = map_get(m2, &keys1[i], &retrieved_value);
        if (rc != CS_SUCCESS || retrieved_value != values1[i]) {
            return (test_res){
                .test_name = "test_map_swap", .reason = "m2 contents incorrect after swap", .return_code = CS_UNKNOWN};
        }
    }

    map_free(&m1);
    map_free(&m2);
    return (test_res){.test_name = "test_map_swap", .reason = "none", .return_code = CS_SUCCESS};
}

int main(int argc, char **argv) {
    test tests[] = {
        test_map_init,
        test_map_insert_1,
        test_map_insert_2,
        test_map_insert_3,
        test_map_get,
        test_map_delete,
        test_map_clear,
        test_map_swap,
    };

    if (argc < 3) {
        printf("Usage: %s [debug_file] [seed]\n", argv[0]);
        return EXIT_FAILURE;
    } 

    DEBUG_OUT = fopen(argv[1], "w");
    if (DEBUG_OUT == NULL) {
        fprintf(DEBUG_OUT, "Failed to open debug file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    int seed = atoi(argv[2]);
    fprintf(DEBUG_OUT, "Using seed: %d\n", seed);
    srand(seed);

    unittest(tests, sizeof(tests) / sizeof(test));

    fclose(DEBUG_OUT);
    return 0;
}