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

cs_codes __hash_table_rehash(hash_table *ht) {
    int old_cap = ht->cap;
    vector **old_buckets = ht->buckets;
    char *old_is_oversized = ht->is_oversized;

    ht->cap *= 2;
    ht->buckets = realloc(ht->buckets, sizeof(vector**) * ht->cap);
    ht->is_oversized = realloc(ht->is_oversized, sizeof(char) * ht->cap);
    if (ht->buckets == NULL || ht->is_oversized == NULL) {
        ht->buckets = old_buckets;
        ht->is_oversized = old_is_oversized;
        ht->cap = old_cap;
        return CS_MEM;
    }

    for (int i = old_cap; i < ht->cap; i++) {
        ht->buckets[i] = NULL;
        ht->is_oversized[i] = 0;
    }

    ht->oversized_buckets = 0;
    for (int i = 0; i < old_cap; i++) {
        ht->is_oversized[i] = 0; // Reset oversize status, will be updated as we move elements
        vector *bucket = ht->buckets[i];
        vector *new_bucket = ht->buckets[i + old_cap];
        if (bucket == NULL) {
            continue;
        }

        int bucket_size = vector_size(*bucket);
        int current_size = 0;
        int corresponding_size = 0;
        while (current_size + corresponding_size < bucket_size) {
            void *el = vector_at(*bucket, current_size);
            size_t new_idx = __hash_table_get_bucket_index(*ht, el);
            if (new_idx == i) {
                // Current element stays in the same bucket
                current_size++;
            }
            else {
                // Current element moves to the new bucket
                // As to not move every element to the left by eliminating the current element
                // we will swap the current element with the last element in the vector and then 
                // insert it in the new bucket
                if (corresponding_size == 0) {
                    // First time handling this bucket, initialize the new bucket
                    new_bucket = malloc(sizeof(vector));
                    vector_attr_t v_attr = { .min_cap = 1, .shrink_factor = 0 };
                    cs_codes rc = vector_init(new_bucket, ht->attr, v_attr);
                    if (rc != CS_SUCCESS) {
                        return rc;
                    }
                    rc = vector_reserve(new_bucket, bucket_size); // Reserve space for all elements that might be moved to the new bucket
                    if (rc != CS_SUCCESS) {
                        return rc;
                    }
                } 
                void *last_el = vector_at(*bucket, vector_size(*bucket) - 1);
                
                memcpy(new_bucket->vec + corresponding_size * ht->attr.size, el, ht->attr.size);
                memcpy(bucket->vec + current_size * ht->attr.size, last_el, ht->attr.size);
                bucket->size--;

                corresponding_size++;
            }
        }

        bucket->size = current_size;
        if (new_bucket) {
            new_bucket->size = corresponding_size;
            ht->buckets[i + old_cap] = new_bucket;
        }
        if (current_size > ht->oversize_threshold) {
            ht->is_oversized[i] = 1;
            ht->oversized_buckets++;
        }
        
        if (corresponding_size > ht->oversize_threshold) {
            ht->is_oversized[i + old_cap] = 1;
            ht->oversized_buckets++;
        }
    }
    
    return CS_SUCCESS;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes hash_table_init(hash_table *ht, elem_attr_t attr, hash_func_t hash) {
    CS_RETURN_IF(NULL == ht, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);

    ht->cap = 16; // Set a default initial capacity
    ht->oversized_buckets = 0;
    ht->oversize_threshold = 8; // Set a default oversize threshold
    ht->size = 0;
    ht->attr = attr;
    ht->hash = hash;
    ht->buckets = malloc(sizeof(vector**) * ht->cap);
    ht->is_oversized = malloc(sizeof(char) * ht->cap);
    CS_RETURN_IF(NULL == ht->buckets, CS_MEM);
    CS_RETURN_IF(NULL == ht->is_oversized, CS_MEM);

    for (int i = 0; i < ht->cap; i++) {
        ht->buckets[i] = NULL;
        ht->is_oversized[i] = 0;
    }

    return CS_SUCCESS;
}

cs_codes hash_table_add_entry(hash_table *ht, const void *el) {
    CS_RETURN_IF(ht == NULL || el == NULL, CS_NULL);
    int idx = __hash_table_get_bucket_index(*ht, el), rc;
    CS_RETURN_IF(idx < 0 || idx >= ht->cap, CS_SIZE);

    if (ht->buckets[idx] == NULL) {
        ht->buckets[idx] = malloc(sizeof(vector));
        vector_attr_t v_attr = { .min_cap = 1, .shrink_factor = 0 };
        CS_RETURN_IF(ht->buckets[idx] == NULL, CS_MEM);
        rc = vector_init(ht->buckets[idx], ht->attr, v_attr);
        CS_RETURN_IF(rc != CS_SUCCESS, rc);
    }

    rc = vector_push_back(ht->buckets[idx], el);
    CS_RETURN_IF(rc != CS_SUCCESS, rc);
    if (vector_size(*ht->buckets[idx]) > ht->oversize_threshold && !ht->is_oversized[idx]) {
        ht->oversized_buckets++;
        ht->is_oversized[idx] = 1;
    }
    ht->size++;

    if (ht->oversized_buckets > ht->cap / 2) {
        return __hash_table_rehash(ht);
    }

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
    ht->size--;

    if (vector_size(*bucket) <= ht->oversize_threshold && ht->is_oversized[idx]) {
        ht->oversized_buckets--;
        ht->is_oversized[idx] = 0;
    }

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

    elem_attr_t attr = ht1->attr;
    int cap = ht1->cap;
    int size = ht1->size;
    int oversized_buckets = ht1->oversized_buckets;
    int oversize_threshold = ht1->oversize_threshold;
    vector **buckets = ht1->buckets;
    char *is_oversized = ht1->is_oversized;
    hash_func_t hash = ht1->hash;

    ht1->attr = ht2->attr;
    ht1->cap = ht2->cap;
    ht1->size = ht2->size;
    ht1->oversized_buckets = ht2->oversized_buckets;
    ht1->oversize_threshold = ht2->oversize_threshold;
    ht1->buckets = ht2->buckets;
    ht1->is_oversized = ht2->is_oversized;
    ht1->hash = ht2->hash;

    ht2->attr = attr;
    ht2->cap = cap;
    ht2->size = size;
    ht2->oversized_buckets = oversized_buckets;
    ht2->oversize_threshold = oversize_threshold;
    ht2->buckets = buckets;
    ht2->is_oversized = is_oversized;
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
        ht->is_oversized[i] = 0;
    }
    ht->oversized_buckets = 0;
    ht->size = 0;
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
    free(ht->is_oversized);
    free(ht->buckets);
}