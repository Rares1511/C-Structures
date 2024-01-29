#pragma once

#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_CAPACITY 1024

typedef enum cs_codes {
    EMPTY, WRONG_POSITION, SUCCESSFUL_ADDITION, SUCCESSFUL_DELETION, SUCCESSFUL_REPLACEMENT,
    SUCCESSFUL_MULTIPLICATION,
    UNMATCHING_DATA_TYPES, MEMORY_PROBLEM, WRONG_ELEMENT,
    CS_MEM, CS_SUCCESS, CS_SIZE
} cs_codes;

typedef void (*printer) ( void* );
typedef void (*freer) ( void* );
typedef int (*comparer) ( const void*, const void* );

typedef struct univ_attr_t {
    size_t size;
    freer fr;
    printer print;
} univ_attr_t;

void universal_swap ( void *el1, void *el2, size_t len );
int universal_compare ( const void *el1, const void *el2, size_t dim );

#endif