#include <cs/multiset.h>

cs_codes multiset_init(multiset *ms, elem_attr_t attr) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(NULL == ms, CS_NULL);

    elem_attr_t rbt_attr = {
        .comp = __multiset_node_comp,
        .copy = __multiset_node_copy,
        .fr = pair_free,
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

    ms->size = 0;
    ms->el_attr = malloc(sizeof(elem_attr_t));
    ms->count_attr = malloc(sizeof(elem_attr_t));
    CS_RETURN_IF(NULL == ms->el_attr || NULL == ms->count_attr, CS_MEM);
    memcpy(ms->el_attr, &attr, sizeof(elem_attr_t));
    memcpy(ms->count_attr, &count_attr, sizeof(elem_attr_t));

    ms->t = malloc(sizeof(__rbt));
    CS_RETURN_IF(NULL == ms->t, CS_MEM);
    return __rbt_init(ms->t, rbt_attr);
}

void multiset_clear(multiset *ms) {
    CS_RETURN_IF(ms == NULL);
    __rbt_clear(ms->t);
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
    free(ms->el_attr);
    free(ms->count_attr);
    free(ms->t);
}