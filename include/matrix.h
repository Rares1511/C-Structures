#ifndef MATRIX_H
#define MATRXI_H

#include "universal.h"
#include "vector.h"

typedef xuniv_attr_t matrix_attr_t;

typedef struct matrix {
    vector *vec;        /*!< vector holding the (dimensions - 1) matrixes */
    int dimensions;     /*!< number of dimensions of the matrix */
    matrix_attr_t attr; /*!< attributes for the datatype inside the matrix */
} matrix;

#endif