#include "large_number_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

cs_codes large_number_init(large_number *ln, ln_type type, ...)
{
    int rc;

    ln->size = 0;
    ln->exponent = 0;
    ln->sign = POSITIVE;
    ln->cap = _LN_INIT_CAPACITY;
    ln->base = _LN_INIT_BASE;
    ln->aux1 = NULL;
    ln->aux2 = NULL;
    ln->aux3 = NULL;
    ln->vec = malloc(sizeof(long) * ln->cap);
    memset(ln->vec, 0, sizeof(long) * ln->cap);
    if (!ln->vec)
        return CS_MEM;
    va_list arg;
    va_start(arg, 1);

    switch (type)
    {
    case LN_SCALE:
        double scale = va_arg(arg, double);
        rc = large_number_assign(ln, type, scale);
        break;
    case LN_NUM:
        large_number aux = va_arg(arg, large_number);
        rc = large_number_assign(ln, type, aux);
        break;
    case LN_CHAR:
        char *str = va_arg(arg, char *);
        rc = large_number_assign(ln, type, str);
        break;
    default:
        rc = CS_ELEM;
        break;
    }

    va_end(arg);
    return rc;
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
    va_start(arg, _LN_ASSIGN_ARGS_CNT);

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

    va_start(arg, _LN_APPEND_ARGS_CNT);

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

cs_codes large_number_convert(large_number ln, void *ptr, ln_conversion_type type)
{
    int i, rc;

    if (ln.size > 20)
        return CS_SIZE;

    switch (type)
    {
    case LN_CONV_LONG:
        long *scale = malloc(sizeof(long));
        for (i = ln.exponent; i < ln.size; i++)
            *scale = *scale * 10 + ln.vec[i];
        ptr = scale;
        rc = CS_SUCCESS;
        break;
    case LN_CONV_DOUBLE:
        double *scale = malloc(sizeof(double));
        for (i = 0; i < ln.exponent; i++)
            *scale = *scale / 10 + ln.vec[i] / 10;
        for (i = ln.exponent; i < ln.size; i++)
            *scale = *scale * 10 + ln.vec[i];
        ptr = scale;
        rc = CS_SUCCESS;
        break;
    case LN_CONV_CHAR:
        char has_comma = (ln.exponent > 0);
        char *str = malloc(ln.size + 1 + has_comma);
        for (i = ln.size - 1; i >= ln.exponent; i--)
            str[ln.size - 1 - i + has_comma] = ln.vec[i] + '0';
        if (has_comma)
        {
            str[ln.exponent] = '.';
            for (i = ln.exponent - 1; i >= 0; i--)
                str[ln.size - 1 - i] = ln.vec[i] + '0';
        }
        ptr = str;
        rc = CS_SUCCESS;
    default:
        rc = CS_ELEM;
        break;
    }
    return rc;
}

cs_codes large_number_add(large_number *dest, large_number ln1, large_number ln2)
{
    int rc = large_number_assign(dest, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    if (ln1.sign * ln2.sign == NEGATIVE)
        return large_number_minus(dest, ln1, ln2);

    return large_number_add_helper(dest, ln2);
}

cs_codes large_number_minus(large_number *dest, large_number ln1, large_number ln2)
{
    int rc = large_number_assign(dest, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    if (ln1.sign * ln2.sign == NEGATIVE)
        return large_number_add_helper(dest, ln2);
    return large_number_minus_helper(dest, ln2);
}

cs_codes large_number_mul(large_number *dest, large_number ln1, large_number ln2)
{
    int rc = large_number_assign(dest, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    return large_number_mul_helper(dest, ln2);
}

cs_codes large_number_div_num(large_number ln1, large_number ln2, int accuracy, ln_div_type type, ...)
{
    va_list arg;
    int rc;

    va_start(arg, type);
    large_number *quotient = va_arg(arg, large_number *);
    rc = large_number_assign(quotient, LN_NUM, ln1);
    if (rc != CS_SUCCESS)
        return rc;

    switch (type)
    {
    case LN_QUOTIENT:
        rc = large_number_div_helper(quotient, NULL, ln2, accuracy);
        break;
    case LN_REST:
        large_number *rest = va_arg(arg, large_number *);
        rc = large_number_div_helper(quotient, rest, ln2, accuracy);
        break;
    default:
        return CS_ELEM;
        break;
    }

    va_end(arg);

    return rc;
}

cs_codes large_number_div_scale(large_number ln, long scale, int accuracy, ln_div_type type, ...)
{
    va_list arg;
    int rc;

    va_start(arg, type);
    large_number *quotient = va_arg(arg, large_number *);
    rc = large_number_assign(quotient, LN_NUM, ln);
    if (rc != CS_SUCCESS)
        return rc;

    switch (type)
    {
    case LN_QUOTIENT:
        rc = large_number_div_helper(quotient, NULL, ln, accuracy);
        break;
    case LN_REST:
        long *rest_scale = va_arg(arg, long *);
        large_number rest;
        rc = large_number_div_helper(quotient, &rest, ln, accuracy);
        break;
    default:
        return CS_ELEM;
        break;
    }

    va_end(arg);

    return rc;
}

cs_codes large_number_int(large_number *dest, large_number ln)
{
    int i;
    long *ptr;

    if (dest->cap < ln.size - ln.exponent)
    {
        dest->cap = ln.size - ln.exponent + _LN_INIT_CAPACITY;
        ptr = realloc(dest->vec, sizeof(long) * dest->cap);
        if (!ptr)
            return CS_MEM;
        dest->vec = ptr;
    }

    for (int i = ln.exponent; i < ln.size; i++)
    {
        dest->vec[i - ln.exponent] = ln.vec[i];
    }
    if (dest->size > ln.size - ln.exponent)
        memset(dest->vec + ln.size - ln.exponent, 0, sizeof(long) * (dest->size - ln.size + ln.exponent));
    dest->size = ln.size - ln.exponent;
    dest->sign = ln.sign;
    dest->exponent = 0;
    dest->base = ln.base;

    return CS_SUCCESS;
}

cs_codes large_number_fraction(large_number *dest, large_number ln)
{
    int i;
    long *ptr;

    if (dest->cap < ln.exponent)
    {
        dest->cap = ln.exponent + _LN_INIT_CAPACITY;
        ptr = realloc(dest->vec, sizeof(long) * dest->cap);
        if (!ptr)
            return CS_MEM;
        dest->vec = ptr;
    }

    for (int i = 0; i < ln.exponent; i++)
        dest->vec[i] = ln.vec[i];
    if (dest->size > ln.exponent)
        memset(dest->vec + ln.exponent, 0, sizeof(long) * (dest->size - ln.exponent));
    dest->size = ln.exponent;
    dest->sign = ln.sign;
    dest->exponent = ln.exponent;
    dest->base = ln.base;

    return CS_SUCCESS;
}

cs_codes large_number_set_base(large_number *ln, int base)
{
    if (base == ln->base)
        return CS_SUCCESS;

    ///////////////////////////////// LN1->AUX1 INIT /////////////////////////////////

    if (!ln->aux1)
    {
        ln->aux1 = malloc(sizeof(large_number));
        if (!ln->aux1)
            return CS_MEM;
        large_number_init(ln->aux1, LN_NULL);
    }
    large_number_int(ln->aux1, *ln);

    ///////////////////////////////// LN1->AUX2 INIT /////////////////////////////////

    if (!ln->aux2)
    {
        ln->aux2 = malloc(sizeof(large_number));
        if (!ln->aux2)
            return CS_MEM;
        large_number_init(ln->aux2, LN_NULL);
    }
    large_number_fraction(ln->aux2, *ln);

    ///////////////////////////////// LN1->AUX3 INIT /////////////////////////////////

    if (!ln->aux3)
    {
        ln->aux3 = malloc(sizeof(large_number));
        if (!ln->aux3)
            return CS_MEM;
        large_number_init(ln->aux3, LN_NULL);
    }

    ///////////////////////////////// BASE CHANGE ALGORITHM /////////////////////////////////

    return CS_SUCCESS;
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
        return -ln2.vec[exponent_diff - 1];
    }
    else if (ln1.exponent > ln2.exponent)
    {
        exponent_diff = ln1.exponent - ln2.exponent;
        for (int i = ln1.size - 1; i >= 0; i--)
            if (ln1.vec[i + exponent_diff] != ln2.vec[i])
                return ln1.vec[i + exponent_diff] - ln2.vec[i];
        return ln1.vec[exponent_diff - 1];
    }
    else
    {
        for (int i = ln1.size - 1; i >= 0; i--)
            if (ln1.vec[i] != ln2.vec[i])
                return ln1.vec[i] - ln2.vec[i];
    }

    return 0;
}

void large_number_negative(large_number *ln1) { ln1->sign *= NEGATIVE; }

void large_number_swap(large_number *ln1, large_number *ln2)
{
    int cap = ln1->cap;
    int size = ln1->size;
    int exponent = ln1->exponent;
    int base = ln1->base;
    char sign = ln1->sign;
    long *vec = ln1->vec;

    ln1->cap = ln2->cap;
    ln1->size = ln2->size;
    ln1->exponent = ln2->exponent;
    ln1->base = ln2->base;
    ln1->sign = ln2->sign;
    ln1->vec = ln2->vec;

    ln2->cap = cap;
    ln2->size = size;
    ln2->exponent = exponent;
    ln2->base = base;
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
    if (ln->aux3)
    {
        free(ln->aux3->vec);
        free(ln->aux3);
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