#include <cs/large_number.h>
#include <unittest.h>
#include "test_data/large_numbers.h"

// ============================================================================
// large_number_init - LN_INT tests
// ============================================================================

test_res test_large_number_init_int_positive() {
    large_number ln;
    cs_codes rc = large_number_init(&ln, 10, LN_INT, 12345L);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.size != 5) return (test_res){(char*)__func__, "Size should be 5", CS_UNKNOWN};
    if (ln.sign != __POSITIVE_SIGN) return (test_res){(char*)__func__, "Sign should be positive", CS_UNKNOWN};
    if (ln.base != 10) return (test_res){(char*)__func__, "Base not set correctly", CS_UNKNOWN};
    // Check digits: 12345 -> reversed: {5, 4, 3, 2, 1}
    if (ln.digits[0] != 5 || ln.digits[1] != 4 || ln.digits[2] != 3 || 
        ln.digits[3] != 2 || ln.digits[4] != 1) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_int_negative() {
    large_number ln;
    cs_codes rc = large_number_init(&ln, 10, LN_INT, -9876L);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.sign != __NEGATIVE_SIGN) return (test_res){(char*)__func__, "Sign should be negative", CS_UNKNOWN};
    // Check digits: 9876 -> reversed: {6, 7, 8, 9}
    if (ln.digits[0] != 6 || ln.digits[1] != 7 || ln.digits[2] != 8 || ln.digits[3] != 9) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_int_zero() {
    large_number ln;
    cs_codes rc = large_number_init(&ln, 10, LN_INT, 0L);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.size != 1) return (test_res){(char*)__func__, "Size should be 1 for zero", CS_UNKNOWN};
    if (ln.sign != __POSITIVE_SIGN) return (test_res){(char*)__func__, "Sign should be positive", CS_UNKNOWN};
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_int_large() {
    large_number ln;
    // Test with a large number near LONG_MAX: 9223372036854775807
    cs_codes rc = large_number_init(&ln, 10, LN_INT, 9223372036854775807L);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.size != 19) return (test_res){(char*)__func__, "Size should be 19 for LONG_MAX", CS_UNKNOWN};
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_int_base2() {
    large_number ln;
    // 10 in base 2 = 1010 -> digits: {0, 1, 0, 1}
    cs_codes rc = large_number_init(&ln, 2, LN_INT, 10L);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.base != 2) return (test_res){(char*)__func__, "Base should be 2", CS_UNKNOWN};
    if (ln.size != 4) return (test_res){(char*)__func__, "Size should be 4", CS_UNKNOWN};
    if (ln.digits[0] != 0 || ln.digits[1] != 1 || ln.digits[2] != 0 || ln.digits[3] != 1) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for binary 10", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_int_base16() {
    large_number ln;
    // 255 in base 16 = FF -> digits: {15, 15}
    cs_codes rc = large_number_init(&ln, 16, LN_INT, 255L);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.base != 16) return (test_res){(char*)__func__, "Base should be 16", CS_UNKNOWN};
    if (ln.size != 2) return (test_res){(char*)__func__, "Size should be 2", CS_UNKNOWN};
    if (ln.digits[0] != 15 || ln.digits[1] != 15) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for hex 255", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_init - LN_CHAR tests (for numbers > LONG_MAX)
// ============================================================================

test_res test_large_number_init_char_simple() {
    large_number ln;
    // Initialize with string "12345" - note: digits stored as char values
    cs_codes rc = large_number_init(&ln, 10, LN_CHAR, "12345");
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.size != 5) return (test_res){(char*)__func__, "Size should be 5", CS_UNKNOWN};
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_char_50_digits() {
    large_number ln;
    // 50 digit number - way beyond LONG_MAX (using test_data)
    cs_codes rc = large_number_init(&ln, 10, LN_CHAR, LARGE_NUM_A_STR);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.size != LARGE_NUM_A_SIZE) return (test_res){(char*)__func__, "Size should be 50", CS_UNKNOWN};
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_char_100_digits() {
    large_number ln;
    // 100 nines (using test_data)
    cs_codes rc = large_number_init(&ln, 10, LN_CHAR, VERY_LARGE_ALL_NINES_STR);
    
    if (rc != CS_SUCCESS) return (test_res){(char*)__func__, "Init returned error", rc};
    if (ln.size != VERY_LARGE_ALL_NINES_SIZE) return (test_res){(char*)__func__, "Size should be 100", CS_UNKNOWN};
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_init - LN_NUM tests (copy)
// ============================================================================

test_res test_large_number_init_num_copy() {
    large_number src, dest;
    large_number_init(&src, 10, LN_INT, 12345L);
    
    cs_codes rc = large_number_init(&dest, 10, LN_NUM, src);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&src);
        return (test_res){(char*)__func__, "Init copy returned error", rc};
    }
    
    if (large_number_comp(src, dest) != 0) {
        large_number_free(&src);
        large_number_free(&dest);
        return (test_res){(char*)__func__, "Copy not equal to source", CS_ELEM};
    }
    
    large_number_free(&src);
    large_number_free(&dest);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_init_num_copy_negative() {
    large_number src, dest;
    large_number_init(&src, 10, LN_INT, -9999L);
    
    cs_codes rc = large_number_init(&dest, 10, LN_NUM, src);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&src);
        return (test_res){(char*)__func__, "Init copy returned error", rc};
    }
    
    if (dest.sign != __NEGATIVE_SIGN) {
        large_number_free(&src);
        large_number_free(&dest);
        return (test_res){(char*)__func__, "Sign not copied", CS_ELEM};
    }
    
    large_number_free(&src);
    large_number_free(&dest);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_init - NULL pointer test
// ============================================================================

test_res test_large_number_init_null() {
    cs_codes rc = large_number_init(NULL, 10, LN_INT, 123L);
    
    if (rc != CS_NULL) return (test_res){(char*)__func__, "Expected CS_NULL for NULL pointer", rc};
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_clear
// ============================================================================

test_res test_large_number_clear() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, -12345L);
    
    large_number_clear(&ln);
    
    if (ln.size != 0) return (test_res){(char*)__func__, "Size not reset to 0", CS_UNKNOWN};
    if (ln.sign != __POSITIVE_SIGN) return (test_res){(char*)__func__, "Sign not reset to positive", CS_UNKNOWN};
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_clear_preserves_capacity() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 123456789L);
    int original_capacity = ln.capacity;
    
    large_number_clear(&ln);
    
    if (ln.capacity != original_capacity) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Capacity changed after clear", CS_UNKNOWN};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - Small numbers (using LN_INT)
// ============================================================================

test_res test_large_number_add_small() {
    large_number a, b, result, expected;
    
    // 12345 + 67890 = 80235 (using test_data constants)
    large_number_init(&a, 10, LN_CHAR, SMALL_NUM_1_STR);
    large_number_init(&b, 10, LN_CHAR, SMALL_NUM_2_STR);
    large_number_init(&expected, 10, LN_CHAR, SMALL_SUM_1_2_STR);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "12345 + 67890 != 80235", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_zero() {
    large_number a, b, result;
    
    // 12345 + 0 = 12345
    large_number_init(&a, 10, LN_INT, 12345L);
    large_number_init(&b, 10, LN_INT, 0L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, a) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Adding zero changed the number", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_with_carry() {
    large_number a, b, result, expected;
    
    // 999 + 1 = 1000 (tests carry propagation)
    large_number_init(&a, 10, LN_INT, 999L);
    large_number_init(&b, 10, LN_INT, 1L);
    large_number_init(&expected, 10, LN_INT, 1000L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "999 + 1 != 1000", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_large_carry() {
    large_number a, b, result, expected;
    
    // 999999999 + 1 = 1000000000 (tests long carry chain)
    large_number_init(&a, 10, LN_INT, 999999999L);
    large_number_init(&b, 10, LN_INT, 1L);
    large_number_init(&expected, 10, LN_INT, 1000000000L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "999999999 + 1 != 1000000000", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - Numbers at LONG_MAX boundary
// ============================================================================

test_res test_large_number_add_long_max() {
    large_number a, b, result;
    
    // LONG_MAX + LONG_MAX = 18446744073709551614
    large_number_init(&a, 10, LN_INT, 9223372036854775807L);
    large_number_init(&b, 10, LN_INT, 9223372036854775807L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    // Result should have 20 digits
    if (result.size != 20) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&result);
        return (test_res){(char*)__func__, "LONG_MAX + LONG_MAX should have 20 digits", CS_ELEM};
    }
    
    // Verify last few digits of 18446744073709551614: ...1614
    if (result.digits[0] != 4 || result.digits[1] != 1 || result.digits[2] != 6 || result.digits[3] != 1) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&result);
        return (test_res){(char*)__func__, "LONG_MAX + LONG_MAX last digits mismatch", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - Commutativity and Identity tests
// ============================================================================

test_res test_large_number_add_commutative() {
    large_number a, b, result1, result2;
    
    // Test that A + B = B + A
    large_number_init(&a, 10, LN_INT, 123456789L);
    large_number_init(&b, 10, LN_INT, 987654321L);
    large_number_init(&result1, 10, LN_EMPTY);
    large_number_init(&result2, 10, LN_EMPTY);
    
    large_number_add(&result1, a, b);
    large_number_add(&result2, b, a);
    
    if (large_number_comp(result1, result2) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&result1);
        large_number_free(&result2);
        return (test_res){(char*)__func__, "A + B != B + A (commutativity failed)", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&result1);
    large_number_free(&result2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_identity() {
    large_number a, zero, result;
    
    // Test that A + 0 = A
    large_number_init(&a, 10, LN_INT, 123456789L);
    large_number_init(&zero, 10, LN_INT, 0L);
    large_number_init(&result, 10, LN_EMPTY);
    
    large_number_add(&result, a, zero);
    
    if (large_number_comp(result, a) != 0) {
        large_number_free(&a);
        large_number_free(&zero);
        large_number_free(&result);
        return (test_res){(char*)__func__, "A + 0 != A (identity failed)", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&zero);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - Different sized numbers
// ============================================================================

test_res test_large_number_add_different_sizes() {
    large_number a, b, result, expected;
    
    // 1000000 + 1 = 1000001
    large_number_init(&a, 10, LN_INT, 1000000L);
    large_number_init(&b, 10, LN_INT, 1L);
    large_number_init(&expected, 10, LN_INT, 1000001L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "1000000 + 1 != 1000001", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - Negative numbers
// ============================================================================

test_res test_large_number_add_negative_plus_negative() {
    large_number a, b, result, expected;
    
    // (-100) + (-200) = -300
    large_number_init(&a, 10, LN_INT, -100L);
    large_number_init(&b, 10, LN_INT, -200L);
    large_number_init(&expected, 10, LN_INT, -300L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(-100) + (-200) != -300", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_positive_plus_negative_result_positive() {
    large_number a, b, result, expected;
    
    // 500 + (-200) = 300
    large_number_init(&a, 10, LN_INT, 500L);
    large_number_init(&b, 10, LN_INT, -200L);
    large_number_init(&expected, 10, LN_INT, 300L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "500 + (-200) != 300", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_positive_plus_negative_result_negative() {
    large_number a, b, result, expected;
    
    // 200 + (-500) = -300
    large_number_init(&a, 10, LN_INT, 200L);
    large_number_init(&b, 10, LN_INT, -500L);
    large_number_init(&expected, 10, LN_INT, -300L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "200 + (-500) != -300", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_negative_plus_positive_result_zero() {
    large_number a, b, result, expected;
    
    // (-500) + 500 = 0
    large_number_init(&a, 10, LN_INT, -500L);
    large_number_init(&b, 10, LN_INT, 500L);
    large_number_init(&expected, 10, LN_INT, 0L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(-500) + 500 != 0", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_large_negative() {
    large_number a, b, result, expected;
    
    // (-999999999) + (-1) = -1000000000 (tests carry with negatives)
    large_number_init(&a, 10, LN_INT, -999999999L);
    large_number_init(&b, 10, LN_INT, -1L);
    large_number_init(&expected, 10, LN_INT, -1000000000L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(-999999999) + (-1) != -1000000000", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - Stress test with repeated additions
// ============================================================================

test_res test_large_number_add_repeated() {
    large_number current, one, result;
    
    // Start with 0 and add 1 repeatedly to test growth
    large_number_init(&current, 10, LN_INT, 0L);
    large_number_init(&one, 10, LN_INT, 1L);
    
    // Add 1 exactly 1000 times
    for (int i = 0; i < 1000; i++) {
        large_number_init(&result, 10, LN_EMPTY);
        cs_codes rc = large_number_add(&result, current, one);
        if (rc != CS_SUCCESS) {
            large_number_free(&current);
            large_number_free(&one);
            return (test_res){(char*)__func__, "Add failed during repeated addition", rc};
        }
        large_number_free(&current);
        current = result;
    }
    
    // Result should be 1000
    large_number expected;
    large_number_init(&expected, 10, LN_INT, 1000L);
    
    if (large_number_comp(current, expected) != 0) {
        large_number_free(&current);
        large_number_free(&one);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sum of 1000 ones != 1000", CS_ELEM};
    }
    
    large_number_free(&current);
    large_number_free(&one);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - Multiple consecutive additions
// ============================================================================

test_res test_large_number_add_multiple_three_operands() {
    large_number a, b, c, temp, result, expected;
    
    // (100 + 200) + 300 = 600
    large_number_init(&a, 10, LN_INT, 100L);
    large_number_init(&b, 10, LN_INT, 200L);
    large_number_init(&c, 10, LN_INT, 300L);
    large_number_init(&temp, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    large_number_init(&expected, 10, LN_INT, 600L);
    
    cs_codes rc = large_number_add(&temp, a, b);
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "First add returned error", rc};
    }
    
    rc = large_number_add(&result, temp, c);
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&temp);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Second add returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&temp);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "(100 + 200) + 300 != 600", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&c);
    large_number_free(&temp);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_multiple_five_operands() {
    large_number nums[5], temp, result, expected;
    
    // 1 + 2 + 3 + 4 + 5 = 15
    for (int i = 0; i < 5; i++) {
        large_number_init(&nums[i], 10, LN_INT, (long)(i + 1));
    }
    large_number_init(&expected, 10, LN_INT, 15L);
    large_number_init(&result, 10, LN_NUM, nums[0]);
    
    for (int i = 1; i < 5; i++) {
        large_number_init(&temp, 10, LN_EMPTY);
        cs_codes rc = large_number_add(&temp, result, nums[i]);
        if (rc != CS_SUCCESS) {
            for (int j = 0; j < 5; j++) large_number_free(&nums[j]);
            large_number_free(&result);
            large_number_free(&expected);
            return (test_res){(char*)__func__, "Add in loop returned error", rc};
        }
        large_number_free(&result);
        result = temp;
    }
    
    if (large_number_comp(result, expected) != 0) {
        for (int i = 0; i < 5; i++) large_number_free(&nums[i]);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "1+2+3+4+5 != 15", CS_ELEM};
    }
    
    for (int i = 0; i < 5; i++) large_number_free(&nums[i]);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_multiple_with_mixed_signs() {
    large_number a, b, c, d, temp1, temp2, result, expected;
    
    // 100 + (-50) + 75 + (-25) = 100
    large_number_init(&a, 10, LN_INT, 100L);
    large_number_init(&b, 10, LN_INT, -50L);
    large_number_init(&c, 10, LN_INT, 75L);
    large_number_init(&d, 10, LN_INT, -25L);
    large_number_init(&temp1, 10, LN_EMPTY);
    large_number_init(&temp2, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    large_number_init(&expected, 10, LN_INT, 100L);
    
    large_number_add(&temp1, a, b);        // 100 + (-50) = 50
    large_number_add(&temp2, temp1, c);    // 50 + 75 = 125
    large_number_add(&result, temp2, d);   // 125 + (-25) = 100
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&d);
        large_number_free(&temp1);
        large_number_free(&temp2);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "100 + (-50) + 75 + (-25) != 100", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&c);
    large_number_free(&d);
    large_number_free(&temp1);
    large_number_free(&temp2);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_multiple_large_chain() {
    large_number current, addend, result, expected;
    
    // Start with 999999999 and add 1 three times = 1000000002
    large_number_init(&current, 10, LN_INT, 999999999L);
    large_number_init(&addend, 10, LN_INT, 1L);
    large_number_init(&expected, 10, LN_INT, 1000000002L);
    
    for (int i = 0; i < 3; i++) {
        large_number_init(&result, 10, LN_EMPTY);
        cs_codes rc = large_number_add(&result, current, addend);
        if (rc != CS_SUCCESS) {
            large_number_free(&current);
            large_number_free(&addend);
            large_number_free(&expected);
            return (test_res){(char*)__func__, "Add in loop returned error", rc};
        }
        large_number_free(&current);
        current = result;
    }
    
    if (large_number_comp(current, expected) != 0) {
        large_number_free(&current);
        large_number_free(&addend);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "999999999 + 1 + 1 + 1 != 1000000002", CS_ELEM};
    }
    
    large_number_free(&current);
    large_number_free(&addend);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_multiple_associative() {
    large_number a, b, c, temp1, temp2, result1, result2;
    
    // Test associativity: (a + b) + c == a + (b + c)
    large_number_init(&a, 10, LN_INT, 12345L);
    large_number_init(&b, 10, LN_INT, 67890L);
    large_number_init(&c, 10, LN_INT, 11111L);
    large_number_init(&temp1, 10, LN_EMPTY);
    large_number_init(&temp2, 10, LN_EMPTY);
    large_number_init(&result1, 10, LN_EMPTY);
    large_number_init(&result2, 10, LN_EMPTY);
    
    // (a + b) + c
    large_number_add(&temp1, a, b);
    large_number_add(&result1, temp1, c);
    
    // a + (b + c)
    large_number_add(&temp2, b, c);
    large_number_add(&result2, a, temp2);
    
    if (large_number_comp(result1, result2) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&temp1);
        large_number_free(&temp2);
        large_number_free(&result1);
        large_number_free(&result2);
        return (test_res){(char*)__func__, "(a + b) + c != a + (b + c) (associativity failed)", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&c);
    large_number_free(&temp1);
    large_number_free(&temp2);
    large_number_free(&result1);
    large_number_free(&result2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_add - NULL pointer test
// ============================================================================

test_res test_large_number_add_null_output() {
    large_number a, b;
    large_number_init(&a, 10, LN_INT, 123L);
    large_number_init(&b, 10, LN_INT, 456L);
    
    cs_codes rc = large_number_add(NULL, a, b);
    
    large_number_free(&a);
    large_number_free(&b);
    
    if (rc != CS_NULL) return (test_res){(char*)__func__, "Expected CS_NULL for NULL output", rc};
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_sub - Basic subtraction tests
// ============================================================================

test_res test_large_number_sub_basic() {
    large_number a, b, result, expected;
    
    // 500 - 200 = 300
    large_number_init(&a, 10, LN_INT, 500L);
    large_number_init(&b, 10, LN_INT, 200L);
    large_number_init(&expected, 10, LN_INT, 300L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "500 - 200 != 300", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_result_negative() {
    large_number a, b, result, expected;
    
    // 200 - 500 = -300
    large_number_init(&a, 10, LN_INT, 200L);
    large_number_init(&b, 10, LN_INT, 500L);
    large_number_init(&expected, 10, LN_INT, -300L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "200 - 500 != -300", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_result_zero() {
    large_number a, b, result, expected;
    
    // 500 - 500 = 0
    large_number_init(&a, 10, LN_INT, 500L);
    large_number_init(&b, 10, LN_INT, 500L);
    large_number_init(&expected, 10, LN_INT, 0L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "500 - 500 != 0", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_with_borrow() {
    large_number a, b, result, expected;
    
    // 1000 - 1 = 999 (tests borrow propagation)
    large_number_init(&a, 10, LN_INT, 1000L);
    large_number_init(&b, 10, LN_INT, 1L);
    large_number_init(&expected, 10, LN_INT, 999L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "1000 - 1 != 999", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_large_borrow() {
    large_number a, b, result, expected;
    
    // 1000000000 - 1 = 999999999 (tests long borrow chain)
    large_number_init(&a, 10, LN_INT, 1000000000L);
    large_number_init(&b, 10, LN_INT, 1L);
    large_number_init(&expected, 10, LN_INT, 999999999L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "1000000000 - 1 != 999999999", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_sub - Negative number tests
// ============================================================================

test_res test_large_number_sub_negative_minus_negative() {
    large_number a, b, result, expected;
    
    // (-100) - (-300) = 200
    large_number_init(&a, 10, LN_INT, -100L);
    large_number_init(&b, 10, LN_INT, -300L);
    large_number_init(&expected, 10, LN_INT, 200L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(-100) - (-300) != 200", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_positive_minus_negative() {
    large_number a, b, result, expected;
    
    // 500 - (-200) = 700
    large_number_init(&a, 10, LN_INT, 500L);
    large_number_init(&b, 10, LN_INT, -200L);
    large_number_init(&expected, 10, LN_INT, 700L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "500 - (-200) != 700", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_negative_minus_positive() {
    large_number a, b, result, expected;
    
    // (-500) - 200 = -700
    large_number_init(&a, 10, LN_INT, -500L);
    large_number_init(&b, 10, LN_INT, 200L);
    large_number_init(&expected, 10, LN_INT, -700L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(-500) - 200 != -700", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_zero() {
    large_number a, b, result;
    
    // 12345 - 0 = 12345
    large_number_init(&a, 10, LN_INT, 12345L);
    large_number_init(&b, 10, LN_INT, 0L);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_sub(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        return (test_res){(char*)__func__, "Sub returned error", rc};
    }
    
    if (large_number_comp(result, a) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Subtracting zero changed the number", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_null_output() {
    large_number a, b;
    large_number_init(&a, 10, LN_INT, 123L);
    large_number_init(&b, 10, LN_INT, 456L);
    
    cs_codes rc = large_number_sub(NULL, a, b);
    
    large_number_free(&a);
    large_number_free(&b);
    
    if (rc != CS_NULL) return (test_res){(char*)__func__, "Expected CS_NULL for NULL output", rc};
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_sub - Multiple consecutive subtractions
// ============================================================================

test_res test_large_number_sub_multiple_three_operands() {
    large_number a, b, c, temp, result, expected;
    
    // (1000 - 200) - 300 = 500
    large_number_init(&a, 10, LN_INT, 1000L);
    large_number_init(&b, 10, LN_INT, 200L);
    large_number_init(&c, 10, LN_INT, 300L);
    large_number_init(&temp, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    large_number_init(&expected, 10, LN_INT, 500L);
    
    cs_codes rc = large_number_sub(&temp, a, b);
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "First sub returned error", rc};
    }
    
    rc = large_number_sub(&result, temp, c);
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&temp);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Second sub returned error", rc};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&temp);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "(1000 - 200) - 300 != 500", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&c);
    large_number_free(&temp);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_multiple_five_operands() {
    large_number start, nums[4], temp, result, expected;
    
    // 100 - 10 - 20 - 30 - 40 = 0
    large_number_init(&start, 10, LN_INT, 100L);
    large_number_init(&nums[0], 10, LN_INT, 10L);
    large_number_init(&nums[1], 10, LN_INT, 20L);
    large_number_init(&nums[2], 10, LN_INT, 30L);
    large_number_init(&nums[3], 10, LN_INT, 40L);
    large_number_init(&expected, 10, LN_INT, 0L);
    large_number_init(&result, 10, LN_NUM, start);
    
    for (int i = 0; i < 4; i++) {
        large_number_init(&temp, 10, LN_EMPTY);
        cs_codes rc = large_number_sub(&temp, result, nums[i]);
        if (rc != CS_SUCCESS) {
            large_number_free(&start);
            for (int j = 0; j < 4; j++) large_number_free(&nums[j]);
            large_number_free(&result);
            large_number_free(&expected);
            return (test_res){(char*)__func__, "Sub in loop returned error", rc};
        }
        large_number_free(&result);
        result = temp;
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&start);
        for (int i = 0; i < 4; i++) large_number_free(&nums[i]);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "100-10-20-30-40 != 0", CS_ELEM};
    }
    
    large_number_free(&start);
    for (int i = 0; i < 4; i++) large_number_free(&nums[i]);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_multiple_to_negative() {
    large_number a, b, c, d, temp1, temp2, result, expected;
    
    // (50 - 100) - 25 - 25 = -100
    large_number_init(&a, 10, LN_INT, 50L);
    large_number_init(&b, 10, LN_INT, 100L);
    large_number_init(&c, 10, LN_INT, 25L);
    large_number_init(&d, 10, LN_INT, 25L);
    large_number_init(&temp1, 10, LN_EMPTY);
    large_number_init(&temp2, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    large_number_init(&expected, 10, LN_INT, -100L);
    
    large_number_sub(&temp1, a, b);        // 50 - 100 = -50
    large_number_sub(&temp2, temp1, c);    // -50 - 25 = -75
    large_number_sub(&result, temp2, d);   // -75 - 25 = -100
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&d);
        large_number_free(&temp1);
        large_number_free(&temp2);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "(50 - 100) - 25 - 25 != -100", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&c);
    large_number_free(&d);
    large_number_free(&temp1);
    large_number_free(&temp2);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_multiple_with_negatives() {
    large_number a, b, c, d, temp1, temp2, result, expected;
    
    // 100 - (-50) - 25 - (-75) = 200
    large_number_init(&a, 10, LN_INT, 100L);
    large_number_init(&b, 10, LN_INT, -50L);
    large_number_init(&c, 10, LN_INT, 25L);
    large_number_init(&d, 10, LN_INT, -75L);
    large_number_init(&temp1, 10, LN_EMPTY);
    large_number_init(&temp2, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    large_number_init(&expected, 10, LN_INT, 200L);
    
    large_number_sub(&temp1, a, b);        // 100 - (-50) = 150
    large_number_sub(&temp2, temp1, c);    // 150 - 25 = 125
    large_number_sub(&result, temp2, d);   // 125 - (-75) = 200
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&d);
        large_number_free(&temp1);
        large_number_free(&temp2);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "100 - (-50) - 25 - (-75) != 200", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&c);
    large_number_free(&d);
    large_number_free(&temp1);
    large_number_free(&temp2);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_multiple_large_chain() {
    large_number current, subtrahend, result, expected;
    
    // Start with 1000000002 and subtract 1 three times = 999999999
    large_number_init(&current, 10, LN_INT, 1000000002L);
    large_number_init(&subtrahend, 10, LN_INT, 1L);
    large_number_init(&expected, 10, LN_INT, 999999999L);
    
    for (int i = 0; i < 3; i++) {
        large_number_init(&result, 10, LN_EMPTY);
        cs_codes rc = large_number_sub(&result, current, subtrahend);
        if (rc != CS_SUCCESS) {
            large_number_free(&current);
            large_number_free(&subtrahend);
            large_number_free(&expected);
            return (test_res){(char*)__func__, "Sub in loop returned error", rc};
        }
        large_number_free(&current);
        current = result;
    }
    
    if (large_number_comp(current, expected) != 0) {
        large_number_free(&current);
        large_number_free(&subtrahend);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "1000000002 - 1 - 1 - 1 != 999999999", CS_ELEM};
    }
    
    large_number_free(&current);
    large_number_free(&subtrahend);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_sub_repeated() {
    large_number current, one, result;
    
    // Start with 1000 and subtract 1 repeatedly (1000 times)
    large_number_init(&current, 10, LN_INT, 1000L);
    large_number_init(&one, 10, LN_INT, 1L);
    
    for (int i = 0; i < 1000; i++) {
        large_number_init(&result, 10, LN_EMPTY);
        cs_codes rc = large_number_sub(&result, current, one);
        if (rc != CS_SUCCESS) {
            large_number_free(&current);
            large_number_free(&one);
            return (test_res){(char*)__func__, "Sub failed during repeated subtraction", rc};
        }
        large_number_free(&current);
        current = result;
    }
    
    // Result should be 0
    large_number expected;
    large_number_init(&expected, 10, LN_INT, 0L);
    
    if (large_number_comp(current, expected) != 0) {
        large_number_free(&current);
        large_number_free(&one);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "1000 minus 1000 ones != 0", CS_ELEM};
    }
    
    large_number_free(&current);
    large_number_free(&one);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number - Mixed add and sub operations
// ============================================================================

test_res test_large_number_mixed_add_sub() {
    large_number a, b, c, d, temp1, temp2, result, expected;
    
    // (100 + 50) - 30 + 20 = 140
    large_number_init(&a, 10, LN_INT, 100L);
    large_number_init(&b, 10, LN_INT, 50L);
    large_number_init(&c, 10, LN_INT, 30L);
    large_number_init(&d, 10, LN_INT, 20L);
    large_number_init(&temp1, 10, LN_EMPTY);
    large_number_init(&temp2, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    large_number_init(&expected, 10, LN_INT, 140L);
    
    large_number_add(&temp1, a, b);        // 100 + 50 = 150
    large_number_sub(&temp2, temp1, c);    // 150 - 30 = 120
    large_number_add(&result, temp2, d);   // 120 + 20 = 140
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&c);
        large_number_free(&d);
        large_number_free(&temp1);
        large_number_free(&temp2);
        large_number_free(&result);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "(100 + 50) - 30 + 20 != 140", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&c);
    large_number_free(&d);
    large_number_free(&temp1);
    large_number_free(&temp2);
    large_number_free(&result);
    large_number_free(&expected);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_mixed_add_sub_inverse() {
    large_number a, b, temp, result;
    
    // (a + b) - b = a (verify add/sub are inverse operations)
    large_number_init(&a, 10, LN_INT, 123456789L);
    large_number_init(&b, 10, LN_INT, 987654321L);
    large_number_init(&temp, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    
    large_number_add(&temp, a, b);
    large_number_sub(&result, temp, b);
    
    if (large_number_comp(result, a) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&temp);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(a + b) - b != a", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&temp);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_mixed_sub_add_inverse() {
    large_number a, b, temp, result;
    
    // (a - b) + b = a (verify sub/add are inverse operations)
    large_number_init(&a, 10, LN_INT, 987654321L);
    large_number_init(&b, 10, LN_INT, 123456789L);
    large_number_init(&temp, 10, LN_EMPTY);
    large_number_init(&result, 10, LN_EMPTY);
    
    large_number_sub(&temp, a, b);
    large_number_add(&result, temp, b);
    
    if (large_number_comp(result, a) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&temp);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(a - b) + b != a", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&temp);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_print - Basic tests
// ============================================================================

test_res test_large_number_print_positive() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 12345L);
    
    // Just verify it doesn't crash (output goes to /dev/null)
    FILE *devnull = fopen("/dev/null", "w");
    if (devnull) {
        large_number_print(devnull, &ln);
        fclose(devnull);
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_print_zero() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 0L);
    
    FILE *devnull = fopen("/dev/null", "w");
    if (devnull) {
        large_number_print(devnull, &ln);
        fclose(devnull);
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_print_negative() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, -12345L);
    
    FILE *devnull = fopen("/dev/null", "w");
    if (devnull) {
        large_number_print(devnull, &ln);
        fclose(devnull);
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_print_large() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 9223372036854775807L);
    
    FILE *devnull = fopen("/dev/null", "w");
    if (devnull) {
        large_number_print(devnull, &ln);
        fclose(devnull);
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_free - Basic tests
// ============================================================================

test_res test_large_number_free_basic() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 12345L);
    
    // Should not crash
    large_number_free(&ln);
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// large_number_switch_base - Basic conversion tests
// ============================================================================

test_res test_large_number_switch_base_decimal_to_binary() {
    large_number ln;
    // 10 in decimal = 1010 in binary
    large_number_init(&ln, 10, LN_INT, 10L);
    
    cs_codes rc = large_number_switch_base(&ln, 2);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base returned error", rc};
    }
    if (ln.base != 2) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Base should be 2", CS_UNKNOWN};
    }
    if (ln.size != 4) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Size should be 4 for binary 10", CS_UNKNOWN};
    }
    // 1010 reversed = {0, 1, 0, 1}
    if (ln.digits[0] != 0 || ln.digits[1] != 1 || ln.digits[2] != 0 || ln.digits[3] != 1) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for binary 10", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_binary_to_decimal() {
    large_number ln;
    // 1010 in binary = 10 in decimal
    large_number_init(&ln, 2, LN_INT, 10L); // 10 in base 2 representation -> digits: {0,1,0,1}
    
    cs_codes rc = large_number_switch_base(&ln, 10);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base returned error", rc};
    }
    if (ln.base != 10) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Base should be 10", CS_UNKNOWN};
    }
    if (ln.size != 2) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Size should be 2 for decimal 10", CS_UNKNOWN};
    }
    // 10 reversed = {0, 1}
    if (ln.digits[0] != 0 || ln.digits[1] != 1) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for decimal 10", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_decimal_to_hex() {
    large_number ln;
    // 255 in decimal = FF in hex (digits: {15, 15})
    large_number_init(&ln, 10, LN_INT, 255L);
    
    cs_codes rc = large_number_switch_base(&ln, 16);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base returned error", rc};
    }
    if (ln.base != 16) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Base should be 16", CS_UNKNOWN};
    }
    if (ln.size != 2) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Size should be 2 for hex FF", CS_UNKNOWN};
    }
    if (ln.digits[0] != 15 || ln.digits[1] != 15) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for hex FF", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_hex_to_decimal() {
    large_number ln;
    // FF in hex = 255 in decimal
    large_number_init(&ln, 16, LN_INT, 255L); // 255 in base 16 = FF -> {15, 15}
    
    cs_codes rc = large_number_switch_base(&ln, 10);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base returned error", rc};
    }
    if (ln.base != 10) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Base should be 10", CS_UNKNOWN};
    }
    if (ln.size != 3) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Size should be 3 for decimal 255", CS_UNKNOWN};
    }
    // 255 reversed = {5, 5, 2}
    if (ln.digits[0] != 5 || ln.digits[1] != 5 || ln.digits[2] != 2) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for decimal 255", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_same_base() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 12345L);
    
    cs_codes rc = large_number_switch_base(&ln, 10);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base should return SUCCESS for same base", rc};
    }
    // Verify nothing changed
    if (ln.base != 10 || ln.size != 5) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Number should be unchanged", CS_UNKNOWN};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_null() {
    cs_codes rc = large_number_switch_base(NULL, 10);
    
    if (rc != CS_NULL) {
        return (test_res){(char*)__func__, "switch_base should return CS_NULL for NULL input", rc};
    }
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_invalid_base() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 12345L);
    
    cs_codes rc = large_number_switch_base(&ln, 1); // Base 1 is invalid
    
    if (rc != CS_SIZE) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base should return CS_SIZE for base < 2", rc};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_zero() {
    large_number ln;
    large_number_init(&ln, 10, LN_INT, 0L);
    
    cs_codes rc = large_number_switch_base(&ln, 2);
    
    // Zero in any base is still zero
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base returned error", rc};
    }
    if (ln.base != 2) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Base should be 2", CS_UNKNOWN};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_large_number() {
    large_number ln;
    // 1000000 in decimal = F4240 in hex
    large_number_init(&ln, 10, LN_INT, 1000000L);
    
    cs_codes rc = large_number_switch_base(&ln, 16);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base returned error", rc};
    }
    if (ln.base != 16) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Base should be 16", CS_UNKNOWN};
    }
    // F4240 hex = {0, 4, 2, 4, 15} reversed (0x0F4240)
    if (ln.size != 5) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Size should be 5 for hex F4240", CS_UNKNOWN};
    }
    // 1000000 = 0xF4240 -> digits: {0, 4, 2, 4, 15}
    if (ln.digits[0] != 0 || ln.digits[1] != 4 || ln.digits[2] != 2 || 
        ln.digits[3] != 4 || ln.digits[4] != 15) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for hex F4240", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_roundtrip() {
    large_number ln, original;
    // Test roundtrip: decimal -> binary -> decimal
    large_number_init(&ln, 10, LN_INT, 12345L);
    large_number_init(&original, 10, LN_INT, 12345L);
    
    cs_codes rc = large_number_switch_base(&ln, 2);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "First switch_base failed", rc};
    }
    
    rc = large_number_switch_base(&ln, 10);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "Second switch_base failed", rc};
    }
    
    if (large_number_comp(ln, original) != 0) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "Roundtrip conversion failed", CS_ELEM};
    }
    
    large_number_free(&ln);
    large_number_free(&original);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_base8() {
    large_number ln;
    // 64 in decimal = 100 in octal
    large_number_init(&ln, 10, LN_INT, 64L);
    
    cs_codes rc = large_number_switch_base(&ln, 8);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base returned error", rc};
    }
    if (ln.base != 8) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Base should be 8", CS_UNKNOWN};
    }
    if (ln.size != 3) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Size should be 3 for octal 100", CS_UNKNOWN};
    }
    // 100 reversed = {0, 0, 1}
    if (ln.digits[0] != 0 || ln.digits[1] != 0 || ln.digits[2] != 1) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "Digits mismatch for octal 100", CS_ELEM};
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_big_number_roundtrip() {
    large_number ln, original;
    // 50 digit number - way beyond LONG_MAX
    large_number_init(&ln, 10, LN_CHAR, "12345678901234567890123456789012345678901234567890");
    large_number_init(&original, 10, LN_CHAR, "12345678901234567890123456789012345678901234567890");
    
    // Convert decimal -> binary -> decimal
    cs_codes rc = large_number_switch_base(&ln, 2);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "First switch_base failed", rc};
    }
    if (ln.base != 2) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "Base should be 2 after first conversion", CS_UNKNOWN};
    }
    
    rc = large_number_switch_base(&ln, 10);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "Second switch_base failed", rc};
    }
    
    if (large_number_comp(ln, original) != 0) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "Big number roundtrip conversion failed", CS_ELEM};
    }
    
    large_number_free(&ln);
    large_number_free(&original);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_big_number_to_hex() {
    large_number ln, original;
    // 100 digit number
    large_number_init(&ln, 10, LN_CHAR, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    large_number_init(&original, 10, LN_CHAR, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    
    // Convert to hex and back
    cs_codes rc = large_number_switch_base(&ln, 16);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "switch_base to hex failed", rc};
    }
    if (ln.base != 16) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "Base should be 16", CS_UNKNOWN};
    }
    
    rc = large_number_switch_base(&ln, 10);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "switch_base back to decimal failed", rc};
    }
    
    if (large_number_comp(ln, original) != 0) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "100-digit hex roundtrip failed", CS_ELEM};
    }
    
    large_number_free(&ln);
    large_number_free(&original);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_big_number_multiple_bases() {
    large_number ln, original;
    // Test chain: decimal -> binary -> octal -> hex -> decimal
    large_number_init(&ln, 10, LN_CHAR, "99999999999999999999999999999999");
    large_number_init(&original, 10, LN_CHAR, "99999999999999999999999999999999");
    
    cs_codes rc = large_number_switch_base(&ln, 2);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "switch_base to binary failed", rc};
    }
    
    rc = large_number_switch_base(&ln, 8);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "switch_base to octal failed", rc};
    }
    
    rc = large_number_switch_base(&ln, 16);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "switch_base to hex failed", rc};
    }
    
    rc = large_number_switch_base(&ln, 10);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "switch_base back to decimal failed", rc};
    }
    
    if (large_number_comp(ln, original) != 0) {
        large_number_free(&ln);
        large_number_free(&original);
        return (test_res){(char*)__func__, "Multi-base chain roundtrip failed", CS_ELEM};
    }
    
    large_number_free(&ln);
    large_number_free(&original);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_switch_base_power_of_two() {
    large_number ln;
    // 2^100 = a very large power of 2
    // In binary this should be 1 followed by 100 zeros
    large_number_init(&ln, 10, LN_CHAR, "1267650600228229401496703205376"); // 2^100
    
    cs_codes rc = large_number_switch_base(&ln, 2);
    if (rc != CS_SUCCESS) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "switch_base to binary failed", rc};
    }
    
    // 2^100 in binary = 101 digits (1 followed by 100 zeros)
    if (ln.size != 101) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "2^100 should have 101 binary digits", CS_UNKNOWN};
    }
    
    // Most significant digit should be 1, all others 0
    if (ln.digits[100] != 1) {
        large_number_free(&ln);
        return (test_res){(char*)__func__, "MSB should be 1", CS_ELEM};
    }
    for (int i = 0; i < 100; i++) {
        if (ln.digits[i] != 0) {
            large_number_free(&ln);
            return (test_res){(char*)__func__, "Lower bits should all be 0", CS_ELEM};
        }
    }
    
    large_number_free(&ln);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Tests using test_data/large_numbers.h - Very Large Numbers
// ============================================================================

test_res test_large_number_add_50_digits() {
    large_number a, b, result, expected;
    
    // large_num_a + large_num_b = large_sum_a_b (50 digit numbers)
    large_number_init(&a, 10, LN_CHAR, LARGE_NUM_A_STR);
    large_number_init(&b, 10, LN_CHAR, LARGE_NUM_B_STR);
    large_number_init(&expected, 10, LN_CHAR, LARGE_SUM_A_B_STR);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (result.size != LARGE_SUM_A_B_SIZE) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Result should have 51 digits", CS_ELEM};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "50-digit addition failed", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_100_nines_plus_one() {
    large_number a, b, result, expected;
    
    // 100 nines + 1 = 1 followed by 100 zeros
    large_number_init(&a, 10, LN_CHAR, VERY_LARGE_ALL_NINES_STR);
    large_number_init(&b, 10, LN_CHAR, ONE_DIGIT_STR);
    large_number_init(&expected, 10, LN_CHAR, VERY_LARGE_PLUS_ONE_STR);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (result.size != VERY_LARGE_PLUS_ONE_SIZE) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Result should have 101 digits", CS_ELEM};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "100 nines + 1 failed", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_fibonacci() {
    large_number fib200, fib201, result, expected;
    
    // F(200) + F(201) = F(202)
    large_number_init(&fib200, 10, LN_CHAR, FIB_200_STR);
    large_number_init(&fib201, 10, LN_CHAR, FIB_201_STR);
    large_number_init(&expected, 10, LN_CHAR, FIB_202_STR);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, fib200, fib201);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&fib200);
        large_number_free(&fib201);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (result.size != FIB_202_SIZE) {
        large_number_free(&fib200);
        large_number_free(&fib201);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Fibonacci result has wrong size", CS_ELEM};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&fib200);
        large_number_free(&fib201);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "F(200) + F(201) != F(202)", CS_ELEM};
    }
    
    large_number_free(&fib200);
    large_number_free(&fib201);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_alternating_pattern() {
    large_number a, b, result, expected;
    
    // alternating_10 + alternating_90 = alternating_sum
    large_number_init(&a, 10, LN_CHAR, ALTERNATING_10_STR);
    large_number_init(&b, 10, LN_CHAR, ALTERNATING_90_STR);
    large_number_init(&expected, 10, LN_CHAR, ALTERNATING_SUM_STR);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (result.size != ALTERNATING_SUM_SIZE) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Alternating sum has wrong size", CS_ELEM};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Alternating pattern addition failed", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_power_of_10() {
    large_number a, b, result, expected;
    
    // (10^30 - 1) + 1 = 10^30
    large_number_init(&a, 10, LN_CHAR, POWER_10_30_MINUS_1_STR);
    large_number_init(&b, 10, LN_CHAR, ONE_DIGIT_STR);
    large_number_init(&expected, 10, LN_CHAR, POWER_10_30_STR);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (result.size != POWER_10_30_SIZE) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "Power of 10 result has wrong size", CS_ELEM};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "(10^30 - 1) + 1 != 10^30", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_large_number_add_long_max_doubled() {
    large_number a, b, result, expected;
    
    // LONG_MAX + LONG_MAX = 18446744073709551614
    large_number_init(&a, 10, LN_CHAR, LONG_MAX_STR);
    large_number_init(&b, 10, LN_CHAR, LONG_MAX_2_STR);
    large_number_init(&expected, 10, LN_CHAR, LONG_MAX_DOUBLED_STR);
    large_number_init(&result, 10, LN_EMPTY);
    
    cs_codes rc = large_number_add(&result, a, b);
    
    if (rc != CS_SUCCESS) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        return (test_res){(char*)__func__, "Add returned error", rc};
    }
    
    if (result.size != LONG_MAX_DOUBLED_SIZE) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "LONG_MAX doubled has wrong size", CS_ELEM};
    }
    
    if (large_number_comp(result, expected) != 0) {
        large_number_free(&a);
        large_number_free(&b);
        large_number_free(&expected);
        large_number_free(&result);
        return (test_res){(char*)__func__, "LONG_MAX + LONG_MAX mismatch", CS_ELEM};
    }
    
    large_number_free(&a);
    large_number_free(&b);
    large_number_free(&expected);
    large_number_free(&result);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Tests registry
// ============================================================================

test large_number_tests[] = {
    // Init - LN_INT tests
    test_large_number_init_int_positive,
    test_large_number_init_int_negative,
    test_large_number_init_int_zero,
    test_large_number_init_int_large,
    test_large_number_init_int_base2,
    test_large_number_init_int_base16,
    
    // Init - LN_CHAR tests (for numbers > LONG_MAX)
    test_large_number_init_char_simple,
    test_large_number_init_char_50_digits,
    test_large_number_init_char_100_digits,
    
    // Init - LN_NUM tests (copy)
    test_large_number_init_num_copy,
    test_large_number_init_num_copy_negative,
    
    // Init - Edge cases
    test_large_number_init_null,
    
    // Clear tests
    test_large_number_clear,
    test_large_number_clear_preserves_capacity,
    
    // Add - small numbers
    test_large_number_add_small,
    test_large_number_add_zero,
    test_large_number_add_with_carry,
    test_large_number_add_large_carry,
    
    // Add - LONG_MAX boundary
    test_large_number_add_long_max,
    
    // Add - Mathematical properties
    test_large_number_add_commutative,
    test_large_number_add_identity,
    
    // Add - Different sizes
    test_large_number_add_different_sizes,
    
    // Add - Negative numbers
    test_large_number_add_negative_plus_negative,
    test_large_number_add_positive_plus_negative_result_positive,
    test_large_number_add_positive_plus_negative_result_negative,
    test_large_number_add_negative_plus_positive_result_zero,
    test_large_number_add_large_negative,
    
    // Add - Stress test
    test_large_number_add_repeated,
    
    // Add - Multiple consecutive additions
    test_large_number_add_multiple_three_operands,
    test_large_number_add_multiple_five_operands,
    test_large_number_add_multiple_with_mixed_signs,
    test_large_number_add_multiple_large_chain,
    test_large_number_add_multiple_associative,
    
    // Add - Edge cases
    test_large_number_add_null_output,
    
    // Sub - Basic tests
    test_large_number_sub_basic,
    test_large_number_sub_result_negative,
    test_large_number_sub_result_zero,
    test_large_number_sub_with_borrow,
    test_large_number_sub_large_borrow,
    
    // Sub - Negative numbers
    test_large_number_sub_negative_minus_negative,
    test_large_number_sub_positive_minus_negative,
    test_large_number_sub_negative_minus_positive,
    test_large_number_sub_zero,
    test_large_number_sub_null_output,
    
    // Sub - Multiple consecutive subtractions
    test_large_number_sub_multiple_three_operands,
    test_large_number_sub_multiple_five_operands,
    test_large_number_sub_multiple_to_negative,
    test_large_number_sub_multiple_with_negatives,
    test_large_number_sub_multiple_large_chain,
    test_large_number_sub_repeated,
    
    // Mixed add/sub operations
    test_large_number_mixed_add_sub,
    test_large_number_mixed_add_sub_inverse,
    test_large_number_mixed_sub_add_inverse,
    
    // Print tests
    test_large_number_print_positive,
    test_large_number_print_zero,
    test_large_number_print_negative,
    test_large_number_print_large,
    
    // Free tests
    test_large_number_free_basic,
    
    // Switch base tests
    test_large_number_switch_base_decimal_to_binary,
    test_large_number_switch_base_binary_to_decimal,
    test_large_number_switch_base_decimal_to_hex,
    test_large_number_switch_base_hex_to_decimal,
    test_large_number_switch_base_same_base,
    test_large_number_switch_base_null,
    test_large_number_switch_base_invalid_base,
    test_large_number_switch_base_zero,
    test_large_number_switch_base_large_number,
    test_large_number_switch_base_roundtrip,
    test_large_number_switch_base_base8,
    
    // Switch base tests - big numbers (beyond LONG_MAX)
    test_large_number_switch_base_big_number_roundtrip,
    test_large_number_switch_base_big_number_to_hex,
    test_large_number_switch_base_big_number_multiple_bases,
    test_large_number_switch_base_power_of_two,
    
    // Tests using test_data/large_numbers.h
    test_large_number_add_50_digits,
    test_large_number_add_100_nines_plus_one,
    test_large_number_add_fibonacci,
    test_large_number_add_alternating_pattern,
    test_large_number_add_power_of_10,
    test_large_number_add_long_max_doubled,
};