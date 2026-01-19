#include <cs/map.h>
#include <cs/rbt.h>
#include <cs/pair.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../include/unittest.h"

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


cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr) {
    m->t = malloc(sizeof(rbt));
    if (m->t == NULL) {
        return CS_MEM;
    }
    map_attr_t pair_attr = {
        .comp = map_node_comp,
        .copy = map_node_copy,
        .fr = pair_free,
        .print = pair_print,
        .size = sizeof(pair),
    };
    m->key_attr = key_attr;
    m->val_attr = val_attr;
    return rbt_init(m->t, pair_attr);
}

cs_codes map_insert(map *m, void *key, void *val) {
    if (m == NULL || key == NULL || val == NULL) {
        return CS_ELEM;
    }
    pair data;
    pair_init(&data, &m->key_attr, &m->val_attr);
    pair_set(&data, key, val);

    return rbt_insert(m->t, &data);
}

int map_size(map m) {
    if (m.t == NULL) {
        return 0;
    }
    return m.t->size;
}   

void* map_find(map m, void *key) {
    if (m.t == NULL || key == NULL) {
        return NULL;
    }
    pair search_key;
    pair_init(&search_key, &m.key_attr, &m.val_attr);
    pair_set(&search_key, key, NULL);

    pair* result = (pair*)rbt_find(*(m.t), &search_key);
    pair_free(&search_key);
    if (result == NULL) {
        return NULL;
    }
    return result->second;
}

void map_print(FILE *stream, void *v_m) {
    if (v_m == NULL || stream == NULL) {
        return;
    }
    map *m = (map *)v_m;
    rbt_print(stream, m->t);
}

void map_free(void *v_m) {
    map *m = (map *)v_m;
    rbt_free(m->t);
    free(m->t);
    m->t = NULL;
}