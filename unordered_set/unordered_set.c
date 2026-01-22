#include <cs/unordered_set.h>
#include <cs/hash_table.h>

#include <stdlib.h>

unordered_set *unordered_set_init(unordered_set_attr_t attr, hash_func_t hash_func, int initial_capacity) {
    CS_RETURN_IF(initial_capacity <= 0 || attr.size <= 0 || attr.size > SIZE_TH, NULL);
    unordered_set *uset = (unordered_set *)malloc(sizeof(unordered_set));
    CS_RETURN_IF(uset == NULL, NULL);
    uset->ht = hash_table_init(attr, hash_func, initial_capacity);
    CS_RETURN_IF(uset->ht == NULL, NULL);
    return uset;
}

cs_codes unordered_set_insert(unordered_set *uset, const void *key) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL || key == NULL, CS_NULL);
    if (hash_table_get_entry(*(uset->ht), key) != NULL) {
        return CS_ELEM;
    }
    return hash_table_add_entry(uset->ht, key);
}

cs_codes unordered_set_erase(unordered_set *uset, const void *key) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL || key == NULL, CS_NULL);
    return hash_table_remove_entry(uset->ht, key);
}

void* unordered_set_find(unordered_set uset, const void *key) {
    CS_RETURN_IF(uset.ht == NULL || key == NULL, NULL);
    return hash_table_get_entry(*(uset.ht), key);
}

int unordered_set_empty(unordered_set uset) {
    CS_RETURN_IF(uset.ht == NULL, 1);
    return hash_table_empty(*(uset.ht));
}

int unordered_set_size(unordered_set uset) {
    CS_RETURN_IF(uset.ht == NULL, 0);
    return hash_table_size(*(uset.ht));
}

int unordered_set_count(unordered_set uset, const void *key) {
    CS_RETURN_IF(uset.ht == NULL || key == NULL, 0);
    return hash_table_count(*(uset.ht), key);
}

void unordered_set_clear(unordered_set *uset) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL);
    hash_table_clear(uset->ht);
}

void unordered_set_print(FILE *stream, void *v_uset) {
    CS_RETURN_IF(stream == NULL || v_uset == NULL);
    unordered_set *uset = (unordered_set *)v_uset;
    CS_RETURN_IF(uset == NULL || uset->ht == NULL);
    hash_table_print(stream, uset->ht);
}

void unordered_set_free(void *v_uset) {
    CS_RETURN_IF(v_uset == NULL);
    unordered_set *uset = (unordered_set *)v_uset;
    CS_RETURN_IF(uset == NULL || uset->ht == NULL);
    hash_table_free(uset->ht);
    free(uset);
}