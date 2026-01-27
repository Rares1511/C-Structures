#ifndef __CS_FLAT_SET_H__
#define __CS_FLAT_SET_H__

#include <cs/universal.h>

typedef univ_attr_t flat_set_attr_t;

typedef enum flat_set_type {
    CS_FLAT_SET_DEQUE,
    CS_FLAT_SET_VECTOR,
    CS_FLAT_SET_DEFAULT = CS_FLAT_SET_VECTOR,
} flat_set_type;

typedef struct flat_set {
    void *container;
    flat_set_type type;
} flat_set;

flat_set* flat_set_init(flat_set_attr_t attr, flat_set_type type);
cs_codes flat_set_insert(flat_set *fs, void *data);
cs_codes flat_set_erase(flat_set *fs, void *data);
int flat_set_find(flat_set fs, void *data);
int flat_set_empty(flat_set fs);
int flat_set_size(flat_set fs);
void* flat_set_at(flat_set fs, int index);
void flat_set_clear(flat_set *fs);
void flat_set_swap(flat_set *fs1, flat_set *fs2);
void flat_set_print(FILE *stream, void *v_fs);
void flat_set_free(void *v_fs);

#endif