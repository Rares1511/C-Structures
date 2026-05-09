#include <cs/unordered_set.h>

cs_codes unordered_set_init(unordered_set *uset, elem_attr_t attr, __hash_func_t hash_func) {
    CS_RETURN_IF(NULL == uset, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    uset->ht = malloc(sizeof(__hash_table));
    CS_RETURN_IF(NULL == uset->ht, CS_MEM);
    return __hash_table_init(uset->ht, attr, hash_func);
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
    free(uset->ht);
}