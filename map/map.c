#include <cs/map.h>

map* map_init(map *pool, elem_attr_t key_attr, elem_attr_t val_attr) {
    CS_RETURN_IF(key_attr.size <= 0 || key_attr.size > SIZE_TH || val_attr.size <= 0 || val_attr.size > SIZE_TH, NULL);
    elem_attr_t pair_attr = {
        .comp = __map_node_comp,
        .copy = __map_node_copy,
        .fr = __pair_free_internal,
        .print = pair_print,
        .size = sizeof(pair) + key_attr.size + val_attr.size,
    };

    if (pool == NULL) {
        pool = malloc(sizeof(map));
        CS_RETURN_IF(pool == NULL, NULL);
    }
    map *m = pool;

    m->key_attr = NULL;
    m->val_attr = NULL;
    m->buffer = NULL;
    m->t = NULL;

    m->key_attr = malloc(sizeof(elem_attr_t));
    if (m->key_attr == NULL) {
        map_free(m);
        return NULL;
    }
    m->val_attr = malloc(sizeof(elem_attr_t));
    if (m->val_attr == NULL) {
        map_free(m);
        return NULL;
    }
    m->buffer = malloc(sizeof(pair) + key_attr.size + val_attr.size);
    if (m->buffer == NULL) {
        map_free(m);
        return NULL;
    }

    memcpy(m->key_attr, &key_attr, sizeof(elem_attr_t));
    memcpy(m->val_attr, &val_attr, sizeof(elem_attr_t));

    pair *p = (pair *)m->buffer;
    p->header.magic = CS_PAIR_MAGIC;
    p->first_attr = m->key_attr;
    p->second_attr = m->val_attr;
    p->has_first = 1;
    p->has_second = 1;

    m->t = __rbt_init(pair_attr);
    if (m->t == NULL) {
        map_free(m);
        return NULL;
    }
    return m;
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
    if (m->key_attr) {
        free(m->key_attr);
    }
    if (m->val_attr) {
        free(m->val_attr);
    }
    if (m->buffer) {
        free(m->buffer);
    }
    free(m);
}