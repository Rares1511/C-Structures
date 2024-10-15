#ifndef MATRIX_H
#define MATRXI_H

#include "universal.h"
#include "vector.h"

typedef xuniv_attr_t matrix_attr_t;

typedef struct matrix_t {
    vector *vec;        /*!< vector holding the (dimensions - 1) matrixes */
    int dimensions;     /*!< number of dimensions of the matrix */
    matrix_attr_t attr; /*!< attributes for the datatype inside the matrix */
} matrix_t;

matrix_attr_t MATRIX_ATTR = {sizeof(matrix_t), NULL, NULL, NULL, NULL};

cs_codes matrix_init(matrix_t *m, int dimensions, matrix_attr_t attr);

#endif