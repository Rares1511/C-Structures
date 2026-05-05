#include <cs/pair.h>

cs_codes pair_init(pair* p, elem_attr_t* first_attr, elem_attr_t* second_attr) {
    if (p == NULL || first_attr == NULL || second_attr == NULL) {
        return CS_ELEM;
    }
    if (first_attr->size == 0 || second_attr->size == 0 || first_attr->size > SIZE_TH || second_attr->size > SIZE_TH) {
        return CS_SIZE;
    }
    p->first_attr = first_attr;
    p->second_attr = second_attr;
    p->data = NULL;
    p->has_first = 0;
    p->has_second = 0;
    return CS_SUCCESS;
}

void pair_print(FILE *stream, const void *v_p) {
    CS_RETURN_IF(NULL == v_p || NULL == stream);
    pair p = *(pair*)v_p;
    if (p.first_attr->print) {
        fprintf(stream, "Key: ");
        p.first_attr->print(stream, pair_first(p));
    }
    if (p.second_attr->print) {
        fprintf(stream, " Value: ");
        p.second_attr->print(stream, pair_second(p));
    }
}

void pair_free(void *v_p) {
    CS_RETURN_IF(NULL == v_p);
    pair* p = (pair*)v_p;
    if (p == NULL) {
        return;
    }
    if (p->has_first) {
        if (p->first_attr->fr) {
            p->first_attr->fr(pair_first(p));
        }
    }
    if (p->has_second) {
        if (p->second_attr->fr) {
            p->second_attr->fr(pair_second(p));
        }
    }
    free(p->data);
}