#include "../include/list.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

list_node* list_node_init ( void *el, int size ) {
    list_node *aux = malloc ( sizeof ( list_node ) );
    if ( !aux ) return NULL;
    aux->data = malloc ( size );
    if ( !aux->data ) { free ( aux ); return NULL; }
    memcpy ( aux->data, el, size );
    aux->next = aux;
    aux->prev = aux;
    return aux;
}

cs_codes list_init ( list *l, list_attr_t attr ) {
    if ( attr.size < 0 || attr.size > SIZE_TH ) return CS_SIZE;
    l->attr = attr;
    l->size = 0;
    l->front = NULL;
    return CS_SUCCESS;
}

cs_codes list_push_front ( list *l, void *el ) {
    list_node *aux = list_node_init ( el, l->attr.size );
    if ( !aux ) return CS_MEM;
    if ( l->size != 0 ) {
        aux->prev = l->front->prev;
        l->front->prev->next = aux;
        l->front->prev = aux;
        aux->next = l->front;
    }
    l->front = aux;
    l->size++;
    return CS_SUCCESS;
}

cs_codes list_push_back ( list *l, void *el ) {
    list_node *aux = list_node_init ( el, l->attr.size );
    if ( !aux ) return CS_MEM;
    if ( l->size != 0 ) {
        l->front->prev->next = aux;
        aux->prev = l->front->prev;
        aux->next = l->front;
        l->front->prev = aux;
    }
    else
        l->front = aux;
    l->size++;
    return CS_SUCCESS;
}

cs_codes list_pop_front ( list *l ) {
    if ( l->size == 0 ) return CS_EMPTY;
    l->size--;
    if ( l->size == 0 ) {
        if ( l->attr.fr ) l->attr.fr ( l->front->data );
        free ( l->front->data );
        free ( l->front );
        l->front = NULL;
        return CS_SUCCESS;
    }
    list_node *aux = l->front;
    l->front->prev->next = l->front->next;
    l->front->next->prev = l->front->prev;
    l->front = l->front->next;
    if ( l->attr.fr ) l->attr.fr ( aux->data );
    free ( aux->data );
    free ( aux );
    return CS_SUCCESS;
}

cs_codes list_pop_back ( list *l ) {
    if ( l->size == 0 ) return CS_EMPTY;
    l->size--;
    if ( l->size == 0 ) {
        if ( l->attr.fr ) l->attr.fr ( l->front->data );
        free ( l->front->data );
        free ( l->front );
        return CS_SUCCESS;
    }
    list_node *aux = l->front->prev;
    l->front->prev->prev->next = l->front;
    l->front->prev = l->front->prev->prev;
    if ( l->attr.fr ) l->attr.fr ( aux->data );
    free ( aux->data );
    free ( aux );
    return CS_SUCCESS;
}

cs_codes list_insert ( list *l, int start, int count, void *vec ) {
    if ( start < 0 || start > l->size ) return CS_SIZE;
    list laux;
    list_init ( &laux, l->attr );
    for ( int i = 0; i < count; i++ ) {
        int rc = list_push_back ( &laux, vec + l->attr.size * i );
        if ( rc == CS_MEM ) return rc;
    }
    return list_merge ( l, &laux, start );
}

cs_codes list_erase ( list *l, int start, int count ) {
    if ( start + count > l->size || count == 0 ) return CS_SIZE;
    list_node *node = l->front->prev;
    int i;
    for ( i = 0; i < start; i++ )
        node = node->next;
    list_node *end = node->next;
    for ( i = 0; i < count; i++ ) {
        list_node *aux = end;
        end = end->next;
        if ( l->attr.fr ) l->attr.fr ( aux->data );
        free ( aux->data );
        free ( aux );
    }
    l->size -= count;
    if ( l->size == 0 )
        l->front = NULL;
    else {
        if ( start == 0 )
            l->front = end;
        node->next = end;
        end->prev = node;
    }
    return CS_SUCCESS;
}

cs_codes list_remove ( list *l, void *el ) {
    if ( l->size == 0 ) return CS_EMPTY;
    list_node *node = l->front->next;
    while ( node != l->front ) {
        if ( l->attr.comp ( node->data, el ) == 0 ) {
            list_node *aux = node;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node = node->next;
            if ( l->attr.fr ) l->attr.fr ( aux->data );
            free ( aux->data );
            free ( aux );
            l->size--;
        }
        else
            node = node->next;
    }
    if ( l->attr.comp ( l->front->data, el ) == 0 )
        list_pop_front ( l );
    return CS_SUCCESS;
}

cs_codes list_remove_if ( list *l, condition cond ) {
    if ( l->size == 0 ) return CS_EMPTY;
    list_node *node = l->front->next;
    while ( node != l->front ) {
        if ( cond ( node->data ) == 0 ) {
            list_node *aux = node;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node = node->next;
            if ( l->attr.fr ) l->attr.fr ( aux->data );
            free ( aux->data );
            free ( aux );
            l->size--;
        }
        else
            node = node->next;
    }
    if ( cond ( l->front->data ) == 0 )
        list_pop_front ( l );
    return CS_SUCCESS;
}

cs_codes list_unique ( list *l ) {
    if ( !l->attr.comp ) return CS_FUNC;
    list_node *start = l->front->next;
    while ( start != l->front ) {
        list_node *end = start->next;
        while ( end != l->front && l->attr.comp ( start->data, end->data ) == 0 ) {
            list_node *aux = end;
            end = end->next;
            if ( l->attr.fr ) l->attr.fr ( aux->data );
            free ( aux->data );
            free ( aux );
        }
        start->next = end;
        end->prev = start;
        start = end;
    }
    if ( l->size > 1 && l->attr.comp ( l->front->data, l->front->next->data ) == 0 )
        list_pop_front ( l );
    return CS_SUCCESS;
}

cs_codes list_merge ( list *l1, list *l2, int start ) {
    if ( start < 0 || start > l1->size ) return CS_POS;
    if ( l2->size == 0 ) return CS_SUCCESS;
    list_node *node = l1->front->prev;
    for ( int i = 0; i < start; i++ )
        node = node->next;
    l2->front->prev->next = node->next;
    node->next->prev = l2->front->prev;
    l2->front->prev = node;
    node->next = l2->front;
    if ( start == 0 )
        l1->front = l2->front;
    return CS_SUCCESS;
}

void* list_front ( list l ) {
    if ( l.size == 0 ) return NULL;
    return l.front->data;
}

void* list_back ( list l ) {
    if ( l.size == 0 ) return NULL;
    return l.front->next->data;
}

void list_sort_helper ( list_attr_t attr, list_node *start, list_node *end ) {
    if ( start == end ) return;
    void *pivot = end->data;
    list_node *pivot_node = start;

    for ( list_node *node = start; node != end; node = node->next ) {
        if ( attr.comp ( node->data, pivot ) <= 0 ) {
            universal_swap ( pivot_node->data, node->data, attr.size );
            pivot_node = pivot_node->next;
        }
    }

    universal_swap ( pivot_node->data, pivot, attr.size );

    if ( pivot_node != start )
        list_sort_helper ( attr, start, pivot_node->prev );
    if ( pivot_node != end )
        list_sort_helper ( attr, pivot_node->next, end );
}

cs_codes list_sort ( list *l ) {
    if ( !l->attr.comp ) return CS_FUNC;
    list_sort_helper ( l->attr, l->front, l->front->prev );
    return CS_SUCCESS;
}

void list_set_free ( list *l, freer fr ) { l->attr.fr = fr; }

void list_set_print ( list *l, printer print ) { l->attr.print = print; }

void list_set_comp ( list *l, comparer comp ) { l->attr.comp = comp; }

void list_reverse ( list *l ) {
    l->front = l->front->prev;
    list_node *node = l->front->next;
    while ( node != l->front ) {
        list_node *next = node->next;
        node->next = node->prev;
        node->prev = next;
        node = next;
    }
    list_node *next = l->front->next;
    l->front->next = l->front->prev;
    l->front->prev = next;
}

void list_swap ( list *l1, list *l2 ) {
    list_attr_t attr = l1->attr;
    list_node *front = l1->front;
    int size = l1->size;

    l1->attr = l2->attr;
    l1->front = l2->front;
    l1->size = l2->size;

    l2->attr = attr;
    l2->front = front;
    l2->size = size;
}

void list_clear ( list *l ) {
    list_free ( l );
    l->front = NULL;
    l->size = 0;
}

void list_free ( list *l ) {
    if ( l->size == 0 ) return;
    list_node *node = l->front->next;
    while ( node != l->front ) {
        list_node *aux = node;
        node = node->next;
        if ( l->attr.fr ) l->attr.fr ( aux->data );
        free ( aux->data );
        free ( aux );
    }
    if ( l->attr.fr ) l->attr.fr ( l->front->data );
    free ( l->front->data );
    free ( l->front );
}

void list_print ( list l ) {
    if ( !l.attr.print || l.size == 0 ) return;
    l.attr.print ( l.front->data );
    list_node *node = l.front->next;
    while ( node != l.front ) {
        l.attr.print ( node->data );
        node = node->next;
    }
}