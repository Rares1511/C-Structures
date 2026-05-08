#ifndef __CS_MAP_H__
#define __CS_MAP_H__

#include <cs/universal.h>
#include <cs/rbt.h>
#include <cs/pair.h>

typedef struct map {
    __rbt* t;
    elem_attr_t* key_attr;
    elem_attr_t* val_attr;
} map;

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline void __map_node_copy(void *dest, const void *src) {
    CS_RETURN_IF(dest == NULL || src == NULL);
    const pair* s = (const pair*)src;
    memcpy(dest, src, sizeof(pair) + s->first_attr->size + s->second_attr->size);
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

    // 1. Determine total size needed on the stack
    int k_sz = m->key_attr->size;
    int v_sz = m->val_attr->size;
    int total_sz = sizeof(pair) + k_sz + v_sz;

    // 2. Allocate space on the STACK (VLA)
    // This costs near-zero CPU cycles (just moves the stack pointer)
    char buffer[total_sz]; 
    pair *p = (pair *)buffer;

    // 3. Manually initialize the pair "View" 
    p->header.magic = CS_PAIR_MAGIC;
    p->first_attr  = m->key_attr;
    p->second_attr = m->val_attr;
    p->has_first   = 1;
    p->has_second  = 1;
    // p->data is NOT a pointer anymore if you used char data[]
    // If you kept data as a pointer, we point it to the bytes immediately following the struct
    // But since we want performance, we assume: char data[] (Flexible Array Member)

    // 4. Copy Key and Value into the stack buffer
    if (m->key_attr->copy) m->key_attr->copy(p->data, key);
    else memcpy(p->data, key, k_sz);

    if (m->val_attr->copy) m->val_attr->copy(p->data + k_sz, val);
    else memcpy(p->data + k_sz, val, v_sz);

    // 5. Pass the stack-allocated pair to RBT
    // RBT will malloc(total_sz) once and memcpy(node->data, p, total_sz)
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
    // 1. Stack buffer
    int k_sz = m->key_attr->size;
    char dummy_buf[sizeof(pair) + k_sz];
    pair *dummy = (pair*)dummy_buf;
    
    // 2. Setup (No Malloc!)
    dummy->header.magic = CS_PAIR_MAGIC;
    dummy->first_attr = m->key_attr;
    dummy->has_first = 1;
    if (m->key_attr->copy) m->key_attr->copy(dummy->data, key);
    else memcpy(dummy->data, key, k_sz);

    // 3. Search (Pass pointer)
    pair* result = (pair*)__rbt_find(m->t, dummy);
    
    // 4. Return value pointer directly from the tree node
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

    // 1. Prepare the stack buffer
    char buffer[sizeof(pair) + m->key_attr->size];
    pair *search_key = (pair *)buffer;
    
    // 2. Initialize minimal state for comparison
    search_key->header.magic = CS_PAIR_MAGIC;
    search_key->first_attr = m->key_attr;
    search_key->has_first = 1;
    search_key->has_second = 0;

    if (m->key_attr->copy) m->key_attr->copy(search_key->data, key);
    else memcpy(search_key->data, key, m->key_attr->size);

    // 3. FIX: Pass search_key, NOT &search_key
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