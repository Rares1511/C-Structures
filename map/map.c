#include <cs/map.h>
#include <cs/rbt.h>
#include <cs/pair.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


void map_node_copy(void *dest, const void *src) {
    pair* d = (pair*)dest;
    const pair* s = (const pair*)src;
    if (d == NULL || s == NULL) {
        return;
    }
    d->first_attr = s->first_attr;
    d->second_attr = s->second_attr;
    d->first = s->first;
    d->second = s->second;
}

int map_node_comp(const void *a, const void *b) {
    const pair* pa = (const pair*)a;
    const pair* pb = (const pair*)b;
    
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(pa->first, pb->first);
    }
    return memcmp(pa->first, pb->first, pa->first_attr->size);
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr) {
    CS_RETURN_IF(NULL == m, CS_NULL);
    CS_RETURN_IF(key_attr.size <= 0 || key_attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(val_attr.size <= 0 || val_attr.size > SIZE_TH, CS_SIZE);
    map_attr_t pair_attr = {
        .comp = map_node_comp,
        .copy = map_node_copy,
        .fr = pair_free,
        .print = pair_print,
        .size = sizeof(pair),
    };
    m->key_attr = malloc(sizeof(map_attr_t));
    m->val_attr = malloc(sizeof(map_attr_t));
    CS_RETURN_IF(m->key_attr == NULL || m->val_attr == NULL, CS_MEM);
    memcpy(m->key_attr, &key_attr, sizeof(map_attr_t));
    memcpy(m->val_attr, &val_attr, sizeof(map_attr_t));
    m->t = malloc(sizeof(rbt));
    CS_RETURN_IF(NULL == m->t, CS_MEM);
    return rbt_init(m->t, pair_attr);
}

cs_codes map_insert(map *m, void *key, void *val) {
    CS_RETURN_IF(m == NULL || key == NULL || val == NULL, CS_NULL);
    pair data;
    pair_init(&data, m->key_attr, m->val_attr);
    pair_set(&data, key, val);

    return rbt_insert(m->t, &data);
}

cs_codes map_delete(map *m, void *key) {
    CS_RETURN_IF(m == NULL || key == NULL, CS_NULL);
    pair search_key;
    pair_init(&search_key, m->key_attr, m->val_attr);
    pair_set(&search_key, key, NULL);

    cs_codes result = rbt_delete(m->t, &search_key);
    pair_free(&search_key);
    return result;
}

int map_empty(map m) {
    CS_RETURN_IF(m.t == NULL, 1);
    return rbt_empty(*(m.t));
}

int map_size(map m) {
    CS_RETURN_IF(m.t == NULL, 0);
    return rbt_size(*(m.t));
}   

void* map_find(map m, void *key) {
    CS_RETURN_IF(m.t == NULL || key == NULL, NULL);
    pair search_key;
    pair_init(&search_key, m.key_attr, m.val_attr);
    pair_set(&search_key, key, NULL);

    pair* result = (pair*)rbt_find(*(m.t), &search_key);
    pair_free(&search_key);
    CS_RETURN_IF(result == NULL, NULL);
    return result->second;
}

void map_swap(map *m1, map *m2) {
    CS_RETURN_IF(m1 == NULL || m2 == NULL);
   
    map_attr_t* temp_key_attr = m1->key_attr;
    map_attr_t* temp_val_attr = m1->val_attr;

    m1->key_attr = m2->key_attr;
    m1->val_attr = m2->val_attr;

    m2->key_attr = temp_key_attr;
    m2->val_attr = temp_val_attr;

    rbt_swap(m1->t, m2->t);
}

void map_clear(map *m) {
    CS_RETURN_IF(m == NULL);
    rbt_clear(m->t);
}

void map_print(FILE *stream, void *v_m) {
    CS_RETURN_IF(v_m == NULL || stream == NULL);
    map *m = (map *)v_m;
    rbt_print(stream, m->t);
}

void map_free(void *v_m) {
    CS_RETURN_IF(v_m == NULL);
    map *m = (map *)v_m;
    rbt_free(m->t);
    free(m->key_attr);
    free(m->val_attr);
    free(m->t);
}