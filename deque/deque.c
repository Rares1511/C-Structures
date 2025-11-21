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
    dq->front = DEQUE_INIT_BLOCKS / 2;
    dq->back = DEQUE_INIT_BLOCKS / 2;

    dq->blocks[dq->front].data = malloc(attr.size * DEQUE_BLOCK_SIZE);
    if (dq->blocks[dq->front].data == NULL) {
        free(dq->blocks);
        return CS_MEM;
    }
    dq->blocks[dq->front].front = DEQUE_BLOCK_SIZE / 2;
    dq->blocks[dq->front].back = DEQUE_BLOCK_SIZE / 2;
    return CS_SUCCESS;
}

cs_codes deque_push_back(deque *dq, void *el) {
    if (!dq || !el) {
        return CS_ELEM;
    }

    if (dq->blocks[dq->back].back >= DEQUE_BLOCK_SIZE) {
        dq->back++;
        dq->blocks[dq->back].data = malloc(dq->attr.size * DEQUE_BLOCK_SIZE);
        if (dq->blocks[dq->back].data == NULL) {
            return CS_MEM;
        }
        dq->blocks[dq->back].front = 0;
        dq->blocks[dq->back].back = 0;
    }
    memcpy((char *)dq->blocks[dq->back].data + (dq->blocks[dq->back].back * dq->attr.size), el, dq->attr.size);
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
        dq->blocks[dq->front].data = malloc(dq->attr.size * DEQUE_BLOCK_SIZE);
        if (dq->blocks[dq->front].data == NULL) {
            return CS_MEM;
        }
        dq->blocks[dq->front].front = DEQUE_BLOCK_SIZE;
        dq->blocks[dq->front].back = DEQUE_BLOCK_SIZE;
    }
    dq->blocks[dq->front].front--;
    memcpy((char *)dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size), el, dq->attr.size);
    dq->size++;
    return CS_SUCCESS;
}

void* deque_front(deque *dq) {
    if (!dq || dq->size == 0) {
        return NULL;
    }
    return (char *)dq->blocks[dq->front].data + (dq->blocks[dq->front].front * dq->attr.size);
}

void* deque_back(deque *dq) {
    if (!dq || dq->size == 0) {
        return NULL;
    }
    return (char *)dq->blocks[dq->back].data + ((dq->blocks[dq->back].back - 1) * dq->attr.size);
}