#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/large_number.h"

int main()
{
    clock_t start = clock();

    large_number ln1, ln2;

    large_number_init(&ln1, LN_SCALE, 69);
    large_number_init(&ln2, LN_SCALE, 39);

    large_number_print(ln1);

    large_number_add(&ln1, 2, ln1, ln2);

    large_number_print(ln1);

    large_number_free(&ln1);
    large_number_free(&ln2);

    double time_taken = 1.0 * (clock() - start) / CLOCKS_PER_SEC;
    printf("programmed ran in %f\n", time_taken);
    return 0;
}