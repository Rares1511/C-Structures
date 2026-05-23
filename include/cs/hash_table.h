#ifndef __CS_HASH_TABLE_H__
#define __CS_HASH_TABLE_H__

#include <cs/universal.h>

// Status flags for occupied array
#define __HASH_TABLE_EMPTY 0
#define __HASH_TABLE_OCCUPIED 1
#define __HASH_TABLE_TOMBSTONE 2

#define __HASH_TABLE_TOMBSTONE_VALUE (size_t)-1

// Default values for hash table
#define CS_HASH_TABLE_INIT_CAP 16
#define CS_HASH_TABLE_MAX_LOAD_FACTOR 0.75

// Magic number for hash table validation
#define CS_HASH_TABLE_MAGIC 0xDEADBEEF

typedef size_t (*__hash_func_t)(const void *key);

typedef struct {
    cs_header_t header;
    size_t cap;   // Change from int to size_t
    size_t size;  // Change from int to size_t
    size_t mask;  // Change from int to size_t
    elem_attr_t attr;
    __hash_func_t hash;
    void *keys;             // Contiguous array of keys
    char *occupied;         // Bitset or char array to track slot status
} __hash_table;

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline int __hash_table_comp(const void *a, const void *b, comparer comp, size_t size) {
    if (__builtin_expect(comp != NULL, 1)) {
        return comp(a, b);
    }
    return memcmp(a, b, size);
}

static inline size_t __hash_table_find_index(__hash_table *ht, const void *el) {
    size_t h = ht->hash ? ht->hash(el) : universal_hash_bytes(el, ht->attr.size);
    size_t idx = h & ht->mask;

    while (ht->occupied[idx] != __HASH_TABLE_EMPTY) {
        if (ht->occupied[idx] == __HASH_TABLE_OCCUPIED) {
            void *current_el = (char *)ht->keys + (idx * ht->attr.size);
            if (__hash_table_comp(current_el, el, ht->attr.comp, ht->attr.size) == 0) {
                return idx; // Element found
            }
        }
        idx = (idx + 1) & ht->mask; // Linear probing
    }
    return __HASH_TABLE_TOMBSTONE_VALUE; // Not found
}

static inline void __hash_table_remove_at_index(__hash_table *ht, size_t idx) {
    if (ht->attr.fr) {
        void *el = (char *)ht->keys + (idx * ht->attr.size);
        ht->attr.fr(el);
    }
    ht->occupied[idx] = __HASH_TABLE_TOMBSTONE;
    ht->size--;
}

static inline cs_codes __hash_table_rehash(__hash_table *ht) {
    size_t old_cap = ht->cap;
    void *old_keys = ht->keys;
    char *old_occupied = ht->occupied;

    size_t new_cap = old_cap * 2;
    size_t new_mask = new_cap - 1;

    void *new_keys = malloc(new_cap * ht->attr.size);
    char *new_occupied = calloc(new_cap, sizeof(char));
    memset(new_occupied, 0, new_cap * sizeof(char)); // Ensure all slots start as EMPTY
    
    CS_RETURN_IF(new_keys == NULL || new_occupied == NULL, CS_MEM);

    for (size_t i = 0; i < old_cap; i++) {
        if (old_occupied[i] == __HASH_TABLE_OCCUPIED) { // Only move VALID entries
            void *old_el = (char *)old_keys + (i * ht->attr.size);
            
            // 1. Recalculate hash using the NEW mask
            size_t h = ht->hash ? ht->hash(old_el) : universal_hash_bytes(old_el, ht->attr.size);
            size_t idx = h & new_mask;

            // 2. Linear probe in the NEW occupied array
            while (new_occupied[idx]) {
                idx = (idx + 1) & new_mask;
            }

            // 3. Raw memcpy the whole entry block
            memcpy((char *)new_keys + (idx * ht->attr.size), old_el, ht->attr.size);
            new_occupied[idx] = __HASH_TABLE_OCCUPIED;
        }
    }

    // 4. CRITICAL: Swap pointers ONLY after the loop is done
    ht->cap = new_cap;
    ht->mask = new_mask;
    ht->keys = new_keys;
    ht->occupied = new_occupied;

    free(old_keys);
    free(old_occupied);
    return CS_SUCCESS;
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

static inline __hash_table* __hash_table_init(elem_attr_t attr, __hash_func_t hash) {
    __hash_table *ht = malloc(sizeof(__hash_table));
    CS_RETURN_IF(NULL == ht, NULL);
    CS_RETURN_IF(attr.size == 0 || attr.size > SIZE_TH, NULL);

    ht->cap = CS_HASH_TABLE_INIT_CAP;
    ht->mask = ht->cap - 1;
    ht->size = 0;
    ht->attr = attr;
    ht->hash = hash;

    ht->occupied = calloc(ht->cap, sizeof(char));
    if (ht->occupied == NULL) {
        free(ht);
        return NULL;
    }
    
    ht->keys = malloc(ht->cap * ht->attr.size);
    if (ht->keys == NULL) {
        free(ht->occupied);
        free(ht);
        return NULL;
    }

    ht->header.magic = CS_HASH_TABLE_MAGIC;
    ht->header.type = CS_HASH_TABLE_TYPE;

    return ht;
}

static inline void *__hash_table_add_entry(__hash_table *ht, const void *el, cs_codes *rc) {
    CS_RETURN_IF(ht == NULL || el == NULL, NULL);

    *rc = CS_SUCCESS;
    
    if (ht->size * 4 >= ht->cap * 3) {
        *rc = __hash_table_rehash(ht);
        if (*rc != CS_SUCCESS) return NULL;
    }

    size_t h = ht->hash ? ht->hash(el) : universal_hash_bytes(el, ht->attr.size);
    size_t idx = (size_t)(h & ht->mask);
    size_t first_tombstone = __HASH_TABLE_TOMBSTONE_VALUE;

    while (ht->occupied[idx] != __HASH_TABLE_EMPTY) {
        if (ht->occupied[idx] == __HASH_TABLE_OCCUPIED) {
            void *current_el = (char *)ht->keys + (idx * ht->attr.size);
            if (__hash_table_comp(current_el, el, ht->attr.comp, ht->attr.size) == 0) {
                *rc = CS_ELEM;
                return current_el; // Element already exists, return pointer to it
            }
        }
        if (ht->occupied[idx] == __HASH_TABLE_TOMBSTONE && first_tombstone == __HASH_TABLE_TOMBSTONE_VALUE) {
            first_tombstone = idx;
        }
        idx = (idx + 1) & ht->mask;
    }

    size_t target_idx = (first_tombstone != __HASH_TABLE_TOMBSTONE_VALUE) ? first_tombstone : idx;
    void *destination = (char *)ht->keys + (target_idx * ht->attr.size);

    if (ht->attr.copy) {
        ht->attr.copy(destination, el);
    } else {
        memcpy(destination, el, ht->attr.size);
    }

    ht->occupied[target_idx] = __HASH_TABLE_OCCUPIED;
    ht->size++;
    return destination;
}

static inline cs_codes __hash_table_remove_entry(__hash_table *ht, const void *el) {
    CS_RETURN_IF(ht == NULL || el == NULL, CS_NULL);
    CS_RETURN_IF(ht->header.magic != CS_HASH_TABLE_MAGIC, CS_UNINITIALIZED);

    size_t idx = __hash_table_find_index(ht, el);
    if (idx == __HASH_TABLE_TOMBSTONE_VALUE) {
        return CS_ELEM; 
    }

    __hash_table_remove_at_index(ht, idx);
    return CS_SUCCESS;
}

static inline void* __hash_table_get_entry(__hash_table *ht, const void *el) {
    CS_RETURN_IF(ht == NULL || el == NULL || ht->header.magic != CS_HASH_TABLE_MAGIC, NULL);

    size_t h = ht->hash ? ht->hash(el) : universal_hash_bytes(el, ht->attr.size);
    size_t idx = h & ht->mask;
    size_t start_idx = idx;

    while (ht->occupied[idx] != __HASH_TABLE_EMPTY) {
        if (ht->occupied[idx] == __HASH_TABLE_OCCUPIED) {
            void *current_el = (char *)ht->keys + (idx * ht->attr.size);
            
            if (__hash_table_comp(current_el, el, ht->attr.comp, ht->attr.size) == 0) {
                return current_el;
            }
        }
        idx = (idx + 1) & ht->mask;
        if (idx == start_idx) break;
    }
    return NULL;
}

static inline size_t __hash_table_count(__hash_table *ht, const void *el) {
    CS_RETURN_IF(ht == NULL || el == NULL || ht->header.magic != CS_HASH_TABLE_MAGIC, 0);

    size_t h = ht->hash ? ht->hash(el) : universal_hash_bytes(el, ht->attr.size);
    size_t idx = (h & ht->mask);
    size_t start_idx = idx;
    size_t count = 0;

    while (ht->occupied[idx] != __HASH_TABLE_EMPTY) {
        if (ht->occupied[idx] == __HASH_TABLE_OCCUPIED) {
            void *current_el = (char *)ht->keys + (idx * ht->attr.size);
            if (__hash_table_comp(current_el, el, ht->attr.comp, ht->attr.size) == 0) {
                count++;
            }
        }

        idx = (idx + 1) & ht->mask;

        if (idx == start_idx) break;
    }

    return count;
}

static inline int __hash_table_empty(__hash_table *ht) { return ht->size == 0; };

static inline size_t __hash_table_size(__hash_table *ht) { return ht->size; };

static inline void __hash_table_clear(__hash_table *ht) {
    CS_RETURN_IF(ht == NULL || ht->header.magic != CS_HASH_TABLE_MAGIC);

    if (ht->attr.fr) {
        for (size_t i = 0; i < ht->cap; i++) {
            if (ht->occupied[i] == __HASH_TABLE_OCCUPIED) {
                ht->attr.fr((char *)ht->keys + (i * ht->attr.size));
            }
        }
    }

    memset(ht->occupied, 0, ht->cap * sizeof(char));
    ht->size = 0;
}

static inline void __hash_table_print(FILE *stream, void *v_ht) {
    CS_RETURN_IF(stream == NULL || v_ht == NULL);
    __hash_table *ht = (__hash_table*)v_ht;
    CS_RETURN_IF(ht->header.magic != CS_HASH_TABLE_MAGIC || ht->attr.print == NULL);

    fprintf(stream, "--- Hash Table (Size: %ld, Cap: %ld) ---\n", ht->size, ht->cap);

    for (size_t i = 0; i < ht->cap; i++) {
        fprintf(stream, "[%04ld]: ", i);

        if (ht->occupied[i] == __HASH_TABLE_OCCUPIED) {
            unsigned char *ptr = (unsigned char *)ht->keys + (i * ht->attr.size);
            fprintf(stream, "OCCUPIED | Data: ");
            ht->attr.print(stream, ptr);
        } 
        else if (ht->occupied[i] == __HASH_TABLE_TOMBSTONE) {
            fprintf(stream, "TOMBSTONE (Deleted)");
        } 
        else {
            fprintf(stream, "EMPTY");
        }
        
        fprintf(stream, "\n");
    }
    fprintf(stream, "-------------------------------------\n");
}

static inline void __hash_table_free(void *v_ht) {
    __hash_table *ht = (__hash_table*)v_ht;
    if (!ht || ht->header.magic != CS_HASH_TABLE_MAGIC) return;

    __hash_table_clear(ht);
    
    free(ht->keys);
    free(ht->occupied);
    ht->header.magic = 0;
    free(ht);
}

#endif