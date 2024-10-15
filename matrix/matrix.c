#include "../include/matrix.h"

#include <stdlib.h>

matrix_attr_t MATRIX_ATTR = {sizeof(matrix_t), matrix_free, NULL, NULL, NULL};

cs_codes matrix_init(matrix_t *m, int dimensions, matrix_attr_t attr) {
    if (attr.size < 0 || attr.size > SIZE_TH || dimensions < 0)
        return CS_SIZE;

    m->attr = attr;
    m->vec = malloc(sizeof(vector));
    if (!m->vec)
        return CS_MEM;

    if (dimensions > 1) {
        cs_codes rc = vector_init(m->vec, MATRIX_ATTR);
        if (rc != CS_SUCCESS) {
            free(m->vec);
            return rc;
        }
        for (int i = 0; i < INIT_CAPACITY; i++) {
            rc = vector_push_back(m->vec, malloc(sizeof(matrix_t)));
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

void matrix_free(void *v_m) {
    matrix_t *m = (matrix_t *)v_m;
    vector_free(m->vec);
    free(m->vec);
    m->vec = NULL;
}