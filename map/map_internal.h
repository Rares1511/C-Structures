#include <string.h>
#include <stdlib.h>

#include "../include/map.h"

#define BLACK 0
#define RED 1

map_node *map_node_init(void *key, int key_size, void *val, int val_size);
static inline void map_node_free(map *m, map_node *node)
{
    if (m->key_attr.fr)
    {
        m->key_attr.fr(node->key);
    }
    if (m->val_attr.fr)
    {
        m->val_attr.fr(node->val);
    }
    free(node->key);
    free(node->val);
    free(node);
}
void map_rotate_left(map *m, map_node *node);
void map_rotate_right(map *m, map_node *node);