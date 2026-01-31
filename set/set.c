#include <cs/set.h>
#include <cs/rbt.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>

cs_codes set_init(set *s, set_attr_t attr) {
    CS_RETURN_IF(NULL == s, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    s->t = malloc(sizeof(rbt));
    CS_RETURN_IF(NULL == s->t, CS_MEM);
    return rbt_init(s->t, attr);
}

cs_codes set_insert(set *s, void *data) {
    CS_RETURN_IF(s == NULL, CS_NULL);
    return rbt_insert(s->t, data);
}

cs_codes set_delete(set *s, void *data) {
    CS_RETURN_IF(s == NULL, CS_NULL);
    return rbt_delete(s->t, data);
}

int set_empty(set s) {
    CS_RETURN_IF(s.t == NULL, 1);
    return rbt_empty(*s.t);
}

int set_size(set s) {
    CS_RETURN_IF(s.t == NULL, 0);
    return rbt_size(*s.t);
}

void* set_find(set s, void *data) {
    return rbt_find(*s.t, data);
}

void set_swap(set *s1, set *s2) {
    CS_RETURN_IF(s1 == NULL || s2 == NULL);
    rbt_swap(s1->t, s2->t);
}

void set_clear(set *s) {
    CS_RETURN_IF(s == NULL);
    rbt_clear(s->t);
}

void set_print(FILE *stream, void *v_s) {
    CS_RETURN_IF(v_s == NULL);
    set *s = (set *)v_s;
    rbt_print(stream, s->t);
}

void set_free(void *v_s) {
    CS_RETURN_IF(v_s == NULL);
    set *s = (set *)v_s;
    rbt_free(s->t);
    free(s->t);
}