#include <cs/large_number.h>

#include <string.h>
#include <stdlib.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


cs_codes large_number_sub_helper(large_number *out, const large_number big, large_number small) {
    int i, diff, borrow = 0;
    out->sign = big.sign;
    out->base = big.base;
    for (i = 0; i < small.size; i++) {
        diff = big.digits[i] - small.digits[i] - borrow;
        if (diff < 0) {
            diff += big.base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        out->digits[i] = diff;
        out->size++;
    }

    for (; i < big.size; i++) {
        diff = big.digits[i] - borrow;
        if (diff < 0) {
            diff += big.base;
            borrow = 1;
        } else {
            borrow = 0;
        }
        out->digits[i] = diff;
        out->size++;
    }

    while (out->size > 0 && out->digits[out->size - 1] == 0) {
        out->size--;
    }

    return CS_SUCCESS;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes large_number_init(large_number *ln, unsigned int base, large_number_type type, ...) {
    CS_RETURN_IF(NULL == ln, CS_NULL);
    ln->digits = malloc(sizeof(unsigned int) * INIT_CAPACITY);
    CS_RETURN_IF(NULL == ln->digits, CS_MEM);
    memset(ln->digits, 0, sizeof(unsigned int) * INIT_CAPACITY);
    ln->size = 0;
    ln->capacity = INIT_CAPACITY;
    ln->sign = __POSITIVE_SIGN;
    ln->base = base;

    // Handle variable arguments based on type
    va_list args;
    va_start(args, type);

    switch (type) {
        case LN_CHAR: {
            char *char_digits = va_arg(args, char*);
            int len = 0;
            for (; char_digits[len] != '\0'; len++);
            for (int i = len - 1; i >= 0; i--) {
                if (ln->size >= ln->capacity) {
                    unsigned int *new_digits = realloc(ln->digits, sizeof(unsigned int) * ln->capacity * 2);
                    CS_RETURN_IF(NULL == new_digits, CS_MEM);
                    ln->digits = new_digits;
                    memset(ln->digits + ln->capacity, 0, sizeof(unsigned int) * ln->capacity);
                    ln->capacity *= 2;
                }
                ln->digits[ln->size++] = (unsigned int)(char_digits[i] - '0');
            }
            ln->sign = __POSITIVE_SIGN;
            break;
        }
        case LN_INT: {
            long number = va_arg(args, long);
            ln->sign = __POSITIVE_SIGN;
            if (number < 0) {
                ln->sign = __NEGATIVE_SIGN;
                number = -number;
            }
            do {
                if (ln->size >= ln->capacity) {
                    unsigned int *new_digits = realloc(ln->digits, sizeof(unsigned int) * ln->capacity * 2);
                    CS_RETURN_IF(NULL == new_digits, CS_MEM);
                    ln->digits = new_digits;
                    memset(ln->digits + ln->capacity, 0, sizeof(unsigned int) * ln->capacity);
                    ln->capacity *= 2;
                }
                ln->digits[ln->size++] = number % base;
                number /= base;
            } while (number > 0);
            break;
        }
        case LN_NUM: {
            large_number source = va_arg(args, large_number);
            for (int i = 0; i < source.size; i++) {
                if (i >= ln->capacity) {
                    unsigned int *new_digits = realloc(ln->digits, sizeof(unsigned int) * ln->capacity * 2);
                    CS_RETURN_IF(NULL == new_digits, CS_MEM);
                    ln->digits = new_digits;
                    memset(ln->digits + ln->capacity, 0, sizeof(unsigned int) * ln->capacity);
                    ln->capacity *= 2;
                }
                ln->digits[i] = source.digits[i];
            }
            ln->size = source.size;
            ln->sign = source.sign;
            ln->base = source.base;
            break;
        }
        case LN_EMPTY: {
            // Do nothing, already initialized as empty
            break;
        }
    }

    va_end(args);

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
    out->size = 0;
    for (; out->size < a.size && out->size < b.size; out->size++) {
        sum = a.digits[out->size] + b.digits[out->size];
        sum += carry;

        carry = sum / a.base;
        sum = sum % a.base;

        if (out->size >= out->capacity) {
            unsigned int *new_digits = realloc(out->digits, sizeof(unsigned int) * out->capacity * 2);
            CS_RETURN_IF(NULL == new_digits, CS_MEM);
            out->digits = new_digits;
            memset(out->digits + out->capacity, 0, sizeof(unsigned int) * out->capacity);
            out->capacity *= 2;
        }
        out->digits[out->size] = sum;
    }

    while (out->size < a.size) {
        sum = a.digits[out->size] + carry;

        carry = sum / a.base;
        sum = sum % a.base;

        if (out->size >= out->capacity) {
            unsigned int *new_digits = realloc(out->digits, sizeof(unsigned int) * out->capacity * 2);
            CS_RETURN_IF(NULL == new_digits, CS_MEM);
            out->digits = new_digits;
            memset(out->digits + out->capacity, 0, sizeof(unsigned int) * out->capacity);
            out->capacity *= 2;
        }
        out->digits[out->size++] = sum;
    }

    while (out->size < b.size) {
        sum = b.digits[out->size] + carry;

        carry = sum / b.base;
        sum = sum % b.base;

        if (out->size >= out->capacity) {
            unsigned int *new_digits = realloc(out->digits, sizeof(unsigned int) * out->capacity * 2);
            CS_RETURN_IF(NULL == new_digits, CS_MEM);
            out->digits = new_digits;
            memset(out->digits + out->capacity, 0, sizeof(unsigned int) * out->capacity);
            out->capacity *= 2;
        }
        out->digits[out->size++] = sum;
    }

    while (carry != 0) {
        sum = carry;
        carry = sum / a.base;
        sum = sum % a.base;

        if (out->size >= out->capacity) {
            unsigned int *new_digits = realloc(out->digits, sizeof(unsigned int) * out->capacity * 2);
            CS_RETURN_IF(NULL == new_digits, CS_MEM);
            out->digits = new_digits;
            memset(out->digits + out->capacity, 0, sizeof(unsigned int) * out->capacity);
            out->capacity *= 2;
        }
        out->digits[out->size++] = sum;
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
        out->size = 1;
        memset(out->digits, 0, sizeof(unsigned int) * out->capacity);
        out->sign = __POSITIVE_SIGN;
        out->base = a.base;
        return CS_SUCCESS;
    }
    return CS_SUCCESS;
}

cs_codes large_number_switch_base(large_number *ln, unsigned int new_base) {
    CS_RETURN_IF(NULL == ln, CS_NULL);
    CS_RETURN_IF(new_base < 2, CS_SIZE);
    CS_RETURN_IF(ln->base == new_base, CS_SUCCESS);

    large_number result, temp;
    large_number_init(&result, new_base, LN_EMPTY);
    large_number_init(&temp, ln->base, LN_NUM, *ln);
    
    while (temp.size > 0) {
        unsigned int remainder = 0;

        for (int i = temp.size - 1; i >= 0; i--) {
            unsigned long long current = 1ull * remainder * temp.base + temp.digits[i];
            temp.digits[i] = (unsigned int)(current / new_base);
            remainder = current % new_base;
        }

        if (result.size >= result.capacity) {
            unsigned int *new_digits = realloc(result.digits, sizeof(unsigned int) * result.capacity * 2);
            CS_RETURN_IF(NULL == new_digits, CS_MEM);
            result.digits = new_digits;
            memset(result.digits + result.capacity, 0, sizeof(unsigned int) * result.capacity);
            result.capacity *= 2;
        }
        result.digits[result.size++] = remainder;

        while (temp.size > 0 && temp.digits[temp.size - 1] == 0) {
            temp.size--;
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
    if (a.size != b.size) {
        return a.size - b.size;
    }
    for (int i = a.size - 1; i >= 0; i--) {
        if (a.digits[i] != b.digits[i]) {
            return a.digits[i] - b.digits[i];
        }
    }
    return 0;
}

void large_number_clear(large_number *ln) {
    CS_RETURN_IF(NULL == ln);
    memset(ln->digits, 0, sizeof(unsigned int) * ln->capacity);
    ln->size = 0;
    ln->sign = __POSITIVE_SIGN;
}

void large_number_print(FILE *stream, void *v_ln) {
    CS_RETURN_IF(NULL == v_ln);
    large_number *ln = (large_number *)v_ln;
    if (ln->sign == __NEGATIVE_SIGN) {
        fprintf(stream, "-");
    }
    if (ln->size == 0) {
        fprintf(stream, "0");
        return;
    }
    for (int i = ln->size - 1; i >= 0; i--) {
        fprintf(stream, "%u", ln->digits[i]);
    }
}

void large_number_swap(large_number *a, large_number *b) {
    CS_RETURN_IF(NULL == a || NULL == b);
    
    unsigned int *temp_digits = a->digits;
    int temp_size = a->size;
    int temp_capacity = a->capacity;
    int temp_sign = a->sign;
    unsigned int temp_base = a->base;

    a->digits = b->digits;
    a->size = b->size;
    a->capacity = b->capacity;
    a->sign = b->sign;
    a->base = b->base;

    b->digits = temp_digits;
    b->size = temp_size;
    b->capacity = temp_capacity;
    b->sign = temp_sign;
    b->base = temp_base;
}

void large_number_free(void *v_ln) {
    CS_RETURN_IF(NULL == v_ln);
    large_number *ln = (large_number *)v_ln;
    free(ln->digits);
    ln->digits = NULL;
    ln->size = 0;
    ln->capacity = 0;
}