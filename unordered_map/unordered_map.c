#include <cs/unordered_map.h>

unordered_map* unordered_map_init(unordered_map *pool,
                                elem_attr_t key_attr,
                                 elem_attr_t value_attr,
                                 __hash_func_t hash_func) {
    CS_RETURN_IF(key_attr.size == 0 || value_attr.size == 0 || key_attr.size > SIZE_TH || value_attr.size > SIZE_TH, NULL);
    if (pool == NULL) {
        pool = malloc(sizeof(unordered_map));
        CS_RETURN_IF(pool == NULL, NULL);
    }
    unordered_map *umap = pool;

    umap->ht = NULL;
    umap->key_attr = NULL;
    umap->value_attr = NULL;
    umap->buffer = NULL;
   
    umap->key_attr = malloc(sizeof(elem_attr_t));
    if (umap->key_attr == NULL) {
        unordered_map_free(umap);
        return NULL;
    }
    umap->value_attr = malloc(sizeof(elem_attr_t));
    if (umap->value_attr == NULL) {
        unordered_map_free(umap);
        return NULL;
    }
    umap->buffer = malloc(sizeof(__unordered_map_entry) + sizeof(pair) + key_attr.size + value_attr.size);
    if (umap->buffer == NULL) {
        unordered_map_free(umap);
        return NULL;
    }
    
    memcpy(umap->key_attr, &key_attr, sizeof(elem_attr_t));
    memcpy(umap->value_attr, &value_attr, sizeof(elem_attr_t));
    
    umap->hash_func = hash_func;

    __unordered_map_entry *entry = (__unordered_map_entry *)umap->buffer;
    pair *p = (pair *)entry->data;

    entry->hash_func = hash_func;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    p->has_first = 1;
    p->has_second = 1;
    p->first_attr = umap->key_attr;
    p->second_attr = umap->value_attr;
    
    elem_attr_t entry_attr = {
        .comp = __unordered_map_entry_comp,
        .copy = __unordered_map_entry_copy,
        .fr = __unordered_map_entry_free,
        .print = __unordered_map_entry_print,
        .size = sizeof(__unordered_map_entry) + sizeof(pair) + key_attr.size + value_attr.size,
    };

    umap->ht = __hash_table_init(entry_attr, __unordered_map_entry_hash);
    if (umap->ht == NULL) {
        unordered_map_free(umap);
        return NULL;
    }
    return umap;
}

void unordered_map_swap(unordered_map *umap1, unordered_map *umap2) {
    CS_RETURN_IF(NULL == umap1 || NULL == umap2);

    __hash_table *temp_ht = umap1->ht;
    elem_attr_t *temp_key_attr = umap1->key_attr;
    elem_attr_t *temp_value_attr = umap1->value_attr;
    __hash_func_t temp_hash_func = umap1->hash_func;

    umap1->key_attr = umap2->key_attr;
    umap1->value_attr = umap2->value_attr;
    umap1->hash_func = umap2->hash_func;
    umap1->ht = umap2->ht;

    umap2->ht = temp_ht;
    umap2->key_attr = temp_key_attr;
    umap2->value_attr = temp_value_attr;
    umap2->hash_func = temp_hash_func;
}

void unordered_map_clear(unordered_map *umap) {
    CS_RETURN_IF(NULL == umap);
    __hash_table_clear(umap->ht);
}

void unordered_map_print(FILE *stream, void *v_umap) {
    CS_RETURN_IF(NULL == stream || NULL == v_umap);
    unordered_map *umap = (unordered_map *)v_umap;
    __hash_table_print(stream, umap->ht);
}

void unordered_map_free(void *v_umap) {
    CS_RETURN_IF(NULL == v_umap);
    unordered_map *umap = (unordered_map *)v_umap;
    __hash_table_free(umap->ht);
    if (umap->key_attr) free(umap->key_attr);
    if (umap->value_attr) free(umap->value_attr);
    if (umap->buffer) free(umap->buffer);
    free(umap);
}