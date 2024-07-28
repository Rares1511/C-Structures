#include "large_number_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

cs_codes large_number_init(large_number *ln, ln_type type, ...)
{
    ln->size = 0;
    ln->exponent = 0;
    ln->sign = POSITIVE;
    ln->cap = 100;
    ln->aux1 = NULL;
    ln->aux2 = NULL;
    ln->quotient = NULL;
    ln->vec = malloc(sizeof(long) * ln->cap);
    memset(ln->vec, 0, sizeof(long) * ln->cap);
    if (!ln->vec)
        return CS_MEM;
    va_list arg;
    va_start(arg, 1);
    if (type == LN_SCALE)
    {
        double scale = va_arg(arg, double);
        large_number_assign(ln, type, scale);
    }
    else if (type == LN_NUM)
    {
        large_number aux = va_arg(arg, large_number);
        large_number_assign(ln, type, aux);
    }
    else if (type == LN_CHAR)
    {
        char *str = va_arg(arg, char *);
        large_number_assign(ln, type, str);
    }
    va_end(arg);
    return CS_SUCCESS;
}

cs_codes large_number_assign(large_number *ln, ln_type type, ...)
{
    if (type == LN_NULL)
    {
        memset(ln->vec, 0, sizeof(long) * ln->cap);
        ln->size = 0;
        ln->exponent = 0;
        ln->sign = POSITIVE;
        return CS_SUCCESS;
    }

    va_list arg;
    int i;
    va_start(arg, 1);

    if (type == LN_SCALE)
    {
        double scale = va_arg(arg, double);
        if (scale < 0)
        {
            ln->sign = NEGATIVE;
            scale *= -1;
        }
        else
            ln->sign = POSITIVE;
        long supraunitary, subunitary, copy;
        int count;

        supraunitary = (long)scale;
        scale -= supraunitary;
        while (scale - (long)scale > FLOATING_ERR)
            scale *= 10;
        subunitary = (long)scale;
        count = 0;
        copy = subunitary;
        while (copy > 0)
        {
            count++;
            copy /= 10;
        }
        copy = supraunitary;
        while (copy > 0)
        {
            count++;
            copy /= 10;
        }

        if (ln->cap < count)
        {
            ln->cap = count + INIT_CAPACITY;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }

        memset(ln->vec + count, 0, sizeof(long) * (ln->cap - count));

        while (subunitary > 0)
        {
            ln->vec[ln->size++] = subunitary % 10;
            subunitary /= 10;
        }
        ln->exponent = ln->size;
        while (supraunitary > 0)
        {
            ln->vec[ln->size++] = supraunitary % 10;
            supraunitary /= 10;
        }
    }
    else if (type == LN_NUM)
    {
        large_number src = va_arg(arg, large_number);
        if (ln->cap < src.size)
        {
            ln->cap = src.size + INIT_CAPACITY;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        memset(ln->vec + src.size, 0, sizeof(long) * (ln->cap - src.size));
        ln->size = src.size;
        ln->exponent = src.exponent;
        ln->sign = src.sign;
        for (i = 0; i < src.size; i++)
            ln->vec[i] = src.vec[i];
    }
    else if (type == LN_CHAR)
    {
        char *str = va_arg(arg, char *);
        int start = 0;
        ln->sign = POSITIVE;
        ln->size = strlen(str);
        if (str[0] == '-')
        {
            ln->sign = NEGATIVE;
            ln->size = strlen(str) - 1;
            start = 1;
        }
        else if (str[0] == '+')
        {
            start = 1;
            ln->size = strlen(str) - 1;
        }

        if (ln->cap < ln->size)
        {
            ln->cap = ln->size + INIT_CAPACITY;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        memset(ln->vec + strlen(str), 0, sizeof(long) * (ln->cap - strlen(str)));

        for (i = strlen(str) - 1; i >= start && str[i] != '.'; i--)
        {
            if (!(str[i] >= '0' && str[i] <= '9'))
                return CS_ELEM;
            ln->vec[ln->size - i - 1 + start] = str[i] - '0';
        }
        if (i >= 0)
        {
            ln->size--;
            ln->exponent = ln->size - i + start;
            i--;
        }
        for (; i >= start; i--)
        {
            if (!(str[i] >= '0' && str[i] <= '9'))
                return CS_ELEM;
            ln->vec[ln->size - i - 1 + start] = str[i] - '0';
        }
    }
    va_end(arg);
    return CS_SUCCESS;
}

cs_codes large_number_append(large_number *ln, ln_type type, ...)
{
    va_list arg;
    int i;

    va_start(arg, 1);

    switch (type)
    {
    case LN_NUM:
        large_number aux = va_arg(arg, large_number);
        if (ln->cap < ln->size + aux.size)
        {
            ln->cap = ln->size + aux.size + INIT_CAPACITY;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        memcpy(ln->vec + aux.size, ln->vec, sizeof(long) * ln->size);
        memcpy(ln->vec, aux.vec, sizeof(long) * aux.size);
        ln->size += aux.size;
        break;
    case LN_CHAR:
        char *str = va_arg(arg, char *);
        if (ln->cap < ln->size + strlen(str))
        {
            ln->cap = ln->size + strlen(str) + INIT_CAPACITY;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        memcpy(ln->vec + strlen(str), ln->vec, sizeof(long) * ln->size);
        for (i = 0; i < strlen(str); i++)
        {
            if (!(str[i] >= '0' && str[i] <= '9'))
                return CS_ELEM;
            ln->vec[i] = str[i] - '0';
        }
        ln->size += strlen(str);
        break;
    case LN_SCALE:
        long scale = va_arg(arg, long);
        long copy = scale;
        int count = 0;
        while (copy > 0)
        {
            count++;
            copy /= 10;
        }
        if (ln->cap < ln->size + count)
        {
            ln->cap = ln->size + count + INIT_CAPACITY;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        memcpy(ln->vec + count, ln->vec, sizeof(long) * ln->size);
        for (i = 0; scale > 0; scale /= 10, i++)
        {
            ln->vec[i] = scale % 10;
        }
        ln->size += count;
        break;
    default:
        return CS_ELEM;
        break;
    }

    va_end(arg);

    return CS_SUCCESS;
}

cs_codes large_number_add(large_number *dest, large_number ln1, large_number ln2)
{
    int rc;

    rc = large_number_assign(dest, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    if (ln1.sign * ln2.sign == POSITIVE)
        return large_number_add_helper(dest, ln2);
    return large_number_minus_helper(dest, ln2);
}

cs_codes large_number_minus(large_number *dest, large_number ln1, large_number ln2)
{
    int rc;

    rc = large_number_assign(dest, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    if (ln1.sign * ln2.sign == NEGATIVE)
        return large_number_add_helper(dest, ln2);
    return large_number_minus_helper(dest, ln2);
}

cs_codes large_number_mul(large_number *dest, large_number ln1, large_number ln2)
{
    int rc;

    rc = large_number_assign(dest, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    return large_number_mul_helper(dest, ln2);
}

cs_codes large_number_div(large_number *dest, large_number ln1, large_number ln2, int accuracy)
{
    int rc;

    rc = large_number_assign(dest, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    return large_number_div_helper(dest, ln2, accuracy);
}

int large_number_compare(large_number ln1, large_number ln2)
{
    int exponent_diff;

    if ((ln1.size - ln1.exponent) - (ln2.size - ln2.exponent) != 0)
        return (ln1.size - ln1.exponent) - (ln2.size - ln2.exponent);

    if (ln2.exponent > ln1.exponent)
    {
        exponent_diff = ln2.exponent - ln1.exponent;
        for (int i = ln1.size - 1; i >= 0; i--)
            if (ln1.vec[i] != ln2.vec[i + exponent_diff])
                return ln1.vec[i] - ln2.vec[i + exponent_diff];
    }
    else
    {
        exponent_diff = ln1.exponent - ln2.exponent;
        for (int i = ln2.size - 1; i >= 0; i--)
            if (ln1.vec[i + exponent_diff] != ln2.vec[i])
                return ln1.vec[i + exponent_diff] - ln2.vec[i];
    }

    return 0;
}

void large_number_negative(large_number *ln1) { ln1->sign *= NEGATIVE; }

void large_number_swap(large_number *ln1, large_number *ln2)
{
    int cap = ln1->cap;
    int size = ln1->size;
    int exponent = ln1->exponent;
    char sign = ln1->sign;
    long *vec = ln1->vec;

    ln1->cap = ln2->cap;
    ln1->size = ln2->size;
    ln1->exponent = ln2->exponent;
    ln1->sign = ln2->sign;
    ln1->vec = ln2->vec;

    ln2->cap = cap;
    ln2->size = size;
    ln2->exponent = exponent;
    ln2->sign = sign;
    ln2->vec = vec;
}

void large_number_free(large_number *ln)
{
    if (ln->aux1)
    {
        free(ln->aux1->vec);
        free(ln->aux1);
    }
    if (ln->aux2)
    {
        free(ln->aux2->vec);
        free(ln->aux2);
    }
    if (ln->quotient)
    {
        free(ln->quotient->vec);
        free(ln->quotient);
    }
    free(ln->vec);
}

void large_number_print(large_number ln)
{
    int i;
    if (ln.sign == NEGATIVE)
        printf("-");
    for (i = ln.size - 1; i >= ln.exponent; i--)
    {
        printf("%ld", ln.vec[i]);
        if ((ln.exponent - i) % 3 == 0 && (ln.exponent - i) != 0)
            printf(",");
    }
    if (ln.exponent > 0)
    {
        printf(".");
        for (i = ln.exponent - 1; i >= 0; i--)
        {
            printf("%ld", ln.vec[i]);
        }
    }
    printf("\n");
}