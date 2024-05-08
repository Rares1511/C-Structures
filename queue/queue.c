#include "../include/queue.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

cs_codes queue_init(queue *q, queue_attr_t el_attr)
{
    if (el_attr.size < 0 || el_attr.size > SIZE_TH)
        return CS_SIZE;
    q->attr = el_attr;
    q->size = 0;
    q->start = NULL;
    q->end = NULL;
    return CS_SUCCESS;
}

int queue_empty(queue q)
{
    if (q.size == 0)
        return 1;
    return 0;
}

void *queue_front(queue q)
{
    if (q.size == 0)
        return NULL;
    return q.start->data;
}

void *queue_back(queue q)
{
    if (q.size == 0)
        return NULL;
    return q.end->data;
}

cs_codes queue_push(queue *q, void *el)
{
    queue_node *node = malloc(sizeof(queue_node));
    if (!node)
        return CS_MEM;
    node->data = malloc(q->attr.size);
    if (!node->data)
    {
        free(node);
        return CS_MEM;
    }
    if (q->attr.cp)
        q->attr.cp(node->data, el);
    else
        memcpy(node->data, el, q->attr.size);
    node->next = NULL;
    if (q->size == 0)
        q->start = node;
    else
        q->end->next = node;
    q->end = node;
    q->size++;
    return CS_SUCCESS;
}

cs_codes queue_pop(queue *q)
{
    if (q->size == 0)
        return CS_EMPTY;
    queue_node *aux = q->start;
    q->start = q->start->next;
    q->size--;
    if (q->size == 0)
        q->end = NULL;
    if (q->attr.fr)
        q->attr.fr(aux->data);
    free(aux->data);
    free(aux);
    return CS_SUCCESS;
}

void queue_set_attr(queue *q, queue_attr_t attr) { q->attr = attr; }

void queue_set_free(queue *q, freer fr) { q->attr.fr = fr; }

void queue_set_print(queue *q, printer print) { q->attr.print = print; }

void queue_set_copy(queue *q, deepcopy cp) { q->attr.cp = cp; }

void queue_swap(queue *q1, queue *q2)
{
    queue_node *aux1 = q1->start;
    queue_node *aux2 = q1->end;
    int size = q1->size;
    queue_attr_t attr = q1->attr;

    q1->attr = q2->attr;
    q1->end = q2->end;
    q1->size = q2->size;
    q1->start = q2->start;

    q2->attr = attr;
    q2->end = aux2;
    q2->start = aux1;
    q2->size = size;
}

void queue_clear(queue *q)
{
    while (q->start != NULL)
    {
        queue_node *aux = q->start;
        q->start = q->start->next;
        if (q->attr.fr)
            q->attr.fr(aux->data);
        free(aux->data);
        free(aux);
    }
    q->size = 0;
    q->start = NULL;
    q->end = NULL;
}

void queue_free(void *q_p)
{
    queue *q = (queue *)q_p;
    while (q->start != NULL)
    {
        queue_node *aux = q->start;
        q->start = q->start->next;
        if (q->attr.fr)
            q->attr.fr(aux->data);
        free(aux->data);
        free(aux);
    }
}

void queue_print(void *q_p)
{
    queue q = *(queue *)q_p;
    if (!q.attr.print)
        return;
    queue_node *aux = q.start;
    while (aux != NULL)
    {
        q.attr.print(aux->data);
        aux = aux->next;
    }
}