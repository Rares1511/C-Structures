#include <cs/large_number.h>

#include <string.h>
#include <stdlib.h>

#define __LN_FRAC_EPSILON 1e-10
#define __LN_MAX_FRAC_DIGITS 1024

#define __LN_WHOLE_DIGIT_ORDER 1
#define __LN_FRAC_DIGIT_ORDER -1

#define LN_ENSURE_CAPACITY(ln) \
    do { \
        if ((ln)->whole_size >= (ln)->whole_capacity) { \
            unsigned int *__new_digits = realloc((ln)->whole_digits, sizeof(unsigned int) * (ln)->whole_size * 2); \
            if (NULL == __new_digits) return CS_MEM; \
            (ln)->whole_digits = __new_digits; \
            memset((ln)->whole_digits + (ln)->whole_capacity, 0, sizeof(unsigned int) * (ln)->whole_capacity); \
            (ln)->whole_capacity = (ln)->whole_size * 2; \
        } \
        if ((ln)->frac_size >= (ln)->frac_capacity) { \
            unsigned int *__new_digits = realloc((ln)->frac_digits, sizeof(unsigned int) * (ln)->frac_size * 2); \
            if (NULL == __new_digits) return CS_MEM; \
            (ln)->frac_digits = __new_digits; \
            memset((ln)->frac_digits + (ln)->frac_capacity, 0, sizeof(unsigned int) * (ln)->frac_capacity); \
            (ln)->frac_capacity = (ln)->frac_size * 2; \
        } \
    } while (0)

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes large_number_sub_helper(large_number *out, const large_number big, large_number small) {
    int diff, borrow = 0;
    out->sign = big.sign;
    out->base = big.base;
    out->whole_size = 0;

    out->frac_size = big.frac_size > small.frac_size ? big.frac_size : small.frac_size;
    LN_ENSURE_CAPACITY(out);
    for (int i = out->frac_size - 1; i >= 0; i--) {
        diff = (i < big.frac_size ? big.frac_digits[i] : 0) - (i < small.frac_size ? small.frac_digits[i] : 0) - borrow;
        if (diff < 0) {
            diff += big.base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        out->frac_digits[i] = diff;
    }

    while (out->whole_size < small.whole_size) {
        diff = big.whole_digits[out->whole_size] - small.whole_digits[out->whole_size] - borrow;
        if (diff < 0) {
            diff += big.base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        LN_ENSURE_CAPACITY(out);
        out->whole_digits[out->whole_size++] = diff;
    }

    while (out->whole_size < big.whole_size) {
        diff = big.whole_digits[out->whole_size] - borrow;
        if (diff < 0) {
            diff += big.base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        LN_ENSURE_CAPACITY(out);
        out->whole_digits[out->whole_size++] = diff;
    }

    while (out->whole_size > 0 && out->whole_digits[out->whole_size - 1] == 0) {
        out->whole_size--;
    }

    return CS_SUCCESS;
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes large_number_init(large_number *ln, unsigned int base, large_number_type type, ...) {
    CS_RETURN_IF(NULL == ln, CS_NULL);

    // Whole part initialization
    ln->whole_digits = malloc(sizeof(unsigned int) * LN_INIT_CAPACITY);
    CS_RETURN_IF(NULL == ln->whole_digits, CS_MEM);
    memset(ln->whole_digits, 0, sizeof(unsigned int) * LN_INIT_CAPACITY);
    ln->whole_size = 0;
    ln->whole_capacity = LN_INIT_CAPACITY;

    // Fractional part initialization
    ln->frac_digits = malloc(sizeof(unsigned int) * LN_INIT_CAPACITY);
    CS_RETURN_IF(NULL == ln->frac_digits, CS_MEM);
    memset(ln->frac_digits, 0, sizeof(unsigned int) * LN_INIT_CAPACITY);
    ln->frac_size = 0;
    ln->frac_capacity = LN_INIT_CAPACITY;

    ln->sign = __POSITIVE_SIGN;
    ln->base = base;

    // Handle variable arguments based on type
    va_list args;
    va_start(args, type);

    switch (type) {
        case LN_CHAR: {
            char *char_digits = va_arg(args, char*);
            int len = strlen(char_digits), i;
            char *dot = strchr(char_digits, '.');

            ln->base = 10;

            if (dot == NULL || dot[0] == '\0') {
                // No fractional part, parse whole part only
                ln->sign = __POSITIVE_SIGN;
                ln->whole_size = len;
                LN_ENSURE_CAPACITY(ln);
                for (i = len - 1; i >= 0; i--) {
                    if (i == 0 && char_digits[i] == '-') {
                        ln->sign = __NEGATIVE_SIGN;
                        continue;
                    }
                    if (char_digits[i] < '0' || char_digits[i] > '9') {
                        va_end(args);
                        return CS_ELEM;
                    }
                    ln->whole_digits[len - 1 - i] = (unsigned int)(char_digits[i] - '0');
                }
            } else {
                // we have a fractional part, parse it
                int whole_len = dot - char_digits;
                int frac_len = len - whole_len - 1;
                ln->sign = __POSITIVE_SIGN;
                ln->whole_size = whole_len;
                ln->frac_size = frac_len;
                LN_ENSURE_CAPACITY(ln);
                for (i = whole_len - 1; i >= 0; i--) {
                    if (i == 0 && char_digits[i] == '-') {
                        ln->sign = __NEGATIVE_SIGN;
                        continue;
                    }
                    if (char_digits[i] < '0' || char_digits[i] > '9') {
                        va_end(args);
                        return CS_ELEM;
                    }
                    ln->whole_digits[whole_len - 1 - i] = (unsigned int)(char_digits[i] - '0');
                }
                // Parse fractional part (stored in normal order: first fractional digit at index 0)
                for (i = 0; i < frac_len; i++) {
                    int char_idx = whole_len + 1 + i;
                    if (char_digits[char_idx] < '0' || char_digits[char_idx] > '9') {
                        va_end(args);
                        return CS_ELEM;
                    }
                    ln->frac_digits[i] = (unsigned int)(char_digits[char_idx] - '0');
                }
            }
            
            break;
        }
        case LN_INT: {
            long number = va_arg(args, long);
            ln->sign = __POSITIVE_SIGN;
            ln->base = base;
            if (number < 0) {
                ln->sign = __NEGATIVE_SIGN;
                number = -number;
            }
            do {
                LN_ENSURE_CAPACITY(ln);
                ln->whole_digits[ln->whole_size++] = number % base;
                number /= base;
            } while (number > 0);
            break;
        }
        case LN_NUM: {
            large_number source = va_arg(args, large_number);
            ln->whole_size = source.whole_size;
            ln->frac_size = source.frac_size;
            LN_ENSURE_CAPACITY(ln);
            for (int i = 0; i < source.whole_size; i++) {
                ln->whole_digits[i] = source.whole_digits[i];
            }
            for (int i = 0; i < source.frac_size; i++) {
                ln->frac_digits[i] = source.frac_digits[i];
            }
            ln->sign = source.sign;
            ln->base = source.base;
            break;
        }
        case LN_DOUBLE: {
            double number = va_arg(args, double);
            ln->sign = __POSITIVE_SIGN;
            if (number < 0) {
                ln->sign = __NEGATIVE_SIGN;
                number = -number;
            }
            ln->base = base;
            long long whole_part = (long long)number;
            double frac_part = number - whole_part;
            do {
                LN_ENSURE_CAPACITY(ln);
                ln->whole_digits[ln->whole_size++] = whole_part % base;
                whole_part /= base;
            } while (whole_part > 0);

            while (frac_part > __LN_FRAC_EPSILON && ln->frac_size < __LN_MAX_FRAC_DIGITS) {
                LN_ENSURE_CAPACITY(ln);
                frac_part *= base;
                int digit = (int)frac_part;
                ln->frac_digits[ln->frac_size++] = digit;
                frac_part -= digit;
            }
            break;
        }
        case LN_EMPTY: {
            // Do nothing, already initialized as empty
            break;
        }
    }

    va_end(args);

    if (ln->base != base) {
        return large_number_swap_base(ln, base);
    }

    while (ln->frac_size > 0 && ln->frac_digits[ln->frac_size - 1] == 0) {
        ln->frac_size--;
    }
    while (ln->whole_size > 0 && ln->whole_digits[ln->whole_size - 1] == 0) {
        ln->whole_size--;
    }
    if (ln->whole_size == 0 && ln->frac_size == 0) {
        ln->sign = __POSITIVE_SIGN; // Zero is non-negative
    }

    return CS_SUCCESS;
}

cs_codes large_number_add(large_number *out, const large_number a, const large_number b) {
    CS_RETURN_IF(NULL == out, CS_NULL);
    CS_RETURN_IF(a.base != b.base, CS_SIZE);
    if (a.sign != b.sign) {
        // Handle addition of numbers with different signs as subtraction
        large_number b_neg = b;
        b_neg.sign = a.sign;
        return large_number_sub(out, a, b_neg);
    }
    int sum, carry = 0;
    out->sign = a.sign;
    out->base = a.base;

    // Fractional part addition
    out->frac_size = a.frac_size > b.frac_size ? a.frac_size : b.frac_size;
    LN_ENSURE_CAPACITY(out);
    for (int i = out->frac_size - 1; i >= 0; i--) {
        sum = (i < a.frac_size ? a.frac_digits[i] : 0) + (i < b.frac_size ? b.frac_digits[i] : 0) + carry;
        carry = sum / a.base;
        sum = sum % a.base;
        out->frac_digits[i] = sum;
    }

    while (out->frac_size > 0 && out->frac_digits[out->frac_size - 1] == 0) {
        out->frac_size--;
    }

    // Whole part addition
    out->whole_size = 0;
    for (; out->whole_size < a.whole_size && out->whole_size < b.whole_size; out->whole_size++) {
        sum = a.whole_digits[out->whole_size] + b.whole_digits[out->whole_size];
        sum += carry;

        carry = sum / a.base;
        sum = sum % a.base;

        LN_ENSURE_CAPACITY(out);
        out->whole_digits[out->whole_size] = sum;
    }

    while (out->whole_size < a.whole_size) {
        sum = a.whole_digits[out->whole_size] + carry;

        carry = sum / a.base;
        sum = sum % a.base;

        LN_ENSURE_CAPACITY(out);
        out->whole_digits[out->whole_size++] = sum;
    }

    while (out->whole_size < b.whole_size) {
        sum = b.whole_digits[out->whole_size] + carry;

        carry = sum / b.base;
        sum = sum % b.base;
        LN_ENSURE_CAPACITY(out);
        out->whole_digits[out->whole_size++] = sum;
    }

    while (out->whole_size < b.whole_size) {
        sum = b.whole_digits[out->whole_size] + carry;

        carry = sum / b.base;
        sum = sum % b.base;

        LN_ENSURE_CAPACITY(out);
        out->whole_digits[out->whole_size++] = sum;
    }

    while (carry != 0) {
        sum = carry;
        carry = sum / a.base;
        sum = sum % a.base;

        LN_ENSURE_CAPACITY(out);
        out->whole_digits[out->whole_size++] = sum;
    }

    return CS_SUCCESS;
}

cs_codes large_number_sub(large_number *out, const large_number a, const large_number b) {
    CS_RETURN_IF(NULL == out, CS_NULL);
    CS_RETURN_IF(a.base != b.base, CS_SIZE);
    if (a.sign != b.sign) {
        // Handle subtraction of numbers with different signs as addition
        large_number b_pos = b;
        b_pos.sign = a.sign;
        return large_number_add(out, a, b_pos);
    }
    int comp = large_number_comp(a, b);
    if (comp > 0) {
        return large_number_sub_helper(out, a, b);
    }
    else if (comp < 0) {
        int rc = large_number_sub_helper(out, b, a);
        out->sign *= -1;
        return rc;
    } else {
        out->whole_size = 0;
        memset(out->whole_digits, 0, sizeof(unsigned int) * out->whole_capacity);
        out->sign = __POSITIVE_SIGN;
        out->base = a.base;
        return CS_SUCCESS;
    }
    return CS_SUCCESS;
}

cs_codes large_number_mul(large_number *out, const large_number a, const large_number b) {
    CS_RETURN_IF(NULL == out, CS_NULL);
    CS_RETURN_IF(a.base != b.base, CS_SIZE);

    unsigned int carry;
    unsigned long long product;
    int i, j;

    large_number_clear(out);
    out->sign = a.sign * b.sign;
    out->base = a.base;
    out->whole_size = 0;

    for (i = 0; i < a.whole_size; i++) {
        carry = 0;
        for (j = 0; j < b.whole_size; j++) {
            if (i + j >= out->whole_size) {
                out->whole_size = i + j + 1;
            }
            if (i + j >= out->whole_capacity) {
                LN_ENSURE_CAPACITY(out);
            }
            product = (unsigned long long)a.whole_digits[i] * b.whole_digits[j] + carry;
            if (i + j < out->whole_size) {
                product += out->whole_digits[i + j];
            }
            out->whole_digits[i + j] = (unsigned int)(product % a.base);
            carry = (unsigned int)(product / a.base);
        }
        if (carry > 0) {
            if (i + j >= out->whole_size) {
                out->whole_size = i + j + 1;
            }
            if (i + j >= out->whole_capacity) {
                LN_ENSURE_CAPACITY(out);
            }
            out->whole_digits[i + j] = carry;
        }
    }
    while (out->whole_size > 0 && out->whole_digits[out->whole_size - 1] == 0) {
        out->whole_size--;
    }
    if (out->whole_size == 0) {
        out->sign = __POSITIVE_SIGN;
        out->whole_size = 0;
    }
    return CS_SUCCESS;
}

cs_codes large_number_swap_base(large_number *ln, unsigned int new_base) {
    CS_RETURN_IF(NULL == ln, CS_NULL);
    CS_RETURN_IF(new_base < 2, CS_SIZE);
    CS_RETURN_IF(ln->base == new_base, CS_SUCCESS);

    large_number result, temp;
    large_number_init(&result, new_base, LN_EMPTY);
    large_number_init(&temp, ln->base, LN_NUM, *ln);
    
    while (temp.whole_size > 0) {
        unsigned int remainder = 0;

        for (int i = temp.whole_size - 1; i >= 0; i--) {
            unsigned long long current = 1ull * remainder * temp.base + temp.whole_digits[i];
            temp.whole_digits[i] = (unsigned int)(current / new_base);
            remainder = current % new_base;
        }

        LN_ENSURE_CAPACITY(&result);
        result.whole_digits[result.whole_size++] = remainder;

        while (temp.whole_size > 0 && temp.whole_digits[temp.whole_size - 1] == 0) {
            temp.whole_size--;
        }
    }
    large_number_free(&temp);

    result.sign = ln->sign;
    result.base = new_base;

    large_number_swap(&result, ln);
    large_number_free(&result);
    
    return CS_SUCCESS;
}

int large_number_comp(const large_number a, const large_number b) {
    CS_RETURN_IF(a.base != b.base, CS_SIZE);
    // If signs differ, the positive number is greater
    if (a.sign != b.sign) {
        return a.sign - b.sign;
    }
    // Compare whole part sizes first (more digits means greater magnitude)
    if (a.whole_size != b.whole_size) {
        return a.whole_size - b.whole_size;
    }
    // Compare whole part (reverse order: index 0 is least significant)
    for (int i = a.whole_size - 1; i >= 0; i--) {
        if (a.whole_digits[i] != b.whole_digits[i]) {
            return a.whole_digits[i] - b.whole_digits[i];
        }
    }
    // Compare fractional part (normal order: index 0 is most significant)
    int max_frac = a.frac_size > b.frac_size ? a.frac_size : b.frac_size;
    for (int i = 0; i < max_frac; i++) {
        unsigned int a_digit = (i < a.frac_size) ? a.frac_digits[i] : 0;
        unsigned int b_digit = (i < b.frac_size) ? b.frac_digits[i] : 0;
        if (a_digit != b_digit) {
            return a_digit - b_digit;
        }
    }
    return 0; // They are equal
}

int large_number_equal(const large_number a, const large_number b) {
    return large_number_comp(a, b) == 0;
}

void large_number_clear(large_number *ln) {
    CS_RETURN_IF(NULL == ln);
    memset(ln->whole_digits, 0, sizeof(unsigned int) * ln->whole_capacity);
    memset(ln->frac_digits, 0, sizeof(unsigned int) * ln->frac_capacity);
    ln->whole_size = 0;
    ln->frac_size = 0;
    ln->sign = __POSITIVE_SIGN;
}

void large_number_print(FILE *stream, void *v_ln) {
    CS_RETURN_IF(NULL == v_ln);
    large_number *ln = (large_number *)v_ln;
    if (ln->sign == __NEGATIVE_SIGN) {
        fprintf(stream, "-");
    }
    if (ln->whole_size == 0) {
        fprintf(stream, "0");
        return;
    }
    for (int i = ln->whole_size - 1; i >= 0; i--) {
        fprintf(stream, "%u", ln->whole_digits[i]);
    }
    if (ln->frac_size > 0) {
        fprintf(stream, ".");
        for (int i = 0; i < ln->frac_size; i++) {
            fprintf(stream, "%u", ln->frac_digits[i]);
        }
    }
}

void large_number_swap(large_number *a, large_number *b) {
    CS_RETURN_IF(NULL == a || NULL == b);
    
    unsigned int *temp_digits = a->whole_digits;
    int temp_size = a->whole_size;
    int temp_capacity = a->whole_capacity;
    unsigned int *temp_frac_digits = a->frac_digits;
    int temp_frac_size = a->frac_size;
    int temp_frac_capacity = a->frac_capacity;
    int temp_sign = a->sign;
    unsigned int temp_base = a->base;

    a->whole_digits = b->whole_digits;
    a->whole_size = b->whole_size;
    a->whole_capacity = b->whole_capacity;
    a->frac_digits = b->frac_digits;
    a->frac_size = b->frac_size;
    a->frac_capacity = b->frac_capacity;
    a->sign = b->sign;
    a->base = b->base;

    b->whole_digits = temp_digits;
    b->whole_size = temp_size;
    b->whole_capacity = temp_capacity;
    b->frac_digits = temp_frac_digits;
    b->frac_size = temp_frac_size;
    b->frac_capacity = temp_frac_capacity;
    b->sign = temp_sign;
    b->base = temp_base;
}

void large_number_free(void *v_ln) {
    CS_RETURN_IF(NULL == v_ln);
    large_number *ln = (large_number *)v_ln;
    free(ln->whole_digits);
    free(ln->frac_digits);
    ln->frac_digits = NULL;
    ln->frac_size = 0;
    ln->frac_capacity = 0;
    ln->whole_digits = NULL;
    ln->whole_size = 0;
    ln->whole_capacity = 0;
}