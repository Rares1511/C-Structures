#include <cs/unordered_map.h>

#pragma region Helper Structs
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        START OF HELPER STRUCT SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


typedef struct unordered_map_entry {
    __hash_func_t hash_func;
    char data[]; /* Flexible array member to hold the pair */
} unordered_map_entry;

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                         END OF HELPER STRUCT SECTION                                       ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static inline void __unordered_map_entry_copy(void *dest, const void *src) {
    unordered_map_entry *d = (unordered_map_entry *)dest;
    const unordered_map_entry *s = (const unordered_map_entry *)src;
    
    d->hash_func = s->hash_func;
    
    pair *ps = (pair *)s->data;
    pair *pd = (pair *)d->data;

    // 1. Copy pair metadata
    memcpy(pd, ps, sizeof(pair));

    // 2. Use the ACTUAL helper functions to get pointers
    // These helpers likely do: (char*)p + sizeof(pair)
    void *src_key = pair_first(ps);
    void *dest_key = pair_first(pd);
    void *src_val = pair_second(ps);
    void *dest_val = pair_second(pd);

    // 3. Perform Deep Copies
    if (ps->first_attr->copy) 
        ps->first_attr->copy(dest_key, src_key);
    else 
        memcpy(dest_key, src_key, ps->first_attr->size);

    if (ps->second_attr->copy) 
        ps->second_attr->copy(dest_val, src_val);
    else 
        memcpy(dest_val, src_val, ps->second_attr->size);
}

static inline void __unordered_map_entry_print(FILE *stream, const void *el) {
    CS_RETURN_IF(NULL == el || stream == NULL);
    const unordered_map_entry *entry = (const unordered_map_entry *)el;
    pair_print(stream, (pair *)entry->data);
}

static inline void __unordered_map_entry_free(void *el) {
    CS_RETURN_IF(NULL == el);
    unordered_map_entry *entry = (unordered_map_entry *)el;
    pair_free((pair *)entry->data);
}

static inline int __unordered_map_entry_comp(const void *a, const void *b) {
    unordered_map_entry* ea = (unordered_map_entry*)a;
    unordered_map_entry* eb = (unordered_map_entry*)b;
    pair* pa = (pair*)ea->data;
    pair* pb = (pair*)eb->data;
    
    if (pa->first_attr->comp != NULL) {
        return pa->first_attr->comp(pair_first(pa), pair_first(pb));
    }
    return memcmp(pair_first(pa), pair_first(pb), pa->first_attr->size);
}

static inline size_t __unordered_map_entry_hash(const void *el) {
    CS_RETURN_IF(NULL == el, 0);
    unordered_map_entry *entry = (unordered_map_entry *)el;
    pair *p = (pair *)entry->data;
    void *key = pair_first(p);
    if (entry->hash_func == NULL) {
        return universal_hash_bytes(key, p->first_attr->size);
    }
    return entry->hash_func(key);
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes unordered_map_init(unordered_map *umap,
                                 elem_attr_t key_attr,
                                 elem_attr_t value_attr,
                                 __hash_func_t hash_func) {
    CS_RETURN_IF(NULL == umap, CS_NULL);
    CS_RETURN_IF(key_attr.size == 0 || value_attr.size == 0, CS_SIZE);
    CS_RETURN_IF(key_attr.size > SIZE_TH || value_attr.size > SIZE_TH, CS_SIZE);
   
    umap->ht = malloc(sizeof(__hash_table));
    umap->key_attr = malloc(sizeof(elem_attr_t));
    umap->value_attr = malloc(sizeof(elem_attr_t));
    CS_RETURN_IF(NULL == umap->ht || NULL == umap->key_attr || NULL == umap->value_attr, CS_MEM);
    memcpy(umap->key_attr, &key_attr, sizeof(elem_attr_t));
    memcpy(umap->value_attr, &value_attr, sizeof(elem_attr_t));
    umap->hash_func = hash_func;

    elem_attr_t entry_attr = {
        .comp = __unordered_map_entry_comp,
        .copy = __unordered_map_entry_copy,
        .fr = __unordered_map_entry_free,
        .print = __unordered_map_entry_print,
        .size = sizeof(unordered_map_entry) + sizeof(pair) + key_attr.size + value_attr.size,
    };

    return __hash_table_init(umap->ht, entry_attr, __unordered_map_entry_hash);
}

cs_codes unordered_map_add_entry(unordered_map *umap, const void *key, const void *value) {
    CS_RETURN_IF(NULL == umap || NULL == key || NULL == value, CS_NULL);
    int k_sz = umap->key_attr->size;
    int v_sz = umap->value_attr->size;
    char buffer[sizeof(unordered_map_entry) + sizeof(pair) + k_sz + v_sz];
    unordered_map_entry *entry = (unordered_map_entry *)buffer;
    pair *p = (pair *)entry->data;

    entry->hash_func = umap->hash_func;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    p->has_first = 1;
    p->has_second = 1;
    p->first_attr = umap->key_attr;
    p->second_attr = umap->value_attr;
    memcpy(pair_first(p), key, k_sz);
    memcpy(pair_second(p), value, v_sz);

    return __hash_table_add_entry(umap->ht, entry);
}

cs_codes unordered_map_remove_entry(unordered_map *umap, const void *key) {
    CS_RETURN_IF(NULL == umap || NULL == key, CS_NULL);
    int k_sz = umap->key_attr->size;
    char buffer[sizeof(unordered_map_entry) + sizeof(pair) + k_sz];
    unordered_map_entry *entry = (unordered_map_entry *)buffer;
    pair *p = (pair *)entry->data;

    entry->hash_func = umap->hash_func;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    p->has_first = 1;
    p->has_second = 0;
    p->first_attr = umap->key_attr;
    p->second_attr = umap->value_attr;
    memcpy(pair_first(p), key, k_sz);
    
    return __hash_table_remove_entry(umap->ht, entry);
}

void *unordered_map_get_entry(unordered_map *umap, const void *key) {
    CS_RETURN_IF(NULL == key, NULL);
    int k_sz = umap->key_attr->size;
    char buffer[sizeof(unordered_map_entry) + sizeof(pair) + k_sz];
    unordered_map_entry *entry = (unordered_map_entry *)buffer;
    pair *p = (pair *)entry->data;

    entry->hash_func = umap->hash_func;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    p->has_first = 1;
    p->has_second = 0;
    p->first_attr = umap->key_attr;
    p->second_attr = umap->value_attr;
    memcpy(pair_first(p), key, k_sz);
    
    void *found_entry = __hash_table_get_entry(umap->ht, entry);
    CS_RETURN_IF(NULL == found_entry, NULL);
    return pair_second((pair *)(((unordered_map_entry *)found_entry)->data));
}

int unordered_map_empty(unordered_map *umap) {
    return __hash_table_empty(umap->ht);
}

int unordered_map_size(unordered_map *umap) {
    return __hash_table_size(umap->ht);
}

int unordered_map_count(unordered_map *umap, const void *key) {
    CS_RETURN_IF(NULL == key, 0);
    int k_sz = umap->key_attr->size;
    char buffer[sizeof(unordered_map_entry) + sizeof(pair) + k_sz];
    unordered_map_entry *entry = (unordered_map_entry *)buffer;
    pair *p = (pair *)entry->data;

    entry->hash_func = umap->hash_func;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    p->has_first = 1;
    p->has_second = 0;
    p->first_attr = umap->key_attr;
    p->second_attr = umap->value_attr;
    memcpy(pair_first(p), key, k_sz);

    return __hash_table_count(umap->ht, entry);
}

void unordered_map_swap(unordered_map *umap1, unordered_map *umap2) {
    CS_RETURN_IF(NULL == umap1 || NULL == umap2);
    __hash_table_swap(umap1->ht, umap2->ht);

    elem_attr_t *temp_key_attr = umap1->key_attr;
    elem_attr_t *temp_value_attr = umap1->value_attr;
    __hash_func_t temp_hash_func = umap1->hash_func;

    umap1->key_attr = umap2->key_attr;
    umap1->value_attr = umap2->value_attr;
    umap1->hash_func = umap2->hash_func;

    umap2->key_attr = temp_key_attr;
    umap2->value_attr = temp_value_attr;
    umap2->hash_func = temp_hash_func;
}

void unordered_map_clear(unordered_map *umap) {
    CS_RETURN_IF(NULL == umap);
    __hash_table_clear(umap->ht);
}

void unordered_map_print(FILE *stream, void *v_umap) {
    CS_RETURN_IF(NULL == stream || NULL == v_umap);
    unordered_map *umap = (unordered_map *)v_umap;
    __hash_table_print(stream, umap->ht);
}

void unordered_map_free(void *v_umap) {
    CS_RETURN_IF(NULL == v_umap);
    unordered_map *umap = (unordered_map *)v_umap;
    __hash_table_free(umap->ht);
    free(umap->ht);
    free(umap->key_attr);
    free(umap->value_attr);
}