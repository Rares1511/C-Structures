#include <cs/deque.h>

cs_codes _deque_grow_internal(deque *dq, int direction) {
    int active_blocks = dq->back - dq->front + 1;

    if (direction == __DEQUE_GROW_INTERNAL_BACK) {
        int next_back = dq->back + 1;
        if (next_back >= dq->block_cap) {
            if (dq->front > dq->block_cap / 4) {
                // We have enough capacity, just need to shift blocks to the front
                int new_front = (dq->block_cap - active_blocks) / 2;
                memmove(&dq->blocks[new_front], &dq->blocks[dq->front], sizeof(deque_block_t) * active_blocks);
                dq->front = new_front;
                dq->back = dq->front + active_blocks - 1;
                next_back = dq->back + 1;
            }
            else {
                dq->block_cap += dq->block_cap / 2;
                dq->blocks = realloc(dq->blocks, sizeof(deque_block_t) * dq->block_cap);
                if (dq->blocks == NULL) {
                    return CS_MEM;
                }
            }
        }
        dq->blocks[next_back].data = malloc(dq->attr.size * dq->dq_attr.block_size);
        if (dq->blocks[next_back].data == NULL) {
            return CS_MEM;
        }
        dq->blocks[next_back].front = 0;
        dq->blocks[next_back].back = 0;
        dq->back = next_back;
    } 
    else if (direction == __DEQUE_GROW_INTERNAL_FRONT) {
        if (dq->front - 1 < 0) {
            if ((dq->block_cap - dq->back) > dq->block_cap / 4) {
                // We have enough capacity, just need to shift blocks to the back
                int new_front = (dq->block_cap - active_blocks) / 2;
                memmove(&dq->blocks[new_front], &dq->blocks[dq->front], sizeof(deque_block_t) * active_blocks);
                dq->front = new_front;
                dq->back = dq->front + active_blocks - 1;
            }
            else {
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
        }
        int next_front = dq->front - 1;
        dq->blocks[next_front].data = malloc(dq->attr.size * dq->dq_attr.block_size);
        if (dq->blocks[next_front].data == NULL) {
            return CS_MEM;
        }
        dq->blocks[next_front].front = dq->dq_attr.block_size;
        dq->blocks[next_front].back = dq->dq_attr.block_size;
        dq->front = next_front;
    }
    
    return CS_SUCCESS;
}

cs_codes deque_init(deque *dq, elem_attr_t attr, deque_attr_t dq_attr) {
    CS_RETURN_IF(NULL == dq, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(dq_attr.min_cap < 0 || dq_attr.min_cap > DEQUE_INIT_BLOCKS, CS_SIZE);
    CS_RETURN_IF(dq_attr.block_size < 0 || dq_attr.block_size > DEQUE_BLOCK_SIZE, CS_SIZE);

    if (dq_attr.min_cap == 0) {
        dq_attr.min_cap = DEQUE_INIT_BLOCKS;
    }
    if (dq_attr.block_size == 0) {
        dq_attr.block_size = DEQUE_BLOCK_SIZE;
    }

    dq->attr = attr;
    dq->dq_attr = dq_attr;
    dq->size = 0;
    dq->block_cap = dq_attr.min_cap;
    dq->front = dq->block_cap / 2;
    dq->back = dq->block_cap / 2;

    dq->blocks = malloc(sizeof(deque_block_t) * dq->dq_attr.min_cap);
    if (NULL == dq->blocks) {
        return CS_MEM;
    }

    dq->blocks[dq->front].data = malloc(attr.size * dq->dq_attr.block_size);
    if (NULL == dq->blocks[dq->front].data) {
        free(dq->blocks);
        return CS_MEM;
    }

    dq->blocks[dq->front].front = dq->block_cap / 2;
    dq->blocks[dq->front].back = dq->block_cap / 2;

    dq->header.type = CS_DEQUE_TYPE;
    dq->header.magic = CS_DEQUE_MAGIC;
    return CS_SUCCESS;
}

cs_codes deque_insert_at(deque *dq, const void *el, int index) {
    CS_RETURN_IF(dq == NULL || el == NULL, CS_NULL);
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC, CS_UNINITIALIZED);
    int size = dq->size;
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
            dq->attr.fr(deque_at(dq, 1));
        }
        for (int i = 1; i < index; i++) {
            void *src = deque_at(dq, i + 1);
            void *dest = deque_at(dq, i);
            memcpy(dest, src, dq->attr.size);
        }
    } else {
        // Shift elements towards back - make space at back first
        void *last_el = dq->blocks[dq->back].data + ((dq->blocks[dq->back].back - 1) * dq->attr.size);
        deque_push_back(dq, last_el);
        // Free the duplicate at position size-1 (was last before push, now second-to-last)
        if (dq->attr.fr) {
            dq->attr.fr(deque_at(dq, size));
        }
        for (int i = size; i > index; i--) {
            void *src = deque_at(dq, i - 1);
            void *dest = deque_at(dq, i);
            memcpy(dest, src, dq->attr.size);
        }
    }

    void *target = deque_at(dq, index);
    if (dq->attr.copy) {
        dq->attr.copy(target, el);
    } else {
        memcpy(target, el, dq->attr.size);
    }

    return CS_SUCCESS;
}

cs_codes deque_erase(deque *dq, int index) {
    CS_RETURN_IF(dq == NULL, CS_NULL);
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC, CS_UNINITIALIZED);
    int size = dq->size;
    CS_RETURN_IF(index < 0 || index >= size, CS_POS);

    if (index == 0) {
        return deque_pop_front(dq);
    } else if (index == size - 1) {
        return deque_pop_back(dq);
    }

    void *temp = malloc(dq->attr.size);
    CS_RETURN_IF(temp == NULL, CS_MEM);
    memcpy(temp, deque_at(dq, index), dq->attr.size);

    if (index < size / 2) {
        // Shift elements towards back
        for (int i = index; i > 0; i--) {
            void *src = deque_at(dq, i - 1);
            void *dest = deque_at(dq, i);
            memcpy(dest, src, dq->attr.size);
        }
        memcpy(deque_at(dq, 0), temp, dq->attr.size);
        free(temp);
        return deque_pop_front(dq);
    } else {
        // Shift elements towards front
        for (int i = index; i < size - 1; i++) {
            void *src = deque_at(dq, i + 1);
            void *dest = deque_at(dq, i);
            memcpy(dest, src, dq->attr.size);
        }
        memcpy(deque_at(dq, size - 1), temp, dq->attr.size);
        free(temp);
        return deque_pop_back(dq);
    }
}

void deque_swap(deque *dq1, deque *dq2) {
    CS_RETURN_IF(dq1 == NULL || dq2 == NULL || dq1->header.magic != CS_DEQUE_MAGIC || dq2->header.magic != CS_DEQUE_MAGIC);

    elem_attr_t temp_attr = dq1->attr;
    deque_attr_t temp_dq_attr = dq1->dq_attr;
    deque_block_t *temp_blocks = dq1->blocks;
    int temp_size = dq1->size;
    int temp_front = dq1->front;
    int temp_back = dq1->back;

    dq1->attr = dq2->attr;
    dq1->dq_attr = dq2->dq_attr;
    dq1->blocks = dq2->blocks;
    dq1->size = dq2->size;
    dq1->front = dq2->front;
    dq1->back = dq2->back;

    dq2->attr = temp_attr;
    dq2->dq_attr = temp_dq_attr;
    dq2->blocks = temp_blocks;
    dq2->size = temp_size;
    dq2->front = temp_front;
    dq2->back = temp_back;
}

void deque_clear(deque *dq) {
    CS_RETURN_IF(dq == NULL || dq->header.magic != CS_DEQUE_MAGIC);
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
    dq->size = 0;
    dq->front = dq->block_cap / 2;
    dq->back = dq->block_cap / 2;
    dq->blocks[dq->front].data = malloc(dq->attr.size * dq->dq_attr.block_size);
    if (dq->blocks[dq->front].data != NULL) {
        dq->blocks[dq->front].front = dq->block_cap / 2;
        dq->blocks[dq->front].back = dq->block_cap / 2;
    }
}

void deque_print(FILE *stream, const void* v_dq) {
    CS_RETURN_IF(stream == NULL || v_dq == NULL);
    deque *dq = (deque *)v_dq;
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC || deque_empty(dq) || !dq->attr.print);
    for (int i = dq->front; i <= dq->back; i++) {
        for (int j = dq->blocks[i].front; j < dq->blocks[i].back; j++) {
            dq->attr.print(stream, dq->blocks[i].data + (j * dq->attr.size));
        }
    }
}

void deque_free(void *v_dq) {
    CS_RETURN_IF(v_dq == NULL);
    deque *dq = (deque *)v_dq;
    CS_RETURN_IF(dq->header.magic != CS_DEQUE_MAGIC);
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
    dq->header.magic = 0; // Invalidate the deque
    free(dq->blocks);
}