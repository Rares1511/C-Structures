#include "list.h"

/* initializes the list with the elements having a sizeof dim */
list *list_initiate ( size_t dim ) {
    list *l = malloc ( LIST_SIZE );
    if ( !l ) return NULL;
    l->next = l;
    l->prev = l;
    l->data = NULL;
    l->dim = dim;
    l->size = 0;
    return l;
}

void list_node_free ( list *l ) {
    free ( l->data );
    free ( l );
}

/* frees the memory that the list uses */
void list_free ( list *l ) {
    list *aux = l->next, *freer;
    while ( aux != l ) {
        freer = aux;
        aux = aux->next;
        list_node_free ( freer );
    }
    list_node_free ( l );
}

void list_import_data ( list *l, void *el ) {
    l->data = malloc ( l->dim );
    memcpy ( l->data, el, l->dim );
}

/* pushes the element at the front of the list */
enum return_codes list_push_front ( AL *l, void *el ) {
    if ( list_empty ( (*l) ) ) {
        list_import_data ( *l, el );
        (*l)->size++;
        return SUCCESSFUL_ADDITION;
    }
    list *aux = list_initiate ( (*l)->dim );
    if ( !aux ) return MEMORY_PROBLEM;
    list_import_data ( aux, el );
    aux->next = (*l);
    aux->prev = (*l)->prev;
    (*l)->prev->next = aux;
    (*l)->prev = aux;
    aux->size = (*l)->size;
    (*l) = aux;
    (*l)->size++;
    return SUCCESSFUL_ADDITION;
}

/* pushes the element at the back of the list */
enum return_codes list_push_back ( AL *l, void *el ) {
    if ( list_empty ( (*l) ) ) {
        list_import_data ( *l, el );
        (*l)->size++;
        return SUCCESSFUL_ADDITION;
    }
    list *aux = list_initiate ( (*l)->dim );
    if ( !aux ) return MEMORY_PROBLEM;
    list_import_data ( aux, el );
    (*l)->prev->next = aux;
    (*l)->prev = aux;
    aux->next = (*l);
    aux->prev = (*l)->prev;
    (*l)->size++;
    return SUCCESSFUL_ADDITION;
}

/* pops the element at the front of the list */
enum return_codes list_pop_front ( AL *l ) {
    if ( list_empty ( (*l) ) ) return EMPTY;
    list *aux = *l;
    (*l)->next->prev = (*l)->prev;
    (*l)->prev->next = (*l)->next;
    (*l) = (*l)->next;
    list_node_free ( aux );
    (*l)->size--;
    return SUCCESSFUL_DELETION;
}

/* pops the element at the back of the list */
enum return_codes list_pop_back ( AL *l ) {
    if ( list_empty ( (*l) ) ) return EMPTY;
    list *aux = (*l)->prev;
    (*l)->prev = (*l)->prev->prev;
    (*l)->prev->next = (*l);
    list_node_free ( aux );
    (*l)->size--;
    return SUCCESSFUL_DELETION;
}

/* returns the element at the front of the list */
void *list_front ( list *l ) {
    if ( list_empty ( l ) ) return NULL;
    void *ptr = malloc ( l->dim );
    memcpy ( ptr, l->data, l->dim );
    return ptr;
}

/* returns the element at the back of the list */
void *list_back ( list *l ) {
    if ( list_empty ( l ) ) return NULL;
    void *ptr = malloc ( l->dim );
    memcpy ( ptr, l->prev->data, l->dim );
    return ptr;
}

/* prints the list with the printer function given */
void list_print ( list *l, printer print ) {
    if ( list_empty ( l ) ) return;
    list *aux = l->next;
    print ( l->data );
    while ( aux != l ) {
        print ( aux->data );
        aux = aux->next;
    }
}