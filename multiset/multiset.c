#include <cs/multiset.h>

multiset* multiset_init(multiset *pool, elem_attr_t attr) {
    CS_RETURN_IF(attr.size == 0 || attr.size > SIZE_TH, NULL);

    if (pool == NULL) {
        pool = malloc(sizeof(multiset));
        CS_RETURN_IF(pool == NULL, NULL);
    }

    multiset *ms = pool;
    ms->count_attr = NULL;
    ms->el_attr = NULL;
    ms->t = NULL;
    ms->size = 0;

    elem_attr_t rbt_attr = {
        .comp = __multiset_node_comp,
        .copy = __multiset_node_copy,
        .fr = __pair_free_internal,
        .print = __multiset_node_print,
        .size = sizeof(pair) + attr.size + sizeof(int) // pair struct + element + count
    };

    elem_attr_t count_attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = NULL,
        .comp = NULL
    };

    ms->el_attr = malloc(sizeof(elem_attr_t));
    if (ms->el_attr == NULL) {
        multiset_free(ms);
        return NULL;
    }
    ms->count_attr = malloc(sizeof(elem_attr_t));
    if (ms->count_attr == NULL) {
        multiset_free(ms);
        return NULL;
    }
    memcpy(ms->el_attr, &attr, sizeof(elem_attr_t));
    memcpy(ms->count_attr, &count_attr, sizeof(elem_attr_t));

    ms->t = __rbt_init(rbt_attr);
    if (ms->t == NULL) {
        multiset_free(ms);
        return NULL;
    }
    return ms;
}

void multiset_clear(multiset *ms) {
    CS_RETURN_IF(ms == NULL);
    __rbt_clear(ms->t);
    ms->size = 0;
}

void multiset_swap(multiset *ms1, multiset *ms2) {
    CS_RETURN_IF(ms1 == NULL || ms2 == NULL);
    elem_attr_t* temp_el_attr = ms1->el_attr;
    elem_attr_t* temp_count_attr = ms1->count_attr;
    __rbt *temp_t = ms1->t;
    int temp_size = ms1->size;

    ms1->el_attr = ms2->el_attr;
    ms1->count_attr = ms2->count_attr;
    ms1->size = ms2->size;
    ms1->t = ms2->t;

    ms2->el_attr = temp_el_attr;
    ms2->count_attr = temp_count_attr;
    ms2->size = temp_size;
    ms2->t = temp_t;
}

void multiset_print(FILE *stream, void *v_ms) {
    CS_RETURN_IF(v_ms == NULL || stream == NULL);
    multiset *ms = (multiset *)v_ms;
    __rbt_print(stream, ms->t);
}

void multiset_free(void *v_ms) {
    CS_RETURN_IF(v_ms == NULL);
    multiset *ms = (multiset *)v_ms;
    __rbt_free(ms->t);
    if (ms->el_attr) {
        free(ms->el_attr);
    }
    if (ms->count_attr) {
        free(ms->count_attr);
    }
    free(ms);
}