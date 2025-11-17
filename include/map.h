#ifndef MAP_H
#define MAP_H

#include "universal.h"

typedef xuniv_attr_t map_attr_t;

#define RED 0
#define BLACK 1

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

/*!
 * Initializes a new map
 * @param[in] m - pointer to the map to be initialized
 * @param[in] key_attr - attributes of the key datatype
 * @param[in] val_attr - attributes of the value datatype
 * @return CS_SUCCESS on success, CS_MEM on memory allocation failure
 */
cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr);

/*!
 * Inserts a new key-value pair into the map
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @param[in] val - pointer to the value data
 * @return CS_SUCCESS on success, CS_ELEM if the key already exists, CS_MEM on memory allocation failure
 */
cs_codes map_insert(map *m, void *key, void *val);


/*!
 * Retrieves the value associated with a given key in the map
 * @param[in] m - the map
 * @param[in] key - pointer to the key data
 * @param[out] value - pointer to the memory where the value will be copied
 * @return CS_SUCCESS on success, CS_ELEM if the key does not exist
 */
cs_codes map_get(map m, void *key, void *value);

/*!
 * Deletes a key-value pair from the map
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @return CS_SUCCESS on success, CS_ELEM if the key does not exist
 */
cs_codes map_delete(map *m, void *key);

/*!
 * Prints the map's contents
 * @param[in] v_m - pointer to the map
 */
void map_print(void *v_m);

/*!
 * Frees all resources associated with the map
 * @param[in] v_m - pointer to the map
 */
void map_free(void *v_m);

#endif