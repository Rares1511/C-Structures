#include <cs/pair.h>

pair* pair_init(elem_attr_t* first_attr, elem_attr_t* second_attr) {
    CS_RETURN_IF(first_attr == NULL || second_attr == NULL || first_attr->size == 0 || second_attr->size == 0 || first_attr->size > SIZE_TH || second_attr->size > SIZE_TH, NULL);
    pair *p = malloc(sizeof(pair) + first_attr->size + second_attr->size);
    if (p == NULL) {
        return NULL;
    }
    p->first_attr = first_attr;
    p->second_attr = second_attr;
    p->has_first = 0;
    p->has_second = 0;
    p->header.magic = CS_PAIR_MAGIC;
    p->header.type = CS_PAIR_TYPE;
    return p;
}

void pair_print(FILE *stream, const void *v_p) {
    CS_RETURN_IF(NULL == v_p || NULL == stream);
    pair *p = (pair*)v_p;
    CS_RETURN_IF(p->header.magic != CS_PAIR_MAGIC);
    if (p->first_attr->print) {
        fprintf(stream, "Key: ");
        p->first_attr->print(stream, pair_first(p));
    }
    if (p->second_attr->print) {
        fprintf(stream, " Value: ");
        p->second_attr->print(stream, pair_second(p));
    }
}

void pair_free(void *v_p) {
    CS_RETURN_IF(NULL == v_p);
    pair* p = (pair*)v_p;
    CS_RETURN_IF(p->header.magic != CS_PAIR_MAGIC);
    __pair_free_internal(p);
    free(p);
}