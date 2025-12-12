#ifndef _UNORDERED_SET_H
#define _UNORDERED_SET_H

#include <cs/universal.h>

typedef xuniv_attr_t unordered_set_attr_t;
typedef struct hash_table hash_table;
typedef int (*hash_func_t)(const void *key);

typedef struct unordered_set {
    hash_table *ht;
} unordered_set;

cs_codes unordered_set_init(unordered_set *uset, unordered_set_attr_t attr, hash_func_t hash_func, int initial_capacity);
cs_codes unordered_set_insert(unordered_set *uset, const void *key);
cs_codes unordered_set_erase(unordered_set *uset, const void *key);
void* unordered_set_find(unordered_set uset, const void *key);
int unordered_set_count(unordered_set uset, const void *key);

#endif