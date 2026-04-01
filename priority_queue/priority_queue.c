#include <cs/priority_queue.h>
#include <cs/vector.h>
#include <cs/deque.h>

#include <stdlib.h>
#include <string.h>

#pragma region HELPER_FUNCTIONS
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


void *priority_queue_at(priority_queue pq, int index) {
    switch (pq.type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            return vector_at(*((vector*)pq.container), index);
        case CS_PRIORITY_QUEUE_DEQUE:
            return deque_at(*((deque*)pq.container), index);
        default:
            return NULL;
    }
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion HELPER_FUNCTIONS

cs_codes priority_queue_init(priority_queue *pq, elem_attr_t attr, priority_queue_type type) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(pq == NULL, CS_NULL);

    pq->type = type;
    switch (type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            pq->container = malloc(sizeof(vector));
            CS_RETURN_IF(NULL == pq->container, CS_MEM);
            vector_attr_t v_attr = { .min_cap = 0, .shrink_factor = 0 };
            return vector_init((vector*)pq->container, attr, v_attr);
        case CS_PRIORITY_QUEUE_DEQUE:
            pq->container = malloc(sizeof(deque));
            CS_RETURN_IF(NULL == pq->container, CS_MEM);
            deque_attr_t dq_attr = { .min_cap = 0, .block_size = 0 };
            return deque_init((deque*)pq->container, attr, dq_attr);
        default:
            return CS_ELEM;
    }
    return CS_SUCCESS;
}

cs_codes priority_queue_push(priority_queue *pq, void *data) {
    CS_RETURN_IF(pq == NULL || data == NULL, CS_NULL);
    int rc, size = 0;
    comparer comp = NULL;
    void *temp = NULL;

    switch (pq->type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            rc = vector_push_back((vector*)pq->container, data);
            comp = ((vector*)pq->container)->attr.comp;
            size = ((vector*)pq->container)->attr.size;
            break;
        case CS_PRIORITY_QUEUE_DEQUE:
            rc = deque_push_back((deque*)pq->container, data);
            comp = ((deque*)pq->container)->attr.comp;
            size = ((deque*)pq->container)->attr.size;
            break;
        default:
            rc = CS_FUNC;
    }
    CS_RETURN_IF(rc != CS_SUCCESS, rc);

    temp = malloc(size);
    CS_RETURN_IF(temp == NULL, CS_MEM);

    int idx = priority_queue_size(*pq);
    while (idx > 1) {
        int parent_idx = idx / 2;
        void *current = priority_queue_at(*pq, idx - 1);
        void *parent = priority_queue_at(*pq, parent_idx - 1);
        if (comp(current, parent) > 0) {
            memcpy(temp, current, size);
            memcpy(current, parent, size);
            memcpy(parent, temp, size);
        }
        else {
            break;
        }
        idx = parent_idx;
    }
    free(temp);
    return CS_SUCCESS;
}

cs_codes priority_queue_pop(priority_queue *pq) {
    CS_RETURN_IF(pq == NULL, CS_NULL);
    CS_RETURN_IF(priority_queue_empty(*pq), CS_EMPTY);
    
    int rc, size = 0;
    comparer comp = NULL;
    void *temp = NULL;
    freer fr = NULL;

    switch (pq->type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            size = ((vector*)pq->container)->attr.size;
            comp = ((vector*)pq->container)->attr.comp;
            fr = ((vector*)pq->container)->attr.fr;
            break;
        case CS_PRIORITY_QUEUE_DEQUE:
            size = ((deque*)pq->container)->attr.size;
            comp = ((deque*)pq->container)->attr.comp;
            fr = ((deque*)pq->container)->attr.fr;
            break;
        default:
            return CS_FUNC;
    }

    int last_index = priority_queue_size(*pq) - 1;
    
    // Special case: only one element, just pop it
    if (last_index == 0) {
        switch (pq->type) {
            case CS_PRIORITY_QUEUE_VECTOR:
                return vector_pop_back((vector*)pq->container);
            case CS_PRIORITY_QUEUE_DEQUE:
                return deque_pop_back((deque*)pq->container);
            default:
                return CS_FUNC;
        }
    }

    temp = malloc(size);
    CS_RETURN_IF(temp == NULL, CS_MEM);

    void *top = priority_queue_at(*pq, 0);
    void *last = priority_queue_at(*pq, last_index);
    
    // Free the top element's internal data before overwriting
    if (fr) fr(top);
    
    // Shallow copy last to top (now owns last's internal data)
    memcpy(top, last, size);
    
    // Zero out last so pop_back's free call won't double-free
    memset(last, 0, size);

    switch (pq->type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            rc = vector_pop_back((vector*)pq->container);
            break;
        case CS_PRIORITY_QUEUE_DEQUE:
            rc = deque_pop_back((deque*)pq->container);
            break;
        default:
            rc = CS_FUNC;
    }
    CS_RETURN_IF(rc != CS_SUCCESS, rc);

    int idx = 1;
    while (1) {
        int left_idx = idx * 2;
        int right_idx = idx * 2 + 1;
        int largest_idx = idx;

        void *current = priority_queue_at(*pq, idx - 1);
        void *left = priority_queue_at(*pq, left_idx - 1);
        void *right = priority_queue_at(*pq, right_idx - 1);

        if (left_idx <= priority_queue_size(*pq) && comp(left, current) > 0) {
            largest_idx = left_idx;
        }
        if (right_idx <= priority_queue_size(*pq) && comp(right, priority_queue_at(*pq, largest_idx - 1)) > 0) {
            largest_idx = right_idx;
        }
        if (largest_idx != idx) {
            void *largest = priority_queue_at(*pq, largest_idx - 1);
            memcpy(temp, current, size);
            memcpy(current, largest, size);
            memcpy(largest, temp, size);
            idx = largest_idx;
        }
        else {
            break;
        }
    }
    free(temp);
    return CS_SUCCESS;
}

void* priority_queue_top(priority_queue pq) {
    if (priority_queue_empty(pq)) {
        return NULL;
    }
    return priority_queue_at(pq, 0);
}

int priority_queue_empty(priority_queue pq) {
    switch (pq.type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            return vector_empty(*((vector*)pq.container));
        case CS_PRIORITY_QUEUE_DEQUE:
            return deque_empty(*((deque*)pq.container));
        default:
            return 1;
    }
}

int priority_queue_size(priority_queue pq) {
    switch (pq.type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            return vector_size(*((vector*)pq.container));
        case CS_PRIORITY_QUEUE_DEQUE:
            return deque_size(*((deque*)pq.container));
        default:
            return 0;
    }
}

void priority_queue_clear(priority_queue *pq) {
    CS_RETURN_IF(pq == NULL);
    switch (pq->type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            vector_clear((vector*)pq->container);
            break;
        case CS_PRIORITY_QUEUE_DEQUE:
            deque_clear((deque*)pq->container);
            break;
        default:
            break;
    }
}

void priority_queue_swap(priority_queue *pq1, priority_queue *pq2) {
    CS_RETURN_IF(pq1 == NULL || pq2 == NULL);
    void *temp_container = pq1->container;
    priority_queue_type temp_type = pq1->type;

    pq1->container = pq2->container;
    pq1->type = pq2->type;

    pq2->container = temp_container;
    pq2->type = temp_type;
}

void priority_queue_print(FILE *stream, void *v_pq) {
    CS_RETURN_IF(v_pq == NULL);
    priority_queue *pq = (priority_queue*) v_pq;

    switch (pq->type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            vector_print(stream, (vector*)pq->container);
            break;
        case CS_PRIORITY_QUEUE_DEQUE:
            deque_print(stream, (deque*)pq->container);
            break;
        default:
            break;
    }
}

void priority_queue_free(void *v_pq) {
    CS_RETURN_IF(v_pq == NULL);
    priority_queue *pq = (priority_queue*) v_pq;

    switch (pq->type) {
        case CS_PRIORITY_QUEUE_VECTOR:
            vector_free(pq->container);
            break;
        case CS_PRIORITY_QUEUE_DEQUE:
            deque_free(pq->container);
            break;
        default:
            break;
    }
    free(pq->container);
}