#ifndef __CS_RBTREE_H__
#define __CS_RBTREE_H__

#include <cs/universal.h>
#include <math.h>

#define __RBT_NODE_BLACK_COLOR 0
#define __RBT_NODE_RED_COLOR   1

#define CS_RBT_MAGIC 0x52425445 // "RBTE"

typedef struct __rbt_node {
    struct __rbt_node *left;
    struct __rbt_node *right;
    struct __rbt_node *father;
    char color;
    char data[];
} __rbt_node;

typedef struct __rbt {
    cs_header_t header;
    int size;
    __rbt_node *root;
    elem_attr_t attr;
} __rbt;

#pragma region Helper Structs
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER STRUCT SECTION                                        ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

typedef struct {
    __rbt_node *node;
    int tab_size;
} __rbt_print_stack_item;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER STRUCT SECTION                                         ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

/*! 
 * Frees a rbt node and its associated data
 * @param[in] s - pointer to the rbt containing the node
 * @param[in] node - pointer to the rbt node to be freed
 */
static inline __rbt_node* __rbt_node_init(const void *data, elem_attr_t attr) {
    __rbt_node *node = malloc(sizeof(__rbt_node) + attr.size);
    if (!node) return NULL;

    node->left = node->right = node->father = NULL;
    node->color = __RBT_NODE_RED_COLOR;

    if (data != NULL) {
        if (attr.copy) attr.copy(node->data, data);
        else memcpy(node->data, data, attr.size);
    }
    
    return node;
}

/*!
 * Frees a rbt node and its associated data
 * @param[in] node - pointer to the rbt node to be freed
 * @param[in] attr - attributes of the rbt datatype
 */
static inline void __rbt_node_free(__rbt_node *node, elem_attr_t attr) {
    if (attr.fr != NULL) {
        attr.fr(node->data);
    }
    free(node);
}

/*!
 * Compares a rbt node's data with a given data using the rbt's comparison function
 * or the universal comparison if no custom function is provided
 * @param[in] s - pointer to the rbt containing the node
 * @param[in] data1 - pointer to the first data to compare
 * @param[in] data2 - pointer to the second data to compare
 * @return Negative value if data1 is less than data2,
 *         positive value if data1 is greater than data2,
 *         zero if they are equal
 */
static inline int __rbt_node_compare(__rbt *s, void *data1, void *data2) {
    if (s->attr.comp == NULL) {
        return memcmp(data1, data2, s->attr.size);
    }
    return s->attr.comp(data1, data2);
}

/*!
 * Finds a rbt node by its data using standard binary search tree search
 * @param[in] s - pointer to the rbt
 * @param[in] data - pointer to the data
 * @return Pointer to the found rbt node or NULL if not found
 */
static inline __rbt_node *__rbt_node_find(__rbt *s, void *data) {
    __rbt_node *node = s->root;

    while (node != NULL) {
        int cmp = __rbt_node_compare(s, data, node->data);
        if (cmp == 0) {
            return node;
        } else if (cmp < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return NULL;
}

/*!
 * Finds the minimum node in a subtree rooted at the given node
 * @param[in] node - pointer to the root of the subtree
 * @return Pointer to the minimum rbt node in the subtree
 */
static inline __rbt_node *__rbt_node_minimum(__rbt_node *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/*!
 * Transplants one subtree in place of another in the rbt
 * @param[in] t - pointer to the rbt
 * @param[in] u - pointer to the subtree to be replaced
 * @param[in] v - pointer to the subtree to replace with
 */ 
static inline void __rbt_transplant(__rbt *t, __rbt_node *u, __rbt_node *v) {
    if (u->father == NULL) {
        t->root = v;
    } else if (u == u->father->left) {
        u->father->left = v;
    } else {
        u->father->right = v;
    }
    if (v != NULL) {
        v->father = u->father;
    }
}

/*!
 * Performs a left rotation around the given node
 * @param[in] t - pointer to the rbt
 * @param[in] x - pointer to the node around which to perform the rotation
 */
static inline void __rbt_left_rotate(__rbt *t, __rbt_node *x) {
    __rbt_node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->father = x;
    }
    y->father = x->father;
    if (x->father == NULL) {
        t->root = y;
    } else if (x == x->father->left) {
        x->father->left = y;
    } else {
        x->father->right = y;
    }
    y->left = x;
    x->father = y;
}

/*!
 * Performs a right rotation around the given node
 * @param[in] t - pointer to the rbt
 * @param[in] x - pointer to the node around which to perform the rotation
 */
static inline void __rbt_right_rotate(__rbt *t, __rbt_node *x) {
    __rbt_node *y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->father = x;
    }
    y->father = x->father;
    if (x->father == NULL) {
        t->root = y;
    } else if (x == x->father->right) {
        x->father->right = y;
    } else {
        x->father->left = y;
    }
    y->right = x;
    x->father = y;
}

/*!
 * Fixes the red-black tree properties after insertion
 * @param[in] t - pointer to the rbt
 * @param[in] node - pointer to the newly inserted node
 * @return cs_codes Status code indicating success or type of error
 */
static inline cs_codes __rbt_insert_fixup(__rbt *t, __rbt_node *node) {
    __rbt_node *uncle;

    while (node->father && node->father->color == __RBT_NODE_RED_COLOR) {
        if (node->father->father && node->father == node->father->father->left) {
            uncle = node->father->father->right;

            // Case 1: Uncle is red -> recolor
            if (uncle && uncle->color == __RBT_NODE_RED_COLOR) {
                node->father->color = __RBT_NODE_BLACK_COLOR;
                uncle->color = __RBT_NODE_BLACK_COLOR;
                node->father->father->color = __RBT_NODE_RED_COLOR;
                node = node->father->father;
            }
            // Case 2: Uncle is black and node is right child -> rotate
            else {
                if (node == node->father->right) {
                    node = node->father;
                    __rbt_left_rotate(t, node);
                }

                // Case 3: Uncle is black and node is left child -> rotate and recolor
                node->father->color = __RBT_NODE_BLACK_COLOR;
                node->father->father->color = __RBT_NODE_RED_COLOR;
                __rbt_right_rotate(t, node->father->father);
            }
        } else {
            uncle = node->father->father->left;
            // Case 1: Uncle is red -> recolor
            if (uncle && uncle->color == __RBT_NODE_RED_COLOR) {
                node->father->color = __RBT_NODE_BLACK_COLOR;
                uncle->color = __RBT_NODE_BLACK_COLOR;
                node->father->father->color = __RBT_NODE_RED_COLOR;
                node = node->father->father;
            }
            // Case 2: Uncle is black and node is left child -> rotate
            else {
                if (node == node->father->left) {
                    node = node->father;
                    __rbt_right_rotate(t, node);
                }

                // Case 3: Uncle is black and node is right child -> rotate and recolor
                node->father->color = __RBT_NODE_BLACK_COLOR;
                node->father->father->color = __RBT_NODE_RED_COLOR;
                __rbt_left_rotate(t, node->father->father);
            }
        }
    }

    t->root->color = __RBT_NODE_BLACK_COLOR;

    return CS_SUCCESS;
}


/*!
 * Inserts a new node into the rbt and fixes the tree properties
 * @param[in] t - pointer to the rbt
 * @param[in] new_node - pointer to the new node to be inserted
 * @return Pointer to the inserted node or the existing node if a duplicate is found
 */
static inline __rbt_node* __rbt_insert_internal(__rbt *t, __rbt_node *new_node) {
    __rbt_node *node = t->root;
    __rbt_node *prev = NULL;

    while (node != NULL) {
        int cmp = __rbt_node_compare(t, new_node->data, node->data);
        prev = node;
        if (cmp == 0) {
            return node;
        } else if (cmp < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (t->size == 0) {
        t->root = new_node;
    } else {
        int cmp = __rbt_node_compare(t, new_node->data, prev->data);
        if (cmp < 0) {
            prev->left = new_node;
        } else {
            prev->right = new_node;
        }
        new_node->father = prev;
    }

    if (__rbt_insert_fixup(t, new_node) != CS_SUCCESS) {
        return NULL;
    }

    t->size++;

    return new_node;
}

/*!
 * Fixes the red-black tree properties after deletion
 * @param[in] t - pointer to the rbt
 * @param[in] x - pointer to the node to start fixup from
 * @param[in] x_father - pointer to the father of node x
 * @return CS_SUCCESS on success
 */
static inline cs_codes __rbt_delete_fixup(__rbt *t, __rbt_node *x, __rbt_node *x_father) {
    __rbt_node *w;

    while (x != t->root && (x == NULL || x->color == __RBT_NODE_BLACK_COLOR)) {
        if (x == x_father->left) {
            w = x_father->right;

            // Case 1: Sibling is red
            if (w != NULL && w->color == __RBT_NODE_RED_COLOR) {
                w->color = __RBT_NODE_BLACK_COLOR;
                x_father->color = __RBT_NODE_RED_COLOR;
                __rbt_left_rotate(t, x_father);
                w = x_father->right;
            }

            if (w != NULL && ((w->left == NULL || w->left->color == __RBT_NODE_BLACK_COLOR) &&
                (w->right == NULL || w->right->color == __RBT_NODE_BLACK_COLOR))) {
                // Case 2: Sibling is black with two black children
                w->color = __RBT_NODE_RED_COLOR;
                x = x_father;
                x_father = x->father;
            } else {
                if (w != NULL && (w->right == NULL || w->right->color == __RBT_NODE_BLACK_COLOR)) {
                    // Case 3: Sibling is black with left red child and right black child
                    if (w->left != NULL) {
                        w->left->color = __RBT_NODE_BLACK_COLOR;
                    }
                    w->color = __RBT_NODE_RED_COLOR;
                    __rbt_right_rotate(t, w);
                    w = x_father->right;
                }

                // Case 4: Sibling is black with right red child
                if (w != NULL) {
                    w->color = x_father->color;
                    if (w->right != NULL) {
                        w->right->color = __RBT_NODE_BLACK_COLOR;
                    }
                }
                x_father->color = __RBT_NODE_BLACK_COLOR;
                __rbt_left_rotate(t, x_father);
                x = t->root;
            }
        } else {
            w = x_father->left;
            // Case 1: Sibling is red
            if (w != NULL && w->color == __RBT_NODE_RED_COLOR) {
                w->color = __RBT_NODE_BLACK_COLOR;
                x_father->color = __RBT_NODE_RED_COLOR;
                __rbt_right_rotate(t, x_father);
                w = x_father->left;
            }

            if (w != NULL && ((w->right == NULL || w->right->color == __RBT_NODE_BLACK_COLOR) &&
                (w->left == NULL || w->left->color == __RBT_NODE_BLACK_COLOR))) {
                // Case 2: Sibling is black with two black children
                w->color = __RBT_NODE_RED_COLOR;
                x = x_father;
                x_father = x->father;
            } else {
                if (w != NULL && (w->left == NULL || w->left->color == __RBT_NODE_BLACK_COLOR)) {
                    // Case 3: Sibling is black with right red child and left black child
                    if (w->right != NULL) {
                        w->right->color = __RBT_NODE_BLACK_COLOR;
                    }
                    w->color = __RBT_NODE_RED_COLOR;
                    __rbt_left_rotate(t, w);
                    w = x_father->left;
                }

                // Case 4: Sibling is black with left red child
                if (w != NULL) {
                    w->color = x_father->color;
                    if (w->left != NULL) {
                        w->left->color = __RBT_NODE_BLACK_COLOR;
                    }
                }
                x_father->color = __RBT_NODE_BLACK_COLOR;
                __rbt_right_rotate(t, x_father);
                x = t->root;
            }
        }
    }

    if (x != NULL) {
        x->color = __RBT_NODE_BLACK_COLOR;
    }

    return CS_SUCCESS;
}

/*!
 * Deletes a rbt node from the rbt using the standard red-black tree deletion algorithm
 * @param[in] t - pointer to the rbt
 * @param[in] delete_node - pointer to the rbt node to be deleted
 * @return CS_SUCCESS on success
 */
static inline cs_codes __rbt_delete_internal(__rbt *t, __rbt_node *delete_node) {
    __rbt_node *y = delete_node, *x, *x_father;
    char original_color = y->color;

    if (delete_node->left == NULL) {
        x = delete_node->right;
        x_father = delete_node->father;
        __rbt_transplant(t, delete_node, delete_node->right);
    } else if (delete_node->right == NULL) {
        x = delete_node->left;
        x_father = delete_node->father;
        __rbt_transplant(t, delete_node, delete_node->left);
    } else {
        y = __rbt_node_minimum(delete_node->right);
        original_color = y->color;
        x = y->right;
        x_father = y->father;

        if (y->father == delete_node) {
            x_father = y;
        } else {
            __rbt_transplant(t, y, y->right);
            y->right = delete_node->right;
            y->right->father = y;
        }
        __rbt_transplant(t, delete_node, y);
        y->left = delete_node->left;
        y->left->father = y;
        y->color = delete_node->color;
    }

    if (original_color == __RBT_NODE_BLACK_COLOR) {
        cs_codes rc = __rbt_delete_fixup(t, x, x_father);
        if (rc != CS_SUCCESS) {
            return rc;
        }
    }

    __rbt_node_free(delete_node, t->attr);

    t->size--;

    return CS_SUCCESS;
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

static inline cs_codes __rbt_init(__rbt *t, elem_attr_t attr) {
    CS_RETURN_IF(NULL == t, CS_NULL);
    CS_RETURN_IF(attr.size < 0 || attr.size > SIZE_TH, CS_SIZE);

    t->root = NULL;
    t->size = 0;
    t->attr = attr;

    t->header.magic = CS_RBT_MAGIC;
    t->header.type = CS_RBT_TYPE;
    return CS_SUCCESS;
}

static inline cs_codes __rbt_insert(__rbt *t, void *data) {
    CS_RETURN_IF(t == NULL || data == NULL, CS_NULL);
    CS_RETURN_IF(t->header.magic != CS_RBT_MAGIC, CS_UNINITIALIZED);
    __rbt_node *new_node;

    new_node = __rbt_node_init(data, t->attr);
    CS_RETURN_IF(new_node == NULL, CS_MEM);
    __rbt_node *inserted_node = __rbt_insert_internal(t, new_node);

    if (inserted_node == NULL) {
        __rbt_node_free(new_node, t->attr);
        return CS_ELEM;
    }

    if (inserted_node != new_node) {
        __rbt_node_free(new_node, t->attr);
        return CS_ELEM;
    }

    return CS_SUCCESS;
}

static inline cs_codes __rbt_delete(__rbt *t, void *data) {
    CS_RETURN_IF(t == NULL || data == NULL, CS_NULL);
    CS_RETURN_IF(t->header.magic != CS_RBT_MAGIC, CS_UNINITIALIZED);
    __rbt_node *delete_node = __rbt_node_find(t, data);

    CS_RETURN_IF(delete_node == NULL, CS_ELEM);

    return __rbt_delete_internal(t, delete_node);
}

static inline int __rbt_empty(__rbt *t) { return (t->root == NULL); }

static inline int __rbt_size(__rbt *t) { return t->size; }

static inline void* __rbt_find(__rbt *t, void *data) {
    CS_RETURN_IF(data == NULL || t == NULL || t->header.magic != CS_RBT_MAGIC, NULL);
    __rbt_node *node = __rbt_node_find(t, data);
    CS_RETURN_IF(node == NULL, NULL);
    return node->data;
}

static inline void __rbt_swap(__rbt *t1, __rbt *t2) {
    CS_RETURN_IF(t1 == NULL || t2 == NULL || t1->header.magic != CS_RBT_MAGIC || t2->header.magic != CS_RBT_MAGIC);

    cs_header_t temp_header = t1->header;
    __rbt_node *temp_root = t1->root;
    int temp_size = t1->size;
    elem_attr_t temp_attr = t1->attr;

    t1->header = t2->header;
    t1->root = t2->root;
    t1->size = t2->size;
    t1->attr = t2->attr;

    t2->header = temp_header;
    t2->root = temp_root;
    t2->size = temp_size;
    t2->attr = temp_attr;
}

static inline void __rbt_clear(__rbt *t) {
    CS_RETURN_IF(t == NULL || t->header.magic != CS_RBT_MAGIC);
    __rbt_node *node = t->root, *next;

    while (node != NULL) {
        if (node->left != NULL) {
            node = node->left;
        } else if (node->right != NULL) {
            node = node->right;
        } else {
            next = node->father;
            __rbt_node_free(node, t->attr);
            if (next != NULL) {
                if (next->left == node) {
                    next->left = NULL;
                } else {
                    next->right = NULL;
                }
            }
            node = next;
        }
    }

    t->size = 0;
    t->root = NULL;
}

static inline void __rbt_print(FILE *stream, void *v_t) {
    CS_RETURN_IF(v_t == NULL || stream == NULL);
    __rbt *t = (__rbt *)v_t;
    CS_RETURN_IF(t->header.magic != CS_RBT_MAGIC);

    __rbt_print_stack_item *stack = malloc(sizeof(__rbt_print_stack_item) * (int)ceil(log2(t->size + 1)));
    int stack_size = 0;

    stack[stack_size++] = (__rbt_print_stack_item){t->root, 0};

    while (stack_size > 0) {
        __rbt_print_stack_item item = stack[--stack_size];
        __rbt_node *node = item.node;
        int tab_size = item.tab_size;

        for (int i = 0; i < tab_size; i++) {
            fprintf(stream, "  ");
        }
        if (node->father && node == node->father->left) {
            fprintf(stream, "L-> ");
        } else if (node->father && node == node->father->right) {
            fprintf(stream, "R-> ");
        } else {
            fprintf(stream, "Root-> ");
        }

        fprintf(stream, "Data: ");
        if (t->attr.print) {
            t->attr.print(stream, node->data);
        }

        fprintf(stream, " -> Color: %s", node->color == __RBT_NODE_RED_COLOR ? "RED" : "BLACK");

        fprintf(stream, "\n");

        if (node->right != NULL)
            stack[stack_size++] = (__rbt_print_stack_item){node->right, tab_size + 1};
        if (node->left != NULL)
            stack[stack_size++] = (__rbt_print_stack_item){node->left, tab_size + 1};
    }

    free(stack);
}

static inline void __rbt_free(void *v_t) {
    CS_RETURN_IF(v_t == NULL);
    __rbt *t = (__rbt *)v_t;
    CS_RETURN_IF(t->header.magic != CS_RBT_MAGIC);
    __rbt_node *node = t->root, *next;
    while (node != NULL) {
        if (node->left != NULL) {
            node = node->left;
        } else if (node->right != NULL) {
            node = node->right;
        } else {
            next = node->father;
            __rbt_node_free(node, t->attr);
            if (next != NULL) {
                if (next->left == node) {
                    next->left = NULL;
                } else {
                    next->right = NULL;
                }
            }
            node = next;
        }
    }

    t->root = NULL;
    t->header.magic = 0;
}

#endif