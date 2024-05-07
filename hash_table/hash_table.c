#include "../include/hash_table.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

cs_codes hash_table_init ( hash_table *ht, hash_table_attr_t attr, int size, hash_function hf ) {
    if ( attr.size < 0 || attr.size > SIZE_TH ) return CS_SIZE;
    ht->attr = attr;
    ht->hf = hf;
    ht->size = size;
    ht->table = malloc ( size * sizeof ( vector ) );
    if ( !ht->table ) return CS_MEM;
    int rc;
    for ( int i = 0; i < size; i++ ) {
        rc = vector_init ( &ht->table[i], ht->attr );
        if ( rc == CS_MEM ) {
            for ( int j = 0; j < i; j++ )
                vector_free ( &ht->table[j] );
            free ( ht->table );
            return rc;
        }
    }
    return CS_SUCCESS;
}

cs_codes hash_table_add_entry ( hash_table *ht, void *el ) {
    int pos = ht->hf ( el );
    if ( pos < 0 || pos >= ht->size ) return CS_SIZE;
    return vector_push_back ( &ht->table[pos], el );
}

cs_codes hash_table_delete_entry ( hash_table *ht, void *el ) {
    int htpos = ht->hf ( el );
    if ( htpos < 0 || htpos >= ht->size ) return CS_SIZE;
    int pos = vector_find ( ht->table[htpos], el );
    if ( pos == -1 ) return CS_POS;
    return vector_erase ( &ht->table[htpos], pos );
}

vector *hash_table_get_values ( hash_table ht, int key ) {
    if ( key < 0 || key >= ht.size ) return NULL;
    return &ht.table[key];
}

void hash_table_set_free ( hash_table *ht, freer fr ) { 
    ht->attr.fr = fr; 
    for ( int i = 0; i < ht->size; i++ )
        vector_set_free ( &ht->table[i], fr );
}

void hash_table_set_print ( hash_table *ht, printer print ) {
    ht->attr.print = print;
    for ( int i = 0; i < ht->size; i++ )
        vector_set_print ( &ht->table[i], print );
}

void hash_table_set_comp ( hash_table *ht, comparer comp ) {
    ht->attr.comp = comp;
    for ( int i = 0; i < ht->size; i++ )
        vector_set_comp ( &ht->table[i], comp );
}

void hash_table_swap ( hash_table *ht1, hash_table *ht2 ) {
    hash_table_attr_t attr = ht1->attr;
    hash_function hf = ht1->hf;
    int size = ht1->size;
    vector *table = ht1->table;

    ht1->attr = ht2->attr;
    ht1->hf = ht2->hf;
    ht1->size = ht2->size;
    ht1->table = ht2->table;

    ht2->attr = attr;
    ht2->hf = hf;
    ht2->size = size;
    ht2->table = table;
}

void hash_table_clear ( hash_table *ht ) {
    for ( int i = 0; i < ht->size; i++ )
        vector_clear ( &ht->table[i] );
}

void hash_table_free ( hash_table *ht ) {
    for ( int i = 0; i < ht->size; i++ )
        vector_free ( &ht->table[i] );
    free ( ht->table );
}

void hash_table_print ( hash_table ht ) {
    for ( int i = 0; i < ht.size; i++ ) {
        printf ( "%d: ", i );
        vector_print ( &ht.table[i] );
        printf ( "\n" );
    }
}