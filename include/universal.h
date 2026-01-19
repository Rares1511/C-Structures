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

typedef void (*printer)(FILE *, const void *);
typedef void (*freer)(void *);
typedef int (*comparer)(const void *, const void *);
typedef void (*deepcopy)(void *, const void *);

typedef struct univ_attr_t {
    int size;      /*!< size of the datatype */
    freer fr;      /*!< freer function for the datatype */
    deepcopy copy; /*!< function to deepcopy data if a separate function is needed */
    printer print; /*!< printer function for the datatype */
    comparer comp; /*!< compare function for the datatype */
} univ_attr_t;

/*!
 * A simple universal hash function for byte arrays.
 * @param[in] data Pointer to the data to hash.
 * @param[in] size Size of the data in bytes.
 * @return A size_t hash value.
 */
static size_t universal_hash_bytes(const void *data, size_t size) {
    const unsigned char *bytes = (const unsigned char *)data;
    size_t hash = 1469598103934665603ULL;  // FNV offset basis (64-bit)

    for (size_t i = 0; i < size; ++i) {
        hash ^= (size_t)bytes[i];
        hash *= 1099511628211ULL;          // FNV prime (64-bit)
    }

    return hash;
}

#endif