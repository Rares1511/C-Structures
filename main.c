#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/large_number.h"

#define BUFFER_SIZE 10000
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
    double time_taken;

    large_number_init(&ln1, LN_NULL);
    large_number_init(&ln2, LN_NULL);

    start = clock();

    large_number_assign(&ln1, LN_CHAR, "245");
    large_number_assign(&ln2, LN_CHAR, "16");

    large_number_print(ln1);
    large_number_print(ln2);

    large_number_div_num(ln1, ln2, 5, LN_QUOTIENT, &ln1);

    printf("Quotient: ");
    large_number_print(ln1);
    printf("Rest:     ");
    large_number_print(ln2);

    time_taken = 0.1 * (clock() - start) / CLOCKS_PER_SEC;
    printf("Seriable programm: %f\n", time_taken);

    large_number_free(&ln1);
    large_number_free(&ln2);

    return 0;
}