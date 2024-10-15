#ifndef MATRIX_H
#define MATRIX_H

#include "universal.h"
#include "vector.h"

#define MAX_MATRIX_DIM 5

typedef xuniv_attr_t matrix_attr_t;

typedef struct matrix_t {
    vector *vec;        /*!< vector holding the (dimensions - 1) matrixes */
    int dimensions;     /*!< number of dimensions of the matrix */
    matrix_attr_t attr; /*!< attributes for the datatype inside the matrix */
} matrix_t;

cs_codes matrix_init(matrix_t *m, int dimensions, matrix_attr_t attr);
cs_codes matrix_insert_at(matrix_t *m, int *position, void *el);
cs_codes matrix_push_back(matrix_t *m, int *position, void *el);
int matrix_empty(matrix_t m);
void *matrix_at(matrix_t m, int *position);
void matrix_print(void *v_m);
void matrix_free(void *v_m);

#endif