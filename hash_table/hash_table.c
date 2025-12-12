#include <cs/hash_table.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * A simple universal hash function for byte arrays.
 * @param[in] data Pointer to the data to hash.
 * @param[in] size Size of the data in bytes.
 * @return A size_t hash value.
 */
size_t universal_hash_bytes(const void *data, size_t size) {
    const unsigned char *bytes = (const unsigned char *)data;
    size_t hash = 1469598103934665603ULL;  // FNV offset basis (64-bit)

    for (size_t i = 0; i < size; ++i) {
        hash ^= (size_t)bytes[i];
        hash *= 1099511628211ULL;          // FNV prime (64-bit)
    }

    return hash;
}

/*!
 * Computes the bucket index for a given element in the hash table.
 * @param[in] ht The hash table.
 * @param[in] el Pointer to the element.
 * @return The index of the bucket where the element should be located.
 */
size_t __hash_table_get_bucket_index(hash_table ht, const void *el) {
    int idx;
    if (ht.hash)
        idx = ht.hash(el) % ht.cap;
    else
        idx = (int)(universal_hash_bytes(el, ht.attr.size) % ht.cap);
    if (idx < 0) {
        idx += ht.cap;
    }
    return (size_t) idx;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes hash_table_init(hash_table *ht, hash_table_attr_t attr, hash_func_t hash, int initial_capacity) {
    if (ht == NULL) {
        return CS_ELEM;
    }
    if (initial_capacity <= 0 || attr.size <= 0 || attr.size > SIZE_TH) {
        return CS_SIZE;
    }

    ht->cap = initial_capacity;
    ht->size = 0;
    ht->attr = attr;
    ht->hash = hash;
    ht->buckets = malloc(sizeof(vector*) * initial_capacity);
    if (ht->buckets == NULL) {
        return CS_MEM;
    }
    for (int i = 0; i < initial_capacity; i++) {
        ht->buckets[i] = NULL;
    }

    return CS_SUCCESS;
}

cs_codes hash_table_add_entry(hash_table *ht, const void *el) {
    if (ht == NULL || el == NULL) {
        return CS_ELEM;
    }

    int idx = __hash_table_get_bucket_index(*ht, el);
    if (idx < 0 || idx >= ht->cap) {
        return CS_SIZE;
    }

    if (ht->buckets[idx] == NULL) {
        ht->buckets[idx] = malloc(sizeof(vector));
        if (NULL == ht->buckets[idx]) {
            return CS_MEM;
        }
        int rc = vector_init(ht->buckets[idx], ht->attr);
        if (CS_SUCCESS != rc) {
            return rc;
        }
    }

    return vector_push_back(ht->buckets[idx], el);
}

cs_codes hash_table_remove_entry(hash_table *ht, const void *el) {
    if (ht == NULL || el == NULL) {
        return CS_ELEM;
    }

    int idx = __hash_table_get_bucket_index(*ht, el);
    if (idx < 0 || idx >= ht->cap) {
        return CS_SIZE;
    }
    if (ht->buckets[idx] == NULL) {
        return CS_ELEM;
    }

    vector *bucket = ht->buckets[idx];
    int bucket_idx = vector_find(*bucket, el);
    if (bucket_idx == -1) {
        return CS_ELEM;
    }
    return vector_erase(bucket, bucket_idx);
}

void* hash_table_get_entry(hash_table ht, const void *el) {
    if (el == NULL) {
        return NULL;
    }

    int idx = __hash_table_get_bucket_index(ht, el);
    if (idx < 0 || idx >= ht.cap) {
        return NULL;
    }
    if (ht.buckets[idx] == NULL) {
        return NULL;
    }

    vector *bucket = ht.buckets[idx];
    int bucket_idx = vector_find(*bucket, el);
    if (bucket_idx == -1) {
        return NULL;
    }
    return vector_at(*bucket, bucket_idx);
}

int hash_table_count(hash_table ht, const void *el) {
    if (el == NULL) {
        return 0;
    }

    int idx = __hash_table_get_bucket_index(ht, el);
    if (idx < 0 || idx >= ht.cap) {
        return 0;
    }
    if (ht.buckets[idx] == NULL) {
        return 0;
    }

    vector *bucket = ht.buckets[idx];
    return vector_count(*bucket, el);
}

void hash_table_swap(hash_table *ht1, hash_table *ht2) {
    if (ht1 == NULL || ht2 == NULL) {
        return;
    }
    hash_table_attr_t attr = ht1->attr;
    int cap = ht1->cap;
    int size = ht1->size;
    vector **buckets = ht1->buckets;
    hash_func_t hash = ht1->hash;

    ht1->attr = ht2->attr;
    ht1->cap = ht2->cap;
    ht1->size = ht2->size;
    ht1->buckets = ht2->buckets;
    ht1->hash = ht2->hash;

    ht2->attr = attr;
    ht2->cap = cap;
    ht2->size = size;
    ht2->buckets = buckets;
    ht2->hash = hash;
}

void hash_table_clear(hash_table *ht) {
    if (ht == NULL) {
        return;
    }

    for (int i = 0; i < ht->cap; i++) {
        if (ht->buckets[i] != NULL) {
            vector_free(ht->buckets[i]);
            free(ht->buckets[i]);
            ht->buckets[i] = NULL;
        }
    }
    ht->size = 0;
}

void hash_table_print(FILE *stream, void *v_ht) {
    if (v_ht == NULL) {
        return;
    }
    hash_table *ht = (hash_table*)v_ht;

    for (int i = 0; i < ht->cap; i++) {
        if (ht->buckets[i] != NULL) {
            fprintf(stream, "Bucket %d: ", i);
            vector_print(stream, ht->buckets[i]);
        }
    }
}

void hash_table_free(void *v_ht) {
    if (v_ht == NULL) {
        return;
    }

    hash_table *ht = (hash_table*)v_ht;
    hash_table_clear(ht);
    free(ht->buckets);
    ht->buckets = NULL;
}