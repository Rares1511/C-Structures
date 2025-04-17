#include "deque_internal.h"

cs_codes deque_init(deque *dq, deque_attr_t attr) {
    if (attr.size < 0 || attr.size > SIZE_TH) {
        return CS_SIZE;
    }
    dq->back = NULL;
    dq->front = NULL;
    dq->attr = attr;
    dq->size = 0;
    return CS_SUCCESS;
}

cs_codes deque_push_back(deque *dq, void *el) {
    deque_node_t *node = deque_node_init(el, dq->attr.size, dq->attr.copy);
    if (!node)
        return CS_MEM;
    if (dq->size == 0) {
        dq->front = node;
        dq->back = node;
    } else {
        dq->back->next = node;
        node->prev = dq->back;
        dq->back = node;
    }
    dq->size++;
    return CS_SUCCESS;
}

cs_codes deque_push_front(deque *dq, void *el) {
    deque_node_t *node = deque_node_init(el, dq->attr.size, dq->attr.copy);
    if (!node)
        return CS_MEM;
    if (dq->size == 0) {
        dq->front = node;
        dq->back = node;
    } else {
        node->next = dq->front;
        dq->front->prev = node;
        dq->front = node;
    }
    dq->size++;
    return CS_SUCCESS;
}

cs_codes deque_pop_front(deque *dq) {
    if (dq->size == 0)
        return CS_SIZE;
    deque_node_t *aux = dq->front;
    dq->size--;
    dq->front = dq->front->next;
    if (dq->size != 0)
        dq->front->prev = NULL;
    else
        dq->back = NULL;
    if (dq->attr.fr)
        dq->attr.fr(aux->data);
    free(aux->data);
    free(aux);
    return CS_SUCCESS;
}

cs_codes deque_pop_back(deque *dq) {
    if (dq->size == 0)
        return CS_SIZE;
    deque_node_t *aux = dq->back;
    dq->size--;
    dq->back = dq->back->prev;
    if (dq->size != 0)
        dq->back->next = NULL;
    else
        dq->front = NULL;
    if (dq->attr.fr)
        dq->attr.fr(aux->data);
    free(aux->data);
    free(aux);
    return CS_SUCCESS;
}

cs_codes deque_clone(deque *dest, deque src) {
    deque_init(dest, src.attr);
    if (src.size == 0) {
        return CS_SUCCESS;
    }
    deque_node_t *aux = src.front;
    while (aux != NULL) {
        cs_codes res = deque_push_back(dest, aux->data);
        if (res != CS_SUCCESS) {
            deque_free(dest);
            return res;
        }
        aux = aux->next;
    }

    return CS_SUCCESS;
}

void deque_front(deque dq, void *el) {
    if (dq.size == 0)
        return;
    memcpy(el, dq.front->data, dq.attr.size);
}

void deque_back(deque dq, void *el) {
    if (dq.size == 0)
        return;
    memcpy(el, dq.back->data, dq.attr.size);
}

void deque_set_attr(deque *dq, deque_attr_t attr) {
    if (dq->size == 0) {
        dq->attr = attr;
    } else if (attr.size != dq->attr.size) {
        return;
    }
}

void deque_set_free(deque *dq, freer fr) { dq->attr.fr = fr; }

void deque_set_print(deque *dq, printer print) { dq->attr.print = print; }

void deque_set_copy(deque *dq, deepcopy copy) {
    if (dq->size == 0) {
        dq->attr.copy = copy;
    }
}

void deque_clear(deque *dq) {
    while (dq->front != NULL) {
        deque_node_t *aux = dq->front;
        dq->front = dq->front->next;
        if (dq->attr.fr)
            dq->attr.fr(aux->data);
        free(aux->data);
        free(aux);
    }
    dq->back = NULL;
    dq->front = NULL;
    dq->size = 0;
}

void deque_swap(deque *dq1, deque *dq2) {
    deque_attr_t attr = dq1->attr;
    deque_node_t *front = dq1->front;
    deque_node_t *back = dq1->back;
    int size = dq1->size;

    dq1->attr = dq2->attr;
    dq1->back = dq2->back;
    dq1->front = dq2->front;
    dq1->size = dq2->size;

    dq2->attr = attr;
    dq2->back = back;
    dq2->front = front;
    dq2->size = size;
}

void deque_free(deque *dq) {
    while (dq->front != NULL) {
        deque_node_t *aux = dq->front;
        dq->front = dq->front->next;
        if (dq->attr.fr)
            dq->attr.fr(aux->data);
        free(aux->data);
        free(aux);
    }
    dq->back = NULL;
    dq->front = NULL;
    dq->size = 0;
}

void deque_print(deque dq) {
    if (!dq.attr.print)
        return;
    deque_node_t *aux = dq.front;
    while (aux != NULL) {
        dq.attr.print(aux->data);
        aux = aux->next;
    }
}