#include "hash_table.h"

/* initializes the hash table knowing the elements will have a sizeof of dim, the hash
table will have size entries and it uses the hf function */
hash_table *hash_table_initiate ( size_t dim, size_t size, hash_function hf ) {
    hash_table *ht = malloc ( HASH_TABLE_SIZE );
    if ( !ht ) return NULL;
    ht->size = size;
    ht->hf = hf;
    ht->table = calloc ( ht->size, VECTOR_SIZE );
    if ( !ht->table ) {
        free ( ht );
        return NULL;
    }
    for ( size_t i = 0; i < ht->size; i++ ) {
        vector *aux = vector_initiate ( dim );
        if ( !aux ) {
            for ( size_t j = 0; j < i; j++ )
                vector_free ( &ht->table[j] );
            free ( ht->table );
            free ( ht );
            return NULL;
        }
        ht->table[i] = *aux;
    }
    return ht;
}

/* add the given element to the has table */
enum return_codes hash_table_add_entry ( hash_table *ht, void *el ) {
    if ( ht->hf ( el ) > ht->size ) return WRONG_ELEMENT;
    return vector_push_back ( &ht->table[ht->hf ( el )], el );
}

/* deletes the given element from the hast table if it exists */
enum return_codes hash_table_delete_entry ( hash_table *ht, void *el ) {
    if ( ht->hf ( el ) > ht->size ) return WRONG_ELEMENT;
    size_t pos = vector_find ( &ht->table[ht->hf ( el )], el );
    if ( pos == -1 ) return WRONG_ELEMENT;
    return vector_erase ( &ht->table[ht->hf ( el )], pos );
}

/* frees the memory the hash table uses */
void hash_table_free ( hash_table *ht ) {
    for ( size_t i = 0; i < ht->size; i++ )
        vector_free ( &ht->table[i] );
    free ( ht->table );
    free ( ht );
}

/* returns the vector that holds the values with the key given */
vector hash_table_get_values ( hash_table *ht, int key ) {
    return ht->table[key];
}

/* prints the hash table using the print function for printing individual elements */
void hash_table_print ( hash_table *ht, printer print ) {
    for ( size_t i = 0; i < ht->size; i++ ) {
        if ( ht->table[i].size == 0 ) continue;
        printf ( "key %d: ", i );
        vector_print ( &ht->table[i], print );
    }
}