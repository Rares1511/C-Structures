#ifndef LARGE_NUMBER_INTERNAL_H
#define LARGE_NUMBER_INTERNAL_H

#define _XOPEN_SOURCE 600 /* Or higher */

#include "../include/large_number.h"

///////////////////// LIBRARIES /////////////////////

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

///////////////////// DATA STRUCTURES /////////////////////

typedef struct large_number_thread_arg_t
{
    large_number *ln1;               /*!< first number in the addition and the destionation for the result*/
    large_number *ln2;               /*!< second number in the addition */
    pthread_barrier_t *barrier;      /*!< barrier to control the threads in the computation of multiplying */
    pthread_barrier_t *last_barrier; /*!< barrier to control the threads in the computation of multiplying for the last iterration */
    int thread_id;                   /*!< id of the thread */
    int thread_count;                /*!< total numbers of threads */
} large_number_thread_arg_t;

///////////////////// FUNCTIONS /////////////////////

/*!
 * Adds two large numbers structures and saves the result in the first number and returns the code for the operation
 * @param[in] ln1  First large number in the addition and the destionation for the sum
 * @param[in] ln2  Second large number in the addition
 * @return CS_SUCCESS for a successful addition or CS_MEM if a memory problem occured during reallocation of memory
 */
cs_codes large_number_add_helper(large_number *ln1, large_number ln2);

/*!
 * Multiplies two large numbers structures and saves the result in the first number and returns the code for the operation
 * @param[in] ln1  First large number in the multiplication and the destionation for the product
 * @param[in] ln2  Second large number in the multiplication
 * @return CS_SUCCESS for a successful multiplication or CS_MEM if a memory problem occured during reallocation of memory
 */
cs_codes large_number_mul_helper(large_number *ln1, large_number ln2);

#endif