#include <cs/vector.h>
#include <stdlib.h>
#include <string.h>

#include "../include/unittest.h"

cs_codes vector_init(vector *vec, vector_attr_t attr) {
    if (vec == NULL)
        return CS_NULL;
    if (attr.size < 0 || attr.size > SIZE_TH)
        return CS_SIZE;
    vec->cap = INIT_CAPACITY;
    vec->attr = attr;
    metadata_init(&vec->meta, 1);
    vec->vec = malloc(vec->attr.size * vec->cap);
    if (!vec->vec)
        return CS_MEM;
    return CS_SUCCESS;
}

cs_codes vector_insert_at(vector *vec, const void *el, int pos) {
    if (vec == NULL)
        return CS_NULL;
    if (metadata_is_init(vec->meta) == 0)
        return CS_FUNC;
    int size = vector_size(*vec);
    if (pos > size || pos < 0)
        return CS_POS;
    if (size == vec->cap) {
        vec->vec = realloc(vec->vec, (vec->cap + INIT_CAPACITY) * vec->attr.size);
        if (!vec->vec)
            return CS_MEM;
        vec->cap += INIT_CAPACITY;
    }
    if (pos != size) {
        if (vec->attr.copy) {
            for (int i = size; i > pos; i--)
                vec->attr.copy(vec->vec + i * vec->attr.size, vec->vec + (i - 1) * vec->attr.size);
        } else {
            memmove(vec->vec + (pos + 1) * vec->attr.size, vec->vec + pos * vec->attr.size,
                    (size - pos) * vec->attr.size);
        }
    }
    if (vec->attr.copy)
        vec->attr.copy(vec->vec + vec->attr.size * pos, el);
    else
        memcpy(vec->vec + vec->attr.size * pos, el, vec->attr.size);
    metadata_size_inc(&vec->meta, 1);
    return CS_SUCCESS;
}

cs_codes vector_push_back(vector *vec, const void *el) { return vector_insert_at(vec, el, vector_size(*vec)); }

cs_codes vector_erase(vector *vec, int pos) {
    if (vec == NULL)
        return CS_NULL;
    if (!metadata_is_init(vec->meta))
        return CS_FUNC;
    int size = vector_size(*vec);
    if (vector_empty(*vec))
        return CS_EMPTY;
    if (pos >= size || pos < 0)
        return CS_POS;
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + vec->attr.size * pos);
    if (pos != size - 1)
        memcpy(vec->vec + vec->attr.size * pos, vec->vec + vec->attr.size * (pos + 1),
               (size - pos - 1) * vec->attr.size);
    metadata_size_inc(&vec->meta, -1);
    return CS_SUCCESS;
}

cs_codes vector_pop_back(vector *vec) { return vector_erase(vec, vector_size(*vec) - 1); }

void *vector_at(vector vec, int pos) {
    if (!metadata_is_init(vec.meta))
        return NULL;
    if (pos >= vector_size(vec) || vector_empty(vec))
        return NULL;
    return vec.vec + vec.attr.size * pos;
}

int vector_count(vector vec, const void *el) {
    if (!metadata_is_init(vec.meta))
        return CS_FUNC;
    int count = 0, size = vector_size(vec);
    for (int i = 0; i < size; i++) {
        if ((vec.attr.comp && vec.attr.comp(vec.vec + i * vec.attr.size, el) == 0) || memcmp(vec.vec + i * vec.attr.size, el, vec.attr.size) == 0)
            count++;
    }
    return count;
}

cs_codes vector_replace(vector *vec, const void *el, int pos) {
    if (!vec)
        return CS_NULL;
    if (!metadata_is_init(vec->meta))
        return CS_FUNC;
    if (vector_empty(*vec))
        return CS_EMPTY;
    int size = vector_size(*vec);
    if (pos >= size || pos < 0)
        return CS_POS;
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + vec->attr.size * pos);
    if (vec->attr.copy)
        vec->attr.copy(vec->vec + vec->attr.size * pos, el);
    else
        memcpy(vec->vec + vec->attr.size * pos, el, vec->attr.size);
    return CS_SUCCESS;
}

int vector_find(vector vec, const void *el) {
    if (!metadata_is_init(vec.meta))
        return CS_FUNC;
    int size = vector_size(vec);
    for (int i = 0; i < size; i++) {
        if ((vec.attr.comp && vec.attr.comp(vec.vec + i * vec.attr.size, el) == 0) || memcmp(vec.vec + i * vec.attr.size, el, vec.attr.size) == 0)
            return i;
    }
    return CS_ELEM;
}

void vector_clear(vector *vec) {
    if (vec == NULL)
        return;
    if (!metadata_is_init(vec->meta))
        return;
    if (vec->attr.fr) {
        int size = vector_size(*vec);
        for (int i = 0; i < size; i++)
            vec->attr.fr(vec->vec + vec->attr.size * i);
    }
    metadata_init(&vec->meta, 0);
}

void vector_swap(vector *v1, vector *v2) {
    if (v1 == NULL || v2 == NULL)
        return;
    if (!metadata_is_init(v1->meta) || !metadata_is_init(v2->meta))
        return;
    void *aux = v1->vec;
    vector_attr_t attr = v1->attr;
    metadata_t meta = v1->meta;
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
    if (vec == NULL)
        return;
    if (!metadata_is_init(vec->meta))
        return;
    if (!vec->attr.comp)
        return;
    qsort(vec->vec, vec->meta.size, vec->attr.size, vec->attr.comp);
}

void vector_print(FILE *stream, void *v_vec) {
    if (stream == NULL || v_vec == NULL)
        return;
    vector *vec = (vector *)v_vec;
    if (!metadata_is_init(vec->meta))
        return;
    if (vec->attr.print == NULL)
        return;
    int size = vector_size(*vec);
    for (int i = 0; i < size; i++)
        vec->attr.print(stream, vec->vec + i * vec->attr.size);
}

void vector_free(void *v_vec) {
    vector *vec = (vector *)v_vec;
    if (!metadata_is_init(vec->meta))
        return;
    if (vec->attr.fr) {
        int size = vector_size(*vec);
        for (int i = 0; i < size; i++)
            vec->attr.fr(vec->vec + i * vec->attr.size);
    }
    free(vec->vec);
    vec->vec = NULL;
}