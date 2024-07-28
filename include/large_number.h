#ifndef LARGE_NUMBER_H
#define LARGE_NUMBER_H

#include "universal.h"

#define LARGE_NUMBER_INIT_CAPACITY 100

#define POSITIVE 1
#define NEGATIVE -1

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
    char sign;                     /*!< tells if the number is positive or negative */
    int size;                      /*!< current size of the number */
    int cap;                       /*!< current maximum capacity of the number */
    int exponent;                  /*!< exponent for determing the  */
    long *vec;                     /*!< vector holding the actual number */
    struct large_number *aux1;     /*!< auxiliar large number for computing multiplication and division */
    struct large_number *aux2;     /*!< auxiliar large number for computing division */
    struct large_number *quotient; /*!< auxiliar large number structure for saving the result of division*/
} large_number;

///////////////////// FUNCTIONS /////////////////////

cs_codes large_number_init(large_number *ln, ln_type type, ...);
cs_codes large_number_assign(large_number *ln, ln_type type, ...);
cs_codes large_number_append(large_number *ln, ln_type type, ...);
cs_codes large_number_add(large_number *dest, large_number ln1, large_number ln2);
cs_codes large_number_minus(large_number *dest, large_number ln1, large_number ln2);
cs_codes large_number_mul(large_number *dest, large_number ln1, large_number ln2);
cs_codes large_number_div(large_number *dest, large_number ln1, large_number ln2, int accuracy);
int large_number_compare(large_number ln1, large_number ln2);
void large_number_negative(large_number *ln1);
void large_number_swap(large_number *ln1, large_number *ln2);
void large_number_free(large_number *ln);
void large_number_print(large_number ln);

#endif