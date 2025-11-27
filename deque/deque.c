#include <cs/deque.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes deque_init(deque *dq, deque_attr_t attr) {
    if (attr.size < 0 || attr.size > SIZE_TH) {
        return CS_SIZE;
    }
    dq->blocks = malloc(sizeof(deque_block_t) * DEQUE_INIT_BLOCKS);
    if (dq->blocks == NULL) {
        return CS_MEM;
    }

    dq->attr = attr;
    dq->size = 0;
    dq->block_size = DEQUE_BLOCK_SIZE;
    dq->block_cap = DEQUE_INIT_BLOCKS;
    dq->front = dq->block_cap / 2;
    dq->back = dq->block_cap / 2;

    dq->blocks[dq->front].data = malloc(attr.size * dq->block_size);
    if (dq->blocks[dq->front].data == NULL) {
        free(dq->blocks);
        return CS_MEM;
    }
    dq->blocks[dq->front].front = dq->block_cap / 2;
    dq->blocks[dq->front].back = dq->block_cap / 2;
    return CS_SUCCESS;
}

cs_codes deque_push_back(deque *dq, void *el) {
    if (!dq || !el) {
        return CS_ELEM;
    }

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
    memcpy(dq->blocks[dq->back].data + (dq->blocks[dq->back].back * dq->attr.size), el, dq->attr.size);
    dq->blocks[dq->back].back++;
    dq->size++;
    return CS_SUCCESS;
}

cs_codes deque_push_front(deque *dq, void *el) {
    if (!dq || !el) {
        return CS_ELEM;
    }

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
    memcpy(dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size), el, dq->attr.size);
    dq->size++;
    return CS_SUCCESS;
}

cs_codes deque_pop_back(deque *dq) {
    if (!dq || dq->size == 0 ) {
        return CS_EMPTY;
    }

    dq->blocks[dq->back].back--;
    if (dq->attr.fr) {
        dq->attr.fr(dq->blocks[dq->back].data + (dq->blocks[dq->back].back * dq->attr.size));
    }
    dq->size--;

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
    if (!dq || dq->size == 0 ) {
        return CS_EMPTY;
    }

    if (dq->attr.fr) {
        dq->attr.fr(dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size));
    }
    dq->blocks[dq->front].front++;
    dq->size--;

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

void* deque_front(deque dq) {
    if (dq.size == 0) {
        return NULL;
    }
    return dq.blocks[dq.front].data + (dq.blocks[dq.front].front * dq.attr.size);
}

void* deque_back(deque dq) {
    if (dq.size == 0) {
        return NULL;
    }
    return dq.blocks[dq.back].data + ((dq.blocks[dq.back].back - 1) * dq.attr.size);
}

void *deque_at(deque dq, int index) {
    if (index < 0 || index >= dq.size) {
        return NULL;
    }

    if (index < dq.blocks[dq.front].back - dq.blocks[dq.front].front) {
        return dq.blocks[dq.front].data + ((dq.blocks[dq.front].front + index) * dq.attr.size);
    }
    index -= (dq.blocks[dq.front].back - dq.blocks[dq.front].front);
    int offset = index % dq.block_size;
    index = index / dq.block_size;

    return dq.blocks[dq.front + 1 + index].data + (offset * dq.attr.size);
}

void deque_swap(deque *dq1, deque *dq2) {
    if (!dq1 || !dq2) {
        return;
    }
    deque_attr_t temp_attr = dq1->attr;
    deque_block_t *temp_blocks = dq1->blocks;
    int temp_size = dq1->size;
    int temp_front = dq1->front;
    int temp_back = dq1->back;

    dq1->attr = dq2->attr;
    dq1->blocks = dq2->blocks;
    dq1->size = dq2->size;
    dq1->front = dq2->front;
    dq1->back = dq2->back;

    dq2->attr = temp_attr;
    dq2->blocks = temp_blocks;
    dq2->size = temp_size;
    dq2->front = temp_front;
    dq2->back = temp_back;
}

void deque_print(FILE *stream, void *v_dq) {
    if (!v_dq || !stream) {
        return;
    }
    deque *dq = (deque *)v_dq;
    if (!dq || !dq->attr.print) {
        return;
    }
    for (int i = dq->front; i <= dq->back; i++) {
        for (int j = dq->blocks[i].front; j < dq->blocks[i].back; j++) {
            dq->attr.print(stream, dq->blocks[i].data + (j * dq->attr.size));
        }
    }
}

void deque_free(void *v_dq) {
    if (!v_dq) {
        return;
    }
    deque *dq = (deque *)v_dq;
    if (!dq) {
        return;
    }
    for (int i = dq->front; i <= dq->back; i++) {
        if (dq->attr.fr) {
            for (int j = dq->blocks[i].front; j < dq->blocks[i].back; j++) {
                dq->attr.fr(dq->blocks[i].data + (j * dq->attr.size));
            }
        }
        free(dq->blocks[i].data);
    }
    free(dq->blocks);
}