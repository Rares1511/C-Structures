#include <cs/unordered_set.h>
#include <cs/hash_table.h>

#include <stdlib.h>

cs_codes unordered_set_init(unordered_set *uset, unordered_set_attr_t attr, hash_func_t hash_func, int initial_capacity) {
    if (uset == NULL) {
        return CS_ELEM;
    }

    uset->ht = malloc(sizeof(hash_table));
    if (uset->ht == NULL) {
        return CS_MEM;
    }

    return hash_table_init(uset->ht, attr, hash_func, initial_capacity);
}

cs_codes unordered_set_insert(unordered_set *uset, const void *key) {
    if (uset == NULL || uset->ht == NULL || key == NULL) {
        return CS_ELEM;
    }
    if (hash_table_get_entry(*(uset->ht), key) != NULL) {
        return CS_ELEM;
    }
    return hash_table_add_entry(uset->ht, key);
}

cs_codes unordered_set_erase(unordered_set *uset, const void *key) {
    if (uset == NULL || uset->ht == NULL || key == NULL) {
        return CS_ELEM;
    }
    return hash_table_remove_entry(uset->ht, key);
}

void* unordered_set_find(unordered_set uset, const void *key) {
    if (uset.ht == NULL || key == NULL) {
        return NULL;
    }
    return hash_table_get_entry(*(uset.ht), key);
}

int unordered_set_count(unordered_set uset, const void *key) {
    if (uset.ht == NULL || key == NULL) {
        return 0;
    }
    return hash_table_count(*(uset.ht), key);
}

void unordered_set_clear(unordered_set *uset) {
    if (uset == NULL || uset->ht == NULL) {
        return;
    }
    hash_table_clear(uset->ht);
}

void unordered_set_free(void *v_uset) {
    if (v_uset == NULL) {
        return;
    }
    unordered_set *uset = (unordered_set *)v_uset;
    if (uset == NULL || uset->ht == NULL) {
        return;
    }
    hash_table_free(uset->ht);
    free(uset->ht);
    uset->ht = NULL;
}

void unordered_set_print(FILE *stream, void *v_uset) {
    if (stream == NULL || v_uset == NULL) {
        return;
    }
    unordered_set *uset = (unordered_set *)v_uset;
    if (uset == NULL || uset->ht == NULL) {
        return;
    }
    hash_table_print(stream, uset->ht);
}