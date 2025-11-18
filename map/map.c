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
int map_node_compare(map m, void *key1, void *key2) {
    if (m.key_attr.comp == NULL) {
        return universal_compare(key1, key2, m.key_attr.size);
    }
    return m.key_attr.comp(key1, key2);
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
 * Finds a map node by its key using standard binary search tree search
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @return Pointer to the found map node or NULL if not found
 */
map_node *map_node_find(map m, void *key) {
    map_node *node = m.root;

    while (node != NULL) {
        int cmp = map_node_compare(m, key, node->key);
        if (cmp == 0) {
            return node;
        } else if (cmp < 0) {
            node = node->left_child;
        } else {
            node = node->right_child;
        }
    }

    return NULL;
}

/*!
 * Finds the minimum node in a subtree rooted at the given node
 * @param[in] node - pointer to the root of the subtree
 * @return Pointer to the minimum map node in the subtree
 */
map_node *map_node_minimum(map_node *node) {
    while (node->left_child != NULL) {
        node = node->left_child;
    }
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
        int cmp = map_node_compare(*m, new_node->key, node->key);
        prev = node;
        if (cmp == 0) {
            return CS_ELEM;
        } else if (cmp < 0) {
            node = node->left_child;
        } else {
            node = node->right_child;
        }
    }

    if (m->size == 0) {
        m->root = new_node;
    } else {
        int cmp = map_node_compare(*m, new_node->key, prev->key);
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
 * Transplants one subtree in place of another in the map
 * @param[in] m - pointer to the map
 * @param[in] u - pointer to the subtree to be replaced
 * @param[in] v - pointer to the subtree to replace with
 */
void map_transplant(map *m, map_node *u, map_node *v) {
    if (u->father == NULL) {
        m->root = v;
    } else if (u == u->father->left_child) {
        u->father->left_child = v;
    } else {
        u->father->right_child = v;
    }
    if (v != NULL) {
        v->father = u->father;
    }
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

/*!
 * Fixes the red-black tree properties after deletion
 * @param[in] m - pointer to the map
 * @param[in] x - pointer to the node that replaced the deleted node
 * @param[in] x_father - pointer to the father of node x
 * @return CS_SUCCESS on success
 */
cs_codes map_delete_fixup(map *m, map_node *x, map_node *x_father) {
    map_node *w;
    
    while (x != m->root && (x == NULL || x->color == BLACK)) {
        if (x == x->father->left_child) {
            w = x_father->right_child;

            // Case 1: Sibling is red
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                x_father->color = RED;
                map_left_rotate(m, x_father);
                w = x_father->right_child;
            }

            if (w != NULL && ((w->left_child == NULL || w->left_child->color == BLACK) &&
                (w->right_child == NULL || w->right_child->color == BLACK))) {
                // Case 2: Sibling is black with two black children
                w->color = RED;
                x = x_father;
                x_father = x->father;
            } else {
                if (w != NULL && (w->right_child == NULL || w->right_child->color == BLACK)) {
                    // Case 3: Sibling is black with left red child and right black child
                    if (w->left_child != NULL) {
                        w->left_child->color = BLACK;
                    }
                    w->color = RED;
                    map_right_rotate(m, w);
                    w = x_father->right_child;
                }

                // Case 4: Sibling is black with right red child
                if (w != NULL) {
                    w->color = x_father->color;
                    if (w->right_child != NULL) {
                        w->right_child->color = BLACK;
                    }
                }
                x_father->color = BLACK;
                map_left_rotate(m, x_father);
                x = m->root;
            }
        } else {
            w = x_father->left_child;

            // Case 1: Sibling is red
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                x_father->color = RED;
                map_right_rotate(m, x_father);
                w = x_father->left_child;
            }

            if (w != NULL && ((w->right_child == NULL || w->right_child->color == BLACK) &&
                (w->left_child == NULL || w->left_child->color == BLACK))) {
                // Case 2: Sibling is black with two black children
                w->color = RED;
                x = x_father;
                x_father = x->father;
            } else {
                if (w != NULL && (w->left_child == NULL || w->left_child->color == BLACK)) {
                    // Case 3: Sibling is black with right red child and left black child
                    if (w->right_child != NULL) {
                        w->right_child->color = BLACK;
                    }
                    w->color = RED;
                    map_left_rotate(m, w);
                    w = x_father->left_child;
                }

                // Case 4: Sibling is black with left red child
                if (w != NULL) {
                    w->color = x_father->color;
                    if (w->left_child != NULL) {
                        w->left_child->color = BLACK;
                    }
                }
                x_father->color = BLACK;
                map_right_rotate(m, x_father);
                x = m->root;
            }
        }
    }

    if (x != NULL) {
        x->color = BLACK;
    }

    return CS_SUCCESS;
}

/*! 
 * Deletes a map node from the map using standard binary search tree deletion
 * @param[in] m - pointer to the map
 * @param[in] delete_node - pointer to the map node to be deleted
 * @return CS_SUCCESS on success
 */
cs_codes map_delete_standard(map *m, map_node *delete_node) {
    map_node *y = delete_node, *x, *x_father;
    char original_color = y->color;

    if (delete_node->left_child == NULL) {
        x = delete_node->right_child;
        x_father = delete_node->father;
        map_transplant(m, delete_node, delete_node->right_child);
    } else if (delete_node->right_child == NULL) {
        x = delete_node->left_child;
        x_father = delete_node->father;
        map_transplant(m, delete_node, delete_node->left_child);
    } else {
        y = map_node_minimum(delete_node->right_child);
        original_color = y->color;
        x = y->right_child;
        x_father = y->father;

        if (y->father == delete_node) {
            x_father = y;
        } else {
            map_transplant(m, y, y->right_child);
            y->right_child = delete_node->right_child;
            y->right_child->father = y;
        }
        map_transplant(m, delete_node, y);
        y->left_child = delete_node->left_child;
        y->left_child->father = y;
        y->color = delete_node->color;
    }

    map_node_free(m, delete_node);
    m->size--;

    if (original_color == BLACK) {
        return map_delete_fixup(m, x, x_father);
    }

    return CS_SUCCESS;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


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
    map_node *node = map_node_find(m, key);
    if (node != NULL) {
        if (value == NULL) {
            return CS_SUCCESS;
        }
        if (m.val_attr.copy != NULL) {
            m.val_attr.copy(value, node->val);
        } else {
            memcpy(value, node->val, m.val_attr.size);
        }
        return CS_SUCCESS;
    }

    return CS_ELEM;
}

cs_codes map_delete(map *m, void *key) {
    int rc;
    map_node *delete_node = map_node_find(*m, key);

    if (delete_node == NULL) {
        return CS_ELEM;
    }

    rc = map_delete_standard(m, delete_node);
    if (rc != CS_SUCCESS) {
        return rc;
    }

    return CS_SUCCESS;
}

void map_swap(map *m1, map *m2) {
    map_node *temp_root = m1->root;
    size_t temp_size = m1->size;
    map_attr_t temp_key_attr = m1->key_attr;
    map_attr_t temp_val_attr = m1->val_attr;

    m1->root = m2->root;
    m1->size = m2->size;
    m1->key_attr = m2->key_attr;
    m1->val_attr = m2->val_attr;

    m2->root = temp_root;
    m2->size = temp_size;
    m2->key_attr = temp_key_attr;
    m2->val_attr = temp_val_attr;
}

void map_clear(map *m) {
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

void map_set_attr(map *m, map_attr_t key_attr, map_attr_t val_attr) {
    m->key_attr = key_attr;
    m->val_attr = val_attr;
}

void map_set_free(map *m, freer key_fr, freer val_fr) {
    m->key_attr.fr = key_fr;
    m->val_attr.fr = val_fr;
}

void map_set_comp(map *m, comparer key_comp, comparer val_comp) {
    m->key_attr.comp = key_comp;
    m->val_attr.comp = val_comp;
}

void map_set_print(map *m, printer key_print, printer val_print) {
    m->key_attr.print = key_print;
    m->val_attr.print = val_print;
}

void map_set_copy(map *m, deepcopy key_copy, deepcopy val_copy) {
    m->key_attr.copy = key_copy;
    m->val_attr.copy = val_copy;
}

void map_set_stream(map *m, FILE *key_stream, FILE *val_stream) {
    m->key_attr.stream = key_stream;
    m->val_attr.stream = val_stream;
}

void map_key_set_attr(map *m, map_attr_t key_attr) {
    m->key_attr = key_attr;
}

void map_key_set_free(map *m, freer key_fr) {
    m->key_attr.fr = key_fr;
}

void map_key_set_comp(map *m, comparer key_comp) {
    m->key_attr.comp = key_comp;
}

void map_key_set_print(map *m, printer key_print) {
    m->key_attr.print = key_print;
}

void map_key_set_copy(map *m, deepcopy key_copy) {
    m->key_attr.copy = key_copy;
}

void map_key_set_stream(map *m, FILE *key_stream) {
    m->key_attr.stream = key_stream;
}

void map_val_set_attr(map *m, map_attr_t val_attr) {
    m->val_attr = val_attr;
}

void map_val_set_free(map *m, freer val_fr) {
    m->val_attr.fr = val_fr;
}

void map_val_set_comp(map *m, comparer val_comp) {
    m->val_attr.comp = val_comp;
}

void map_val_set_print(map *m, printer val_print) {
    m->val_attr.print = val_print;
}   

void map_val_set_copy(map *m, deepcopy val_copy) {
    m->val_attr.copy = val_copy;
}

void map_val_set_stream(map *m, FILE *val_stream) {
    m->val_attr.stream = val_stream;
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