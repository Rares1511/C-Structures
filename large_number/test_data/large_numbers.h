/**
 * @file large_numbers.h
 * @brief Test data for large_number unit tests
 * 
 * Contains string constants representing very large numbers (beyond long range)
 * Maximum long: 9,223,372,036,854,775,807 (about 19 whole_digits)
 * We test with numbers having 50+ whole_digits to ensure we're beyond any native type
 */

#ifndef __LARGE_NUMBERS_TEST_DATA_H__
#define __LARGE_NUMBERS_TEST_DATA_H__

// ----------------------------------------------------------------------------
// Small numbers (for basic sanity checks)
// ----------------------------------------------------------------------------

// 12345 + 67890 = 80235
#define SMALL_NUM_1_STR "12345"
#define SMALL_NUM_2_STR "67890"
#define SMALL_SUM_1_2_STR "80235"

// ----------------------------------------------------------------------------
// Medium numbers (around long boundary)
// ----------------------------------------------------------------------------

// LONG_MAX + LONG_MAX = 18,446,744,073,709,551,614
#define LONG_MAX_STR "9223372036854775807"
#define LONG_MAX_2_STR "9223372036854775807"
#define LONG_MAX_DOUBLED_SIZE 20
#define LONG_MAX_DOUBLED_STR "18446744073709551614"

// ----------------------------------------------------------------------------
// Large numbers (50 whole_digits - far beyond any native type)
// ----------------------------------------------------------------------------

// A + B = 111111111011111111101111111110111111111011111111100
#define LARGE_NUM_A_SIZE 50
#define LARGE_NUM_A_STR "12345678901234567890123456789012345678901234567890"
#define LARGE_NUM_B_STR "98765432109876543210987654321098765432109876543210"
#define LARGE_SUM_A_B_SIZE 51
#define LARGE_SUM_A_B_STR "111111111011111111101111111110111111111011111111100"

// ----------------------------------------------------------------------------
// Very large numbers (100 whole_digits)
// ----------------------------------------------------------------------------

// 100 nines + 1 = 1 followed by 100 zeros
#define VERY_LARGE_ALL_NINES_SIZE 100
#define VERY_LARGE_ALL_NINES_STR "9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
#define ONE_DIGIT_STR "1"
#define VERY_LARGE_PLUS_ONE_SIZE 101
#define VERY_LARGE_PLUS_ONE_STR "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"

// ----------------------------------------------------------------------------
// Power of 10 numbers
// ----------------------------------------------------------------------------

// (10^30 - 1) + 1 = 10^30
#define POWER_10_30_SIZE 31
#define POWER_10_30_STR "1000000000000000000000000000000"
#define POWER_10_30_MINUS_1_STR "999999999999999999999999999999"

// ----------------------------------------------------------------------------
// Fibonacci large numbers
// ----------------------------------------------------------------------------

// F(200) + F(201) = F(202)
#define FIB_200_STR "280571172992510140037611932413038677189525"
#define FIB_201_STR "453973694165307953197296969697410619233826"
#define FIB_202_SIZE 42
#define FIB_202_STR "734544867157818093234908902110449296423351"

// ----------------------------------------------------------------------------
// Alternating digit patterns
// ----------------------------------------------------------------------------

// 101010...10 + 909090...90 = 1010101010101010101010101010100
#define ALTERNATING_10_STR "101010101010101010101010101010"
#define ALTERNATING_90_STR "909090909090909090909090909090"
#define ALTERNATING_SUM_SIZE 31
#define ALTERNATING_SUM_STR "1010101010101010101010101010100"

// ----------------------------------------------------------------------------
// Multiplication test data
// ----------------------------------------------------------------------------

// Basic: 12345 * 67890 = 838102050
#define MUL_SMALL_1_STR "12345"
#define MUL_SMALL_2_STR "67890"
#define MUL_SMALL_RESULT_STR "838102050"

// 999 * 999 = 998001
#define MUL_999_STR "999"
#define MUL_999_RESULT_STR "998001"

// 10^25 * 10^25 = 10^50
#define MUL_POWER_10_25_STR "10000000000000000000000000"
#define MUL_POWER_10_50_SIZE 51
#define MUL_POWER_10_50_STR "100000000000000000000000000000000000000000000000000"

// Large: A * B (50 digit numbers)
// 12345678901234567890123456789012345678901234567890 * 2 = 24691357802469135780246913578024691357802469135780
#define MUL_LARGE_A_DOUBLED_SIZE 50
#define MUL_LARGE_A_DOUBLED_STR "24691357802469135780246913578024691357802469135780"

// Factorial-style: 100! has 158 whole_digits, too complex, but we can test smaller
// 20! = 2432902008176640000
#define FACTORIAL_20_STR "2432902008176640000"

// Square of large number: (10^20)^2 = 10^40
#define MUL_SQUARE_10_20_STR "100000000000000000000"
#define MUL_SQUARE_10_40_SIZE 41
#define MUL_SQUARE_10_40_STR "10000000000000000000000000000000000000000"

// Multiplication by power of 10: 12345 * 10000 = 123450000
#define MUL_BY_10000_STR "10000"
#define MUL_12345_BY_10000_STR "123450000"

#endif // __LARGE_NUMBERS_TEST_DATA_H__
