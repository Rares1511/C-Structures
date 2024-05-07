#pragma once

#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#define INIT_CAPACITY 1024

#define SIZE_TH 1000

typedef enum cs_codes
{
    CS_MEM = -1,
    CS_SUCCESS = -2,
    CS_SIZE = -3,
    CS_POS = -4,
    CS_EMPTY = -5,
    CS_FUNC = -6,
    CS_ELEM = -7,
    CS_COMP = -8
} cs_codes;

typedef void (*printer)(void *);
typedef void (*freer)(void *);
typedef int (*comparer)(const void *, const void *);
typedef void (*deepcopy)(void *, const void *);

typedef struct univ_attr_t
{
    int size;      /*!< size of the datatype */
    freer fr;      /*!< freer function for the datatype */
    deepcopy cp;   /*!< function to deepcopy data if a separate function is needed */
    printer print; /*!< printer function for the datatype */
} univ_attr_t;

typedef struct xuniv_attr_t
{
    int size;      /*!< size of the datatype */
    freer fr;      /*!< freer function for the datatype */
    printer print; /*!< printer function for the datatype */
    deepcopy cp;   /*!< function to deepcopy data if a separate function is needed */
    comparer comp; /*!< compare function for the datatype */
} xuniv_attr_t;

/*!
 * Swaps two memory zones considering they are both of size len
 * @param[in] el1 - starting point of the first memory zone
 * @param[in] el2 - starting point of the second memory zone
 * @param[in] len - the length in bytes of the memeory zone that will be swapped
 */
void universal_swap(void *el1, void *el2, int len);

/*!
 * Compares two memory zones bytes by bytes considering they are both of size len
 * @param[in] el1 - starting point of the second memory zone
 * @param[in] el2 - starting point of the first memory zone
 * @param[in] len - length of the memory zones
 * @return The difference between the first two different bytes or 0 otherwise
 */
int universal_compare(const void *el1, const void *el2, int dim);

#endif