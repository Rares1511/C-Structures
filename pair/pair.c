#include <cs/pair.h>

#include <string.h>
#include <stdlib.h>

cs_codes pair_init(pair* p, pair_attr_t* first_attr, pair_attr_t* second_attr) {
    if (p == NULL || first_attr == NULL || second_attr == NULL) {
        return CS_ELEM;
    }
    if (first_attr->size == 0 || second_attr->size == 0 || first_attr->size > SIZE_TH || second_attr->size > SIZE_TH) {
        return CS_SIZE;
    }
    p->first = malloc(first_attr->size);
    if (p->first == NULL) {
        return CS_MEM;
    }
    p->second = malloc(second_attr->size);
    if (p->second == NULL) {
        free(p->first);
        return CS_MEM;
    }
    p->first_attr = first_attr;
    p->second_attr = second_attr;
    return CS_SUCCESS;
}

cs_codes pair_set(pair* p, const void* first, const void* second) {
    if (p == NULL) {
        return CS_ELEM;
    }
    if (first) {
        if (p->first_attr->copy) {
            p->first_attr->copy(p->first, first);
        } else {
            memcpy(p->first, first, p->first_attr->size);
        }
    }
    if (second) {
        if (p->second_attr->copy) {
            p->second_attr->copy(p->second, second);
        } else {
            memcpy(p->second, second, p->second_attr->size);
        }
    }
    return CS_SUCCESS;
}

void* pair_first(pair p) {
    return p.first;
}

void* pair_second(pair p) {
    return p.second;
}

void pair_print(FILE *stream, const void *v_p) {
    if (v_p == NULL || stream == NULL) {
        return;
    }
    pair p = *(pair*)v_p;
    if (p.first_attr->print) {
        fprintf(stream, "Key: ");
        p.first_attr->print(stream, p.first);
    }
    if (p.second_attr->print) {
        fprintf(stream, " Value: ");
        p.second_attr->print(stream, p.second);
    }
}

void pair_free(void *v_p) {
    pair* p = (pair*)v_p;
    if (p == NULL) {
        return;
    }
    if (p->first) {
        if (p->first_attr->fr) {
            p->first_attr->fr(p->first);
        }
        free(p->first);
    }
    if (p->second) {
        if (p->second_attr->fr) {
            p->second_attr->fr(p->second);
        }
        free(p->second);
    }
}