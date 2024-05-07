#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "vector.h"

typedef int (*hash_function) ( void* );
typedef xuniv_attr_t hash_table_attr_t;

typedef struct hash_table {
    hash_function      hf;     /*!< hash function for the hash table */
    vector*            table;  /*!< table of the hash table that holds the elements */
    int                size;   /*!< maximum number of lines of the hash table */
    hash_table_attr_t  attr;   /*!< attributes for the datatype inside the hash table */
} hash_table;

/*!
 * Initializes the hash table structure with the data offered
 * @param[out] ht       Hash table that will be initialized
 * @param[in]  el_attr  Attributes for the elements inside the hash table 
 * @param[in]  size     Maximum number of lines in the hash table
 * @param[in]  hf       Hash function that gets the line for the elements
 * @return CS_SIZE if a negative or too big size has been given, CS_MEM if a memory 
 * problem has occurred or CS_SUCCESS upon a successful initialization
 */
cs_codes hash_table_init ( hash_table *ht, hash_table_attr_t el_attr, int size, hash_function hf );

/*!
 * Adds a new entry into the hash function at the corresponding line using the hash function
 * @param[out] ht  Hash table that will have the element added
 * @param[in]  el  Element that will be added
 * @return CS_SIZE if the hash function provided a wrong position, CS_MEM if a memory
 * problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes hash_table_add_entry ( hash_table *ht, void *el );

/*!
 * Deletes the element from the hash table
 * @param[out] ht  Hash table that will have the element deleted
 * @param[in]  el  Element that will be deleted if it exists in the hash table
 * @return CS_SIZE if the hash function provided a wrong position, CS_POS if the element
 * doesn't exist in the hash table or CS_SUCCESS for a successful deletion
 */
cs_codes hash_table_delete_entry ( hash_table *ht, void *el );

/*!
 * Returns a reference to the line that has been asked for, or NULL if the key given
 * is wrong
 * @param[in] ht  Hash table whose line will be accessed
 * @param[in] key Line that has been asked for
 */
vector *hash_table_get_values ( hash_table ht, int key );

/*!
 * Sets the new free function for the hash table elements
 * @param[out] ht  Hash table whose free function will be changed
 * @param[in]  fr  New free function for the datatype inside the hash table
 */
void hash_table_set_free ( hash_table *ht, freer fr );

/*!
 * Sets the new print function for the hash table elements
 * @param[out] ht     Hash table whose print function will be changed
 * @param[in]  print  New print function for the datatype inside the hash table
 */
void hash_table_set_print ( hash_table *ht, printer print );

/*!
 * Sets the new comp function for the hash table elements
 * @param[out] ht    Hash table whose comp function will be changed
 * @param[in]  comp  New comp function for the datatype inside the hash table
 */
void hash_table_set_comp ( hash_table *ht, comparer comp );

/*!
 * Swaps the two given hash tables
 * @param[in] ht1,ht2  The two hash tables that will be swapped
 */
void hash_table_swap ( hash_table *ht1, hash_table *ht2 );

/*!
 * Clears the hash table and prepares it for next usage
 * @param[in] ht  Hash table that will be cleared
 */
void hash_table_clear ( hash_table *ht );

/*!
 * Frees the memory that the hash table used
 * @param[in] ht  Hash table whose memory will be freed
 */
void hash_table_free ( hash_table *ht );

/*!
 * Prints the elements of the hash table by printing each line
 * @param[in] ht  Hash table whose elements will be printed
 */
void hash_table_print ( hash_table ht );

#endif