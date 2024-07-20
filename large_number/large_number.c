#include "large_number_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

cs_codes large_number_init(large_number *ln, ln_type type, ...)
{
    ln->size = 0;
    ln->cap = 100;
    ln->aux = NULL;
    ln->vec = malloc(sizeof(long) * ln->cap);
    if (!ln->vec)
        return CS_MEM;
    va_list arg;
    va_start(arg, 1);
    if (type == LN_NULL)
    {
        ln->vec[0] = 0;
        ln->size = 0;
    }
    else if (type == LN_SCALE)
    {
        long scale = va_arg(arg, long);
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
        ln->vec[0] = 0;
        ln->size = 0;
        return CS_SUCCESS;
    }
    va_list arg;
    va_start(arg, 1);
    if (type == LN_SCALE)
    {
        long scale = va_arg(arg, long);
        int count = 0;
        for (long copy = scale; copy != 0; copy /= 10)
            count++;
        if (count > ln->cap)
        {
            ln->cap = count * 3 / 2;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        for (int i = 0; scale != 0; scale /= 10, i++)
            ln->vec[i] = scale % 10;
        ln->size = count;
    }
    else if (type == LN_NUM)
    {
        large_number src = va_arg(arg, large_number);
        if (src.size > ln->cap)
        {
            ln->cap = src.cap;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        ln->size = src.size;
        for (int i = 0; i < src.size; i++)
            ln->vec[i] = src.vec[i];
    }
    else if (type == LN_CHAR)
    {
        char *str = va_arg(arg, char *);
        ln->size = strlen(str);
        if (ln->size > ln->cap)
        {
            ln->cap = ln->size + 10;
            ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
            if (!ln->vec)
                return CS_MEM;
        }
        for (int i = ln->size - 1; i >= 0; i--)
            ln->vec[ln->size - i - 1] = str[i] - '0';
    }
    va_end(arg);
    return CS_SUCCESS;
}

cs_codes large_number_add(large_number *dest, int ln_count, ...)
{
    va_list arg;
    int rc;
    va_start(arg, ln_count);

    large_number aux = va_arg(arg, large_number);
    large_number_assign(dest, LN_NUM, aux);

    for (int i = 1; i < ln_count; i++)
    {
        aux = va_arg(arg, large_number);
        rc = large_number_add_helper(dest, aux);
        if (rc != CS_SUCCESS)
            return rc;
    }

    va_end(arg);
    return CS_SUCCESS;
}

cs_codes large_number_mul(large_number *dest, int ln_count, ...)
{
    va_list arg;
    int rc;
    va_start(arg, ln_count);

    large_number aux = va_arg(arg, large_number);
    large_number_assign(dest, LN_NUM, aux);

    for (int i = 1; i < ln_count; i++)
    {
        aux = va_arg(arg, large_number);
        rc = large_number_mul_helper(dest, aux);
        if (rc != CS_SUCCESS)
            return rc;
    }

    va_end(arg);
    return CS_SUCCESS;
}

void large_number_swap(large_number *ln1, large_number *ln2)
{
    int cap = ln1->cap;
    int size = ln1->size;
    long *vec = ln1->vec;

    ln1->cap = ln2->cap;
    ln1->size = ln2->size;
    ln1->vec = ln2->vec;

    ln2->cap = cap;
    ln2->size = size;
    ln2->vec = vec;
}

void large_number_free(large_number *ln)
{
    if (ln->aux)
    {
        free(ln->aux->vec);
        free(ln->aux);
    }
    free(ln->vec);
}

void large_number_print(large_number ln)
{
    for (int i = ln.size - 1; i >= 0; i--)
    {
        printf("%d", ln.vec[i]);
        if (i % 3 == 0 && i != 0)
            printf(",");
    }
    printf("\n");
}