#include <cs/unordered_multiset.h>
#include <cs/hash_table.h>

#include <stdlib.h>
#include <string.h>

cs_codes unordered_multiset_init(unordered_multiset *umset, 
                            unordered_multiset_attr_t attr, 
                            hash_func_t hash_func, 
                            int initial_capacity) {
    CS_RETURN_IF(NULL == umset, CS_NULL);
    CS_RETURN_IF(initial_capacity <= 0 || attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    umset->ht = malloc(sizeof(hash_table));
    CS_RETURN_IF(NULL == umset->ht, CS_MEM);
    return hash_table_init(umset->ht, attr, hash_func, initial_capacity);
}

cs_codes unordered_multiset_insert(unordered_multiset *umset, const void *key) {
    CS_RETURN_IF(umset == NULL || key == NULL, CS_NULL);
    return hash_table_add_entry(umset->ht, key);
}

cs_codes unordered_multiset_erase(unordered_multiset *umset, const void *key) {
    CS_RETURN_IF(umset == NULL || key == NULL, CS_NULL);
    return hash_table_remove_entry(umset->ht, key);
}

void unordered_multiset_clear(unordered_multiset *umset) {
    CS_RETURN_IF(umset == NULL);
    hash_table_clear(umset->ht);
}

void *unordered_multiset_find(unordered_multiset umset, const void *key) {
    CS_RETURN_IF(umset.ht == NULL || key == NULL, NULL);
    return hash_table_get_entry(*(umset.ht), key);
}

int unordered_multiset_count(unordered_multiset umset, const void *key) {
    CS_RETURN_IF(umset.ht == NULL || key == NULL, 0);
    return hash_table_count(*(umset.ht), key);
}

int unordered_multiset_empty(unordered_multiset umset) {
    CS_RETURN_IF(umset.ht == NULL, 1);
    return hash_table_empty(*(umset.ht));
}

int unordered_multiset_size(unordered_multiset umset) {
    CS_RETURN_IF(umset.ht == NULL, 0);
    return hash_table_size(*(umset.ht));
}

void unordered_multiset_print(FILE *stream, void *v_umset) {
    CS_RETURN_IF(v_umset == NULL);
    unordered_multiset *umset = (unordered_multiset *)v_umset;
    hash_table_print(stream, umset->ht);
}

void unordered_multiset_free(void *v_umset) {
    CS_RETURN_IF(v_umset == NULL);
    unordered_multiset *umset = (unordered_multiset *)v_umset;
    hash_table_free(umset->ht);
    free(umset->ht);
}