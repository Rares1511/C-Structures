#include <cs/multimap.h>

multimap* multimap_init(multimap *pool, elem_attr_t key_attr,
                           elem_attr_t value_attr) {
    CS_RETURN_IF(key_attr.size == 0 || key_attr.size > SIZE_TH || value_attr.size == 0 || value_attr.size > SIZE_TH, NULL);
    if (pool == NULL) {
        pool = malloc(sizeof(multimap));
        CS_RETURN_IF(pool == NULL, NULL);
    }
    multimap *mm = pool;

    mm->key_attr = NULL;
    mm->value_attr = NULL;
    mm->vec_attr = NULL;
    mm->t = NULL;
    mm->size = 0;
    mm->buffer = NULL;

    elem_attr_t vec_attr = {
        .size = sizeof(vector),
        .fr = __vector_free_internal,
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
    if (mm->key_attr == NULL) {
        multimap_free(mm);
        return NULL;
    }
    mm->value_attr = malloc(sizeof(elem_attr_t));
    if (mm->value_attr == NULL) {
        multimap_free(mm);
        return NULL;
    }
    mm->vec_attr = malloc(sizeof(elem_attr_t));
    if (mm->vec_attr == NULL) {
        multimap_free(mm);
        return NULL;
    }
    mm->buffer = malloc(sizeof(pair) + key_attr.size + vec_attr.size);
    if (mm->buffer == NULL) {
        multimap_free(mm);
        return NULL;
    }
    
    memset(mm->buffer, 0, sizeof(pair) + key_attr.size + vec_attr.size);
    memcpy(mm->key_attr, &key_attr, sizeof(elem_attr_t));
    memcpy(mm->value_attr, &value_attr, sizeof(elem_attr_t));
    memcpy(mm->vec_attr, &vec_attr, sizeof(elem_attr_t));

    pair *p = (pair *)mm->buffer;
    p->header.magic = CS_PAIR_MAGIC;
    p->first_attr = mm->key_attr;
    p->second_attr = mm->vec_attr;
    p->has_first = 1;
    p->has_second = 1;

    vector* vec = (vector *)(p->data + key_attr.size);
    vec->header.magic = CS_VECTOR_MAGIC;
    vec->vec = NULL;
    vec->cap = 2;
    vec->size = 0;
    vec->v_attr = (vector_attr_t){2, 1};
    vec->attr = *mm->value_attr;

    mm->t = __rbt_init(rbt_attr);
    if (mm->t == NULL) {
        multimap_free(mm);
        return NULL;
    }
    return mm;
}

void multimap_clear(multimap *mm) {
    CS_RETURN_IF(mm == NULL);
    __rbt_clear(mm->t);
    mm->size = 0;
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
    if (mm->key_attr) free(mm->key_attr);
    if (mm->value_attr) free(mm->value_attr);
    if (mm->vec_attr) free(mm->vec_attr);
    if (mm->buffer) free(mm->buffer);
    free(mm);
}