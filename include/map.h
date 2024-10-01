#ifndef MAP_H
#define MAP_H

#include "universal.h"

typedef xuniv_attr_t map_attr_t;

typedef struct map
{
    struct map_node *root; /*!< size of the datatype */
    int size;              /*!< size of the datatype */
    map_attr_t key_attr;   /*!< size of the datatype */
    map_attr_t val_attr;   /*!< size of the datatype */
} map;

typedef struct map_node
{
    char color;                   /*!< size of the datatype */
    void *key;                    /*!< size of the datatype */
    void *val;                    /*!< size of the datatype */
    struct map_node *left_child;  /*!< size of the datatype */
    struct map_node *right_child; /*!< size of the datatype */
    struct map_node *father;      /*!< size of the datatype */
} map_node;

cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr);
cs_codes map_insert(map *m, void *key, void *val);
void map_print(map m);
void map_free(void *m);

#endif