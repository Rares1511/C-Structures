#include <cs/set.h>

#include "../include/unittest.h"

#define TEST_SIZE 2048
#define VALUE_RANGE 10000

FILE *DEBUG_OUT = NULL;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       START OF HELPER FUNCTIONS SECTION                                    ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


int rbt_check_black_height(set_node *node) {
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
    if (node->color == SET_NODE_RED_COLOR) {
        if ((node->left_child && node->left_child->color == SET_NODE_RED_COLOR) ||
            (node->right_child && node->right_child->color == SET_NODE_RED_COLOR)) {
            return -1;
        }
    }

    // Add 1 if this node is black
    return left_bh + (node->color == SET_NODE_BLACK_COLOR ? 1 : 0);
}

int rbt_check_bst(set_node *node, set_attr_t key_attr, void *min_key, void *max_key) {
    if (!node) return 1;

    // If min_key != NULL, node->key must be > min_key
    if (min_key) {
        if (key_attr.comp && key_attr.comp(node->data, min_key) <= 0)
            return 0;
        else if (memcmp(node->data, min_key, key_attr.size) <= 0)
            return 0;
    }

    // If max_key != NULL, node->key must be < max_key
    if (max_key) {
        if (key_attr.comp && key_attr.comp(node->data, max_key) >= 0)
            return 0;
        else if (memcmp(node->data, max_key, key_attr.size) >= 0)
            return 0;
    }

    // Left: all keys < node->key
    if (!rbt_check_bst(node->left_child, key_attr, min_key, node->data))
        return 0;

    // Right: all keys > node->key
    if (!rbt_check_bst(node->right_child, key_attr, node->data, max_key))
        return 0;

    return 1;
}

int rbt_is_valid(set_node *root, set_attr_t key_attr) {
    // 1. Root must be black (if non-empty)
    if (root && root->color != SET_NODE_BLACK_COLOR)
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


test_res test_set_init() {
    DEBUG_PRINT("Starting test_set_init...\n");
    set s;
    set_attr_t attr = { 
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .stream = DEBUG_OUT,
        .print = NULL
    };

    int rc = set_init(&s, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set_init failed",
            .return_code = rc,
        };
    }

    if (!rbt_is_valid(s.root, s.attr)) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set structure invalid after initialization",
            .return_code = CS_ELEM,
        };
    }

    set_free(&s);

    DEBUG_PRINT("test_set_init passed\n");

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_set_insert() {
    set s;
    set_attr_t attr = { 
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .stream = DEBUG_OUT,
        .print = NULL
    };
    int fr[VALUE_RANGE];
    memset(fr, 0, sizeof(fr));

    int rc = set_init(&s, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set_init failed",
            .return_code = rc,
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int value = rand() % VALUE_RANGE;
        while (fr[value]) {
            value = rand() % VALUE_RANGE;
        }
        fr[value] = 1;

        rc = set_insert(&s, &value);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "set_insert failed",
                .return_code = rc,
            };
        }

        if (!rbt_is_valid(s.root, s.attr)) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "set structure invalid after insertions",
                .return_code = CS_ELEM,
            };
        }
    }

    if (s.size != TEST_SIZE) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set size mismatch after insertions",
            .return_code = CS_SIZE,
        };
    }

    for (int i = 0; i < VALUE_RANGE; i++) {
        if (fr[i]) {
            int *found = (int *)set_find(s, &i);
            if (found == NULL || *found != i) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set_find failed to locate inserted element",
                    .return_code = CS_ELEM,
                };
            }
        }
    }

    if (!rbt_is_valid(s.root, s.attr)) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set structure invalid after insertions",
            .return_code = CS_ELEM,
        };
    }

    set_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_set_insert_duplicate() {
    set s;
    set_attr_t attr = { 
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .stream = DEBUG_OUT,
        .print = NULL
    };
    int fr[VALUE_RANGE], actual_size = 0;
    memset(fr, 0, sizeof(fr));

    int rc = set_init(&s, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set_init failed",
            .return_code = rc,
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int value = rand() % VALUE_RANGE;
        rc = set_insert(&s, &value);
        if (fr[value]) {
            if (rc != CS_ELEM) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "duplicate not identified correctly",
                    .return_code = rc,
                };
            }
        } else {
            actual_size++;
            fr[value] = 1;
            if (rc != CS_SUCCESS) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set_insert failed",
                    .return_code = rc,
                };
            }

            if (!rbt_is_valid(s.root, s.attr)) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set structure invalid after insertions",
                    .return_code = CS_ELEM,
                };
            }
        }
    }

    if (s.size != actual_size) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set size mismatch after insertions",
            .return_code = CS_SIZE,
        };
    }

    for (int i = 0; i < VALUE_RANGE; i++) {
        if (fr[i]) {
            int *found = (int *)set_find(s, &i);
            if (found == NULL || *found != i) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set_find failed to locate inserted element",
                    .return_code = CS_ELEM,
                };
            }
        }
    }

    set_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_set_delete() {
    set s;
    set_attr_t attr = { 
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .stream = DEBUG_OUT,
        .print = NULL
    };
    int fr[VALUE_RANGE];
    memset(fr, 0, sizeof(fr));

    int rc = set_init(&s, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set_init failed",
            .return_code = rc,
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int value = rand() % VALUE_RANGE;
        while (fr[value]) {
            value = rand() % VALUE_RANGE;
        }
        fr[value] = 1;

        rc = set_insert(&s, &value);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "set_insert failed",
                .return_code = rc,
            };
        }
    }

    for (int i = 0; i < VALUE_RANGE; i++) {
        if (fr[i]) {
            rc = set_delete(&s, &i);
            if (rc != CS_SUCCESS) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set_delete failed",
                    .return_code = rc,
                };
            }
            fr[i] = 0;

            int *found = (int *)set_find(s, &i);
            if (found != NULL) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set_find located deleted element",
                    .return_code = CS_ELEM,
                };
            }

            if (!rbt_is_valid(s.root, s.attr)) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set structure invalid after deletion",
                    .return_code = CS_ELEM,
                };
            }
        }
    }

    set_free(&s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

test_res test_set_swap() {
    set s1, s2;
    set_attr_t attr = { 
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .stream = DEBUG_OUT,
        .print = NULL
    };
    int fr1[VALUE_RANGE], fr2[VALUE_RANGE];
    memset(fr1, 0, sizeof(fr1));
    memset(fr2, 0, sizeof(fr2));

    int rc = set_init(&s1, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set_init failed for s1",
            .return_code = rc,
        };
    }

    rc = set_init(&s2, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "set_init failed for s2",
            .return_code = rc,
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int value1 = rand() % VALUE_RANGE;
        while (fr1[value1]) {
            value1 = rand() % VALUE_RANGE;
        }
        fr1[value1] = 1;

        int value2 = rand() % VALUE_RANGE;
        while (fr2[value2]) {
            value2 = rand() % VALUE_RANGE;
        }
        fr2[value2] = 1;

        rc = set_insert(&s1, &value1);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "set_insert failed for s1",
                .return_code = rc,
            };
        }

        if (!rbt_is_valid(s1.root, s1.attr)) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "set structure invalid after insertions in s1",
                .return_code = CS_ELEM,
            };
        }

        rc = set_insert(&s2, &value2);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "set_insert failed for s2",
                .return_code = rc,
            };
        }

        if (!rbt_is_valid(s2.root, s2.attr)) {
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "set structure invalid after insertions in s2",
                .return_code = CS_ELEM,
            };
        }
    }

    set_swap(&s1, &s2);

    for (int i = 0; i < VALUE_RANGE; i++) {
        if (fr1[i]) {
            int *found = (int *)set_find(s2, &i);
            if (found == NULL || *found != i) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set_find failed to locate element from s1 in s2 after swap",
                    .return_code = CS_ELEM,
                };
            }
        }
        if (fr2[i]) {
            int *found = (int *)set_find(s1, &i);
            if (found == NULL || *found != i) {
                return (test_res){
                    .test_name = (char *)__func__,
                    .reason = "set_find failed to locate element from s2 in s1 after swap",
                    .return_code = CS_ELEM,
                };
            }
        }
    }

    set_free(&s1);
    set_free(&s2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

int main(int argc, char **argv) {
    printf("Starting set unit tests...\n");
    test tests[] = {
        test_set_init,
        test_set_insert,
        test_set_insert_duplicate,
        test_set_delete,
        test_set_swap,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}