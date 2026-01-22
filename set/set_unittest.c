#include <cs/set.h>
#include <cs/rbt.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       START OF HELPER FUNCTIONS SECTION                                    ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


int rbt_check_black_height(rbt_node *node) {
    if (node == NULL) {
        // NULL leaves count as a black node in many definitions.
        // If you prefer not to, return 0 here and adjust logic accordingly.
        return 1;
    }

    int left_bh  = rbt_check_black_height(node->left);
    int right_bh = rbt_check_black_height(node->right);

    // If any subtree is already invalid, propagate error
    if (left_bh == -1 || right_bh == -1)
        return -1;

    // Property: both subtrees must have same black height
    if (left_bh != right_bh)
        return -1;

    // Property: red node cannot have red children
    if (node->color == __RBT_NODE_RED_COLOR) {
        if ((node->left && node->left->color == __RBT_NODE_RED_COLOR) ||
            (node->right && node->right->color == __RBT_NODE_RED_COLOR)) {
            return -1;
        }
    }

    // Add 1 if this node is black
    return left_bh + (node->color == __RBT_NODE_BLACK_COLOR ? 1 : 0);
}

int rbt_check_bst(rbt_node *node, rbt_attr_t attr, void *min_key, void *max_key) {
    if (!node) return 1;

    // If min_key != NULL, node->key must be > min_key
    if (min_key) {
        if (attr.comp && attr.comp(node->data, min_key) <= 0)
            return 0;
        else if (memcmp(node->data, min_key, attr.size) <= 0)
            return 0;
    }

    // If max_key != NULL, node->key must be < max_key
    if (max_key) {
        if (attr.comp && attr.comp(node->data, max_key) >= 0)
            return 0;
        else if (memcmp(node->data, max_key, attr.size) >= 0)
            return 0;
    }

    // Left: all keys < node->key
    if (!rbt_check_bst(node->left, attr, min_key, node->data))
        return 0;

    // Right: all keys > node->key
    if (!rbt_check_bst(node->right, attr, node->data, max_key))
        return 0;

    return 1;
}

int rbt_is_valid(rbt *t) {
    // 1. Root must be black (if non-empty)
    if (t->root && t->root->color != __RBT_NODE_BLACK_COLOR)
        return 0;

    // 2. BST property
    if (!rbt_check_bst(t->root, t->attr, NULL, NULL))
        return 0;

    // 3. Red–Black properties + black height
    int bh = rbt_check_black_height(t->root);
    if (bh == -1)
        return 0;

    return 1;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


test_res test_set_init() {
    set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = print_int
    };

    set *s = set_init(attr);
    if (s == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "set_init returned NULL",
            .return_code = CS_UNKNOWN
        };
    };

    set_free(s);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    printf("Starting set unit tests...\n");
    test tests[] = {
        test_set_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}