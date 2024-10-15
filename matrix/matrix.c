#include "../include/matrix.h"

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
            rc = matrix_init(vector_at(*m->vec, i), dimensions - 1, m->attr);
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