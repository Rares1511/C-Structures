#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/large_number.h"

#define BUFFER_SIZE 5000
#define TEST_SIZE 1

char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

int check(large_number ln)
{
    int temp = 0, x = 0;
    for (int i = 0; i < ln.size; i++)
    {
        x = temp;
        if (i < BUFFER_SIZE - 1)
            x += (buffer1[BUFFER_SIZE - i - 2] - '0') + (buffer2[BUFFER_SIZE - i - 2] - '0');
        temp = x / 10;
        x %= 10;
        if (x != ln.vec[i])
            return 0;
    }
    return 1;
}

int main()
{
    clock_t start;
    large_number ln1, ln2;
    int successes, fails, res;
    double time_taken;

    large_number_init(&ln1, LN_NULL);
    large_number_init(&ln2, LN_NULL);

    start = clock();
    successes = 0, fails = 0;
    for (int test = 0; test < TEST_SIZE; test++)
    {
        buffer1[0] = rand() % 9 + 1 + '0';
        buffer2[0] = rand() % 9 + 1 + '0';

        for (int i = 1; i < BUFFER_SIZE - 1; i++)
        {
            buffer1[i] = rand() % 10 + '0';
            buffer2[i] = rand() % 10 + '0';
        }
        buffer1[BUFFER_SIZE - 1] = 0;
        buffer2[BUFFER_SIZE - 1] = 0;

        large_number_assign(&ln1, LN_CHAR, buffer1);
        large_number_assign(&ln2, LN_CHAR, buffer2);

        large_number_mul(&ln1, 2, ln1, ln2);
    }

    time_taken = 1.0 * (clock() - start) / CLOCKS_PER_SEC;
    printf("Seriable programm: %f\n", time_taken);

    large_number_free(&ln1);
    large_number_free(&ln2);

    return 0;
}