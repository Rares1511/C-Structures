#ifndef PAIR_H
#define PAIR_H

#include "../universal/universal.h"

typedef univ_attr_t pair_attr_t;

typedef struct pair_type {
    void *fst, *snd;
    pair_attr_t fst_attr, snd_attr;
} pair;

#define PAIR_SIZE sizeof ( pair )

void pair_init ( pair *p, pair_attr_t fst_attr, pair_attr_t snd_attr );
cs_codes pair_set_fst_value ( pair *p, void *el );
cs_codes pair_set_snd_value ( pair *p, void *el );
cs_codes pair_set_both_value ( pair *p, void *first, void *second );
void pair_set_fst_free ( pair *p, freer fst_free );
void pair_set_snd_free ( pair *p, freer snd_free );
void pair_set_both_free ( pair *p, freer fst_free, freer snd_free );
void pair_set_fst_print ( pair *p, printer fst_print );
void pair_set_snd_print ( pair *p, printer snd_print );
void pair_set_both_print ( pair *p, printer fst_print, printer snd_print );
cs_codes pair_set_fst_size ( pair *p, size_t fst_size );
cs_codes pair_set_snd_size ( pair *p, size_t snd_size );
cs_codes pair_set_both_size ( pair *p, size_t fst_size, size_t snd_size );
cs_codes pair_set_fst_attr ( pair *p, pair_attr_t fst_attr );
cs_codes pair_set_snd_attr ( pair *p, pair_attr_t snd_attr );
cs_codes pair_set_both_attr ( pair *p, pair_attr_t fst_attr, pair_attr_t snd_attr );
cs_codes pair_swap ( pair *pair1, pair *pair2 );
void *pair_get_first ( pair p );
void *pair_get_second ( pair p );
void pair_get_both ( pair p, void *first, void *second );
void pair_print_first ( void *v_p );
void pair_print_second ( void *v_p );
void pair_print_both ( void *v_p );
void pair_free ( void *v_p );
int pair_compare ( pair pair1, pair pair2, comparer first_compare, comparer second_compare );

#endif