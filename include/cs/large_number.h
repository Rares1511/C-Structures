#ifndef __CS_LARGE_NUMBER_H__
#define __CS_LARGE_NUMBER_H__

#include <cs/universal.h>
#include <stdarg.h>

#define __POSITIVE_SIGN 1
#define __NEGATIVE_SIGN -1

typedef enum large_number_type {
    LN_CHAR,
    LN_INT,
    LN_NUM,
    LN_EMPTY,
} large_number_type;

typedef struct large_number {
    unsigned int *digits;
    int size;
    int capacity;
    int sign; // 1 for positive, -1 for negative
    unsigned int base;
} large_number;

/*!
 * @brief Initializes a large_number structure
 * @param[in,out] ln Pointer to large_number to initialize
 * @param[in] base Base of the number system (e.g., 10 for decimal)
 * @param[in] type Type of the input value (LN_CHAR, LN_INT, LN_NUM)
 * @param[in] ... Variable arguments based on type:
 *                 - LN_CHAR: char* (string of digits)
 *                 - LN_INT: long (integer value)
 *                 - LN_NUM: large_number (another large_number to copy)
 * @return CS_SUCCESS on success, error code otherwise
 */
cs_codes large_number_init(large_number *ln, unsigned int base, large_number_type type, ...);

/*!
 * @brief Adds two large_number values and stores the result in out
 * @param[out] out Pointer to large_number to store the result
 * @param[in] a First large_number operand
 * @param[in] b Second large_number operand
 * @return CS_SUCCESS on success, error code otherwise
 */
cs_codes large_number_add(large_number *out, const large_number a, const large_number b);

/*!
 * @brief Subtracts two large_number values and stores the result in out
 * @param[out] out Pointer to large_number to store the result
 * @param[in] a First large_number operand
 * @param[in] b Second large_number operand
 * @return CS_SUCCESS on success, error code otherwise
 */
cs_codes large_number_sub(large_number *out, const large_number a, const large_number b);

/*!
 * @brief Converts a large_number to a different base
 * @param[in,out] ln Pointer to large_number to convert
 * @param[in] new_base The target base to convert to (must be >= 2)
 * @return CS_SUCCESS on success, CS_NULL if ln is NULL, CS_SIZE if base < 2
 */
cs_codes large_number_switch_base(large_number *ln, unsigned int new_base);

/*!
 * @brief Compares two large_number values
 * @param[in] a First large_number to compare
 * @param[in] b Second large_number to compare
 * @return Positive if a > b, negative if a < b, zero if equal
 */
int large_number_comp(const large_number a, const large_number b);

/*!
 * @brief Checks if two large_number values are equal
 * @param[in] a First large_number to compare
 * @param[in] b Second large_number to compare
 * @return 1 if equal, 0 otherwise
 */
int large_number_equal(const large_number a, const large_number b);

/*!
 * @brief Clears the contents of a large_number, resetting its size and sign
 * @param[in,out] ln Pointer to large_number to clear
 */
void large_number_clear(large_number *ln);

/*!
 * @brief Prints the large_number to the specified output stream
 * @param[in] stream Output stream (e.g., stdout)
 * @param[in] v_ln Pointer to large_number to print
 */
void large_number_print(FILE *stream, void *v_ln);

/*!
 * @brief Swaps the contents of two large_number structures
 * @param[in,out] a Pointer to the first large_number
 * @param[in,out] b Pointer to the second large_number
 */
void large_number_swap(large_number *a, large_number *b);

/*!
 * @brief Frees the resources allocated for a large_number
 * @param[in,out] v_ln Pointer to large_number to free
 */
void large_number_free(void *v_ln);

#endif