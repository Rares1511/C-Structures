#ifndef LIST_H
#define LIST_H

#include "..\universal\universal.h"

typedef struct list_type {
    void *data;
    size_t dim;
    struct list_type *next, *prev;
} list, *AL;

#define LIST_SIZE sizeof ( list )

list *list_initiate ( size_t dim );
enum return_codes list_push_front ( AL *l, void *el );
enum return_codes list_push_back ( AL *l, void *el );
enum return_codes list_pop_front ( AL *l );
enum return_codes list_pop_back ( AL *l );
void *list_front ( list *l );
void *list_back ( list *l );
void list_print ( list *l, printer print );
void list_free ( list *l );

#endif