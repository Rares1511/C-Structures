#ifndef MAP_H
#define MAP_H

#include "universal.h"

typedef xuniv_attr_t map_attr_t;

typedef struct map_node {
    void*             key;         /*!< size of the datatype */
    void*             val;
    char              color;        /*!< size of the datatype */
    struct map_node*  left_child;   /*!< size of the datatype */
    struct map_node*  right_child;  /*!< size of the datatype */
    struct map_node*  father;       /*!< size of the datatype */
} map_node;

typedef struct map {
    map_node*   root;      /*!< size of the datatype */
    int         size;      /*!< size of the datatype */
    map_attr_t  key_attr;  /*!< size of the datatype */
    map_attr_t  val_attr;  /*!< size of the datatype */
} map;

cs_codes map_init ( map *m, map_attr_t key_attr, map_attr_t val_attr );
cs_codes map_insert ( map *m, void *key, void *value );
cs_codes map_update ( map *m, void *key, void *value );
cs_codes map_erase ( map *m, void *key );
void *map_find ( map m, void *key );
void map_swap ( map *m1, map *m2 );
void map_free ( map *m );
void map_print ( map *m );
void map_print_debug ( map *m );

#endif