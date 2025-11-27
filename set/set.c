#include <cs/set.h>
#include <cs/rbt.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../include/unittest.h"

cs_codes set_init(set *s, set_attr_t attr) {
    s->t = malloc(sizeof(rbt));
    if (s->t == NULL) {
        return CS_MEM;
    }
    return rbt_init(s->t, attr);
}

cs_codes set_insert(set *s, void *data) {
    return rbt_insert(s->t, data);
}

cs_codes set_delete(set *s, void *data) {
    return rbt_delete(s->t, data);
}

int set_size(set s) {
    return s.t->size;
}

void* set_find(set s, void *data) {
    return rbt_find(*s.t, data);
}

void set_swap(set *s1, set *s2) {
    rbt_swap(s1->t, s2->t);
}

void set_clear(set *s) {
    rbt_clear(s->t);
}

void set_print(void *v_s) {
    set *s = (set *)v_s;
    rbt_print(s->t);
}

void set_free(void *v_s) {
    set *s = (set *)v_s;
    set_clear(s);
    free(s->t);
}