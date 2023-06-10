#ifndef MAP_H
#define MAP_H

#include <stdarg.h>
#include <stdint.h>

#include "universal.h"

typedef struct map_node {
    void *data;
    uint8_t color;
    struct map_node *left_child, *right_child;
    struct map_node *father;
} map_node;

typedef struct map {
    map_node *root;
    size_t size, key_dim, value_dim;
    comparer key_compare, value_compare;
} map;

#define MAP_SIZE sizeof ( map )
#define MAP_NODE_SIZE sizeof ( map_node )

#define RED 0
#define BLACK 1

#define map_begin(m) m->vec
#define map_end(m) m->vec + m->used_cap
#define map_empty(m) m->size == 0
#define map_size(m) m->size

map *map_initiate ( size_t key_dim, size_t value_dim );
void map_set_key_compare ( map *m, comparer compare );
void map_set_value_compare ( map *m, comparer compare );
enum return_codes map_insert ( map *m, void *key, void *value );
void map_free ( map *m );
void map_print ( map *m, printer key_print, printer value_print );

#endif