#include <cs/hash_table.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


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
#pragma endregion

cs_codes hash_table_init(hash_table *ht, hash_table_attr_t attr, hash_func_t hash, int initial_capacity) {
    CS_RETURN_IF(NULL == ht, CS_NULL);
    CS_RETURN_IF(initial_capacity <= 0 || attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);

    ht->cap = initial_capacity;
    ht->meta = (metadata_t *)malloc(sizeof(metadata_t));
    CS_RETURN_IF(NULL == ht->meta, CS_NULL);
    metadata_init(ht->meta);
    ht->attr = attr;
    ht->hash = hash;
    ht->buckets = malloc(sizeof(vector**) * initial_capacity);
    if (ht->buckets == NULL) {
        free(ht->meta);
        return CS_NULL;
    }
    for (int i = 0; i < initial_capacity; i++) {
        ht->buckets[i] = NULL;
    }

    return CS_SUCCESS;
}

cs_codes hash_table_add_entry(hash_table *ht, const void *el) {
    CS_RETURN_IF(ht == NULL || el == NULL, CS_NULL);
    int idx = __hash_table_get_bucket_index(*ht, el), rc;
    CS_RETURN_IF(idx < 0 || idx >= ht->cap, CS_SIZE);

    if (ht->buckets[idx] == NULL) {
        ht->buckets[idx] = malloc(sizeof(vector));
        CS_RETURN_IF(ht->buckets[idx] == NULL, CS_MEM);
        rc = vector_init(ht->buckets[idx], ht->attr);
        CS_RETURN_IF(rc != CS_SUCCESS, rc);
    }

    rc = vector_push_back(ht->buckets[idx], el);
    CS_RETURN_IF(rc != CS_SUCCESS, rc);
    metadata_size_inc(ht->meta, 1);
    return CS_SUCCESS;
}

cs_codes hash_table_remove_entry(hash_table *ht, const void *el) {
    CS_RETURN_IF(ht == NULL || el == NULL, CS_NULL);
    int idx = __hash_table_get_bucket_index(*ht, el);
    CS_RETURN_IF(idx < 0 || idx >= ht->cap, CS_SIZE);
    CS_RETURN_IF(ht->buckets[idx] == NULL, CS_ELEM);

    vector *bucket = ht->buckets[idx];
    int bucket_idx = vector_find(*bucket, el);
    CS_RETURN_IF(bucket_idx == -1, CS_ELEM);
    int rc = vector_erase(bucket, bucket_idx);
    CS_RETURN_IF(rc != CS_SUCCESS, rc);
    metadata_size_inc(ht->meta, -1);
    return CS_SUCCESS;
}

void* hash_table_get_entry(hash_table ht, const void *el) {
    CS_RETURN_IF(el == NULL, NULL);
    int idx = __hash_table_get_bucket_index(ht, el);
    CS_RETURN_IF(idx < 0 || idx >= ht.cap, NULL);
    CS_RETURN_IF(ht.buckets[idx] == NULL, NULL);

    vector *bucket = ht.buckets[idx];
    int bucket_idx = vector_find(*bucket, el);
    CS_RETURN_IF(bucket_idx == -1, NULL);
    return vector_at(*bucket, bucket_idx);
}

int hash_table_count(hash_table ht, const void *el) {
    CS_RETURN_IF(el == NULL, 0);
    int idx = __hash_table_get_bucket_index(ht, el);
    CS_RETURN_IF(idx < 0 || idx >= ht.cap, 0);
    CS_RETURN_IF(ht.buckets[idx] == NULL, 0);

    vector *bucket = ht.buckets[idx];
    return vector_count(*bucket, el);
}

void hash_table_swap(hash_table *ht1, hash_table *ht2) {
    CS_RETURN_IF(ht1 == NULL || ht2 == NULL);

    hash_table_attr_t attr = ht1->attr;
    int cap = ht1->cap;
    metadata_t *meta = ht1->meta;
    vector **buckets = ht1->buckets;
    hash_func_t hash = ht1->hash;

    ht1->attr = ht2->attr;
    ht1->cap = ht2->cap;
    ht1->meta = ht2->meta;
    ht1->buckets = ht2->buckets;
    ht1->hash = ht2->hash;

    ht2->attr = attr;
    ht2->cap = cap;
    ht2->meta = meta;
    ht2->buckets = buckets;
    ht2->hash = hash;
}

void hash_table_clear(hash_table *ht) {
    CS_RETURN_IF(ht == NULL);

    for (int i = 0; i < ht->cap; i++) {
        if (ht->buckets[i] != NULL) {
            vector_free(ht->buckets[i]);
            free(ht->buckets[i]);
            ht->buckets[i] = NULL;
        }
    }
    metadata_size_inc(ht->meta, -hash_table_size(*ht));
}

void hash_table_print(FILE *stream, void *v_ht) {
    CS_RETURN_IF(stream == NULL || v_ht == NULL);
    hash_table *ht = (hash_table*)v_ht;

    for (int i = 0; i < ht->cap; i++) {
        if (ht->buckets[i] != NULL) {
            fprintf(stream, "Bucket %d: ", i);
            vector_print(stream, ht->buckets[i]);
        }
    }
}

void hash_table_free(void *v_ht) {
    CS_RETURN_IF(v_ht == NULL);
    hash_table *ht = (hash_table*)v_ht;
    for (int i = 0; i < ht->cap; i++) {
        if (ht->buckets[i] != NULL) {
            vector_free(ht->buckets[i]);
            free(ht->buckets[i]);
            ht->buckets[i] = NULL;
        }
    }
    free(ht->buckets);
    free(ht->meta);
}