#include "large_number_internal.h"

#include <stdio.h>

cs_codes large_number_add_helper(large_number *ln1, large_number ln2)
{
    if (ln2.size > ln1->cap)
    {
        ln1->cap = ln2.size + 10;
        ln1->vec = realloc(ln1->vec, sizeof(long) * ln1->cap);
        if (!ln1->vec)
            return CS_MEM;
    }

    int temp = 0, i;
    for (i = 0; i < ln1->size && i < ln2.size; i++)
    {
        ln1->vec[i] = ln1->vec[i] + ln2.vec[i] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
    }

    while (i < ln1->size)
    {
        ln1->vec[i] = ln1->vec[i] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
        i++;
    }

    while (i < ln2.size)
    {
        ln1->vec[i] = ln2.vec[i] + temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
        i++;
    }

    while (temp != 0)
    {
        ln1->vec[i] = temp;
        temp = ln1->vec[i] / 10;
        ln1->vec[i] %= 10;
        i++;
    }

    ln1->size = i;

    return CS_SUCCESS;
}

void *large_number_mul_helper_thread(void *arg)
{
    // LOCAL VARIABLES
    large_number_thread_arg_t ln_thread_arg = *(large_number_thread_arg_t *)arg;
    int i, j;

    for (i = ln_thread_arg.thread_id; i < ln_thread_arg.ln1->size - ln_thread_arg.ln1->size % ln_thread_arg.thread_count; i += ln_thread_arg.thread_count)
    {
        for (j = 0; j < ln_thread_arg.ln2->size; j++)
        {
            ln_thread_arg.ln1->aux->vec[i + j] += ln_thread_arg.ln1->vec[i] * ln_thread_arg.ln2->vec[j];
            pthread_barrier_wait(ln_thread_arg.barrier);
        }
    }

    i = ln_thread_arg.ln1->size - ln_thread_arg.ln1->size % ln_thread_arg.thread_count + ln_thread_arg.thread_id;
    if (i < ln_thread_arg.ln1->size)
    {
        for (j = 0; j < ln_thread_arg.ln2->size; j++)
        {
            ln_thread_arg.ln1->aux->vec[i + j] += ln_thread_arg.ln1->vec[i] * ln_thread_arg.ln2->vec[j];
            pthread_barrier_wait(ln_thread_arg.last_barrier);
        }
    }

    return NULL;
}

cs_codes large_number_mul_helper(large_number *ln1, large_number ln2)
{
    // LOCAL VARIABLES
    long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN) < ln1->size ? sysconf(_SC_NPROCESSORS_ONLN) : ln1->size;
    pthread_t tid[number_of_processors];
    large_number_thread_arg_t ln_thread_args[number_of_processors];
    pthread_barrier_t barrier, last_barrier;
    int i, temp, last_barrier_threads = ln1->size % number_of_processors;

    if (!ln1->aux)
    {
        ln1->aux = malloc(sizeof(large_number));
        if (!ln1->aux)
            return CS_MEM;
        large_number_init(ln1->aux, LN_NULL);
    }
    if (ln1->aux->cap < ln1->size + ln2.size)
    {
        ln1->aux->cap = ln1->size + ln2.size + LARGE_NUMBER_INIT_CAPACITY;
        ln1->aux->vec = realloc(ln1->aux->vec, sizeof(long) * ln1->aux->cap);
        if (!ln1->aux)
            return CS_MEM;
    }
    memset(ln1->aux->vec, 0, sizeof(long) * ln1->aux->cap);

    pthread_barrier_init(&barrier, NULL, number_of_processors);
    if (last_barrier_threads)
        pthread_barrier_init(&last_barrier, NULL, last_barrier_threads);

    for (i = 0; i < number_of_processors; i++)
    {
        ln_thread_args[i].ln1 = ln1;
        ln_thread_args[i].ln2 = &ln2;
        ln_thread_args[i].thread_id = i;
        ln_thread_args[i].thread_count = number_of_processors;
        ln_thread_args[i].barrier = &barrier;
        ln_thread_args[i].last_barrier = &last_barrier;
        pthread_create(&tid[i], NULL, large_number_mul_helper_thread, &ln_thread_args[i]);
    }

    for (i = 0; i < number_of_processors; i++)
        pthread_join(tid[i], NULL);

    pthread_barrier_destroy(&barrier);
    if (last_barrier_threads)
        pthread_barrier_destroy(&last_barrier);

    ln1->aux->size = 0;
    temp = ln1->aux->vec[ln1->aux->size] / 10;
    ln1->aux->vec[ln1->aux->size] %= 10;
    ln1->aux->size++;
    while (temp > 0 || ln1->aux->size < ln1->size + ln2.size)
    {
        ln1->aux->vec[ln1->aux->size] += temp;
        temp = ln1->aux->vec[ln1->aux->size] / 10;
        ln1->aux->vec[ln1->aux->size] %= 10;
        ln1->aux->size++;
        if (ln1->aux->size == ln1->aux->cap)
        {
            ln1->aux->cap = ln1->aux->size + LARGE_NUMBER_INIT_CAPACITY;
            ln1->aux = realloc(ln1->aux, sizeof(long) * ln1->aux->cap);
            if (!ln1->aux)
                return CS_MEM;
        }
    }

    large_number_swap(ln1, ln1->aux);

    return CS_SUCCESS;
}