#include "list.h"

/* initializes the list with the elements having a sizeof dim */
list *list_initiate ( size_t dim ) {
    list *l = malloc ( LIST_SIZE );
    if ( !l ) return NULL;
    l->next = l;
    l->prev = l;
    l->data = NULL;
    l->dim = dim;
    return l;
}

/* frees the memory that the list uses */
void list_free ( list *l ) {
    list *aux = l->next, *freer;
    while ( aux != l ) {
        freer = aux;
        aux = aux->next;
        free ( freer->data );
        free ( freer );
    }
    free ( l->data );
    free ( l );
}

void list_import_data ( list *l, void *el ) {
    l->data = malloc ( l->dim );
    memcpy ( l->data, el, l->dim );
}

/* pushes the element at the front of the list */
enum return_codes list_push_front ( AL *l, void *el ) {
    if ( (*l)->data == NULL ) {
        list_import_data ( *l, el );
        return SUCCESSFUL_ADDITION;
    }
    list *aux = list_initiate ( (*l)->dim );
    if ( !aux ) return MEMORY_PROBLEM;
    list_import_data ( aux, el );
    aux->next = (*l);
    aux->prev = (*l)->prev;
    (*l)->prev->next = aux;
    (*l)->prev = aux;
    (*l) = aux;
    return SUCCESSFUL_ADDITION;
}

/* pushes the element at the back of the list */
enum return_codes list_push_back ( AL *l, void *el ) {
    if ( (*l)->data == NULL ) {
        list_import_data ( *l, el );
        return SUCCESSFUL_ADDITION;
    }
    list *aux = list_initiate ( (*l)->dim );
    if ( !aux ) return MEMORY_PROBLEM;
    list_import_data ( aux, el );
    (*l)->prev->next = aux;
    (*l)->prev = aux;
    aux->next = (*l);
    aux->prev = (*l)->prev;
    return SUCCESSFUL_ADDITION;
}

/* pops the element at the front of the list */
enum return_codes list_pop_front ( AL *l ) {
    if ( (*l)->data == NULL ) return EMPTY;
    list *aux = *l;
    (*l)->next->prev = (*l)->prev;
    (*l)->prev->next = (*l)->next;
    (*l) = (*l)->next;
    aux->next = aux;
    aux->prev = aux;
    list_free ( aux );
    return SUCCESSFUL_DELETION;
}

/* pops the element at the back of the list */
enum return_codes list_pop_back ( AL *l ) {
    if ( (*l)->data == NULL ) return EMPTY;
    list *aux = (*l)->prev;
    (*l)->prev = (*l)->prev->prev;
    (*l)->prev->next = (*l);
    aux->next = aux;
    aux->prev = aux;
    list_free ( aux );
    return SUCCESSFUL_DELETION;
}

/* returns the element at the front of the list */
void *list_front ( list *l ) {
    return l->data;
}

/* returns the element at the back of the list */
void *list_back ( list *l ) {
    return l->prev->data;
}

/* prints the list with the printer function given */
void list_print ( list *l, printer print ) {
    if ( l->data == NULL ) {
        printf ( "\n" );
        return;
    }
    list *aux = l->next;
    print ( l->data );
    while ( aux != l ) {
        print ( aux->data );
        aux = aux->next;
    }
    printf ( "\n" );
}