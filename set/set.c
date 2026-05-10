#include <cs/set.h>

cs_codes set_init(set *s, elem_attr_t attr) {
    CS_RETURN_IF(NULL == s, CS_NULL);
    CS_RETURN_IF(attr.size == 0 || attr.size > SIZE_TH, CS_SIZE);
    s->t = malloc(sizeof(__rbt));
    CS_RETURN_IF(NULL == s->t, CS_MEM);
    return __rbt_init(s->t, attr);
}

void set_swap(set *s1, set *s2) {
    CS_RETURN_IF(s1 == NULL || s2 == NULL);
    __rbt_swap(s1->t, s2->t);
}

void set_clear(set *s) {
    CS_RETURN_IF(s == NULL);
    __rbt_clear(s->t);
}

void set_print(FILE *stream, void *v_s) {
    CS_RETURN_IF(v_s == NULL);
    set *s = (set *)v_s;
    __rbt_print(stream, s->t);
}

void set_free(void *v_s) {
    CS_RETURN_IF(v_s == NULL);
    set *s = (set *)v_s;
    __rbt_free(s->t);
    free(s->t);
}