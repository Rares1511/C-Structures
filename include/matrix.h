#ifndef MATRIX_H
#define MATRXI_H

#include "universal.h"
#include "vector.h"

typedef xuniv_attr_t matrix_attr_t;

typedef struct matrix {
    vector *vec; 
} matrix;

#endif