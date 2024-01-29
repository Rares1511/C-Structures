#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../vector/vector.h"

typedef size_t (*hash_function) ( void* );

typedef struct hash_table {
    hash_function hf;
    size_t size;
    vector *table;
} hash_table;

#define HASH_TABLE_SIZE sizeof ( hash_table )

hash_table *hash_table_init ( size_t dim, size_t size, hash_function hf );
enum return_codes hash_table_add_entry ( hash_table *ht, void *el );
enum return_codes hash_table_delete_entry ( hash_table *ht, void *el, comparer comp );
void hash_table_free ( hash_table *ht );
vector hash_table_get_values ( hash_table *ht, int key );
void hash_table_print ( hash_table *ht, printer print );

#endif