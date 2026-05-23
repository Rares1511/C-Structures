#include <cs/set.h>

set* set_init(set *pool, elem_attr_t attr) {
    if (pool == NULL) {
        pool = malloc(sizeof(set));
        CS_RETURN_IF(pool == NULL, NULL);
    }

    set *s = pool;
    CS_RETURN_IF(NULL == s || attr.size == 0 || attr.size > SIZE_TH, NULL);
    s->t = __rbt_init(attr);
    if (s->t == NULL) {
        free(s);
        return NULL;
    }
    return s;
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
    free(s);
}