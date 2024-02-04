#include "../include/heap.h"

#include <string.h>
#include <stdlib.h>

int heap_compare ( heap h, void *el1, void *el2 ) {
    if ( h.attr.comp ) {
        return h.attr.comp ( el1, el2 );
    }
    return universal_compare ( el1, el2, h.attr.size );
}

cs_codes heap_init ( heap *h, heap_attr_t attr ) {
    if ( attr.size < 0 || attr.size > SIZE_TH ) return CS_SIZE;
    h->attr = attr;
    h->cap = INIT_CAPACITY;
    h->size = 0;
    h->vec = malloc ( h->attr.size * h->cap );
    if ( !h->vec ) return CS_MEM;
    return CS_SUCCESS;
}

cs_codes heap_push ( heap *h, void *el ) {
    if ( h->size == h->cap ) {
        h->vec = realloc ( h->vec, ( h->cap + INIT_CAPACITY ) * h->attr.size );
        if ( !h->vec ) return CS_MEM;
        h->cap += INIT_CAPACITY;
    }
    memcpy ( h->vec + h->size * h->attr.size, el, h->attr.size );
    h->size++;
    int pos = h->size;
    while ( pos > 1 && heap_compare ( *h, h->vec + h->attr.size * ( pos / 2 - 1 ), h->vec + h->attr.size * ( pos - 1 ) ) < 0 ) {
        universal_swap ( h->vec + h->attr.size * ( pos / 2 - 1 ), h->vec + h->attr.size * ( pos - 1 ), h->attr.size );
        pos /= 2;
    }
    return CS_SUCCESS;
}

cs_codes heap_pop ( heap *h ) {
    if ( h->size == 0 ) return CS_EMPTY;
    h->size--;
    if ( h->size != 0 )
        universal_swap ( h->vec, h->vec + h->size * h->attr.size, h->attr.size );
    if ( h->attr.fr ) h->attr.fr ( h->vec + h->attr.size * h->size );
    if ( h->size == 0 ) return CS_SUCCESS;
    int pos = 1;
    while ( 2 * pos < h->size + 1 ) {
        int next_pos = 2 * pos;
        void *comp_ptr = comp_ptr = h->vec + h->attr.size * ( 2 * pos - 1 );;
        if ( 2 * pos < h->size && heap_compare ( *h, h->vec + h->attr.size * ( 2 * pos - 1 ), h->vec + h->attr.size * ( 2 * pos ) ) < 0 ) {
            comp_ptr = h->vec + h->attr.size * ( 2 * pos );
            next_pos++;
        }
        if ( heap_compare ( *h, h->vec + h->attr.size * ( pos - 1 ), comp_ptr ) < 0 ) {
            universal_swap ( h->vec + h->attr.size * ( pos - 1 ), comp_ptr, h->attr.size );
            pos = next_pos;
        }
        else break;
    }
    return CS_SUCCESS;
}

int heap_empty ( heap h ) { if ( h.size == 0 ) return 1; return 0; }

void *heap_top ( heap h ) {
    if ( h.size == 0 ) return NULL;
    return h.vec;
}

void heap_set_free ( heap *h, freer fr ) { h->attr.fr = fr; }

void heap_set_print ( heap *h, printer print ) { h->attr.print = print; }

void heap_set_comp ( heap *h, comparer comp ) { 
    h->attr.comp = comp;
    heap haux;
    int rc = heap_init ( &haux, h->attr );
    if ( rc == CS_SIZE ) return;

    while ( h->size > 0 ) {
        void *top = heap_top ( *h );
        heap_pop ( h );
        heap_push ( &haux, top );
    }

    heap_swap ( h, &haux );
    free ( haux.vec );
}

void heap_swap ( heap *h1, heap *h2 ) { 
    heap_attr_t attr = h1->attr;
    int cap = h1->cap;
    int size = h1->size;
    void *vec = h1->vec;

    h1->attr = h2->attr;
    h1->cap = h2->cap;
    h1->size = h2->size;
    h1->vec = h2->vec;

    h2->attr = attr;
    h2->cap = cap;
    h2->size = size;
    h2->vec = vec;
}

void heap_clear ( heap *h ) {
    if ( h->attr.fr )
        for ( int i = 0; i < h->size; i++ )
            h->attr.fr ( h->vec + i * h->attr.size );
    h->size = 0;
}

void heap_free ( heap *h ) {
    if ( h->attr.fr )
        for ( int i = 0; i < h->size; i++ )
            h->attr.fr ( h->vec + i * h->attr.size );
    free ( h->vec );
}

void heap_print ( heap h ) {
    for ( int i = 0; i < h.size; i++ ) 
        h.attr.print ( h.vec + h.attr.size * i );
}