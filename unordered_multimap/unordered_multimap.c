#include <cs/unordered_multimap.h>
#include <cs/pair.h>
#include <cs/hash_table.h>

#include <stdlib.h>
#include <string.h>

#pragma region Helper Structs
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        START OF HELPER STRUCT SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


typedef struct unordered_multimap_entry {
    pair *data;
    hash_func_t hash_func;
} unordered_multimap_entry;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                         END OF HELPER STRUCT SECTION                                       ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


void unordered_multimap_entry_init(unordered_multimap umap, const void *key, const void *value, unordered_multimap_entry *out) {
    if (!out) {
        return;
    }
    out->data = malloc(sizeof(pair));
    if (!out->data) {
        free(out);
        return;
    }
    pair_init(out->data, &umap.key_attr, &umap.value_attr);
    pair_set(out->data, key, value);
    out->hash_func = umap.hash_func;
}

void unordered_multimap_entry_copy(void *dest, const void *src) {
    unordered_multimap_entry *d = (unordered_multimap_entry *)dest;
    const unordered_multimap_entry *s = (const unordered_multimap_entry *)src;
    d->data = s->data;
    d->hash_func = s->hash_func;
}

void unordered_multimap_entry_print(FILE *stream, const void *el) {
    if (!stream || !el) {
        return;
    }
    const unordered_multimap_entry *entry = (const unordered_multimap_entry *)el;
    pair_print(stream, &entry->data);
}

void unordered_multimap_entry_free(void *el) {
    if (!el) {
        return;
    }
    unordered_multimap_entry *entry = (unordered_multimap_entry *)el;
    pair_free(&entry->data);
    free(entry->data);
}

int unordered_multimap_entry_comp(const void *a, const void *b) {
    const pair* pa = (const pair*)a;
    const pair* pb = (const pair*)b;
    
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(pa->first, pb->first);
    }
    return memcmp(pa->first, pb->first, pa->first_attr->size);
}

size_t unordered_multimap_entry_hash(const void *el) {
    // Simple hash function for demonstration purposes
    unordered_multimap_entry *entry = (unordered_multimap_entry *)el;
    if (entry == NULL) {
        return 0;
    }
    if (entry->hash_func == NULL) {
        return universal_hash_bytes(entry->data->first, entry->data->first_attr->size);
    }
    return entry->hash_func(entry->data->first);
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes unordered_multimap_init(unordered_multimap *ummap,
                                 unordered_multimap_attr_t key_attr,
                                 unordered_multimap_attr_t value_attr,
                                 hash_func_t hash_func,
                                 int initial_capacity) {
    CS_RETURN_IF(NULL == ummap, CS_NULL);
    CS_RETURN_IF(initial_capacity <= 0, CS_SIZE);
    CS_RETURN_IF(key_attr.size <= 0 || value_attr.size <= 0, CS_SIZE);
    CS_RETURN_IF(key_attr.size > SIZE_TH || value_attr.size > SIZE_TH, CS_SIZE);
    ummap->ht = malloc(sizeof(hash_table));
    CS_RETURN_IF(NULL == ummap->ht, CS_MEM);

    hash_table_attr_t ht_attr = {
        .size = sizeof(unordered_multimap_entry),
        .print = unordered_multimap_entry_print,
        .fr = unordered_multimap_entry_free,
        .comp = unordered_multimap_entry_comp,
        .copy = unordered_multimap_entry_copy,
    };

    ummap->hash_func = hash_func;
    ummap->key_attr = key_attr;
    ummap->value_attr = value_attr;

    return hash_table_init(ummap->ht, ht_attr, unordered_multimap_entry_hash, initial_capacity);
}

cs_codes unordered_multimap_add_entry(unordered_multimap *umap, const void *key, const void *value) {
    CS_RETURN_IF(NULL == umap || NULL == key || NULL == value, CS_NULL);

    unordered_multimap_entry entry;
    unordered_multimap_entry_init(*umap, key, value, &entry);

    cs_codes code = hash_table_add_entry(umap->ht, &entry);
    unordered_multimap_entry_free(&entry);
    return code;
}

cs_codes unordered_multimap_remove_entry(unordered_multimap *umap, const void *key) {
    CS_RETURN_IF(NULL == umap || NULL == key, CS_NULL);

    unordered_multimap_entry entry;
    unordered_multimap_entry_init(*umap, key, NULL, &entry);
    int rc = hash_table_remove_entry(umap->ht, &entry);
    unordered_multimap_entry_free(&entry);
    return rc;
}

void *unordered_multimap_get_entry(unordered_multimap umap, const void *key) {
    CS_RETURN_IF(NULL == umap.ht || NULL == key, NULL);

    unordered_multimap_entry entry;
    unordered_multimap_entry_init(umap, key, NULL, &entry);
    void *res = hash_table_get_entry(*(umap.ht), &entry);
    unordered_multimap_entry_free(&entry);
    if (res) {
        unordered_multimap_entry *found_entry = (unordered_multimap_entry *)res;
        return found_entry->data->second;
    }
    return NULL;
}

int unordered_multimap_empty(unordered_multimap umap) {
    CS_RETURN_IF(NULL == umap.ht, 1);
    return hash_table_empty(*(umap.ht));
}

int unordered_multimap_size(unordered_multimap umap) {
    CS_RETURN_IF(NULL == umap.ht, 0);
    return hash_table_size(*(umap.ht));
}

int unordered_multimap_count(unordered_multimap umap, const void *key) {
    CS_RETURN_IF(NULL == umap.ht || NULL == key, 0);

    unordered_multimap_entry entry;
    unordered_multimap_entry_init(umap, key, NULL, &entry);
    int count = hash_table_count(*(umap.ht), &entry);
    unordered_multimap_entry_free(&entry);
    return count;
}

void unordered_multimap_swap(unordered_multimap *umap1, unordered_multimap *umap2) {
    CS_RETURN_IF(NULL == umap1 || NULL == umap2);

    hash_table *temp_ht = umap1->ht;
    hash_func_t temp_hash_func = umap1->hash_func;
    unordered_multimap_attr_t temp_key_attr = umap1->key_attr;
    unordered_multimap_attr_t temp_value_attr = umap1->value_attr;

    umap1->ht = umap2->ht;
    umap1->hash_func = umap2->hash_func;
    umap1->key_attr = umap2->key_attr;
    umap1->value_attr = umap2->value_attr;

    umap2->ht = temp_ht;
    umap2->hash_func = temp_hash_func;
    umap2->key_attr = temp_key_attr;
    umap2->value_attr = temp_value_attr;
}

void unordered_multimap_clear(unordered_multimap *umap) {
    CS_RETURN_IF(NULL == umap);
    hash_table_clear(umap->ht);
}

void unordered_multimap_print(FILE *stream, void *v_umap) {
    CS_RETURN_IF(NULL == stream || NULL == v_umap);
    unordered_multimap *umap = (unordered_multimap *)v_umap;
    hash_table_print(stream, umap->ht);
}

void unordered_multimap_free(void *v_umap) {
    CS_RETURN_IF(NULL == v_umap);
    unordered_multimap *umap = (unordered_multimap *)v_umap;
    hash_table_free(umap->ht);
    free(umap->ht);
}