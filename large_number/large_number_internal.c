#include "large_number_internal.h"

#include <stdio.h>

static inline cs_codes large_number_check_memory(large_number *ln, int cap)
{
    if (cap > ln->cap)
    {
        int old_cap = ln->cap;
        ln->cap = cap + INIT_CAPACITY;
        ln->vec = realloc(ln->vec, sizeof(long) * ln->cap);
        if (!ln->vec)
            return CS_MEM;
        memset(ln->vec + old_cap, 0, sizeof(long) * (ln->cap - old_cap));
    }
    return CS_SUCCESS;
}

cs_codes large_number_add_helper(large_number *ln1, large_number ln2)
{
    int temp = 0, i, rc, exponent_diff;

    rc = large_number_check_memory(ln1, ln2.size);
    if (rc != CS_SUCCESS)
        return rc;

    exponent_diff = ln2.exponent - ln1->exponent;

    if (exponent_diff > 0)
    {
        rc = large_number_check_memory(ln1, ln1->size + exponent_diff);
        if (rc != CS_SUCCESS)
            return rc;
        for (i = ln1->size - 1; i >= 0; i--)
            ln1->vec[i + exponent_diff] = ln1->vec[i];
        for (i = 0; i < exponent_diff; i++)
            ln1->vec[i] = 0;
        ln1->size += exponent_diff;
        ln1->exponent = ln2.exponent;
    }
    exponent_diff = ln1->exponent - ln2.exponent;
    for (i = exponent_diff; i - exponent_diff < ln2.size && i < ln1->size; i++)
    {
        if (i == ln1->cap)
        {
            rc = large_number_check_memory(ln1, i + 1);
            if (rc != CS_SUCCESS)
                return rc;
        }
        ln1->vec[i] += ln2.vec[i - exponent_diff] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
    }
    for (; i - exponent_diff < ln2.size; i++)
    {
        if (i == ln1->cap)
        {
            rc = large_number_check_memory(ln1, i + 1);
            if (rc != CS_SUCCESS)
                return rc;
        }
        ln1->vec[i] = ln2.vec[i - exponent_diff] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
    }
    for (; temp > 0 || i < ln1->size; i++)
    {
        if (i == ln1->cap)
        {
            rc = large_number_check_memory(ln1, i + 1);
            if (rc != CS_SUCCESS)
                return rc;
        }
        ln1->vec[i] += temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
    }
    ln1->size = i;

    return CS_SUCCESS;
}

cs_codes large_number_minus_helper(large_number *ln1, large_number ln2)
{
    int temp = 0, i, rc, comp, exponent_diff;

    comp = large_number_compare(*ln1, ln2);
    exponent_diff = ln1->exponent - ln2.exponent;

    if (exponent_diff < 0)
    {
        rc = large_number_check_memory(ln1, ln1->size - exponent_diff);
        if (rc != CS_SUCCESS)
            return rc;
        for (i = ln1->size - 1; i >= 0; i--)
            ln1->vec[i - exponent_diff] = ln1->vec[i];
        for (i = -1 - exponent_diff; i >= 0; i--)
            ln1->vec[i] = 0;
        ln1->size -= exponent_diff;
        ln1->exponent -= exponent_diff;
        exponent_diff = 0;
    }

    if (comp > 0)
    {
        for (i = exponent_diff; i - exponent_diff < ln2.size; i++)
        {
            ln1->vec[i] -= ln2.vec[i - exponent_diff] + temp;
            temp = 0;
            if (ln1->vec[i] < 0)
            {
                temp = 1;
                ln1->vec[i] += 10;
            }
        }
        for (; temp > 0; i++)
        {
            ln1->vec[i] -= temp;
            temp = 0;
            if (ln1->vec[i] < 0)
            {
                temp = 1;
                ln1->vec[i] += 10;
            }
        }
    }
    else if (comp < 0)
    {
        ln1->sign *= NEGATIVE;
        for (i = 0; i < exponent_diff; i++)
        {
            ln1->vec[i] = 0 - ln1->vec[i] - temp;
            temp = 0;
            if (ln1->vec[i] < 0)
            {
                temp = 1;
                ln1->vec[i] += 10;
            }
        }
        for (i = exponent_diff; i < ln2.size + exponent_diff; i++)
        {
            ln1->vec[i] = ln2.vec[i - exponent_diff] - ln1->vec[i] - temp;
            temp = 0;
            if (ln1->vec[i] < 0)
            {
                temp = 1;
                ln1->vec[i] += 10;
            }
        }
    }
    else
    {
        memset(ln1->vec, 0, sizeof(long) * ln1->cap);
        ln1->size = 0;
    }

    while (ln1->size > 0 && ln1->vec[ln1->size - 1] == 0)
        ln1->size--;

    return CS_SUCCESS;
}

void *large_number_mul_helper_thread(void *arg)
{
    // LOCAL VARIABLES
    large_number_thread_arg_t ln_thread_arg = *(large_number_thread_arg_t *)arg;
    int i, j, k, jmax, jmin;
    int *max_size = malloc(sizeof(int));
    *max_size = 0;

    for (i = ln_thread_arg.thread_id; i < ln_thread_arg.ln1->size + ln_thread_arg.ln2->size; i += ln_thread_arg.thread_count)
    {
        ln_thread_arg.ln1->aux1->vec[i] = 0;
        jmax = ln_thread_arg.ln1->size - 1;
        if (jmax > i)
            jmax = i;
        jmin = 0;
        if (i - ln_thread_arg.ln2->size + 1 > 0)
            jmin = i - ln_thread_arg.ln2->size + 1;
        for (j = jmin; j <= jmax; j++)
        {
            k = i - j;
            *max_size = *max_size > i ? *max_size : i;
            ln_thread_arg.ln1->aux1->vec[i] += ln_thread_arg.ln1->vec[j] * ln_thread_arg.ln2->vec[k];
        }
    }
    (*max_size)++;

    return (void *)max_size;
}

cs_codes large_number_mul_helper(large_number *ln1, large_number ln2)
{
    // LOCAL VARIABLES
    long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN) < ln1->size ? sysconf(_SC_NPROCESSORS_ONLN) : ln1->size;
    pthread_t tid[number_of_processors];
    large_number_thread_arg_t ln_thread_args[number_of_processors];
    int i, temp, *ret_value;

    if (!ln1->aux1)
    {
        ln1->aux1 = malloc(sizeof(large_number));
        if (!ln1->aux1)
            return CS_MEM;
        large_number_init(ln1->aux1, LN_NULL);
    }
    if (ln1->aux1->cap < ln1->size + ln2.size)
    {
        ln1->aux1->cap = ln1->size + ln2.size + LARGE_NUMBER_INIT_CAPACITY;
        ln1->aux1->vec = realloc(ln1->aux1->vec, sizeof(long) * ln1->aux1->cap);
        if (!ln1->aux1)
            return CS_MEM;
    }
    memset(ln1->aux1->vec, 0, sizeof(long) * ln1->aux1->cap);
    for (i = 0; i < number_of_processors; i++)
    {
        ln_thread_args[i].ln1 = ln1;
        ln_thread_args[i].ln2 = &ln2;
        ln_thread_args[i].thread_id = i;
        ln_thread_args[i].thread_count = number_of_processors;
        pthread_create(&tid[i], NULL, large_number_mul_helper_thread, &ln_thread_args[i]);
    }

    ln1->aux1->size = 0;

    for (i = 0; i < number_of_processors; i++)
    {
        pthread_join(tid[i], (void *)&ret_value);
        ln1->aux1->size = ln1->aux1->size > *ret_value ? ln1->aux1->size : *ret_value;
        free(ret_value);
    }

    for (temp = 0, i = 0; temp > 0 || i < ln1->aux1->size; i++)
    {
        ln1->aux1->vec[i] += temp;
        temp = ln1->aux1->vec[i] / 10;
        ln1->aux1->vec[i] %= 10;
        if (i == ln1->aux1->cap)
        {
            ln1->aux1->cap = ln1->aux1->size + LARGE_NUMBER_INIT_CAPACITY;
            ln1->aux1 = realloc(ln1->aux1, sizeof(long) * ln1->aux1->cap);
            if (!ln1->aux1)
                return CS_MEM;
        }
    }

    ln1->aux1->size = ln1->aux1->size > i ? ln1->aux1->size : i;
    ln1->aux1->exponent = ln1->exponent + ln2.exponent;
    ln1->aux1->sign = ln1->sign * ln2.sign;

    large_number_swap(ln1, ln1->aux1);
    memset(ln1->aux1->vec, 0, sizeof(long) * ln1->aux1->cap);

    return CS_SUCCESS;
}

cs_codes large_number_div_helper(large_number *ln1, large_number ln2, int accuracy)
{
    int rc, i, comp, exponent_started;

    if (ln2.size == 0 && ln2.vec[0] == 0)
        return CS_ELEM;

    if (!ln1->aux1)
    {
        ln1->aux1 = malloc(sizeof(large_number));
        if (!ln1->aux1)
            return CS_MEM;
        large_number_init(ln1->aux1, LN_NULL);
    }
    if (ln1->aux1->cap < ln1->size + ln2.size)
    {
        ln1->aux1->cap = ln1->size + ln2.size + LARGE_NUMBER_INIT_CAPACITY;
        ln1->aux1->vec = realloc(ln1->aux1->vec, sizeof(long) * ln1->aux1->cap);
        if (!ln1->aux1->vec)
            return CS_MEM;
    }
    large_number_assign(ln1->aux1, LN_NULL);

    if (!ln1->aux2)
    {
        ln1->aux2 = malloc(sizeof(large_number));
        if (!ln1->aux2)
            return CS_MEM;
        large_number_init(ln1->aux2, LN_NULL);
    }
    if (ln1->aux2->cap < ln1->size + ln2.size)
    {
        ln1->aux2->cap = ln1->size + ln2.size + LARGE_NUMBER_INIT_CAPACITY;
        ln1->aux2->vec = realloc(ln1->aux2->vec, sizeof(long) * ln1->aux2->cap);
        if (!ln1->aux2->vec)
            return CS_MEM;
    }

    if (!ln1->quotient)
    {
        ln1->quotient = malloc(sizeof(large_number));
        if (!ln1->quotient)
            return CS_MEM;
        large_number_init(ln1->quotient, LN_NULL);
    }
    else
        large_number_assign(ln1->quotient, LN_NULL);

    for (i = 0; i < ln2.size; i++)
    {
        ln1->aux1->vec[ln2.size - 1 - i] = ln1->vec[ln1->size - 1];
        ln1->size--;
    }
    ln1->aux1->size = ln2.size;
    ln1->aux1->exponent = 0;
    ln1->aux1->sign = ln1->sign * ln2.sign;

    exponent_started = 0;
    while (accuracy > 0 && (ln1->aux1->size > 0 || ln1->size > 0))
    {
        comp = large_number_compare(*ln1->aux1, ln2);
        if (comp >= 0)
        {
            rc = large_number_assign(ln1->aux2, LN_NUM, ln2);
            if (rc != CS_SUCCESS)
                return rc;
            for (i = 0; large_number_compare(*ln1->aux1, *ln1->aux2) > 0; i++)
            {
                rc = large_number_add(ln1->aux2, *ln1->aux2, ln2);
                if (rc != CS_SUCCESS)
                    return rc;
            }
            large_number_append(ln1->quotient, LN_SCALE, i);
            if (exponent_started)
            {
                ln1->quotient->exponent++;
                accuracy--;
            }
            large_number_minus(ln1->aux2, *ln1->aux2, ln2);
            large_number_minus(ln1->aux1, *ln1->aux1, *ln1->aux2);
        }
        else if (ln1->size > 0)
        {
            large_number_append(ln1->aux1, LN_SCALE, ln1->vec[--ln1->size]);
        }
        else
        {
            large_number_append(ln1->aux1, LN_CHAR, "0");
            exponent_started = 1;
        }
    }

    large_number_swap(ln1, ln1->quotient);
    memset(ln1->quotient->vec, 0, sizeof(long) * ln1->aux1->cap);

    return CS_SUCCESS;
}