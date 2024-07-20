#ifndef LARGE_NUMBER_H
#define LARGE_NUMBER_H

#include "universal.h"

#define LARGE_NUMBER_INIT_CAPACITY 100

///////////////////// DATA STRUCTURES /////////////////////

typedef enum ln_type
{
    LN_SCALE,
    LN_NUM,
    LN_CHAR,
    LN_NULL
} ln_type;

typedef struct large_number
{
    int size;                 /*!< current size of the number*/
    int cap;                  /*!< current maximum capacity of the number */
    long *vec;                /*!< vector holding the actual number */
    struct large_number *aux; /*!< auxiliar large number for computing multiplication */
} large_number;

///////////////////// FUNCTIONS /////////////////////

cs_codes large_number_init(large_number *ln, ln_type type, ...);
cs_codes large_number_assign(large_number *ln, ln_type type, ...);
cs_codes large_number_add(large_number *dest, int ln_count, ...);
cs_codes large_number_mul(large_number *dest, int ln_count, ...);
void large_number_swap(large_number *ln1, large_number *ln2);
void large_number_free(large_number *ln);
void large_number_print(large_number ln);

#endif