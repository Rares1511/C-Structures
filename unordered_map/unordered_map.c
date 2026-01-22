#include <cs/unordered_map.h>
#include <cs/hash_table.h>
#include <cs/pair.h>

#include <stdlib.h>
#include <string.h>

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        START OF HELPER STRUCT SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


typedef struct unordered_map_entry {
    pair *data;
    hash_func_t hash_func;
} unordered_map_entry;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                         END OF HELPER STRUCT SECTION                                       ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


void unordered_map_entry_init(unordered_map umap, const void *key, const void *value, unordered_map_entry *out) {
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

void unordered_map_entry_copy(void *dest, const void *src) {
    unordered_map_entry *d = (unordered_map_entry *)dest;
    const unordered_map_entry *s = (const unordered_map_entry *)src;
    d->data = s->data;
    d->hash_func = s->hash_func;
}

void unordered_map_entry_print(FILE *stream, const void *el) {
    if (!stream || !el) {
        return;
    }
    const unordered_map_entry *entry = (const unordered_map_entry *)el;
    pair_print(stream, &entry->data);
}

void unordered_map_entry_free(void *el) {
    if (!el) {
        return;
    }
    unordered_map_entry *entry = (unordered_map_entry *)el;
    pair_free(&entry->data);
    free(entry->data);
}

int unordered_map_entry_comp(const void *a, const void *b) {
    const pair* pa = (const pair*)a;
    const pair* pb = (const pair*)b;
    
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(pa->first, pb->first);
    }
    return memcmp(pa->first, pb->first, pa->first_attr->size);
}

size_t unordered_map_entry_hash(const void *el) {
    // Simple hash function for demonstration purposes
    unordered_map_entry *entry = (unordered_map_entry *)el;
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


unordered_map *unordered_map_init(unordered_map_attr_t key_attr,
                                 unordered_map_attr_t value_attr,
                                 hash_func_t hash_func,
                                 int initial_capacity) {
    CS_RETURN_IF(initial_capacity <= 0, NULL);
    CS_RETURN_IF(key_attr.size == 0 || value_attr.size == 0, NULL);
    CS_RETURN_IF(key_attr.size > SIZE_TH || value_attr.size > SIZE_TH, NULL);
    unordered_map *umap = malloc(sizeof(unordered_map));
    CS_RETURN_IF(NULL == umap, NULL);

    umap->key_attr = key_attr;
    umap->value_attr = value_attr;
    umap->hash_func = hash_func;

    unordered_map_attr_t entry_attr = {
        .comp = unordered_map_entry_comp,
        .copy = unordered_map_entry_copy,
        .fr = unordered_map_entry_free,
        .print = unordered_map_entry_print,
        .size = sizeof(unordered_map_entry),
    };

    umap->ht = hash_table_init(entry_attr, unordered_map_entry_hash, initial_capacity);
    CS_RETURN_IF(NULL == umap->ht, NULL);
    return umap;
}

cs_codes unordered_map_add_entry(unordered_map *umap, const void *key, const void *value) {
    CS_RETURN_IF(NULL == umap || NULL == key || NULL == value, CS_NULL);
    unordered_map_entry entry;
    unordered_map_entry_init(*umap, key, value, &entry);
    return hash_table_add_entry(umap->ht, &entry);
}

cs_codes unordered_map_remove_entry(unordered_map *umap, const void *key) {
    CS_RETURN_IF(NULL == umap || NULL == key, CS_NULL);

    unordered_map_entry entry;
    unordered_map_entry_init(*umap, key, NULL, &entry);
    int rc = hash_table_remove_entry(umap->ht, &entry);
    unordered_map_entry_free(&entry);
    return rc;
}

void *unordered_map_get_entry(unordered_map umap, const void *key) {
    CS_RETURN_IF(NULL == key, NULL);

    unordered_map_entry entry;
    unordered_map_entry_init(umap, key, NULL, &entry);
    void *found_entry = hash_table_get_entry(*umap.ht, &entry);
    unordered_map_entry_free(&entry);
    return ((unordered_map_entry *)found_entry)->data->second;
}

int unordered_map_empty(unordered_map umap) {
    return hash_table_empty(*umap.ht);
}

int unordered_map_size(unordered_map umap) {
    return hash_table_size(*umap.ht);
}

int unordered_map_count(unordered_map umap, const void *key) {
    CS_RETURN_IF(NULL == key, 0);

    unordered_map_entry entry;
    unordered_map_entry_init(umap, key, NULL, &entry);

    int count = hash_table_count(*umap.ht, &entry);
    unordered_map_entry_free(&entry);
    return count;
}

void unordered_map_swap(unordered_map *umap1, unordered_map *umap2) {
    CS_RETURN_IF(NULL == umap1 || NULL == umap2);
    hash_table_swap(umap1->ht, umap2->ht);

    unordered_map_attr_t temp_key_attr = umap1->key_attr;
    unordered_map_attr_t temp_value_attr = umap1->value_attr;
    hash_func_t temp_hash_func = umap1->hash_func;

    umap1->key_attr = umap2->key_attr;
    umap1->value_attr = umap2->value_attr;
    umap1->hash_func = umap2->hash_func;

    umap2->key_attr = temp_key_attr;
    umap2->value_attr = temp_value_attr;
    umap2->hash_func = temp_hash_func;
}

void unordered_map_clear(unordered_map *umap) {
    CS_RETURN_IF(NULL == umap);
    hash_table_clear(umap->ht);
    umap->ht = NULL;
    umap->key_attr = (unordered_map_attr_t){0};
    umap->value_attr = (unordered_map_attr_t){0};
    umap->hash_func = NULL;
}

void unordered_map_print(FILE *stream, void *v_umap) {
    CS_RETURN_IF(NULL == stream || NULL == v_umap);
    unordered_map *umap = (unordered_map *)v_umap;
    hash_table_print(stream, umap->ht);
}

void unordered_map_free(void *v_umap) {
    CS_RETURN_IF(NULL == v_umap);
    unordered_map *umap = (unordered_map *)v_umap;
    hash_table_free(umap->ht);
    free(umap);
}