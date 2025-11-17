#include <cs/map.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/unittest.h"


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER STRUCT SECTION                                        ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


typedef struct {
    map_node *node;
    int tab_size;
} map_print_stack_item;


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER STRUCT SECTION                                         ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝



// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*! 
 * Frees a map node and its associated key and value data
 * @param[in] m - pointer to the map containing the node
 * @param[in] node - pointer to the map node to be freed
 */
void map_node_free(map *m, map_node *node) {
    if (m->key_attr.fr != NULL) {
        m->key_attr.fr(node->key);
    }
    free(node->key);

    if (m->val_attr.fr != NULL) {
        m->val_attr.fr(node->val);
    }
    free(node->val);

    free(node);
}

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

    while (node != NULL) {
        int cmp = map_node_compare(m, new_node->key, node->key);
        DEBUG_PRINT("Comparing keys: %d and %d\n", *(int *)new_node->key, *(int *)node->key);
        prev = node;
        if (cmp == 0) {
            return CS_ELEM;
        } else if (cmp < 0) {
            DEBUG_PRINT("Going left from key: %d\n", *(int *)node->key);
            node = node->left_child;
        } else {
            DEBUG_PRINT("Going right from key: %d\n", *(int *)node->key);
            node = node->right_child;
        }
    }

    if (m->size == 0) {
        m->root = new_node;
    } else {
        int cmp = map_node_compare(m, new_node->key, prev->key);
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

/*!
 * Performs a left rotation around the given node
 * @param[in] m - pointer to the map
 * @param[in] x - pointer to the node around which to perform the rotation
 */
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

/*!
 * Performs a right rotation around the given node
 * @param[in] m - pointer to the map
 * @param[in] x - pointer to the node around which to perform the rotation
 */
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

/*! 
 * Fixes the red-black tree properties after insertion
 * @param[in] m - pointer to the map
 * @param[in] node - pointer to the newly inserted node
 * @return CS_SUCCESS on success
 */
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


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr) {
    DEBUG_PRINT("Initializing map\n");
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
        map_node_free(m, new_node);
        return rc;
    }

    rc = map_insert_fixup(m, new_node);
    if (rc != CS_SUCCESS) {
        return rc;
    }

    return CS_SUCCESS;
}

cs_codes map_get(map m, void *key, void *value) {
    map_node *node = m.root;

    while (node != NULL) {
        int cmp = map_node_compare(&m, key, node->key);
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
    
    map_print_stack_item *stack = malloc(sizeof(map_print_stack_item) * m->size);
    int stack_size = 0;

    stack[stack_size++] = (map_print_stack_item){m->root, 0};

    while (stack_size > 0) {
        map_print_stack_item item = stack[--stack_size];
        map_node *node = item.node;
        int tab_size = item.tab_size;

        for (int i = 0; i < tab_size; i++) {
            fprintf(m->key_attr.stream, "  ");
        }

        fprintf(m->key_attr.stream, "Key: ");
        if (m->key_attr.print) {
            m->key_attr.print(m->key_attr.stream, node->key);
        }

        fprintf(m->key_attr.stream, ", Value: ");
        if (m->val_attr.print) {
            m->val_attr.print(m->val_attr.stream, node->val);
        }

        fprintf(m->key_attr.stream, " -> Color: %s", node->color == RED ? "RED" : "BLACK");

        fprintf(m->key_attr.stream, "\n");

        if (node->right_child != NULL)
            stack[stack_size++] = (map_print_stack_item){node->right_child, tab_size + 1};
        if (node->left_child != NULL)
            stack[stack_size++] = (map_print_stack_item){node->left_child, tab_size + 1};
    }

    free(stack);
}

void map_free(void *v_m) {
    map *m = (map *)v_m;
    map_node *node = m->root, *next;

    while (node != NULL) {
        if (node->left_child != NULL) {
            node = node->left_child;
        } else if (node->right_child != NULL) {
            node = node->right_child;
        } else {
            next = node->father;
            map_node_free(m, node);
            if (next != NULL) {
                if (next->left_child == node) {
                    next->left_child = NULL;
                } else {
                    next->right_child = NULL;
                }
            }
            node = next;
        }
    }

    m->root = NULL;
    m->size = 0;
}