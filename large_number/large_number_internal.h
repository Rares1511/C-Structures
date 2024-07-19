#ifndef LARGE_NUMBER_INTERNAL_H
#define LARGE_NUMBER_INTERNAL_H

#include "../include/large_number.h"

#include <stdlib.h>

///////////////////// DATA STRUCTURES /////////////////////

///////////////////// FUNCTIONS /////////////////////

/*!
 * Adds two large numbers structures and saves the result in the first number and returns the code for the operation
 * @param[in] ln1  First large number in the addition and the destionation for the sum
 * @param[in] ln2  Second large number in the addition
 * @return CS_SUCCESS for a successful addition or CS_MEM if a memory problem occured during reallocation of memory
 */
cs_codes large_number_add_helper(large_number *ln1, large_number ln2);

#endif