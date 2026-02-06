/**
 * @file large_numbers.h
 * @brief Test data for large_number unit tests
 * 
 * Contains very large numbers (beyond long range) as digit arrays for testing
 * Maximum long: 9,223,372,036,854,775,807 (about 19 digits)
 * We test with numbers having 50+ digits to ensure we're beyond any native type
 */

#ifndef __LARGE_NUMBERS_TEST_DATA_H__
#define __LARGE_NUMBERS_TEST_DATA_H__

// ============================================================================
// Helper macro to define test number with expected result
// Digits are stored in REVERSE order (least significant first)
// ============================================================================

// ----------------------------------------------------------------------------
// Small numbers (for basic sanity checks)
// ----------------------------------------------------------------------------

// Number: 12345
static unsigned int small_num_1[] = {5, 4, 3, 2, 1};
#define SMALL_NUM_1_SIZE 5

// Number: 67890
static unsigned int small_num_2[] = {0, 9, 8, 7, 6};
#define SMALL_NUM_2_SIZE 5

// Expected: 12345 + 67890 = 80235
static unsigned int small_sum_1_2[] = {5, 3, 2, 0, 8};
#define SMALL_SUM_1_2_SIZE 5

// ----------------------------------------------------------------------------
// Medium numbers (around long boundary)
// ----------------------------------------------------------------------------

// Number: 9,223,372,036,854,775,807 (LONG_MAX)
static unsigned int long_max[] = {7, 0, 8, 5, 7, 7, 4, 5, 8, 6, 3, 0, 2, 7, 3, 3, 2, 2, 9};
#define LONG_MAX_SIZE 19

// Number: 9,223,372,036,854,775,807 (LONG_MAX again)
static unsigned int long_max_2[] = {7, 0, 8, 5, 7, 7, 4, 5, 8, 6, 3, 0, 2, 7, 3, 3, 2, 2, 9};
#define LONG_MAX_2_SIZE 19

// Expected: LONG_MAX + LONG_MAX = 18,446,744,073,709,551,614
static unsigned int long_max_doubled[] = {4, 1, 6, 1, 5, 5, 9, 0, 7, 3, 7, 0, 4, 4, 7, 6, 4, 4, 8, 1};
#define LONG_MAX_DOUBLED_SIZE 20

// ----------------------------------------------------------------------------
// Large numbers (50 digits - far beyond any native type)
// ----------------------------------------------------------------------------

// Number A (50 digits): 
// 12345678901234567890123456789012345678901234567890
static unsigned int large_num_a[] = {
    0, 9, 8, 7, 6, 5, 4, 3, 2, 1,  // last 10 digits
    0, 9, 8, 7, 6, 5, 4, 3, 2, 1,  // next 10 digits
    0, 9, 8, 7, 6, 5, 4, 3, 2, 1,  // next 10 digits
    0, 9, 8, 7, 6, 5, 4, 3, 2, 1,  // next 10 digits
    0, 9, 8, 7, 6, 5, 4, 3, 2, 1   // first 10 digits
};
#define LARGE_NUM_A_SIZE 50

// Number B (50 digits): 
// 98765432109876543210987654321098765432109876543210
static unsigned int large_num_b[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
#define LARGE_NUM_B_SIZE 50

// Expected: A + B = 111111111011111111101111111110111111111011111111100
// 12345678901234567890123456789012345678901234567890
// +98765432109876543210987654321098765432109876543210
// =111111111011111111101111111110111111111011111111100
// Reversed: [0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, ...]
static unsigned int large_sum_a_b[] = {
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
#define LARGE_SUM_A_B_SIZE 51

// ----------------------------------------------------------------------------
// Very large numbers (100 digits)
// ----------------------------------------------------------------------------

// Number C (100 digits) - all 9s
// 9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999
static unsigned int very_large_all_nines[100] = {
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9
};
#define VERY_LARGE_ALL_NINES_SIZE 100

// Number: 1 (single digit)
static unsigned int one_digit[] = {1};
#define ONE_DIGIT_SIZE 1

// Expected: 999...999 (100 nines) + 1 = 1000...000 (1 followed by 100 zeros)
static unsigned int very_large_plus_one[101] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1
};
#define VERY_LARGE_PLUS_ONE_SIZE 101

// ----------------------------------------------------------------------------
// Zero tests
// ----------------------------------------------------------------------------

// Number: 0
static unsigned int zero_num[] = {0};
#define ZERO_NUM_SIZE 1

// ----------------------------------------------------------------------------
// Power of 10 numbers
// ----------------------------------------------------------------------------

// Number: 10^30 = 1000000000000000000000000000000
static unsigned int power_10_30[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1
};
#define POWER_10_30_SIZE 31

// Number: 10^30 - 1 = 999999999999999999999999999999 (30 nines)
static unsigned int power_10_30_minus_1[] = {
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9
};
#define POWER_10_30_MINUS_1_SIZE 30

// Expected: (10^30 - 1) + 1 = 10^30
// Same as power_10_30
#define POWER_10_30_SUM_SIZE 31

// ----------------------------------------------------------------------------
// Fibonacci-like large numbers (F(200) is about 42 digits)
// ----------------------------------------------------------------------------

// Fibonacci(200) = 280571172992510140037611932413038677189525
static unsigned int fib_200[] = {
    5, 2, 5, 9, 8, 1, 7, 7, 6, 8,
    3, 0, 3, 1, 4, 2, 3, 9, 1, 1,
    6, 7, 3, 0, 0, 4, 1, 0, 1, 5,
    2, 9, 9, 2, 7, 1, 1, 7, 5, 0,
    8, 2
};
#define FIB_200_SIZE 42

// Fibonacci(201) = 453973694165307953197296969697410619233826
static unsigned int fib_201[] = {
    6, 2, 8, 3, 3, 2, 9, 1, 6, 0,
    1, 4, 7, 9, 6, 9, 6, 9, 6, 9,
    2, 7, 9, 1, 3, 5, 9, 7, 0, 3,
    5, 6, 1, 4, 9, 6, 3, 7, 9, 3,
    5, 4
};
#define FIB_201_SIZE 42

// Fibonacci(202) = F(200) + F(201) = 734544867157818093234908902110449296423351
static unsigned int fib_202[] = {
    1, 5, 3, 3, 2, 4, 6, 9, 2, 9,
    4, 4, 0, 1, 1, 2, 0, 9, 8, 0,
    9, 4, 3, 2, 3, 9, 0, 8, 1, 8,
    7, 5, 1, 7, 6, 8, 4, 4, 5, 4,
    3, 7
};
#define FIB_202_SIZE 42

// ----------------------------------------------------------------------------
// Alternating digit patterns
// ----------------------------------------------------------------------------

// Pattern: 101010101010101010101010101010 (30 digits)
static unsigned int alternating_10[] = {
    0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
    0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
    0, 1, 0, 1, 0, 1, 0, 1, 0, 1
};
#define ALTERNATING_10_SIZE 30

// Pattern: 909090909090909090909090909090 (30 digits)
static unsigned int alternating_90[] = {
    0, 9, 0, 9, 0, 9, 0, 9, 0, 9,
    0, 9, 0, 9, 0, 9, 0, 9, 0, 9,
    0, 9, 0, 9, 0, 9, 0, 9, 0, 9
};
#define ALTERNATING_90_SIZE 30

// Sum: 101010...10 + 909090...90 = 1010101010101010101010101010100 (31 digits)
static unsigned int alternating_sum[] = {
    0, 0, 1, 0, 1, 0, 1, 0, 1, 0,
    1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
    1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
    1
};
#define ALTERNATING_SUM_SIZE 31

#endif // __LARGE_NUMBERS_TEST_DATA_H__
