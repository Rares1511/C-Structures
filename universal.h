#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_CAPACITY 1024

enum return_codes {
    EMPTY, WRONG_POSITION, SUCCESSFUL_ADDITION, SUCCESSFUL_DELETION, SUCCESSFUL_REPLACEMENT,
    UNMATCHING_DATA_TYPES, MEMORY_PROBLEM, WRONG_ELEMENT
};

typedef void (*printer) ( void* );
typedef int (*comparer) ( const void*, const void* );

void universal_swap ( void *el1, void *el2, size_t len );
int universal_compare ( void *el1, void *el2, size_t dim );

#endif