#ifndef LARGE_NUMBER_H
#define LARGE_NUMBER_H

#include <stdint.h>
#include "../universal/universal.h"

#define LARGE_NUMBER_INIT_CAP 128

typedef struct large_number {
    uint8_t *arr;
    size_t size, cap;
} large_number;

#define LARGE_NUMBER_SIZE sizeof ( large_number )

large_number *large_number_init ( int64_t starting_value );
enum return_codes large_number_add ( large_number* large_num1, large_number *large_num2, large_number *sum );
enum return_codes large_number_add_scale ( large_number* large_num1, int64_t scale, large_number *sum );
enum return_codes large_number_mul ( large_number *large_num1, large_number *large_num2, large_number *prod );
enum return_codes large_number_mul_scale ( large_number *large_num, int64_t scale, large_number *prod );
void large_number_swap ( large_number *large_num1, large_number *large_num2 );
void large_number_free ( large_number *large_num );
void large_number_print ( large_number *large_num );

#endif