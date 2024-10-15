#include "../include/matrix.h"

#include <stdio.h>
#include <stdlib.h>

matrix_attr_t MATRIX_ATTR = {sizeof(matrix_t), matrix_free, NULL, NULL, NULL};

cs_codes matrix_init(matrix_t *m, int dimensions, matrix_attr_t attr) {
    if (attr.size < 0 || attr.size > SIZE_TH || dimensions < 0 || dimensions > MAX_MATRIX_DIM)
        return CS_SIZE;

    m->attr = attr;
    m->vec = malloc(sizeof(vector));
    m->dimensions = dimensions;
    if (!m->vec)
        return CS_MEM;

    if (dimensions > 1) {
        cs_codes rc = vector_init(m->vec, MATRIX_ATTR);
        if (rc != CS_SUCCESS) {
            free(m->vec);
            return rc;
        }
        for (int i = 0; i < INIT_CAPACITY; i++) {
            matrix_t aux;
            rc = vector_push_back(m->vec, &aux);
            if (rc != CS_SUCCESS) {
                free(m->vec);
                return rc;
            }
            rc = matrix_init(vector_at(*m->vec, i), dimensions - 1, m->attr);
            if (rc != CS_SUCCESS) {
                free(m->vec);
                return rc;
            }
        }
    } else {
        cs_codes rc = vector_init(m->vec, m->attr);
        if (rc != CS_SUCCESS) {
            free(m->vec);
            return rc;
        }
    }

    return CS_SUCCESS;
}

cs_codes matrix_insert_at(matrix_t *m, int *position, void *el) {
    if (position[0] < 0 || position[0] >= m->vec->size)
        return CS_POS;

    if (m->dimensions > 1) {
        return matrix_insert_at((matrix_t *)vector_at(*m->vec, position[0]), position + 1, el);
    } else {
        return vector_insert_at(m->vec, el, position[0]);
    }
}

cs_codes matrix_push_back(matrix_t *m, int *position, void *el) {
    if (m->dimensions > 1) {
        if (position[0] < 0 || position[0] >= m->vec->size)
            return CS_POS;

        return matrix_push_back((matrix_t *)vector_at(*m->vec, position[0]), position + 1, el);
    } else {
        return vector_push_back(m->vec, el);
    }
}

int matrix_empty(matrix_t m) {
    /* Matrix has more dimensions so we look in its submatrix for elements until reaching a 1-d
     * matrix*/
    if (m.dimensions > 1) {
        for (int i = 0; i < m.vec->size; i++) {
            if (!matrix_empty(*(matrix_t *)vector_at(*m.vec, i)))
                return 0;
        }
        return 1;
    }
    /* Matrix has only dimension (it's a vector so we check for elements)*/
    else {
        return m.vec->size == 0;
    }
}

void *matrix_at(matrix_t m, int *position) {
    if (position[0] < 0 || position[0] >= m.vec->size)
        return NULL;

    if (m.dimensions > 1) {
        return matrix_at(*(matrix_t *)vector_at(*m.vec, position[0]), position + 1);
    } else {
        return vector_at(*m.vec, position[0]);
    }
}

void matrix_print(void *v_m) {
    matrix_t *m = (matrix_t *)v_m;

    if (m->dimensions > 1) {
        for (int i = 0; i < m->vec->size; i++) {
            matrix_t *submatrix = (matrix_t *)vector_at(*m->vec, i);
            if (!matrix_empty(*submatrix)) {
                printf("[%d] ", i);
                matrix_print(submatrix);
            }
        }
    } else {
        vector_print(m->vec);
        printf("\n");
    }
}

void matrix_free(void *v_m) {
    matrix_t *m = (matrix_t *)v_m;
    vector_free(m->vec);
    free(m->vec);
    m->vec = NULL;
}