#ifndef __CS_MAP_H__
#define __CS_MAP_H__

#include <cs/universal.h>

typedef univ_attr_t map_attr_t;
typedef struct rbt rbt;

typedef struct map {
    rbt *t;
    map_attr_t key_attr;
    map_attr_t val_attr;
} map;

/*!
 * Initializes a new map
 * @param[in] key_attr - attributes of the key datatype
 * @param[in] val_attr - attributes of the value datatype
 * @return pointer to the initialized map, or NULL on memory allocation failure
 */
map *map_init(map_attr_t key_attr, map_attr_t val_attr);

/*!
 * Inserts a new key-value pair into the map
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @param[in] val - pointer to the value data
 * @return CS_SUCCESS on success, CS_ELEM if the key already exists, CS_MEM on memory allocation failure
 */
cs_codes map_insert(map *m, void *key, void *val);

/*!
 * Checks if the map is empty
 * @param[in] m - the map
 * @return 1 if the map is empty, 0 otherwise
 */
int map_empty(map m);

/*!
 * Retrieves the number of key-value pairs in the map
 * @param[in] m - the map
 * @return number of key-value pairs in the map
 */
int map_size(map m);

/*!
 * Retrieves the value associated with a given key in the map
 * @param[in] m - the map
 * @param[in] key - pointer to the key data
 * @param[out] value - pointer to the memory where the value will be copied
 * @return CS_SUCCESS on success, CS_ELEM if the key does not exist
 */
void* map_find(map m, void *key);

/*!
 * Deletes a key-value pair from the map
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @return CS_SUCCESS on success, CS_ELEM if the key does not exist
 */
cs_codes map_delete(map *m, void *key);

/*!
 * Swaps the contents of two maps
 * @param[in] m1 - pointer to the first map
 * @param[in] m2 - pointer to the second map
 */
void map_swap(map *m1, map *m2);

/*!
 * Clears all key-value pairs from the map
 * @param[in] m - pointer to the map
 */
void map_clear(map *m);

/*!
 * Sets various attributes of the map
 * @param[in] m - pointer to the map
 * @param[in] key_attr - attributes of the key datatype
 * @param[in] val_attr - attributes of the value datatype
 */
void map_set_attr(map *m, map_attr_t key_attr, map_attr_t val_attr);

/*!
 * Sets the freer functions for the map
 * @param[in] m - pointer to the map
 * @param[in] key_fr - freer function for the key datatype
 * @param[in] val_fr - freer function for the value datatype
 */
void map_set_free(map *m, freer key_fr, freer val_fr);

/*!
 * Sets the printer functions for the map
 * @param[in] m - pointer to the map
 * @param[in] key_pr - printer function for the key datatype
 * @param[in] val_pr - printer function for the value datatype
 */
void map_set_print(map *m, printer key_pr, printer val_pr);

/*!
 * Sets the comparer functions for the map
 * @param[in] m - pointer to the map
 * @param[in] key_comp - comparer function for the key datatype
 * @param[in] val_comp - comparer function for the value datatype
 */
void map_set_comp(map *m, comparer key_comp, comparer val_comp);

/*!
 * Sets the deepcopy functions for the map
 * @param[in] m - pointer to the map
 * @param[in] key_copy - deepcopy function for the key datatype
 * @param[in] val_copy - deepcopy function for the value datatype
 */
void map_set_copy(map *m, deepcopy key_copy, deepcopy val_copy);

/*!
 * Sets the output streams for the map
 * @param[in] m - pointer to the map
 * @param[in] key_stream - output stream for the key datatype
 * @param[in] val_stream - output stream for the value datatype
 */
void map_set_stream(map *m, FILE *key_stream, FILE *val_stream);

/*!
 * Sets various attributes of the key datatype
 * @param[in] m - pointer to the map
 * @param[in] key_attr - attributes of the key datatype
 */
void map_key_set_attr(map *m, map_attr_t key_attr);

/*!
 * Sets the freer function for the key datatype
 * @param[in] m - pointer to the map
 * @param[in] key_fr - freer function for the key datatype
 */
void map_key_set_free(map *m, freer key_fr);

/*!
 * Sets the comparer function for the key datatype
 * @param[in] m - pointer to the map
 * @param[in] key_comp - comparer function for the key datatype
 */
void map_key_set_comp(map *m, comparer key_comp);

/*!
 * Sets the printer function for the key datatype
 * @param[in] m - pointer to the map
 * @param[in] key_pr - printer function for the key datatype
 */
void map_key_set_print(map *m, printer key_pr);

/*!
 * Sets the deepcopy function for the key datatype
 * @param[in] m - pointer to the map
 * @param[in] key_copy - deepcopy function for the key datatype
 */
void map_key_set_copy(map *m, deepcopy key_copy);

/*!
 * Sets the output stream for the key datatype
 * @param[in] m - pointer to the map
 * @param[in] key_stream - output stream for the key datatype
 */
void map_key_set_stream(map *m, FILE *key_stream);

/*!
 * Sets various attributes of the value datatype
 * @param[in] m - pointer to the map
 * @param[in] val_attr - attributes of the value datatype
 */
void map_val_set_attr(map *m, map_attr_t val_attr);

/*!
 * Sets the freer function for the value datatype
 * @param[in] m - pointer to the map
 * @param[in] val_fr - freer function for the value datatype
 */
void map_val_set_free(map *m, freer val_fr);

/*!
 * Sets the comparer function for the value datatype
 * @param[in] m - pointer to the map
 * @param[in] val_comp - comparer function for the value datatype
 */
void map_val_set_comp(map *m, comparer val_comp);

/*!
 * Sets the printer function for the value datatype
 * @param[in] m - pointer to the map
 * @param[in] val_pr - printer function for the value datatype
 */
void map_val_set_print(map *m, printer val_pr);

/*!
 * Sets the deepcopy function for the value datatype
 * @param[in] m - pointer to the map
 * @param[in] val_copy - deepcopy function for the value datatype
 */
void map_val_set_copy(map *m, deepcopy val_copy);

/*!
 * Sets the output stream for the value datatype
 * @param[in] m - pointer to the map
 * @param[in] val_stream - output stream for the value datatype
 */
void map_val_set_stream(map *m, FILE *val_stream);

/*!
 * Prints the map's contents
 * @param[in] v_m - pointer to the map
 */
void map_print(FILE *stream, void *v_m);

/*!
 * Frees all resources associated with the map
 * @param[in] v_m - pointer to the map
 */
void map_free(void *v_m);

#endif