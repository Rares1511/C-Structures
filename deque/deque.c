#include "../include/deque.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

deque_node *deque_node_init ( void *data, int size ) {
    deque_node *node = malloc ( sizeof ( deque_node ) );
    if ( !node ) return NULL;
    node->data = malloc ( size );
    if ( !node->data ) { free ( node ); return NULL; }
    memcpy ( node->data, data, size );
    node->next = NULL;
    node->prev = NULL;
    return node;
}

cs_codes deque_init ( deque *dq, deque_attr_t attr ) {
    if ( attr.size < 0 || attr.size > SIZE_TH ) return CS_SIZE;
    dq->back = NULL;
    dq->front = NULL;
    dq->attr = attr;
    dq->size = 0;
    return CS_SUCCESS;
}

cs_codes deque_push_back ( deque *dq, void *el ) {
    deque_node *node = deque_node_init ( el, dq->attr.size );
    if ( !node ) return CS_MEM;
    if ( dq->size == 0 ) {
        dq->front = node;
        dq->back = node;
    }
    else {
        dq->back->next = node;
        node->prev = dq->back;
        dq->back = node;
    }
    dq->size++;
    return CS_SUCCESS;
}

cs_codes deque_push_front ( deque *dq, void *el ) {
    deque_node *node = deque_node_init ( el, dq->attr.size );
    if ( !node ) return CS_MEM;
    if ( dq->size == 0 ) {
        dq->front = node;
        dq->back = node;
    }
    else {
        node->next = dq->front;
        dq->front->prev = node;
        dq->front = node;
    }
    dq->size++;
    return CS_SUCCESS;
}

cs_codes deque_pop_front ( deque *dq ) {
    if ( dq->size == 0 ) return CS_SIZE;
    deque_node *aux = dq->front;
    dq->size--;
    dq->front = dq->front->next;
    if ( dq->size != 0 )
        dq->front->prev = NULL;
    else
        dq->back = NULL;
    if ( dq->attr.fr ) dq->attr.fr ( aux->data );
    free ( aux->data );
    free ( aux );
    return CS_SUCCESS;
}

cs_codes deque_pop_back ( deque *dq ) {
    if ( dq->size == 0 ) return CS_SIZE;
    deque_node *aux = dq->back;
    dq->size--;
    dq->back = dq->back->prev;
    if ( dq->size != 0 )
        dq->back->next = NULL;
    else
        dq->front = NULL;
    if ( dq->attr.fr ) dq->attr.fr ( aux->data );
    free ( aux->data );
    free ( aux );
    return CS_SUCCESS;
}

void *deque_front ( deque dq ) {
    if ( dq.size == 0 ) return NULL;
    return dq.front->data;
}

void *deque_back ( deque dq ) {
   if ( dq.size == 0 ) return NULL;
   return dq.back->data;
}

void deque_set_free ( deque *dq, freer fr ) { dq->attr.fr = fr; }

void deque_set_print ( deque *dq, printer print ) { dq->attr.print = print; }

void deque_clear ( deque *dq ) {
    while ( dq->front != NULL ) {
        deque_node *aux = dq->front;
        dq->front = dq->front->next;
        if ( dq->attr.fr ) dq->attr.fr ( aux->data );
        free ( aux->data );
        free ( aux );
    }
    dq->back = NULL;
    dq->front = NULL;
    dq->size = 0;
}

void deque_swap ( deque *dq1, deque *dq2 ) {
    deque_attr_t attr = dq1->attr;
    deque_node *front = dq1->front;
    deque_node *back = dq1->back;
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

void deque_free ( deque *dq ) {
    while ( dq->front != NULL ) {
        deque_node *aux = dq->front;
        dq->front = dq->front->next;
        if ( dq->attr.fr ) dq->attr.fr ( aux->data );
        free ( aux->data );
        free ( aux );
    }
}

void deque_print ( deque dq ) {
    if ( !dq.attr.print ) return;
    deque_node *aux = dq.front;
    while ( aux != NULL ) {
        dq.attr.print ( aux->data );
        aux = aux->next;
    }
}