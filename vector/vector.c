#include <cs/vector.h>
#include <stdlib.h>
#include <string.h>

#define VEC_GROW(vec) \
    do { \
        if ((vec)->size == (vec)->cap) { \
            int __new_cap = (vec)->cap * 2; \
            void *__new_vec = realloc((vec)->vec, __new_cap * (vec)->attr.size); \
            if (NULL == __new_vec) return CS_MEM; \
            (vec)->vec = __new_vec; \
            (vec)->cap = __new_cap; \
        } \
    } while (0)

#define VEC_SHRINK(vec) \
    do { \
        if ((vec)->v_attr.shrink_factor > 1 && (vec)->size < (vec)->cap / (vec)->v_attr.shrink_factor && (vec)->cap > (vec)->v_attr.min_cap) { \
            int new_cap = (vec)->cap / 2; \
            if (new_cap < (vec)->v_attr.min_cap) new_cap = (vec)->v_attr.min_cap; \
            void *__new_vec = realloc((vec)->vec, new_cap * (vec)->attr.size); \
            if (NULL == __new_vec) return CS_MEM; \
            (vec)->vec = __new_vec; \
            (vec)->cap = new_cap; \
        } \
    } while (0)

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * Partitions the array for quicksort
 * @param[in] base  Base pointer of the array
 * @param[in] low   Low index
 * @param[in] high  High index
 * @param[in] size  Size of each element
 * @return The partition index
 */
int vector_partition(void *base, int low, int high, int size) {
    void* pivot = base + high * size;
    void* temp = malloc(size);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (memcmp(base + j * size, pivot, size) < 0) {
            i++;
            memcpy(temp, base + i * size, size);
            memcpy(base + i * size, base + j * size, size);
            memcpy(base + j * size, temp, size);
        }
    }

    memcpy(temp, base + (i + 1) * size, size);
    memcpy(base + (i + 1) * size, base + high * size, size);
    memcpy(base + high * size, temp, size);
    free(temp);
    return i + 1;
}

/*!
 * Quicksorts the array in the case of no compare function being provided
 * @param[in] base  Base pointer of the array
 * @param[in] low   Low index
 * @param[in] high  High index
 * @param[in] size  Size of each element
 */
void vector_qsort(void *base, int low, int high, int size) {
    if (low < high) {
        int pivot_index = vector_partition(base, low, high, size);
        vector_qsort(base, low, pivot_index - 1, size);
        vector_qsort(base, pivot_index + 1, high, size);
    }
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes vector_init(vector *v, elem_attr_t attr, vector_attr_t v_attr) {
    CS_RETURN_IF(NULL == v, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(v_attr.min_cap < 0 || v_attr.min_cap > VECTOR_INIT_CAPACITY, CS_SIZE);
    CS_RETURN_IF(v_attr.shrink_factor < 0 || v_attr.shrink_factor > VECTOR_INIT_CAPACITY, CS_SIZE);

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

    return CS_SUCCESS;
}

cs_codes vector_insert_at(vector *vec, const void *el, int pos) {
    CS_RETURN_IF(vec == NULL || el == NULL, CS_NULL);
    int size = vec->size;
    CS_RETURN_IF(pos > size || pos < 0, CS_POS);
    VEC_GROW(vec);
    int elem_size = vec->attr.size;
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

cs_codes vector_push_back(vector *vec, const void *el) {
    CS_RETURN_IF(vec == NULL || el == NULL, CS_NULL);
    if (__builtin_expect(vec->size == vec->cap, 0)) {
        VEC_GROW(vec);
    }
    int elem_size = vec->attr.size;
    char *dest = (char *)vec->vec + elem_size * vec->size;
    if (__builtin_expect(vec->attr.copy != NULL, 0))
        vec->attr.copy(dest, el);
    else
        memcpy(dest, el, elem_size);
    vec->size++;
    return CS_SUCCESS;
}

cs_codes vector_erase(vector *vec, int pos) {
    CS_RETURN_IF(vec == NULL, CS_NULL);
    int size = vec->size;
    CS_RETURN_IF(size == 0, CS_EMPTY);
    CS_RETURN_IF(pos >= size || pos < 0, CS_POS);
    int elem_size = vec->attr.size;
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + elem_size * pos);
    if (pos != size - 1)
        memmove(vec->vec + elem_size * pos, vec->vec + elem_size * (pos + 1),
               (size - pos - 1) * elem_size);
    vec->size--;
    VEC_SHRINK(vec);
    return CS_SUCCESS;
}

cs_codes vector_pop_back(vector *vec) {
    CS_RETURN_IF(vec == NULL, CS_NULL);
    CS_RETURN_IF(vec->size == 0, CS_EMPTY);
    if (__builtin_expect(vec->attr.fr != NULL, 0))
        vec->attr.fr((char *)vec->vec + vec->attr.size * (vec->size - 1));
    vec->size--;
    VEC_SHRINK(vec);
    return CS_SUCCESS;
}

void *vector_at(vector vec, int pos) {
    CS_RETURN_IF(pos >= vector_size(vec) || pos < 0 || vector_empty(vec), NULL);
    return vec.vec + vec.attr.size * pos;
}

int vector_count(vector vec, const void *el) {
    CS_RETURN_IF(el == NULL, CS_NULL);
    int count = 0, size = vec.size;
    int elem_size = vec.attr.size;
    comparer comp = vec.attr.comp;
    void *base = vec.vec;
    if (comp) {
        for (int i = 0; i < size; i++) {
            if (comp(base + i * elem_size, el) == 0)
                count++;
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (memcmp(base + i * elem_size, el, elem_size) == 0)
                count++;
        }
    }
    return count;
}

cs_codes vector_replace(vector *vec, const void *el, int pos) {
    CS_RETURN_IF(el == NULL || vec == NULL, CS_NULL);
    CS_RETURN_IF(vector_empty(*vec), CS_EMPTY);
    int size = vector_size(*vec);
    CS_RETURN_IF(pos >= size || pos < 0, CS_POS);
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + vec->attr.size * pos);
    if (vec->attr.copy)
        vec->attr.copy(vec->vec + vec->attr.size * pos, el);
    else
        memcpy(vec->vec + vec->attr.size * pos, el, vec->attr.size);
    return CS_SUCCESS;
}

cs_codes vector_reserve(vector *vec, int new_cap) {
    CS_RETURN_IF(vec == NULL, CS_NULL);
    CS_RETURN_IF(new_cap <= 0, CS_SIZE);
    if (new_cap < vec->cap) {
        return CS_SUCCESS; // No need to shrink, use vector_shrink if you want to shrink based on size
    }

    int old_cap = vec->cap;
    vec->cap = new_cap;
    void *new_vec = realloc(vec->vec, vec->cap * vec->attr.size);
    if (new_vec == NULL) {
        vec->cap = old_cap; // Revert to old capacity on failure
        return CS_MEM;
    }
    vec->vec = new_vec;
    return CS_SUCCESS;
}

int vector_find(vector vec, const void *el) {
    CS_RETURN_IF(el == NULL, CS_NULL);
    int size = vec.size;
    int elem_size = vec.attr.size;
    comparer comp = vec.attr.comp;
    void *base = vec.vec;
    if (comp) {
        for (int i = 0; i < size; i++) {
            if (comp(base + i * elem_size, el) == 0)
                return i;
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (memcmp(base + i * elem_size, el, elem_size) == 0)
                return i;
        }
    }
    return CS_ELEM;
}

void vector_swap(vector *v1, vector *v2) {
    CS_RETURN_IF(v1 == NULL || v2 == NULL);

    void *aux = v1->vec;
    elem_attr_t attr = v1->attr;
    vector_attr_t v_attr = v1->v_attr;
    int size = v1->size;
    int cap = v1->cap;

    v1->attr = v2->attr;
    v1->v_attr = v2->v_attr;
    v1->size = v2->size;
    v1->cap = v2->cap;
    v1->vec = v2->vec;

    v2->attr = attr;
    v2->v_attr = v_attr;
    v2->size = size;
    v2->cap = cap;
    v2->vec = aux;
}

void vector_sort(vector *vec) {
    CS_RETURN_IF(vec == NULL);
    int size = vector_size(*vec);
    if (vec->attr.comp == NULL) {
        vector_qsort(vec->vec, 0, size - 1, vec->attr.size);
    }
    else {
        qsort(vec->vec, size, vec->attr.size, vec->attr.comp);
    }
}

void vector_clear(vector *vec) {
    CS_RETURN_IF(vec == NULL);
    int size = vector_size(*vec);
    if (vec->attr.fr) {
        for (int i = 0; i < size; i++)
            vec->attr.fr(vec->vec + i * vec->attr.size);
    }
    vec->size = 0;
}

void vector_print(FILE *stream, const void *v_vec) {
    CS_RETURN_IF(stream == NULL || v_vec == NULL);
    const vector *vec = (const vector *)v_vec;
    CS_RETURN_IF(vec->attr.print == NULL);
    int size = vector_size(*vec);
    for (int i = 0; i < size; i++) {
        vec->attr.print(stream, vec->vec + i * vec->attr.size);
    }
}

void vector_free(void *v_vec) {
    CS_RETURN_IF(v_vec == NULL);
    vector *vec = (vector *)v_vec;
    int size = vector_size(*vec);
    if (vec->attr.fr) {
        for (int i = 0; i < size; i++) {
            vec->attr.fr(vec->vec + i * vec->attr.size);
        }
    }
    vec->size = 0;
    free(vec->vec);
}