#pragma once
#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include <stdio.h>

#define INIT_CAPACITY 256

#define SIZE_TH 1000

#define NULL ((void *)0)

typedef enum cs_codes {
    CS_SUCCESS = 0,
    CS_MEM = -1,
    CS_SIZE = -2,
    CS_POS = -3,
    CS_EMPTY = -4,
    CS_FUNC = -5,
    CS_ELEM = -6,
    CS_COMP = -7,
    CS_UNKNOWN = -8,
    CS_FILE = -9
} cs_codes;

typedef void (*printer)(FILE *stream, void *);
typedef void (*freer)(void *);
typedef int (*comparer)(const void *, const void *);
typedef void (*deepcopy)(void *, const void *);

typedef struct univ_attr_t {
    int size;      /*!< size of the datatype */
    freer fr;      /*!< freer function for the datatype */
    deepcopy copy; /*!< function to deepcopy data if a separate function is needed */
    printer print; /*!< printer function for the datatype */
    FILE *stream;  /*!< output stream for the printer function */
} univ_attr_t;

typedef struct xuniv_attr_t {
    int size;      /*!< size of the datatype */
    freer fr;      /*!< freer function for the datatype */
    printer print; /*!< printer function for the datatype */
    deepcopy copy; /*!< function to deepcopy data if a separate function is needed */
    comparer comp; /*!< compare function for the datatype */
    FILE *stream;  /*!< output stream for the printer function */
} xuniv_attr_t;

/*!
 * Swaps two memory zones considering they are both of size len
 * @param[in] el1 - starting point of the first memory zone
 * @param[in] el2 - starting point of the second memory zone
 * @param[in] len - the length in bytes of the memeory zone that will be swapped
 */
void universal_swap(void *el1, void *el2, int len) {
    unsigned char *p1 = (unsigned char *)el1;
    unsigned char *p2 = (unsigned char *)el2;
    unsigned char tmp;

    for (int i = 0; i < len; i++) {
        tmp = p1[i];
        p1[i] = p2[i];
        p2[i] = tmp;
    }
}

#endif