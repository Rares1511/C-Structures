#include <cs/queue.h>
#include <cs/list.h>
#include <cs/deque.h>

#include <stdlib.h>

cs_codes queue_init(queue *q, queue_attr attr, queue_type type) {
    CS_RETURN_IF(NULL == q, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    q->type = type;
    switch (type) {
        case CS_QUEUE_LIST:
            q->container = malloc(sizeof(list));
            CS_RETURN_IF(NULL == q->container, CS_MEM);
            return list_init(q->container, attr);
        case CS_QUEUE_DEQUE:
            q->container = malloc(sizeof(deque));
            CS_RETURN_IF(NULL == q->container, CS_MEM);
            return deque_init(q->container, attr);
        default:
            return CS_ELEM;
    }
    return CS_SUCCESS;
}

cs_codes queue_push(queue *q, void *data) {
    CS_RETURN_IF(q == NULL || data == NULL, CS_NULL);
    switch (q->type) {
    case CS_QUEUE_LIST:
        return list_push_back((list *)q->container, data);
    case CS_QUEUE_DEQUE:
        return deque_push_back((deque *)q->container, data);
    default:
        return CS_FUNC;
    }
}

cs_codes queue_pop(queue *q) {
    CS_RETURN_IF(q == NULL, CS_NULL);
    switch (q->type) {
    case CS_QUEUE_LIST:
        return list_pop_front((list *)q->container);
    case CS_QUEUE_DEQUE:
        return deque_pop_front((deque *)q->container);
    default:
        return CS_FUNC;
    }
}

void *queue_front(queue *q) {
    CS_RETURN_IF(q == NULL, NULL);
    switch (q->type) {
    case CS_QUEUE_LIST:
        return list_front(*((list *)q->container));
    case CS_QUEUE_DEQUE:
        return deque_front(*((deque *)q->container));
    default:
        return NULL;
    }
}

void *queue_back(queue *q) {
    CS_RETURN_IF(q == NULL, NULL);
    switch (q->type) {
    case CS_QUEUE_LIST:
        return list_back(*((list *)q->container));
    case CS_QUEUE_DEQUE:
        return deque_back(*((deque *)q->container));
    default:
        return NULL;
    }
}

int queue_empty(queue q) {
    switch (q.type) {
    case CS_QUEUE_LIST:
        return list_empty(*(list *)q.container);
    case CS_QUEUE_DEQUE:
        return deque_empty(*(deque *)q.container);
    default:
        return 1;
    }
}

int queue_size(queue q) {
    switch (q.type) {
    case CS_QUEUE_LIST:
        return list_size(*(list *)q.container);
    case CS_QUEUE_DEQUE:
        return deque_size(*(deque *)q.container);
    default:
        return 0;
    }
}

void queue_clear(queue *q) {
    CS_RETURN_IF(q == NULL);
    switch (q->type) {
    case CS_QUEUE_LIST:
        list_clear((list *)q->container);
        break;
    case CS_QUEUE_DEQUE:
        deque_clear((deque *)q->container);
        break;
    default:
        break;
    }
}

void queue_swap(queue *q1, queue *q2) {
    CS_RETURN_IF(q1 == NULL || q2 == NULL);
    
    void *container = q1->container;
    queue_type type = q1->type;

    q1->container = q2->container;
    q1->type = q2->type;

    q2->container = container;
    q2->type = type;
}

void queue_print(FILE *stream, void *v_q)  {
    CS_RETURN_IF(v_q == NULL || stream == NULL);
    queue *q = (queue *)v_q;
    switch (q->type) {
    case CS_QUEUE_LIST:
        list_print(stream, q->container);
        break;
    case CS_QUEUE_DEQUE:
        deque_print(stream, q->container);
        break;
    default:
        break;
    }
}

void queue_free(void *v_q) {
    CS_RETURN_IF(v_q == NULL);
    queue *q = (queue *)v_q;
    switch (q->type) {
    case CS_QUEUE_LIST:
        list_free(q->container);
        break;
    case CS_QUEUE_DEQUE:
        deque_free(q->container);
        break;
    default:
        break;
    }
    free(q->container);
}