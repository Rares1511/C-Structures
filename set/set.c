#include <cs/set.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../include/unittest.h"

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER STRUCT SECTION                                        ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


typedef struct {
    set_node *node;
    int tab_size;
} set_print_stack_item;


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER STRUCT SECTION                                         ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*! 
 * Frees a set node and its associated data
 * @param[in] s - pointer to the set containing the node
 * @param[in] node - pointer to the set node to be freed
 */
set_node* set_node_init(void *data, set_attr_t attr) {
    set_node *node = malloc(sizeof(set_node));
    if (node == NULL) {
        return NULL;
    }

    node->data = malloc(attr.size);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    if (attr.copy != NULL) {
        attr.copy(node->data, data);
    } else {
        memcpy(node->data, data, attr.size);
    }

    node->color = SET_NODE_RED_COLOR;
    node->left_child = NULL;
    node->right_child = NULL;
    node->father = NULL;

    return node;
}

/*!
 * Frees a set node and its associated data
 * @param[in] node - pointer to the set node to be freed
 * @param[in] attr - attributes of the set datatype
 */
void set_node_free(set_node *node, set_attr_t attr) {
    if (attr.fr != NULL) {
        attr.fr(node->data);
    }
    free(node->data);
    free(node);
}

/*!
 * Compares a set node's data with a given data using the set's comparison function
 * or the universal comparison if no custom function is provided
 * @param[in] s - pointer to the set containing the node
 * @param[in] data1 - pointer to the first data to compare
 * @param[in] data2 - pointer to the second data to compare
 * @return Negative value if data1 is less than data2,
 *         positive value if data1 is greater than data2,
 *         zero if they are equal
 */
int set_node_compare(set s, void *data1, void *data2) {
    if (s.attr.comp == NULL) {
        return memcmp(data1, data2, s.attr.size);
    }
    return s.attr.comp(data1, data2);
}

/*!
 * Finds a set node by its data using standard binary search tree search
 * @param[in] s - pointer to the set
 * @param[in] data - pointer to the data
 * @return Pointer to the found set node or NULL if not found
 */
set_node *set_node_find(set s, void *data) {
    set_node *node = s.root;

    while (node != NULL) {
        int cmp = set_node_compare(s, data, node->data);
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
 * @return Pointer to the minimum set node in the subtree
 */
set_node *set_node_minimum(set_node *node) {
    while (node->left_child != NULL) {
        node = node->left_child;
    }
    return node;
}

/*!
 * Compares a set node's data with a given data using the set's comparison function
 * or the universal comparison if no custom function is provided
 * @param[in] s - pointer to the set containing the node
 * @param[in] data1 - pointer to the first data to compare
 * @param[in] data2 - pointer to the second data to compare
 * @return Negative value if data1 is less than data2,
 *         positive value if data1 is greater than data2,
 *         zero if they are equal
 */
cs_codes set_insert_standard(set *s, set_node *new_node) {
    set_node *node = s->root;
    set_node *prev = NULL;

    while (node != NULL) {
        int cmp = set_node_compare(*s, new_node->data, node->data);
        prev = node;
        if (cmp == 0) {
            return CS_ELEM;
        } else if (cmp < 0) {
            node = node->left_child;
        } else {
            node = node->right_child;
        }
    }

    if (s->size == 0) {
        s->root = new_node;
    } else {
        int cmp = set_node_compare(*s, new_node->data, prev->data);
        if (cmp < 0) {
            prev->left_child = new_node;
        } else {
            prev->right_child = new_node;
        }
        new_node->father = prev;
    }

    s->size++;

    return CS_SUCCESS;
}

/*!
 * Transplants one subtree in place of another in the set
 * @param[in] s - pointer to the set
 * @param[in] u - pointer to the subtree to be replaced
 * @param[in] v - pointer to the subtree to replace with
 */ 
void set_transplant(set *s, set_node *u, set_node *v) {
    if (u->father == NULL) {
        s->root = v;
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
 * @param[in] s - pointer to the set
 * @param[in] x - pointer to the node around which to perform the rotation
 */
void set_left_rotate(set *s, set_node *x) {
    set_node *y = x->right_child;
    x->right_child = y->left_child;
    if (y->left_child != NULL) {
        y->left_child->father = x;
    }
    y->father = x->father;
    if (x->father == NULL) {
        s->root = y;
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
 * @param[in] s - pointer to the set
 * @param[in] x - pointer to the node around which to perform the rotation
 */
void set_right_rotate(set *s, set_node *x) {
    set_node *y = x->left_child;
    x->left_child = y->right_child;
    if (y->right_child != NULL) {
        y->right_child->father = x;
    }
    y->father = x->father;
    if (x->father == NULL) {
        s->root = y;
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
 * @param[in] s - pointer to the set
 * @param[in] node - pointer to the newly inserted node
 * @return cs_codes Status code indicating success or type of error
 */
cs_codes set_insert_fixup(set *s, set_node *node) {
    set_node *uncle;

    while (node->father && node->father->color == SET_NODE_RED_COLOR) {
        if (node->father->father && node->father == node->father->father->left_child) {
            uncle = node->father->father->right_child;

            // Case 1: Uncle is red -> recolor
            if (uncle && uncle->color == SET_NODE_RED_COLOR) {
                node->father->color = SET_NODE_BLACK_COLOR;
                uncle->color = SET_NODE_BLACK_COLOR;
                node->father->father->color = SET_NODE_RED_COLOR;
                node = node->father->father;
            }
            // Case 2: Uncle is black and node is right child -> rotate
            else {
                if (node == node->father->right_child) {
                    node = node->father;
                    set_left_rotate(s, node);
                }

                // Case 3: Uncle is black and node is left child -> rotate and recolor
                node->father->color = SET_NODE_BLACK_COLOR;
                node->father->father->color = SET_NODE_RED_COLOR;
                set_right_rotate(s, node->father->father);
            }
        } else {
            uncle = node->father->father->left_child;

            // Case 1: Uncle is red -> recolor
            if (uncle && uncle->color == SET_NODE_RED_COLOR) {
                node->father->color = SET_NODE_BLACK_COLOR;
                uncle->color = SET_NODE_BLACK_COLOR;
                node->father->father->color = SET_NODE_RED_COLOR;
                node = node->father->father;
            }
            // Case 2: Uncle is black and node is left child -> rotate
            else {
                if (node == node->father->left_child) {
                    node = node->father;
                    set_right_rotate(s, node);
                }

                // Case 3: Uncle is black and node is right child -> rotate and recolor
                node->father->color = SET_NODE_BLACK_COLOR;
                node->father->father->color = SET_NODE_RED_COLOR;
                set_left_rotate(s, node->father->father);
            }
        }
    }

    s->root->color = SET_NODE_BLACK_COLOR;

    return CS_SUCCESS;
}

/*!
 * Fixes the red-black tree properties after deletion
 * @param[in] s - pointer to the set
 * @param[in] x - pointer to the node to start fixup from
 * @param[in] x_father - pointer to the father of node x
 * @return CS_SUCCESS on success
 */
cs_codes set_delete_fixup(set *s, set_node *x, set_node *x_father) {
    set_node *w;

    while (x != s->root && (x == NULL || x->color == SET_NODE_BLACK_COLOR)) {
        if (x == x_father->left_child) {
            w = x_father->right_child;

            // Case 1: Sibling is red
            if (w != NULL && w->color == SET_NODE_RED_COLOR) {
                w->color = SET_NODE_BLACK_COLOR;
                x_father->color = SET_NODE_RED_COLOR;
                set_left_rotate(s, x_father);
                w = x_father->right_child;
            }

            if (w != NULL && ((w->left_child == NULL || w->left_child->color == SET_NODE_BLACK_COLOR) &&
                (w->right_child == NULL || w->right_child->color == SET_NODE_BLACK_COLOR))) {
                // Case 2: Sibling is black with two black children
                w->color = SET_NODE_RED_COLOR;
                x = x_father;
                x_father = x->father;
            } else {
                if (w != NULL && (w->right_child == NULL || w->right_child->color == SET_NODE_BLACK_COLOR)) {
                    // Case 3: Sibling is black with left red child and right black child
                    if (w->left_child != NULL) {
                        w->left_child->color = SET_NODE_BLACK_COLOR;
                    }
                    w->color = SET_NODE_RED_COLOR;
                    set_right_rotate(s, w);
                    w = x_father->right_child;
                }

                // Case 4: Sibling is black with right red child
                if (w != NULL) {
                    w->color = x_father->color;
                    if (w->right_child != NULL) {
                        w->right_child->color = SET_NODE_BLACK_COLOR;
                    }
                }
                x_father->color = SET_NODE_BLACK_COLOR;
                set_left_rotate(s, x_father);
                x = s->root;
            }
        } else {
            w = x_father->left_child;

            // Case 1: Sibling is red
            if (w != NULL && w->color == SET_NODE_RED_COLOR) {
                w->color = SET_NODE_BLACK_COLOR;
                x_father->color = SET_NODE_RED_COLOR;
                set_right_rotate(s, x_father);
                w = x_father->left_child;
            }

            if (w != NULL && ((w->right_child == NULL || w->right_child->color == SET_NODE_BLACK_COLOR) &&
                (w->left_child == NULL || w->left_child->color == SET_NODE_BLACK_COLOR))) {
                // Case 2: Sibling is black with two black children
                w->color = SET_NODE_RED_COLOR;
                x = x_father;
                x_father = x->father;
            } else {
                if (w != NULL && (w->left_child == NULL || w->left_child->color == SET_NODE_BLACK_COLOR)) {
                    // Case 3: Sibling is black with right red child and left black child
                    if (w->right_child != NULL) {
                        w->right_child->color = SET_NODE_BLACK_COLOR;
                    }
                    w->color = SET_NODE_RED_COLOR;
                    set_left_rotate(s, w);
                    w = x_father->left_child;
                }

                // Case 4: Sibling is black with left red child
                if (w != NULL) {
                    w->color = x_father->color;
                    if (w->left_child != NULL) {
                        w->left_child->color = SET_NODE_BLACK_COLOR;
                    }
                }
                x_father->color = SET_NODE_BLACK_COLOR;
                set_right_rotate(s, x_father);
                x = s->root;
            }
        }
    }

    if (x != NULL) {
        x->color = SET_NODE_BLACK_COLOR;
    }

    return CS_SUCCESS;
}

/*!
 * Deletes a set node from the set using the standard red-black tree deletion algorithm
 * @param[in] s - pointer to the set
 * @param[in] delete_node - pointer to the set node to be deleted
 * @return CS_SUCCESS on success
 */
cs_codes set_delete_standard(set *s, set_node *delete_node) {
    set_node *y = delete_node, *x, *x_father;
    char original_color = y->color;

    if (delete_node->left_child == NULL) {
        x = delete_node->right_child;
        x_father = delete_node->father;
        set_transplant(s, delete_node, delete_node->right_child);
    } else if (delete_node->right_child == NULL) {
        x = delete_node->left_child;
        x_father = delete_node->father;
        set_transplant(s, delete_node, delete_node->left_child);
    } else {
        y = set_node_minimum(delete_node->right_child);
        original_color = y->color;
        x = y->right_child;
        x_father = y->father;

        if (y->father == delete_node) {
            x_father = y;
        } else {
            set_transplant(s, y, y->right_child);
            y->right_child = delete_node->right_child;
            y->right_child->father = y;
        }
        set_transplant(s, delete_node, y);
        y->left_child = delete_node->left_child;
        y->left_child->father = y;
        y->color = delete_node->color;
    }

    set_node_free(delete_node, s->attr);
    s->size--;

    if (original_color == SET_NODE_BLACK_COLOR) {
        return set_delete_fixup(s, x, x_father);
    }

    return CS_SUCCESS;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes set_init(set *s, set_attr_t attr) {
    if (s == NULL) {
        return CS_ELEM;
    }
    if (attr.size < 0 || attr.size > SIZE_TH) {
        return CS_SIZE;
    }

    s->root = NULL;
    s->size = 0;
    s->attr = attr;

    return CS_SUCCESS;
}

cs_codes set_insert(set *s, void *data) {
    int rc;
    set_node *new_node;

    new_node = set_node_init(data, s->attr);
    if (new_node == NULL) {
        return CS_MEM;
    }

    rc = set_insert_standard(s, new_node);
    if (rc != CS_SUCCESS) {
        set_node_free(new_node, s->attr);
        return rc;
    }

    rc = set_insert_fixup(s, new_node);
    if (rc != CS_SUCCESS) {
        return rc;
    }

    return CS_SUCCESS;
}

cs_codes set_delete(set *s, void *data) {
    int rc;
    set_node *delete_node = set_node_find(*s, data);

    if (delete_node == NULL) {
        return CS_ELEM;
    }

    rc = set_delete_standard(s, delete_node);
    if (rc != CS_SUCCESS) {
        return rc;
    }

    return CS_SUCCESS;
}

void* set_find(set s, void *data) {
    set_node *node = set_node_find(s, data);
    if (node == NULL) {
        return NULL;
    }
    return node->data;
}

void set_swap(set *s1, set *s2) {
    set_node *temp_root = s1->root;
    size_t temp_size = s1->size;
    set_attr_t temp_attr = s1->attr;

    s1->root = s2->root;
    s1->size = s2->size;
    s1->attr = s2->attr;

    s2->root = temp_root;
    s2->size = temp_size;
    s2->attr = temp_attr;
}

void set_clear(set *s) {
    set_node *node = s->root, *next;

    while (node != NULL) {
        if (node->left_child != NULL) {
            node = node->left_child;
        } else if (node->right_child != NULL) {
            node = node->right_child;
        } else {
            next = node->father;
            set_node_free(node, s->attr);
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

    s->root = NULL;
    s->size = 0;
}

void set_print(void *v_s) {
    set *s = (set *)v_s;
    
    set_print_stack_item *stack = malloc(sizeof(set_print_stack_item) * (int)ceil(log2(s->size + 1)));
    int stack_size = 0;

    stack[stack_size++] = (set_print_stack_item){s->root, 0};

    while (stack_size > 0) {
        set_print_stack_item item = stack[--stack_size];
        set_node *node = item.node;
        int tab_size = item.tab_size;

        for (int i = 0; i < tab_size; i++) {
            fprintf(s->attr.stream, "  ");
        }

        fprintf(s->attr.stream, "Data: ");
        if (s->attr.print) {
            s->attr.print(s->attr.stream, node->data);
        }

        fprintf(s->attr.stream, " -> Color: %s", node->color == SET_NODE_RED_COLOR ? "RED" : "BLACK");

        fprintf(s->attr.stream, "\n");

        if (node->right_child != NULL)
            stack[stack_size++] = (set_print_stack_item){node->right_child, tab_size + 1};
        if (node->left_child != NULL)
            stack[stack_size++] = (set_print_stack_item){node->left_child, tab_size + 1};
    }

    free(stack);
}

void set_free(void *v_s) {
    set *s = (set *)v_s;
    set_clear(s);
}