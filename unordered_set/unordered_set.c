#include <cs/unordered_set.h>

unordered_set* unordered_set_init(elem_attr_t attr, __hash_func_t hash_func) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, NULL);
    unordered_set *uset = malloc(sizeof(unordered_set));
    CS_RETURN_IF(uset == NULL, NULL);
    uset->ht = __hash_table_init(attr, hash_func);
    if (uset->ht == NULL) {
        free(uset);
        return NULL;
    }
    return uset;
}

void unordered_set_clear(unordered_set *uset) {
    CS_RETURN_IF(uset == NULL || uset->ht == NULL);
    __hash_table_clear(uset->ht);
}

void unordered_set_print(FILE *stream, void *v_uset) {
    CS_RETURN_IF(stream == NULL || v_uset == NULL);
    unordered_set *uset = (unordered_set *)v_uset;
    CS_RETURN_IF(uset == NULL || uset->ht == NULL);
    __hash_table_print(stream, uset->ht);
}

void unordered_set_free(void *v_uset) {
    CS_RETURN_IF(v_uset == NULL);
    unordered_set *uset = (unordered_set *)v_uset;
    CS_RETURN_IF(uset == NULL || uset->ht == NULL);
    __hash_table_free(uset->ht);
    free(uset);
}