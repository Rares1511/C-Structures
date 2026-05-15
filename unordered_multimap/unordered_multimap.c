#include <cs/unordered_multimap.h>

unordered_multimap* unordered_multimap_init(elem_attr_t key_attr,
                                 elem_attr_t value_attr,
                                 __hash_func_t hash_func) {
    
    CS_RETURN_IF(key_attr.size <= 0 || value_attr.size <= 0 || key_attr.size > SIZE_TH || value_attr.size > SIZE_TH, NULL);
    unordered_multimap *ummap = malloc(sizeof(unordered_multimap));
    CS_RETURN_IF(NULL == ummap, NULL);

    ummap->ht = NULL;
    ummap->key_attr = NULL;
    ummap->value_attr = NULL;
    ummap->buffer = NULL;
    ummap->hash_func = NULL;
    ummap->size = 0;

    ummap->buffer = malloc(sizeof(__unordered_multimap_entry) + sizeof(pair) + key_attr.size + sizeof(vector));
    if (ummap->buffer == NULL) {
        unordered_multimap_free(ummap);
        return NULL;
    }
    ummap->key_attr = malloc(sizeof(elem_attr_t));
    if (ummap->key_attr == NULL) {
        unordered_multimap_free(ummap);
        return NULL;
    }
    ummap->value_attr = malloc(sizeof(elem_attr_t));
    if (ummap->value_attr == NULL) {
        unordered_multimap_free(ummap);
        return NULL;
    }

    elem_attr_t ht_attr = {
        .size = sizeof(__unordered_multimap_entry) + sizeof(pair) + key_attr.size + sizeof(vector),
        .print = __unordered_multimap_entry_print,
        .fr = __unordered_multimap_entry_free,
        .comp = __unordered_multimap_entry_comp,
        .copy = __unordered_multimap_entry_copy,
    };

    static elem_attr_t pair_val_attr = {
        .size = sizeof(vector),
        .print = vector_print,
        .fr = __vector_free_internal,
        .comp = NULL,
        .copy = NULL,
    };

    ummap->hash_func = hash_func;
    memcpy(ummap->key_attr, &key_attr, sizeof(elem_attr_t));
    memcpy(ummap->value_attr, &value_attr, sizeof(elem_attr_t));
    memset(ummap->buffer, 0, sizeof(__unordered_multimap_entry) + sizeof(pair) + key_attr.size + sizeof(vector));

    // Initialize the hash function for the entry
    __unordered_multimap_entry *entry = (__unordered_multimap_entry *)ummap->buffer;
    entry->hash_func = hash_func;

    // Initialize the pair for the key-value entry
    pair *p = (pair *)entry->data;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    p->has_first = 1;
    p->has_second = 1;
    p->first_attr = ummap->key_attr;
    p->second_attr = &pair_val_attr;

    // Initialize the vector for the value
    vector *vec = (vector *)pair_second(p);
    vec->header.magic = CS_VECTOR_MAGIC;
    vec->header.type = CS_VECTOR_TYPE;
    vec->vec = NULL;
    vec->cap = 0;
    vec->size = 0;
    vec->v_attr = (vector_attr_t){.min_cap = 2, .shrink_factor = 1};
    vec->attr = value_attr;

    ummap->ht = __hash_table_init(ht_attr, __unordered_multimap_entry_hash);
    if (ummap->ht == NULL) {
        unordered_multimap_free(ummap);
        return NULL;
    }
    return ummap;
}

void unordered_multimap_swap(unordered_multimap *ummap1, unordered_multimap *ummap2) {
    CS_RETURN_IF(NULL == ummap1 || NULL == ummap2);

    __hash_table *temp_ht = ummap1->ht;
    __hash_func_t temp_hash_func = ummap1->hash_func;
    elem_attr_t *temp_key_attr = ummap1->key_attr;
    elem_attr_t *temp_value_attr = ummap1->value_attr;
    char *temp_buffer = ummap1->buffer;
    size_t temp_size = ummap1->size;

    ummap1->ht = ummap2->ht;
    ummap1->hash_func = ummap2->hash_func;
    ummap1->key_attr = ummap2->key_attr;
    ummap1->value_attr = ummap2->value_attr;
    ummap1->buffer = ummap2->buffer;
    ummap1->size = ummap2->size;

    ummap2->ht = temp_ht;
    ummap2->hash_func = temp_hash_func;
    ummap2->key_attr = temp_key_attr;
    ummap2->value_attr = temp_value_attr;
    ummap2->buffer = temp_buffer;
    ummap2->size = temp_size;
}

void unordered_multimap_clear(unordered_multimap *ummap) {
    CS_RETURN_IF(NULL == ummap);
    __hash_table_clear(ummap->ht);
    ummap->size = 0;
}

void unordered_multimap_print(FILE *stream, void *v_ummap) {
    CS_RETURN_IF(NULL == stream || NULL == v_ummap);
    unordered_multimap *ummap = (unordered_multimap *)v_ummap;
    __hash_table_print(stream, ummap->ht);
}

void unordered_multimap_free(void *v_ummap) {
    CS_RETURN_IF(NULL == v_ummap);
    unordered_multimap *ummap = (unordered_multimap *)v_ummap;
    __hash_table_free(ummap->ht);
    if (ummap->buffer) free(ummap->buffer);
    if (ummap->key_attr) free(ummap->key_attr);
    if (ummap->value_attr) free(ummap->value_attr);
    free(ummap);
}