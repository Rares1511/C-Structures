#include <cs/map.h>
#include <cs/rbt.h>
#include <cs/pair.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../include/unittest.h"

void map_node_copy(void *dest, const void *src) {
    pair* d = (pair*)dest;
    const pair* s = (const pair*)src;
    if (d == NULL || s == NULL) {
        return;
    }
    pair_init(d, s->first_attr, s->second_attr);
    pair_set(d, s->first, s->second);
}

cs_codes map_init(map *m, map_attr_t key_attr, map_attr_t val_attr) {
    m->t = malloc(sizeof(rbt));
    if (m->t == NULL) {
        return CS_MEM;
    }
    map_attr_t pair_attr = {
        .comp = NULL,
        .copy = map_node_copy,
        .fr = pair_free,
        .print = pair_print,
        .size = sizeof(pair),
    };
    m->key_attr = key_attr;
    m->val_attr = val_attr;
    return rbt_init(m->t, pair_attr);
}

void map_free(void *v_m) {
    map *m = (map *)v_m;
    rbt_free(m->t);
    free(m->t);
    m->t = NULL;
}