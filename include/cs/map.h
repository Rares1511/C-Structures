#ifndef __CS_MAP_H__
#define __CS_MAP_H__

#include <cs/universal.h>
#include <cs/rbt.h>
#include <cs/pair.h>

typedef struct map {
    __rbt* t;
    elem_attr_t* key_attr;
    elem_attr_t* val_attr;
    char *buffer; /*!< Buffer for temporary storage */
} map;

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline void __map_node_copy(void *dest, const void *src) {
    CS_RETURN_IF(dest == NULL || src == NULL);
    const pair* s = (const pair*)src;
    pair* d = (pair*)dest;

    memcpy(d, s, sizeof(pair));

    if (!s->first_attr->copy && !s->second_attr->copy) {
        memcpy(d->data, s->data, s->first_attr->size + s->second_attr->size);
        return;
    }

    void *dest_key = d->data;
    void *dest_val = d->data + s->first_attr->size;

    if (s->first_attr->copy) 
        s->first_attr->copy(dest_key, s->data);
    else 
        memcpy(dest_key, s->data, s->first_attr->size);

    if (s->second_attr->copy) 
        s->second_attr->copy(dest_val, s->data + s->first_attr->size);
    else 
        memcpy(dest_val, s->data + s->first_attr->size, s->second_attr->size);
}

static inline int __map_node_comp(const void *a, const void *b) {
    const pair* pa = (const pair*)a;
    const pair* pb = (const pair*)b;
    
    // Direct access, no macros, no magic checks
    void *key_a = (void*)pa->data;
    void *key_b = (void*)pb->data;

    if (__builtin_expect(pa->first_attr->comp != NULL, 1)) {
        return pa->first_attr->comp(key_a, key_b);
    }
    return memcmp(key_a, key_b, pa->first_attr->size);
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

/*!
 * Initializes a new map
 * @param[in] key_attr - attributes of the key datatype
 * @param[in] val_attr - attributes of the value datatype
 * @param[out] m - pointer to the map to initialize
 * @return CS_SUCCESS on success, CS_MEM on memory allocation failure
 */
cs_codes map_init(map *m, elem_attr_t key_attr, elem_attr_t val_attr);

/*!
 * Inserts a new key-value pair into the map
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @param[in] val - pointer to the value data
 * @return CS_SUCCESS on success, CS_ELEM if the key already exists, CS_MEM on memory allocation failure
 */
static inline cs_codes map_insert(map *m, void *key, void *val) {
    CS_RETURN_IF(m == NULL || key == NULL || val == NULL, CS_NULL);

    int k_sz = m->key_attr->size;
    int v_sz = m->val_attr->size;
    pair *p = (pair *)m->buffer; // Use pre-allocated buffer for temporary storage
    
    memcpy(p->data, key, k_sz);
    memcpy(p->data + k_sz, val, v_sz);

    return __rbt_insert(m->t, p);
}

/*!
 * Checks if the map is empty
 * @param[in] m - the map
 * @return 1 if the map is empty, 0 otherwise
 */
static inline int map_empty(map *m) {
    CS_RETURN_IF(m == NULL || m->t == NULL, 1);
    return __rbt_empty(m->t);
}

/*!
 * Retrieves the number of key-value pairs in the map
 * @param[in] m - the map
 * @return number of key-value pairs in the map
 */
static inline int map_size(map *m) {
    CS_RETURN_IF(m == NULL || m->t == NULL, 0);
    return __rbt_size(m->t);
}   

/*!
 * Retrieves the value associated with a given key in the map
 * @param[in] m - the map
 * @param[in] key - pointer to the key data
 * @param[out] value - pointer to the memory where the value will be copied
 * @return CS_SUCCESS on success, CS_ELEM if the key does not exist
 */
static inline void* map_find(map *m, void *key) {
    int k_sz = m->key_attr->size;
    pair *dummy = (pair*)m->buffer;
    
    memcpy(dummy->data, key, k_sz);

    pair* result = (pair*)__rbt_find(m->t, dummy);
    
    return result ? pair_second(result) : NULL;
}

/*!
 * Deletes a key-value pair from the map
 * @param[in] m - pointer to the map
 * @param[in] key - pointer to the key data
 * @return CS_SUCCESS on success, CS_ELEM if the key does not exist
 */
static inline cs_codes map_delete(map *m, void *key) {
    CS_RETURN_IF(m == NULL || key == NULL, CS_NULL);

    pair *search_key = (pair *)m->buffer;

    memcpy(search_key->data, key, m->key_attr->size);

    return __rbt_delete(m->t, search_key);
}

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
void map_set_attr(map *m, elem_attr_t key_attr, elem_attr_t val_attr);

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
void map_key_set_attr(map *m, elem_attr_t key_attr);

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
void map_val_set_attr(map *m, elem_attr_t val_attr);

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