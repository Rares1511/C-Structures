#include <cs/map.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* HELPER FUNCTIONS */

/*!
 * Compares a map node's key with a given key using the map's key comparison function
 * or the universal comparison if no custom function is provided
 * @param[in] m - pointer to the map containing the node
 * @param[in] node - pointer to the map node
 * @param[in] key - pointer to the key data to compare
 * @return Negative value if node's key is less than the given key,
 *         positive value if node's key is greater than the given key,
 *         zero if they are equal
 */
int map_node_compare(map *m, void *key1, void *key2) {
    if (m->key_attr.comp == NULL) {
        return universal_compare(key1, key2, m->key_attr.size);
    }
    return m->key_attr.comp(key1, key2);
}

/*!
 * Initializes a new map node with the given key and value
 * @param[in] key - pointer to the key data
 * @param[in] value - pointer to the value data
 * @param[in] key_attr - attributes of the key datatype
 * @param[in] val_attr - attributes of the value datatype
 * @return Pointer to the newly created map node or NULL on memory allocation failure
 */
map_node *map_node_init(void *key, void *value, map_attr_t key_attr, map_attr_t val_attr) {
    map_node *node = (map_node *)malloc(sizeof(map_node));
    if (node == NULL) {
        return NULL;
    }
    node->color = RED;
    node->key = malloc(key_attr.size);
    if (node->key == NULL) {
        free(node);
        return NULL;
    }
    if (key_attr.copy != NULL) {
        key_attr.copy(node->key, key);
    } else {
        memcpy(node->key, key, key_attr.size);
    }
    node->val = malloc(val_attr.size);
    if (node->val == NULL) {
        free(node->key);
        free(node);
        return NULL;
    }
    if (val_attr.copy != NULL) {
        val_attr.copy(node->val, value);
    } else {
        memcpy(node->val, value, val_attr.size);
    }
    node->left_child = NULL;
    node->right_child = NULL;
    node->father = NULL;
    return node;
}

/*!
 * Inserts a new key-value pair into the map using standard binary search tree insertion
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @param[in] value - pointer to the value data
 * @return CS_SUCCESS on success, CS_ELEM if the key already exists, CS_MEM on memory allocation failure
 */
cs_codes map_insert_standard(map *m, map_node *new_node) {
    map_node *node = m->root;
    map_node *prev = NULL;

    printf("Standard inserting node with key: ");

    while (node != NULL) {
        int cmp = map_node_compare(m, node->key, new_node->key);
        if (cmp == 0) {
            return CS_ELEM;
        } else if (cmp < 0) {
            node = node->left_child;
        } else {
            node = node->right_child;
        }
        prev = node;
    }

    printf("Inserting node with key: ");

    if (m->size == 0) {
        m->root = new_node;
    } else {
        int cmp = map_node_compare(m, prev->key, new_node->key);
        if (cmp < 0) {
            prev->left_child = new_node;
        } else {
            prev->right_child = new_node;
        }
        new_node->father = prev;
    }

    m->size++;

    return CS_SUCCESS;
}

void map_left_rotate(map *m, map_node *x) {
    map_node *y = x->right_child;
    x->right_child = y->left_child;
    if (y->left_child != NULL) {
        y->left_child->father = x;
    }
    y->father = x->father;
    if (x->father == NULL) {
        m->root = y;
    } else if (x == x->father->left_child) {
        x->father->left_child = y;
    } else {
        x->father->right_child = y;
    }
    y->left_child = x;
    x->father = y;
}

void map_right_rotate(map *m, map_node *x) {
    map_node *y = x->left_child;
    x->left_child = y->right_child;
    if (y->right_child != NULL) {
        y->right_child->father = x;
    }
    y->father = x->father;
    if (x->father == NULL) {
        m->root = y;
    } else if (x == x->father->right_child) {
        x->father->right_child = y;
    } else {
        x->father->left_child = y;
    }
    y->right_child = x;
    x->father = y;
}

cs_codes map_insert_fixup(map *m, map_node *node) {
    map_node *uncle;

    while (node->father && node->father->color == RED) {
        if (node->father->father && node->father == node->father->father->left_child) {
            uncle = node->father->father->right_child;

            // Case 1: Uncle is red -> recolor
            if (uncle && uncle->color == RED) {
                node->father->color = BLACK;
                uncle->color = BLACK;
                node->father->father->color = RED;
                node = node->father->father;
            }
            // Case 2: Uncle is black and node is right child -> rotate
            else {
                if (node == node->father->right_child) {
                    node = node->father;
                    map_left_rotate(m, node);
                }

                // Case 3: Uncle is black and node is left child -> rotate and recolor
                node->father->color = BLACK;
                node->father->father->color = RED;
                map_right_rotate(m, node->father->father);
            }
        } else {
            uncle = node->father->father->left_child;

            // Case 1: Uncle is red -> recolor
            if (uncle && uncle->color == RED) {
                node->father->color = BLACK;
                uncle->color = BLACK;
                node->father->father->color = RED;
                node = node->father->father;
            }
            // Case 2: Uncle is black and node is left child -> rotate
            else {
                if (node == node->father->left_child) {
                    node = node->father;
                    map_right_rotate(m, node);
                }

                // Case 3: Uncle is black and node is right child -> rotate and recolor
                node->father->color = BLACK;
                node->father->father->color = RED;
                map_left_rotate(m, node->father->father);
            }
        }
    }

    m->root->color = BLACK;

    return CS_SUCCESS;
}

void map_node_print(map_node *node, map_attr_t key_attr, map_attr_t val_attr, int tab_size) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < tab_size; i++) {
        printf(" ");
    }
    printf("Key: ");
    key_attr.print(node->key);
    printf(", Value: ");
    val_attr.print(node->val);
    printf(", Color: %s\n", node->color == RED ? "RED" : "BLACK");

    map_node_print(node->left_child, key_attr, val_attr, tab_size + 2);
    map_node_print(node->right_child, key_attr, val_attr, tab_size + 2);
}

/* END OF HELPER FUNCTIONS */

cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr) {
    m->key_attr = key_attr;
    m->val_attr = val_attr;
    m->root = NULL;
    m->size = 0;

    return CS_SUCCESS;
}

cs_codes map_insert(map *m, void *key, void *value) {
    int rc;
    map_node *new_node;

    new_node = map_node_init(key, value, m->key_attr, m->val_attr);
    if (new_node == NULL) {
        return CS_MEM;
    }

    rc = map_insert_standard(m, new_node);
    if (rc != CS_SUCCESS) {
        return rc;
    }

    printf("After standard insert:\n");

    rc = map_insert_fixup(m, new_node);
    if (rc != CS_SUCCESS) {
        return rc;
    }

    return CS_SUCCESS;
}

cs_codes map_get(map m, void *key, void *value) {
    map_node *node = m.root;

    while (node != NULL) {
        int cmp = map_node_compare(&m, node->key, key);
        if (cmp == 0) {
            if (m.val_attr.copy != NULL) {
                m.val_attr.copy(value, node->val);
            } else {
                memcpy(value, node->val, m.val_attr.size);
            }
            return CS_SUCCESS;
        } else if (cmp < 0) {
            node = node->left_child;
        } else {
            node = node->right_child;
        }
    }

    return CS_ELEM;
}

void map_print(void *v_m) {
    map *m = (map *)v_m;
    map_node_print(m->root, m->key_attr, m->val_attr, 0);
}

void map_free(void *v_m) {
    map *m = (map *)v_m;

    m->root = NULL;
    m->size = 0;
}