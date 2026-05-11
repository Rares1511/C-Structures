#include <cs/map.h>

cs_codes map_init(map *m, elem_attr_t key_attr, elem_attr_t val_attr) {
    CS_RETURN_IF(NULL == m, CS_NULL);
    CS_RETURN_IF(key_attr.size <= 0 || key_attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(val_attr.size <= 0 || val_attr.size > SIZE_TH, CS_SIZE);
    elem_attr_t pair_attr = {
        .comp = __map_node_comp,
        .copy = __map_node_copy,
        .fr = __pair_free_internal,
        .print = pair_print,
        .size = sizeof(pair) + key_attr.size + val_attr.size,
    };
    m->key_attr = malloc(sizeof(elem_attr_t));
    m->val_attr = malloc(sizeof(elem_attr_t));
    CS_RETURN_IF(m->key_attr == NULL || m->val_attr == NULL, CS_MEM);
    memcpy(m->key_attr, &key_attr, sizeof(elem_attr_t));
    memcpy(m->val_attr, &val_attr, sizeof(elem_attr_t));
    m->t = malloc(sizeof(__rbt));
    CS_RETURN_IF(NULL == m->t, CS_MEM);
    return __rbt_init(m->t, pair_attr);
}

void map_swap(map *m1, map *m2) {
    CS_RETURN_IF(m1 == NULL || m2 == NULL);
   
    elem_attr_t* temp_key_attr = m1->key_attr;
    elem_attr_t* temp_val_attr = m1->val_attr;

    m1->key_attr = m2->key_attr;
    m1->val_attr = m2->val_attr;

    m2->key_attr = temp_key_attr;
    m2->val_attr = temp_val_attr;

    __rbt_swap(m1->t, m2->t);
}

void map_clear(map *m) {
    CS_RETURN_IF(m == NULL);
    __rbt_clear(m->t);
}

void map_print(FILE *stream, void *v_m) {
    CS_RETURN_IF(v_m == NULL || stream == NULL);
    map *m = (map *)v_m;
    __rbt_print(stream, m->t);
}

void map_free(void *v_m) {
    CS_RETURN_IF(v_m == NULL);
    map *m = (map *)v_m;
    __rbt_free(m->t);
    free(m->key_attr);
    free(m->val_attr);
    free(m->t);
}