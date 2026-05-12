#include <cs/vector.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

static void vector_radix_sort_generic(vector *restrict vec) {
    size_t n = vec->size;
    size_t sz = vec->attr.size;
    unsigned char *restrict src = (unsigned char *)vec->vec;
    
    // Auxiliary buffer for stability
    unsigned char *restrict dst = malloc(n * sz);
    if (!dst) return; 

    // We sort byte-by-byte, from LSB (index 0) to MSB (index sz-1)
    // Note: This assumes Little Endian for primitives like int/float
    for (size_t byte_idx = 0; byte_idx < sz; byte_idx++) {
        size_t count[256] = {0};

        // 1. Count frequencies of the byte at current offset
        for (size_t i = 0; i < n; i++) {
            unsigned char val = src[i * sz + byte_idx];
            count[val]++;
        }

        // 2. Transform counts to indices (Prefix Sum)
        size_t total = 0;
        for (int j = 0; j < 256; j++) {
            size_t old_count = count[j];
            count[j] = total;
            total += old_count;
        }

        // 3. Move elements to dst buffer based on the byte value
        for (size_t i = 0; i < n; i++) {
            unsigned char val = src[i * sz + byte_idx];
            size_t target_pos = count[val];
            
            // Copy the whole element
            memcpy(dst + (target_pos * sz), src + (i * sz), sz);
            count[val]++;
        }

        // 4. Swap buffers: src now points to the partially sorted data
        // We can't swap the vec->vec pointer directly, so we swap our locals
        unsigned char *tmp = src;
        src = dst;
        dst = tmp;
    }

    // If our final sorted data is in the allocated buffer, copy it back to vec->vec
    if (src != (unsigned char *)vec->vec) {
        memcpy(vec->vec, src, n * sz);
        // src is now the 'dst' malloc'd buffer
        free(src); 
    } else {
        // src is vec->vec, so dst is the malloc'd buffer
        free(dst);
    }
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

/*!
 * Returns the reference at the position given
 * @param[in] vec  Vector used to be given the reference
 * @param[in] pos  Position for the reference
 * @return The reference at the position given or NULL if the position is invalid
 */
inline cs_codes _vector_grow_internal(vector *restrict vec) {
    size_t new_cap = vec->cap * 2;
    void *new_vec = realloc(vec->vec, new_cap * vec->attr.size);
    if (new_vec == NULL) {
        return CS_MEM;
    }
    vec->vec = new_vec;
    vec->cap = new_cap;
    return CS_SUCCESS;
}

/*!
 * Returns the reference at the position given
 * @param[in] vec  Vector used to be given the reference
 * @param[in] pos  Position for the reference
 * @return The reference at the position given or NULL if the position is invalid
 */
inline cs_codes _vector_shrink_internal(vector *restrict vec) {
    size_t new_cap = vec->cap / 2;
    if (new_cap < vec->v_attr.min_cap) {
        new_cap = vec->v_attr.min_cap;
    }
    void *new_vec = realloc(vec->vec, new_cap * vec->attr.size);
    if (new_vec == NULL) {
        return CS_MEM;
    }
    vec->vec = new_vec;
    vec->cap = new_cap;
    return CS_SUCCESS;
}

cs_codes vector_init(vector *restrict v, elem_attr_t attr, vector_attr_t v_attr) {
    CS_RETURN_IF(NULL == v, CS_NULL);
    CS_RETURN_IF(attr.size == 0 || attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(v_attr.min_cap > VECTOR_INIT_CAPACITY, CS_SIZE);
    CS_RETURN_IF(v_attr.shrink_factor > VECTOR_INIT_CAPACITY, CS_SIZE);

    if (v_attr.min_cap == 0) {
        v_attr.min_cap = VECTOR_INIT_CAPACITY;
    }
    if (v_attr.shrink_factor == 0) {
        v_attr.shrink_factor = VECTOR_SHRINK_FACTOR;
    }

    v->attr = attr;
    v->v_attr = v_attr;
    v->cap = v_attr.min_cap;
    v->size = 0;

    v->vec = malloc(v->cap * attr.size);
    CS_RETURN_IF(v->vec == NULL, CS_MEM);

    v->header.magic = CS_VECTOR_MAGIC;
    v->header.type = CS_VECTOR_TYPE;
    return CS_SUCCESS;
}

cs_codes vector_insert_at(vector *restrict vec, const void *restrict el, size_t pos) {
    CS_RETURN_IF(vec == NULL || el == NULL || vec->header.magic != CS_VECTOR_MAGIC, CS_NULL);
    size_t size = vec->size;
    CS_RETURN_IF(pos > size, CS_POS);
    if (__builtin_expect(vec->size == vec->cap, 0)) {
        cs_codes res = _vector_grow_internal(vec);
        if (res != CS_SUCCESS) return res;
    }
    size_t elem_size = vec->attr.size;
    if (pos != size) {
        memmove(vec->vec + (pos + 1) * elem_size, vec->vec + pos * elem_size, 
                            (size - pos) * elem_size);     
    }
    if (vec->attr.copy)
        vec->attr.copy(vec->vec + elem_size * pos, el);
    else
        memcpy(vec->vec + elem_size * pos, el, elem_size);
    vec->size++;
    return CS_SUCCESS;
}

cs_codes vector_erase(vector *restrict vec, size_t pos) {
    CS_RETURN_IF(vec == NULL || vec->header.magic != CS_VECTOR_MAGIC, CS_NULL);
    size_t size = vec->size;
    CS_RETURN_IF(size == 0, CS_EMPTY);
    CS_RETURN_IF(pos >= size, CS_POS);
    size_t elem_size = vec->attr.size;
    freer free_func = vec->attr.fr;
    if (free_func)
        free_func(vec->vec + elem_size * pos);
    if (pos != size - 1)
        memmove(vec->vec + elem_size * pos, vec->vec + elem_size * (pos + 1),
               (size - pos - 1) * elem_size);
    vec->size--;
    if (__builtin_expect(vec->v_attr.shrink_factor > 1 && vec->size < vec->cap / (vec->v_attr.shrink_factor * 2) && vec->cap > vec->v_attr.min_cap, 0)) {
        _vector_shrink_internal(vec);
    }
    return CS_SUCCESS;
}

size_t vector_count(vector *restrict vec, const void *restrict el) {
    CS_RETURN_IF(el == NULL, CS_NULL);
    CS_RETURN_IF(vec == NULL || vec->header.magic != CS_VECTOR_MAGIC, CS_UNINITIALIZED);
    size_t count = 0, size = vec->size;
    size_t elem_size = vec->attr.size;
    comparer comp = vec->attr.comp;
    void *base = vec->vec;
    if (comp) {
        for (size_t i = 0; i < size; i++) {
            if (comp(base + i * elem_size, el) == 0)
                count++;
        }
    } else {
        for (size_t i = 0; i < size; i++) {
            if (memcmp(base + i * elem_size, el, elem_size) == 0)
                count++;
        }
    }
    return count;
}

cs_codes vector_replace(vector *restrict vec, const void *restrict el, size_t pos) {
    CS_RETURN_IF(el == NULL || vec == NULL || vec->header.magic != CS_VECTOR_MAGIC, CS_NULL);
    CS_RETURN_IF(vector_empty(vec), CS_EMPTY);
    size_t size = vector_size(vec);
    CS_RETURN_IF(pos >= size, CS_POS);
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + vec->attr.size * pos);
    if (vec->attr.copy)
        vec->attr.copy(vec->vec + vec->attr.size * pos, el);
    else
        memcpy(vec->vec + vec->attr.size * pos, el, vec->attr.size);
    return CS_SUCCESS;
}

cs_codes vector_reserve(vector *restrict vec, size_t new_cap) {
    CS_RETURN_IF(vec == NULL, CS_NULL);
    CS_RETURN_IF(new_cap <= 0, CS_SIZE);
    if (new_cap < vec->cap) {
        return CS_SUCCESS; // No need to shrink, use vector_shrink if you want to shrink based on size
    }

    size_t old_cap = vec->cap;
    vec->cap = new_cap;
    void *new_vec = realloc(vec->vec, vec->cap * vec->attr.size);
    if (new_vec == NULL) {
        vec->cap = old_cap; // Revert to old capacity on failure
        return CS_MEM;
    }
    vec->vec = new_vec;
    return CS_SUCCESS;
}

cs_codes vector_shrink_to_fit(vector *restrict vec) {
    CS_RETURN_IF(vec == NULL, CS_NULL);
    CS_RETURN_IF(vec->header.magic != CS_VECTOR_MAGIC, CS_UNINITIALIZED);
    size_t size = vec->size;
    if (size >= vec->cap) {
        return CS_SUCCESS; // No need to shrink
    }

    size_t old_cap = vec->cap;
    vec->cap = size > 0 ? size : 1; // Avoid shrinking to zero capacity
    void *new_vec = realloc(vec->vec, vec->cap * vec->attr.size);
    if (new_vec == NULL) {
        vec->cap = old_cap; // Revert to old capacity on failure
        return CS_MEM;
    }
    vec->vec = new_vec;
    return CS_SUCCESS;
}

void vector_swap(vector *restrict v1, vector *restrict v2) {
    CS_RETURN_IF(v1 == NULL || v2 == NULL || v1->header.magic != CS_VECTOR_MAGIC || v2->header.magic != CS_VECTOR_MAGIC);

    cs_header_t temp_header = v1->header;
    void *aux = v1->vec;
    elem_attr_t attr = v1->attr;
    vector_attr_t v_attr = v1->v_attr;
    int size = v1->size;
    int cap = v1->cap;

    v1->header = v2->header;
    v1->attr = v2->attr;
    v1->v_attr = v2->v_attr;
    v1->size = v2->size;
    v1->cap = v2->cap;
    v1->vec = v2->vec;

    v2->header = temp_header;
    v2->attr = attr;
    v2->v_attr = v_attr;
    v2->size = size;
    v2->cap = cap;
    v2->vec = aux;
}

void vector_sort(vector *restrict vec) {
    CS_RETURN_IF(vec == NULL || vec->header.magic != CS_VECTOR_MAGIC || vec->size <= 1);
    size_t n = vec->size;

    if (vec->attr.comp != NULL) {
        qsort(vec->vec, n, vec->attr.size, vec->attr.comp);
    } else {
        vector_radix_sort_generic(vec);
    }
}

void vector_clear(vector *restrict vec) {
    CS_RETURN_IF(vec == NULL || vec->header.magic != CS_VECTOR_MAGIC);
    size_t size = vector_size(vec);
    freer free_func = vec->attr.fr;
    if (free_func) {
        for (size_t i = 0; i < size; i++) {
            free_func(vec->vec + i * vec->attr.size);
        }
    }
    vec->size = 0;
}

void vector_print(FILE *restrict stream, const void *restrict v_vec) {
    CS_RETURN_IF(stream == NULL || v_vec == NULL);
    vector *vec = (vector *)v_vec;
    CS_RETURN_IF(vec->header.magic != CS_VECTOR_MAGIC);
    CS_RETURN_IF(vec->attr.print == NULL);
    size_t size = vector_size(vec);
    for (size_t i = 0; i < size; i++) {
        vec->attr.print(stream, vec->vec + i * vec->attr.size);
    }
}

void vector_free(void *restrict v_vec) {
    CS_RETURN_IF(v_vec == NULL);
    vector *vec = (vector *)v_vec;
    CS_RETURN_IF(vec->header.magic != CS_VECTOR_MAGIC);
    size_t size = vector_size(vec);
    freer free_func = vec->attr.fr;
    if (free_func) {
        for (size_t i = 0; i < size; i++) {
            free_func(vec->vec + i * vec->attr.size);
        }
    }
    vec->size = 0;
    vec->header.magic = 0; // Invalidate the vector
    free(vec->vec);
}