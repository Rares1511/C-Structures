#include <cs/deque.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

cs_codes deque_init(deque *dq, deque_attr_t attr) {
    CS_RETURN_IF(NULL == dq, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    dq->blocks = malloc(sizeof(deque_block_t) * DEQUE_INIT_BLOCKS);
    CS_RETURN_IF(dq->blocks == NULL, CS_MEM);

    dq->attr = attr;
    dq->meta = malloc(sizeof(metadata_t));
    CS_RETURN_IF(dq->meta == NULL, CS_MEM);
    metadata_init(dq->meta);
    dq->block_size = DEQUE_BLOCK_SIZE;
    dq->block_cap = DEQUE_INIT_BLOCKS;
    dq->front = dq->block_cap / 2;
    dq->back = dq->block_cap / 2;

    dq->blocks[dq->front].data = malloc(attr.size * dq->block_size);
    if (NULL == dq->blocks[dq->front].data) {
        free(dq->blocks);
        free(dq->meta);
        return CS_MEM;
    }
    dq->blocks[dq->front].front = dq->block_cap / 2;
    dq->blocks[dq->front].back = dq->block_cap / 2;
    return CS_SUCCESS;
}

cs_codes deque_push_back(deque *dq, const void* el) {
    CS_RETURN_IF(dq == NULL || el == NULL, CS_NULL);

    if (dq->blocks[dq->back].back >= dq->block_size) {
        dq->back++;
        if (dq->back >= dq->block_cap) {
            dq->block_cap += dq->block_cap / 2;
            dq->blocks = realloc(dq->blocks, sizeof(deque_block_t) * dq->block_cap);
            if (dq->blocks == NULL) {
                return CS_MEM;
            }
        }
        dq->blocks[dq->back].data = malloc(dq->attr.size * dq->block_size);
        if (dq->blocks[dq->back].data == NULL) {
            return CS_MEM;
        }
        dq->blocks[dq->back].front = 0;
        dq->blocks[dq->back].back = 0;
    }
    if (dq->attr.copy) {
        dq->attr.copy(dq->blocks[dq->back].data + (dq->blocks[dq->back].back * dq->attr.size), el);
    } else {
        memcpy(dq->blocks[dq->back].data + (dq->blocks[dq->back].back * dq->attr.size), el, dq->attr.size);
    }
    dq->blocks[dq->back].back++;
    metadata_size_inc(dq->meta, 1);
    return CS_SUCCESS;
}

cs_codes deque_push_front(deque *dq, const void* el) {
    CS_RETURN_IF(dq == NULL || el == NULL, CS_NULL);

    if (dq->blocks[dq->front].front <= 0) {
        dq->front--;
        if (dq->front < 0) {
            int old_cap = dq->block_cap;
            dq->block_cap += dq->block_cap / 2;
            dq->blocks = realloc(dq->blocks, sizeof(deque_block_t) * dq->block_cap);
            if (dq->blocks == NULL) {
                return CS_MEM;
            }
            memmove(&dq->blocks[dq->block_cap - old_cap], &dq->blocks[0], sizeof(deque_block_t) * old_cap);
            dq->front += dq->block_cap - old_cap;
            dq->back += dq->block_cap - old_cap;
        }
        dq->blocks[dq->front].data = malloc(dq->attr.size * dq->block_size);
        if (dq->blocks[dq->front].data == NULL) {
            return CS_MEM;
        }
        dq->blocks[dq->front].front = dq->block_size;
        dq->blocks[dq->front].back = dq->block_size;
    }
    dq->blocks[dq->front].front--;
    if (dq->attr.copy) {
        dq->attr.copy(dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size), el);
    } else {
        memcpy(dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size), el, dq->attr.size);
    }
    metadata_size_inc(dq->meta, 1);
    return CS_SUCCESS;
}

cs_codes deque_insert_at(deque *dq, const void *el, int index) {
    CS_RETURN_IF(dq == NULL || el == NULL, CS_NULL);
    int size = deque_size(*dq);
    CS_RETURN_IF(index < 0 || index > size, CS_POS);

    if (index == 0) {
        return deque_push_front(dq, el);
    } else if (index == size) {
        return deque_push_back(dq, el);
    }

    if (index < size / 2) {
        // Shift elements towards front - make space at front first
        void *first_el = dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size);
        deque_push_front(dq, first_el);
        // Free the duplicate at position 1 (was position 0 before push)
        if (dq->attr.fr) {
            dq->attr.fr(deque_at(*dq, 1));
        }
        for (int i = 1; i < index; i++) {
            void *src = deque_at(*dq, i + 1);
            void *dest = deque_at(*dq, i);
            memcpy(dest, src, dq->attr.size);
        }
    } else {
        // Shift elements towards back - make space at back first
        void *last_el = dq->blocks[dq->back].data + ((dq->blocks[dq->back].back - 1) * dq->attr.size);
        deque_push_back(dq, last_el);
        // Free the duplicate at position size-1 (was last before push, now second-to-last)
        if (dq->attr.fr) {
            dq->attr.fr(deque_at(*dq, size));
        }
        for (int i = size; i > index; i--) {
            void *src = deque_at(*dq, i - 1);
            void *dest = deque_at(*dq, i);
            memcpy(dest, src, dq->attr.size);
        }
    }

    void *target = deque_at(*dq, index);
    if (dq->attr.copy) {
        dq->attr.copy(target, el);
    } else {
        memcpy(target, el, dq->attr.size);
    }

    return CS_SUCCESS;
}

cs_codes deque_pop_back(deque *dq) {
    CS_RETURN_IF(dq == NULL, CS_NULL);
    CS_RETURN_IF(deque_empty(*dq), CS_EMPTY);

    dq->blocks[dq->back].back--;
    if (dq->attr.fr) {
        dq->attr.fr(dq->blocks[dq->back].data + (dq->blocks[dq->back].back * dq->attr.size));
    }
    metadata_size_inc(dq->meta, -1);

    if (dq->blocks[dq->back].back <= 0) {
        dq->back--;
        if (dq->back < dq->front) {
            dq->back = dq->front;
            dq->blocks[dq->back].front = dq->block_cap / 2;
            dq->blocks[dq->back].back = dq->block_cap / 2;
        } else {
            free(dq->blocks[dq->back + 1].data);
        }
    }
    return CS_SUCCESS;
}

cs_codes deque_pop_front(deque *dq) {
    CS_RETURN_IF(dq == NULL, CS_NULL);
    CS_RETURN_IF(deque_empty(*dq), CS_EMPTY);

    if (dq->attr.fr) {
        dq->attr.fr(dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size));
    }
    dq->blocks[dq->front].front++;
    metadata_size_inc(dq->meta, -1);

    if (dq->blocks[dq->front].front >= dq->block_size) {
        dq->front++;
        if (dq->front > dq->back) {
            dq->front = dq->back;
            dq->blocks[dq->front].front = dq->block_cap / 2;
            dq->blocks[dq->front].back = dq->block_cap / 2;
        } else {
            free(dq->blocks[dq->front - 1].data);
        }
    }
    return CS_SUCCESS;
}

cs_codes deque_erase(deque *dq, int index) {
    CS_RETURN_IF(dq == NULL, CS_NULL);
    int size = deque_size(*dq);
    CS_RETURN_IF(index < 0 || index >= size, CS_POS);

    if (index == 0) {
        return deque_pop_front(dq);
    } else if (index == size - 1) {
        return deque_pop_back(dq);
    }

    void *temp = malloc(dq->attr.size);
    CS_RETURN_IF(temp == NULL, CS_MEM);
    memcpy(temp, deque_at(*dq, index), dq->attr.size);

    if (index < size / 2) {
        // Shift elements towards back
        for (int i = index; i > 0; i--) {
            void *src = deque_at(*dq, i - 1);
            void *dest = deque_at(*dq, i);
            memcpy(dest, src, dq->attr.size);
        }
        memcpy(deque_at(*dq, 0), temp, dq->attr.size);
        free(temp);
        return deque_pop_front(dq);
    } else {
        // Shift elements towards front
        for (int i = index; i < size - 1; i++) {
            void *src = deque_at(*dq, i + 1);
            void *dest = deque_at(*dq, i);
            memcpy(dest, src, dq->attr.size);
        }
        memcpy(deque_at(*dq, size - 1), temp, dq->attr.size);
        free(temp);
        return deque_pop_back(dq);
    }
}

void* deque_front(deque dq) {
    CS_RETURN_IF(deque_empty(dq), NULL);
    return dq.blocks[dq.front].data + (dq.blocks[dq.front].front * dq.attr.size);
}

void* deque_back(deque dq) {
    CS_RETURN_IF(deque_empty(dq), NULL);
    return dq.blocks[dq.back].data + ((dq.blocks[dq.back].back - 1) * dq.attr.size);
}

void *deque_at(deque dq, int index) {
    int size = deque_size(dq);
    CS_RETURN_IF(index < 0 || index >= size, NULL);

    if (index < dq.blocks[dq.front].back - dq.blocks[dq.front].front) {
        return dq.blocks[dq.front].data + ((dq.blocks[dq.front].front + index) * dq.attr.size);
    }
    index -= (dq.blocks[dq.front].back - dq.blocks[dq.front].front);
    int offset = index % dq.block_size;
    index = index / dq.block_size;

    return dq.blocks[dq.front + 1 + index].data + (offset * dq.attr.size);
}

void deque_swap(deque *dq1, deque *dq2) {
    CS_RETURN_IF(dq1 == NULL || dq2 == NULL);

    deque_attr_t temp_attr = dq1->attr;
    deque_block_t *temp_blocks = dq1->blocks;
    metadata_t *temp_meta = dq1->meta;
    int temp_front = dq1->front;
    int temp_back = dq1->back;

    dq1->attr = dq2->attr;
    dq1->blocks = dq2->blocks;
    dq1->meta = dq2->meta;
    dq1->front = dq2->front;
    dq1->back = dq2->back;

    dq2->attr = temp_attr;
    dq2->blocks = temp_blocks;
    dq2->meta = temp_meta;
    dq2->front = temp_front;
    dq2->back = temp_back;
}

void deque_clear(deque *dq) {
    CS_RETURN_IF(dq == NULL);
    for (int i = dq->front; i <= dq->back; i++) {
        if (dq->attr.fr) {
            for (int j = dq->blocks[i].front; j < dq->blocks[i].back; j++) {
                dq->attr.fr(dq->blocks[i].data + (j * dq->attr.size));
            }
        }
        free(dq->blocks[i].data);
        dq->blocks[i].data = NULL;
        dq->blocks[i].front = 0;
        dq->blocks[i].back = 0;
    }
    metadata_size_inc(dq->meta, -deque_size(*dq));
    dq->front = dq->block_cap / 2;
    dq->back = dq->block_cap / 2;
    dq->blocks[dq->front].data = malloc(dq->attr.size * dq->block_size);
    if (dq->blocks[dq->front].data != NULL) {
        dq->blocks[dq->front].front = dq->block_cap / 2;
        dq->blocks[dq->front].back = dq->block_cap / 2;
    }
}

void deque_print(FILE *stream, const void* v_dq) {
    CS_RETURN_IF(stream == NULL || v_dq == NULL);
    deque *dq = (deque *)v_dq;
    CS_RETURN_IF(deque_empty(*dq) || !dq->attr.print);
    for (int i = dq->front; i <= dq->back; i++) {
        for (int j = dq->blocks[i].front; j < dq->blocks[i].back; j++) {
            dq->attr.print(stream, dq->blocks[i].data + (j * dq->attr.size));
        }
    }
}

void deque_free(void *v_dq) {
    CS_RETURN_IF(v_dq == NULL);
    deque *dq = (deque *)v_dq;
    for (int i = dq->front; i <= dq->back; i++) {
        if (dq->attr.fr) {
            for (int j = dq->blocks[i].front; j < dq->blocks[i].back; j++) {
                dq->attr.fr(dq->blocks[i].data + (j * dq->attr.size));
            }
        }
        free(dq->blocks[i].data);
        dq->blocks[i].data = NULL;
        dq->blocks[i].front = 0;
        dq->blocks[i].back = 0;
    }
    free(dq->meta);
    free(dq->blocks);
}