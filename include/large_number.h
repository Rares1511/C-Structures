#ifndef LARGE_NUMBER_H
#define LARGE_NUMBER_H

#include "universal.h"

#define _LN_INIT_CAPACITY 100
#define _LN_INIT_BASE 10

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

typedef enum ln_conversion_type
{
    LN_CONV_CHAR,
    LN_CONV_LONG,
    LN_CONV_DOUBLE
} ln_conversion_type;

typedef enum ln_div_type
{
    LN_QUOTIENT = 1,
    LN_REST = 2
} ln_div_type;

typedef struct large_number
{
    char sign;                 /*!< tells if the number is positive or negative */
    int size;                  /*!< current size of the number */
    int cap;                   /*!< current maximum capacity of the number */
    int exponent;              /*!< exponent for determing the */
    int base;                  /*!< base that the large number is saved in */
    long *vec;                 /*!< vector holding the actual number */
    struct large_number *aux1; /*!< auxiliar large number */
    struct large_number *aux2; /*!< auxiliar large number */
    struct large_number *aux3; /*!< auxiliar large number structure for saving the result of division*/
} large_number;

///////////////////// FUNCTIONS /////////////////////

/*!
 * Initializes the number by the type given
 * @param[in] ln    The large number structure which will be initialised
 * @param[in] type  The type given for the initialization which can be LN_NULL, LN_NUM, LN_CHAR or LN_SCALE
 * @param[in] ...   This represents the value which will be used for initialisation
 * LN_NUM   -> another large number structure whose value will be given to the number of the function
 * LN_SCALE -> a double variable
 * LN_CHAR  -> a char array containing numbers which will be made into a large number structure
 * LN_NULL  -> no extra parameter needed and the large number will have null value
 * @return CS_SUCCESS for a successful initialisation or CS_MEM for a problem in allocating memory, or CS_ELEM
 * for a wrong type
 */
cs_codes large_number_init(large_number *ln, ln_type type, ...);

/*!
 * Assigns the value of the given parameter depending on the type to the large number structure
 * @param[in] ln    The large number structure which will be given a value
 * @param[in] type  The type given for the assignation which can be LN_NULL, LN_NUM, LN_CHAR or LN_SCALE
 * @param[in] ...   This represents the value which will be used for assign
 * LN_NUM   -> another large number structure whose value will be given to the number of the function
 * LN_SCALE -> a double variable
 * LN_CHAR  -> a char array containing numbers which will be made into a large number structure
 * LN_NULL  -> no extra parameter needed and the large number will have null value
 * @return CS_SUCCESS for a successful assignation or CS_MEM for a problem in allocating memory, or CS_ELEM
 * for a wrong type
 */
cs_codes large_number_assign(large_number *ln, ln_type type, ...);

/*!
 * Appends the given value to the end of the large number structure
 * @param[in] ln    The large number structure which will have the value appended
 * @param[in] type  The type given for the appended value which can be LN_NULL, LN_NUM, LN_CHAR or LN_SCALE
 * @param[in] ...   This represents the value which will be appended to the end of the number
 * LN_NUM   -> another large number structure whose value will be given to the number of the function
 * LN_SCALE -> a double variable
 * LN_CHAR  -> a char array containing numbers which will be made into a large number structure
 * LN_NULL  -> no extra parameter needed and the large number will have null value
 * @return CS_SUCCESS for a successful assignation or CS_MEM for a problem in allocating memory, or CS_ELEM
 * for a wrong type
 */
cs_codes large_number_append(large_number *ln, ln_type type, ...);

/*!
 * Add the two numbers and saves the result in the destination large number
 * @param[out] dest  The destination for the result of the addition
 * @param[in]  ln1   The first number in the adition
 * @param[in]  ln2   The second number in the addition
 * @return CS_SUCCESS for a successful addition or CS_MEM for a problem in allocating memory
 */
cs_codes large_number_add(large_number *dest, large_number ln1, large_number ln2);

/*!
 * Subtracts the second number from the first number and saves the result in the destination large number
 * @param[out] dest  The destination for the result of the subtraction
 * @param[in]  ln1   The first number in the subtraction
 * @param[in]  ln2   The second number in the subtraction
 * @return CS_SUCCESS for a successful subtraction or CS_MEM for a problem in allocating memory
 */
cs_codes large_number_minus(large_number *dest, large_number ln1, large_number ln2);

/*!
 * Multiplies the two numbers and saves the result in the destination large number
 * @param[out] dest  The destination for the result of the multiplication
 * @param[in]  ln1   The first number in the multiplication
 * @param[in]  ln2   The second number in the multiplication
 * @return CS_SUCCESS for a successful multiplication or CS_MEM for a problem in allocating memory
 */
cs_codes large_number_mul(large_number *dest, large_number ln1, large_number ln2);

/*!
 * Divides the first number by the second number and saves the result in the destination large number
 * @param[out] dest  The destination for the result of the division
 * @param[in]  ln1   The first number in the division
 * @param[in]  ln2   The second number in the division
 * @return CS_SUCCESS for a successful division or CS_MEM for a problem in allocating memory
 */
cs_codes large_number_div_num(large_number ln1, large_number ln2, int accuracy, ln_div_type type, ...);

cs_codes large_number_div_scale(large_number ln, long scale, int accuracy, ln_div_type, ...);

/*!
 * Changes the base in which the number is stored and transforms it
 * @param[in] ln   The number that will change base
 * @param[in] base New base for the number
 * @return CS_SUCCESS for a successful transformation or CS_MEM for a problem in allocating memory
 */
cs_codes large_number_set_base(large_number *ln, int base);

/*!
 * Compares the two numbers and return the result depending on their sizes and digits
 * @param[in] ln1  The first number in the comparation
 * @param[in] ln2  The second number in the comparation
 * @return 0 if the two numbers are identical, > 0 if ln1 is bigger or < 0 otherwise
 */
int large_number_compare(large_number ln1, large_number ln2);

/*!
 * Negates the given number
 * @param[out] ln1 The number that will be negated
 */
void large_number_negative(large_number *ln1);

/*!
 * Swaps the two given numbers
 * @param[in] ln1  The first number in the swap
 * @param[in] ln2  The second number in the swap
 */
void large_number_swap(large_number *ln1, large_number *ln2);

/*!
 * Frees the given large number structure memory
 * @param[in] ln The number whose memory will be freed
 */
void large_number_free(large_number *ln);

/*!
 * Prints the number in a human readable format
 * @param[in] ln The number that will be printed
 */
void large_number_print(large_number ln);

#endif