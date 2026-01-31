#include <cs/vector.h>
#include <stdlib.h>
#include <string.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * Compares two elements using either the provided compare function or memcmp
 * @param[in] v1    First element to compare
 * @param[in] v2    Second element to compare
 * @param[in] comp  Compare function to use (can be NULL)
 * @param[in] size  Size of the elements
 * @return Result of the comparison
 */
int vector_compare(const void *v1, const void *v2, comparer comp, int size) {
    if (comp)
        return comp(v1, v2);
    return memcmp(v1, v2, size);
}

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

cs_codes vector_init(vector *v, vector_attr_t attr) {
    CS_RETURN_IF(NULL == v, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    v->vec = malloc(INIT_CAPACITY * attr.size);
    CS_RETURN_IF(v->vec == NULL, CS_MEM);
    v->attr = attr;
    v->cap = INIT_CAPACITY;
    v->meta = malloc(sizeof(metadata_t));
    CS_RETURN_IF(v->meta == NULL, CS_MEM);
    metadata_init(v->meta);
    return CS_SUCCESS;
}

cs_codes vector_insert_at(vector *vec, const void *el, int pos) {
    CS_RETURN_IF(vec == NULL || el == NULL, CS_NULL);
    int size = vector_size(*vec);
    CS_RETURN_IF(pos > size || pos < 0, CS_POS);
    if (size == vec->cap) {
        vec->vec = realloc(vec->vec, (vec->cap + INIT_CAPACITY) * vec->attr.size);
        CS_RETURN_IF(!vec->vec, CS_MEM);
        vec->cap += INIT_CAPACITY;
    }
    if (pos != size) {
        memmove(vec->vec + (pos + 1) * vec->attr.size, vec->vec + pos * vec->attr.size, 
                            (size - pos) * vec->attr.size);     
    }
    if (vec->attr.copy)
        vec->attr.copy(vec->vec + vec->attr.size * pos, el);
    else
        memcpy(vec->vec + vec->attr.size * pos, el, vec->attr.size);
    metadata_size_inc(vec->meta, 1);
    return CS_SUCCESS;
}

cs_codes vector_erase(vector *vec, int pos) {
    CS_RETURN_IF(vec == NULL, CS_NULL);
    int size = vector_size(*vec);
    CS_RETURN_IF(vector_empty(*vec), CS_EMPTY);
    CS_RETURN_IF(pos >= size || pos < 0, CS_POS);
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + vec->attr.size * pos);
    if (pos != size - 1)
        memcpy(vec->vec + vec->attr.size * pos, vec->vec + vec->attr.size * (pos + 1),
               (size - pos - 1) * vec->attr.size);
    metadata_size_inc(vec->meta, -1);
    return CS_SUCCESS;
}

void *vector_at(vector vec, int pos) {
    CS_RETURN_IF(pos >= vector_size(vec) || pos < 0 || vector_empty(vec), NULL);
    return vec.vec + vec.attr.size * pos;
}

int vector_count(vector vec, const void *el) {
    CS_RETURN_IF(el == NULL, CS_NULL);
    int count = 0, size = vector_size(vec);
    for (int i = 0; i < size; i++) {
        if (vector_compare(vec.vec + i * vec.attr.size, el, vec.attr.comp, vec.attr.size) == 0)
            count++;
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

int vector_find(vector vec, const void *el) {
    CS_RETURN_IF(el == NULL, CS_NULL);
    int size = vector_size(vec);
    for (int i = 0; i < size; i++) {
        if (vector_compare(vec.vec + i * vec.attr.size, el, vec.attr.comp, vec.attr.size) == 0)
            return i;
    }
    return CS_ELEM;
}

void vector_swap(vector *v1, vector *v2) {
    CS_RETURN_IF(v1 == NULL || v2 == NULL);

    void *aux = v1->vec;
    vector_attr_t attr = v1->attr;
    metadata_t *meta = v1->meta;
    int cap = v1->cap;

    v1->attr = v2->attr;
    v1->meta = v2->meta;
    v1->cap = v2->cap;
    v1->vec = v2->vec;

    v2->attr = attr;
    v2->cap = cap;
    v2->meta = meta;
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
    metadata_size_inc(vec->meta, -size);
}

void vector_print(FILE *stream, const void *v_vec) {
    CS_RETURN_IF(stream == NULL || v_vec == NULL);
    const vector *vec = (const vector *)v_vec;
    CS_RETURN_IF(vec->attr.print == NULL);
    int size = vector_size(*vec);
    for (int i = 0; i < size; i++)
        vec->attr.print(stream, vec->vec + i * vec->attr.size);
}

void vector_free(void *v_vec) {
    CS_RETURN_IF(v_vec == NULL);
    vector *vec = (vector *)v_vec;
    int size = vector_size(*vec);
    if (vec->attr.fr) {
        for (int i = 0; i < size; i++)
            vec->attr.fr(vec->vec + i * vec->attr.size);
    }
    metadata_size_inc(vec->meta, -size);
    free(vec->vec);
    free(vec->meta);
}