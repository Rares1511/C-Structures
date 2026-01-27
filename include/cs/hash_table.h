#ifndef __CS_HASH_TABLE_H__
#define __CS_HASH_TABLE_H__

#include <cs/universal.h>
#include <cs/vector.h>

typedef univ_attr_t hash_table_attr_t;

typedef size_t (*hash_func_t)(const void *key);

typedef struct hash_table {
    int cap;
    metadata_t *meta;
    hash_table_attr_t attr;
    hash_func_t hash;
    vector **buckets;
} hash_table;

hash_table *hash_table_init(hash_table_attr_t attr, hash_func_t hash, int initial_capacity);
cs_codes hash_table_add_entry(hash_table *ht, const void *el);
cs_codes hash_table_remove_entry(hash_table *ht, const void *el);
void *hash_table_get_entry(hash_table ht, const void *el);
int hash_table_count(hash_table ht, const void *el);
static inline int hash_table_empty(hash_table ht) { return ht.meta->size == 0; };
static inline int hash_table_size(hash_table ht) { return ht.meta->size; };
void hash_table_swap(hash_table *ht1, hash_table *ht2);
void hash_table_clear(hash_table *ht);
void hash_table_print(FILE *stream, void *v_ht);
void hash_table_free(void *v_ht);

#endif