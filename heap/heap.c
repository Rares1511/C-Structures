#include <cs/heap.h>

#include <stdlib.h>
#include <string.h>

#include "../include/unittest.h"

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * Compares two elements inside the heap
 * @param[in] h     Heap whose compare function will be used
 * @param[in] el1  First element to be compared
 * @param[in] el2  Second element to be compared
 * @return A negative integer if el1 < el2, 0 if el1 == el2, or a positive integer if el1 > el2
 */
int heap_compare(heap h, void *el1, void *el2) {
    if (h.attr.comp)
        return h.attr.comp(el1, el2);
    return universal_compare(el1, el2, h.attr.size);
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes heap_init(heap *h, heap_attr_t attr) {
    if (attr.size < 0 || attr.size > SIZE_TH)
        return CS_SIZE;
    h->attr = attr;
    h->cap = INIT_CAPACITY;
    h->size = 0;
    h->vec = malloc(h->attr.size * h->cap);
    if (!h->vec)
        return CS_MEM;
    return CS_SUCCESS;
}

cs_codes heap_push(heap *h, void *el) {
    if (h->size == h->cap) {
        h->vec = realloc(h->vec, (h->cap + INIT_CAPACITY) * h->attr.size);
        if (!h->vec)
            return CS_MEM;
        h->cap += INIT_CAPACITY;
    }
    if (h->attr.copy)
        h->attr.copy(h->vec + h->size * h->attr.size, el);
    else
        memcpy(h->vec + h->size * h->attr.size, el, h->attr.size);
    h->size++;
    int pos = h->size;
    while (pos > 1 && heap_compare(*h, h->vec + h->attr.size * (pos / 2 - 1),
                                   h->vec + h->attr.size * (pos - 1)) < 0) {
        universal_swap(h->vec + h->attr.size * (pos / 2 - 1), h->vec + h->attr.size * (pos - 1),
                       h->attr.size);
        pos /= 2;
    }
    return CS_SUCCESS;
}

cs_codes heap_pop(heap *h) {
    if (h->size == 0)
        return CS_EMPTY;
    h->size--;
    if (h->size != 0)
        universal_swap(h->vec, h->vec + h->size * h->attr.size, h->attr.size);
    if (h->attr.fr)
        h->attr.fr(h->vec + h->attr.size * h->size);
    if (h->size == 0)
        return CS_SUCCESS;
    int pos = 1;
    while (2 * pos < h->size + 1) {
        int next_pos = 2 * pos;
        void *comp_ptr = comp_ptr = h->vec + h->attr.size * (2 * pos - 1);
        ;
        if (2 * pos < h->size && heap_compare(*h, h->vec + h->attr.size * (2 * pos - 1),
                                              h->vec + h->attr.size * (2 * pos)) < 0) {
            comp_ptr = h->vec + h->attr.size * (2 * pos);
            next_pos++;
        }
        if (heap_compare(*h, h->vec + h->attr.size * (pos - 1), comp_ptr) < 0) {
            universal_swap(h->vec + h->attr.size * (pos - 1), comp_ptr, h->attr.size);
            pos = next_pos;
        } else
            break;
    }
    return CS_SUCCESS;
}

void* heap_top(heap h) {
    if (h.size == 0)
        return NULL;
    return h.vec;
}

void heap_set_attr(heap *h, heap_attr_t attr) {
    h->attr = attr;
    heap_set_comp(h, attr.comp);
}

void heap_set_comp(heap *h, comparer comp) {
    void *temp = malloc(h->attr.size);
    if (!temp)
        return;
    h->attr.comp = comp;
    for (int i = h->size - 1; i >= 1; i--) {
        int pos = i;
        while (pos != 0 && heap_compare(*h, h->vec + h->attr.size * (pos / 2), h->vec + h->attr.size * pos) < 0) {
            memcpy(temp, h->vec + h->attr.size * (pos / 2), h->attr.size);
            memcpy(h->vec + h->attr.size * (pos / 2), h->vec + h->attr.size * pos, h->attr.size);
            memcpy(h->vec + h->attr.size * pos, temp, h->attr.size);
            pos /= 2;
        }
    }
    free(temp);
}

void heap_swap(heap *h1, heap *h2) {
    heap_attr_t attr = h1->attr;
    int cap = h1->cap;
    int size = h1->size;
    void *vec = h1->vec;

    h1->attr = h2->attr;
    h1->cap = h2->cap;
    h1->size = h2->size;
    h1->vec = h2->vec;

    h2->attr = attr;
    h2->cap = cap;
    h2->size = size;
    h2->vec = vec;
}

void heap_clear(heap *h) {
    if (h->attr.fr)
        for (int i = 0; i < h->size; i++)
            h->attr.fr(h->vec + i * h->attr.size);
    h->size = 0;
}

void heap_free(void *v_h) {
    heap *h = (heap *)v_h;
    if (h->attr.fr)
        for (int i = 0; i < h->size; i++)
            h->attr.fr(h->vec + i * h->attr.size);
    free(h->vec);
    h->vec = NULL;
}

void heap_print(void *v_h) {
    heap h = *(heap *)v_h;
    if (!h.attr.print)
        return;
    for (int i = 0; i < h.size; i++) {
        h.attr.print(h.attr.stream, h.vec + h.attr.size * i);
        fprintf(h.attr.stream, " ");
    }
}