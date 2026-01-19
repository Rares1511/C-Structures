#include <cs/unordered_map.h>
#include <cs/hash_table.h>
#include <cs/pair.h>

#include <stdlib.h>
#include <string.h>

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        START OF HELPER STRUCT SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


typedef struct unordered_map_entry {
    pair data;
    hash_func_t hash_func;
} unordered_map_entry;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                         END OF HELPER STRUCT SECTION                                       ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


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
}

int unordered_map_entry_comp(const void *a, const void *b) {
    const pair* pa = (const pair*)a;
    const pair* pb = (const pair*)b;
    
    if (pa->second_attr->comp != NULL) {
        return pa->second_attr->comp(pa->second, pb->second);
    }
    return memcmp(pa->second, pb->second, pa->second_attr->size);
}

size_t unordered_map_entry_hash(const void *el) {
    // Simple hash function for demonstration purposes
    unordered_map_entry *entry = (unordered_map_entry *)el;
    if (entry == NULL) {
        return 0;
    }
    if (entry->hash_func == NULL) {
        return universal_hash_bytes(entry->data.first, entry->data.first_attr->size);
    }
    return entry->hash_func(entry->data.first);
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes unordered_map_init(unordered_map *umap,
                                 unordered_map_attr_t key_attr,
                                 unordered_map_attr_t value_attr,
                                 hash_func_t hash_func,
                                 int initial_capacity) {
    if (!umap || initial_capacity <= 0) {
        return CS_SIZE;
    }

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

    umap->ht = malloc(sizeof(hash_table));
    return hash_table_init(umap->ht, entry_attr, unordered_map_entry_hash, initial_capacity);
}

void unordered_map_free(void *v_umap) {
    if (!v_umap) {
        return;
    }

    unordered_map *umap = (unordered_map *)v_umap;
    hash_table_free(umap->ht);
    free(umap->ht);
    umap->ht = NULL;
}