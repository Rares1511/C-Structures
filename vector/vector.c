#include "../include/vector.h"

#include <stdlib.h>
#include <string.h>

cs_codes vector_init(vector *vec, vector_attr_t attr)
{
    if (attr.size < 0 || attr.size > SIZE_TH)
        return CS_SIZE;
    vec->cap = INIT_CAPACITY;
    vec->attr = attr;
    vec->size = 0;
    vec->vec = malloc(vec->attr.size * vec->cap);
    if (!vec->cap)
        return CS_MEM;
    return CS_SUCCESS;
}

cs_codes vector_insert_at(vector *vec, void *el, int pos)
{
    if (pos > vec->size)
        return CS_POS;
    if (vec->size == vec->cap)
    {
        vec->vec = realloc(vec->vec, (vec->cap + INIT_CAPACITY) * vec->attr.size);
        if (!vec->vec)
            return CS_MEM;
        vec->cap += INIT_CAPACITY;
    }
    if (pos != vec->size)
        memcpy(vec->vec + vec->attr.size * (pos + 1), vec->vec + vec->attr.size * pos, (vec->size - pos) * vec->attr.size);
    if (vec->attr.cp)
        vec->attr.cp(vec->vec + vec->attr.size * pos, el);
    else
        memcpy(vec->vec + vec->attr.size * pos, el, vec->attr.size);
    vec->size++;
    return CS_SUCCESS;
}

cs_codes vector_push_back(vector *vec, void *el)
{
    return vector_insert_at(vec, el, vec->size);
}

cs_codes vector_erase(vector *vec, int pos)
{
    if (vec->size == 0)
        return CS_EMPTY;
    if (pos >= vec->size)
        return CS_POS;
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + vec->attr.size * pos);
    if (pos != vec->size - 1)
        memcpy(vec->vec + vec->attr.size * pos, vec->vec + vec->attr.size * (pos + 1), (vec->size - pos - 1) * vec->attr.size);
    vec->size--;
    return CS_SUCCESS;
}

void *vector_at(vector vec, int pos)
{
    if (pos >= vec.size || vec.size == 0)
        return NULL;
    return vec.vec + vec.attr.size * pos;
}

cs_codes vector_replace(vector *vec, void *el, int pos)
{
    if (vec->size == 0)
        return CS_EMPTY;
    if (pos >= vec->size)
        return CS_POS;
    if (vec->attr.fr)
        vec->attr.fr(vec->vec + vec->attr.size * pos);
    if (vec->attr.cp)
        vec->attr.cp(vec->vec + vec->attr.size * pos, el);
    else
        memcpy(vec->vec + vec->attr.size * pos, el, vec->attr.size);
    return CS_SUCCESS;
}

int vector_find(vector vec, void *el)
{
    if (!vec.attr.comp)
        return CS_COMP;
    for (int i = 0; i < vec.size; i++)
    {
        if (vec.attr.comp(vec.vec + i * vec.attr.size, el) == 0)
            return i;
    }
    return CS_ELEM;
}

void vector_set_attr(vector *vec, vector_attr_t attr) { vec->attr = attr; }

void vector_set_free(vector *vec, freer fr) { vec->attr.fr = fr; }

void vector_set_print(vector *vec, printer print) { vec->attr.print = print; }

void vector_set_copy(vector *vec, deepcopy cp) { vec->attr.cp = cp; }

void vector_set_comp(vector *vec, comparer comp) { vec->attr.comp = comp; }

void vector_clear(vector *vec)
{
    if (vec->attr.fr)
        for (int i = 0; i < vec->size; i++)
            vec->attr.fr(vec->vec + vec->attr.size * i);
    vec->size = 0;
}

void vector_swap(vector *v1, vector *v2)
{
    void *aux = v1->vec;
    vector_attr_t attr = v1->attr;
    int size = v1->size;
    int cap = v1->cap;

    v1->attr = v2->attr;
    v1->size = v2->size;
    v1->cap = v2->cap;
    v1->vec = v2->vec;

    v2->attr = attr;
    v2->cap = cap;
    v2->size = size;
    v2->vec = aux;
}

void vector_sort(vector *vec)
{
    if (!vec->attr.comp)
        return;
    qsort(vec->vec, vec->size, vec->attr.size, vec->attr.comp);
}

void vector_free(void *v_vec)
{
    vector *vec = (vector *)v_vec;
    if (vec->attr.fr)
        for (int i = 0; i < vec->size; i++)
            vec->attr.fr(vec->vec + i * vec->attr.size);
    free(vec->vec);
}

void vector_print(void *v_vec)
{
    vector *vec = (vector *)v_vec;
    if (vec->attr.print == NULL)
        return;
    for (int i = 0; i < vec->size; i++)
        vec->attr.print(vec->vec + i * vec->attr.size);
}