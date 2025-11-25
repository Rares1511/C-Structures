#ifndef MAP_H
#define MAP_H

#include <cs/universal.h>

typedef xuniv_attr_t map_attr_t;

#define MAP_NODE_RED_COLOR 0
#define MAP_NODE_BLACK_COLOR 1

typedef struct map {
    struct map_node *root; /*!< root of the map */
    int size;              /*!< number of elements in the map */
    map_attr_t key_attr;   /*!< attributes of the key datatype */
    map_attr_t val_attr;   /*!< attributes of the value datatype */
} map;

typedef struct map_node {
    char color;                   /*!< color of the node */
    void *key;                    /*!< key of the node */
    void *val;                    /*!< value of the node */
    struct map_node *left_child;  /*!< left child of the node */
    struct map_node *right_child; /*!< right child of the node */
    struct map_node *father;      /*!< father of the node */
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
void map_print(void *v_m);

/*!
 * Frees all resources associated with the map
 * @param[in] v_m - pointer to the map
 */
void map_free(void *v_m);

#endif