#include <cs/multimap.h>

cs_codes multimap_init(multimap *mm, elem_attr_t key_attr,
                           elem_attr_t value_attr) {
    CS_RETURN_IF(NULL == mm, CS_NULL);
    CS_RETURN_IF(key_attr.size == 0 || key_attr.size > SIZE_TH || value_attr.size == 0 || value_attr.size > SIZE_TH, CS_SIZE);

    elem_attr_t vec_attr = {
        .size = sizeof(vector),
        .fr = vector_free,
        .print = vector_print,
        .comp = NULL,
        .copy = NULL,
    };

    elem_attr_t rbt_attr = {
        .size = sizeof(pair) + key_attr.size + vec_attr.size,
        .fr = __pair_free_internal,
        .print = pair_print,
        .comp = __multimap_node_comp,
        .copy = __multimap_node_copy,
    };

    mm->key_attr = malloc(sizeof(elem_attr_t));
    mm->value_attr = malloc(sizeof(elem_attr_t));
    mm->vec_attr = malloc(sizeof(elem_attr_t));
    CS_RETURN_IF(NULL == mm->key_attr || NULL == mm->value_attr || NULL == mm->vec_attr, CS_MEM);
    memcpy(mm->key_attr, &key_attr, sizeof(elem_attr_t));
    memcpy(mm->value_attr, &value_attr, sizeof(elem_attr_t));
    memcpy(mm->vec_attr, &vec_attr, sizeof(elem_attr_t));

    mm->t = malloc(sizeof(__rbt));
    mm->size = 0;
    CS_RETURN_IF(NULL == mm->t, CS_MEM);
    return __rbt_init(mm->t, rbt_attr);
}

void multimap_clear(multimap *mm) {
    CS_RETURN_IF(mm == NULL);
    __rbt_clear(mm->t);
}

void multimap_swap(multimap *mm1, multimap *mm2) {
    CS_RETURN_IF(mm1 == NULL || mm2 == NULL);
    __rbt_swap(mm1->t, mm2->t);
    elem_attr_t* temp_key_attr = mm1->key_attr;
    elem_attr_t* temp_value_attr = mm1->value_attr;
    elem_attr_t* temp_vec_attr = mm1->vec_attr;
    int temp_size = mm1->size;

    mm1->key_attr = mm2->key_attr;
    mm1->value_attr = mm2->value_attr;
    mm1->vec_attr = mm2->vec_attr;
    mm1->size = mm2->size;

    mm2->key_attr = temp_key_attr;
    mm2->value_attr = temp_value_attr;
    mm2->vec_attr = temp_vec_attr;
    mm2->size = temp_size;
}

void multimap_print(FILE *stream, void *v_mm) {
    CS_RETURN_IF(v_mm == NULL);
    multimap *mm = (multimap *)v_mm;
    __rbt_print(stream, mm->t);
}

void multimap_free(void *v_mm) {
    CS_RETURN_IF(v_mm == NULL);
    multimap *mm = (multimap *)v_mm;
    __rbt_free(mm->t);
    free(mm->key_attr);
    free(mm->value_attr);
    free(mm->vec_attr);
    free(mm->t);
}