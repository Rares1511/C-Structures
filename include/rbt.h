#ifndef __CS_RBTREE_H__
#define __CS_RBTREE_H__

#include <cs/universal.h>

typedef univ_attr_t rbt_attr_t;

#define __RBT_NODE_BLACK_COLOR 0
#define __RBT_NODE_RED_COLOR   1

typedef struct rbt_node {
    struct rbt_node *left;
    struct rbt_node *right;
    struct rbt_node *father;
    char color;
    void *data;
} rbt_node;

typedef struct rbt {
    rbt_node *root;
    metadata_t *meta;
    rbt_attr_t attr;
} rbt;

rbt *rbt_init(rbt_attr_t attr);
cs_codes rbt_insert(rbt *t, void *data);
cs_codes rbt_delete(rbt *t, void *data);
static inline int rbt_empty(rbt t) { return (t.root == NULL); }
static inline int rbt_size(rbt t) { return t.meta->size; }
void* rbt_find(rbt t, void *data);
void rbt_swap(rbt *t1, rbt *t2);
void rbt_clear(rbt *t);
void rbt_print(FILE *stream, void *v_t);
void rbt_free(void *v_t);

#endif