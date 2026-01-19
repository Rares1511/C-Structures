#ifndef __CS_UNORDERED_MAP_H__
#define __CS_UNORDERED_MAP_H__

#include <cs/universal.h>

typedef univ_attr_t unordered_map_attr_t;
typedef struct hash_table hash_table;
typedef size_t (*hash_func_t)(const void *key);

typedef struct unordered_map {
    hash_table *ht;
    hash_func_t hash_func;
    unordered_map_attr_t key_attr;
    unordered_map_attr_t value_attr;
} unordered_map;

cs_codes unordered_map_init(unordered_map *umap,
                                 unordered_map_attr_t key_attr,
                                 unordered_map_attr_t value_attr,
                                 hash_func_t hash_func,
                                 int initial_capacity);

void unordered_map_free(void *v_umap);

#endif