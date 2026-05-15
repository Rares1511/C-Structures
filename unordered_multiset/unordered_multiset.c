#include <cs/unordered_multiset.h>

unordered_multiset* unordered_multiset_init(elem_attr_t attr, 
                                __hash_func_t hash_func) {
    CS_RETURN_IF(attr.size == 0 || attr.size > SIZE_TH, NULL);
    unordered_multiset *umset = malloc(sizeof(unordered_multiset));
    CS_RETURN_IF(umset == NULL, NULL);

    umset->size = 0;
    umset->attr = NULL;
    umset->count_attr = NULL;
    umset->buffer = NULL;
    umset->ht = NULL;

    elem_attr_t entry_attr = {
        .size = sizeof(__unordered_multiset_entry) + sizeof(pair) + attr.size + sizeof(int), 
        .copy = __unordered_multiset_entry_copy,
        .fr = __unordered_multiset_entry_free,
        .print = __unordered_multiset_entry_print,
        .comp = __unordered_multiset_entry_comp
    };

    static elem_attr_t count_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL
    };                            
    
    umset->attr = malloc(sizeof(elem_attr_t));
    if (umset->attr == NULL) {
        unordered_multiset_free(umset);
        return NULL;
    }
    umset->count_attr = malloc(sizeof(elem_attr_t));
    if (umset->count_attr == NULL) {
        unordered_multiset_free(umset);
        return NULL;
    }
    umset->buffer = malloc(sizeof(__unordered_multiset_entry) + sizeof(pair) + attr.size + sizeof(int));
    if (umset->buffer == NULL) {
        unordered_multiset_free(umset);
        return NULL;
    }

    memset(umset->buffer, 0, sizeof(__unordered_multiset_entry) + sizeof(pair) + attr.size + sizeof(int));
    memcpy(umset->attr, &attr, sizeof(elem_attr_t));
    memcpy(umset->count_attr, &count_attr, sizeof(elem_attr_t));

    __unordered_multiset_entry *buffer_entry = (__unordered_multiset_entry *)umset->buffer;
    pair *buffer_pair = (pair *)buffer_entry->data;
    int *buffer_count = (int *)((char *)buffer_pair->data + attr.size);

    buffer_entry->hash_func = hash_func;
    buffer_pair->header.magic = CS_PAIR_MAGIC;
    buffer_pair->header.type = CS_PAIR_TYPE;
    buffer_pair->first_attr = umset->attr;
    buffer_pair->second_attr = umset->count_attr;
    buffer_pair->has_first = 1;
    buffer_pair->has_second = 1;
    *buffer_count = 0;

    umset->ht = __hash_table_init(entry_attr, __unordered_multiset_entry_hash);
    if (umset->ht == NULL) {
        unordered_multiset_free(umset);
        return NULL;
    }
    return umset;
}

void unordered_multiset_clear(unordered_multiset *restrict umset) {
    CS_RETURN_IF(umset == NULL);
    __hash_table_clear(umset->ht);
    umset->size = 0;
}

void unordered_multiset_print(FILE *restrict stream, void *restrict v_umset) {
    CS_RETURN_IF(v_umset == NULL);
    unordered_multiset *umset = (unordered_multiset *)v_umset;
    __hash_table_print(stream, umset->ht);
}

void unordered_multiset_free(void *restrict v_umset) {
    CS_RETURN_IF(v_umset == NULL);
    unordered_multiset *umset = (unordered_multiset *)v_umset;
    __hash_table_free(umset->ht);
    if (umset->buffer) free(umset->buffer);
    if (umset->attr) free(umset->attr);
    if (umset->count_attr) free(umset->count_attr);
    free(umset);
}