#include "pair.h"

/* initiates the pair given and sets the sizes for the elements it will contain */
void pair_init ( pair *p, pair_attr_t fst_attr, pair_attr_t snd_attr ) {
    p->fst = NULL;
    p->snd = NULL;
    p->fst_attr = fst_attr;
    p->snd_attr = snd_attr;
}

/* changes the value of the first element with the given element */
cs_codes pair_set_fst_value ( pair *p, void *el ) {
    if ( !p->fst ) {
        p->fst = malloc ( p->fst_attr.size );
        if ( !p->fst ) return CS_MEM;
    }
    memcpy ( p->fst, el, p->fst_attr.size );
    return CS_SUCCESS;
}

/* changes the value of the second element with the given element */
cs_codes pair_set_snd_value ( pair *p, void *el ) {
    if ( !p->snd ) {
        p->snd = malloc ( p->snd_attr.size );
        if ( !p->snd ) return CS_MEM;
    }
    memcpy ( p->snd, el, p->snd_attr.size );
    return CS_SUCCESS;
}

/* updates both the first and the second value with the offered values */
cs_codes pair_set_both_value ( pair *p, void *fst, void *snd ) {
    int rc = pair_set_fst_value ( p, fst );
    if ( rc == CS_MEM ) return rc;
    rc = pair_set_snd_value ( p, snd );
    if ( rc == CS_MEM ) return rc;
    return CS_SUCCESS;
}

/* selects the free function for the values inside the pair fst value */
void pair_set_fst_free ( pair *p, freer fst_free ) {
    p->fst_attr.fr = fst_free;
}

/* selects the free function for the values inside the pair snd value */
void pair_set_snd_free ( pair *p, freer snd_free ) {
    p->snd_attr.fr = snd_free;
}

/* selects the free function for the values inside the pair */
void pair_set_both_free ( pair *p, freer fst_free, freer snd_free ) {
    p->fst_attr.fr = fst_free;
    p->snd_attr.fr = snd_free;
}

/* sets a new function for printing the first element of the pair */
void pair_set_fst_print ( pair *p, printer fst_print ) {
    p->fst_attr.print = fst_print;
}

/* sets a new function for printing the second element of the pair */
void pair_set_snd_print ( pair *p, printer snd_print ) {
    p->snd_attr.print = snd_print;
}

/* sets a new function for printing both elements of the pair */
void pair_set_both_print ( pair *p, printer fst_print, printer snd_print ) {
    p->fst_attr.print = fst_print;
    p->snd_attr.print = snd_print;
}

/* sets the new size for the first element of the pair */
cs_codes pair_set_fst_size ( pair *p, size_t fst_size ) {
    if ( fst_size == p->fst_attr.size ) return SUCCESSFUL_REPLACEMENT;
    p->fst = realloc ( p->fst, fst_size );
    if ( !p->fst ) return CS_MEM;
    return CS_SUCCESS;
}

/* sets the new size for the second element of the pair */
cs_codes pair_set_snd_size ( pair *p, size_t snd_size ) {
    if ( snd_size == p->snd_attr.size ) return CS_SUCCESS;
    p->snd = realloc ( p->snd, snd_size );
    if ( !p->fst ) return CS_MEM;
    return CS_SUCCESS;
}

/* sets the new sizes for both elements of the pair */
cs_codes pair_set_both_size ( pair *p, size_t fst_size, size_t snd_size ) {
    int rc = pair_set_fst_size ( p, fst_size );
    if ( rc == CS_MEM ) return rc;
    rc = pair_set_snd_size ( p, snd_size );
    return rc;
}

/* sets the new attribute for the first element of the pair */
cs_codes pair_set_fst_attr ( pair *p, pair_attr_t fst_attr ) {
    if ( p->fst_attr.size != fst_attr.size ) {
        p->fst = realloc ( p->fst, fst_attr.size );
        if ( !p->fst ) return CS_MEM;
    }
    p->fst_attr = fst_attr;
    return CS_SUCCESS;
}

/* sets the new attribute for the second element of the pair */
cs_codes pair_set_snd_attr ( pair *p, pair_attr_t snd_attr ) {
    if ( p->snd_attr.size != snd_attr.size ) {
        p->snd = realloc ( p->snd, snd_attr.size );
        if ( !p->snd ) return CS_MEM;
    }
    p->snd_attr = snd_attr;
    return CS_SUCCESS;
}

/* sets the new attributes for both elements of the pair */
cs_codes pair_set_both_attr ( pair *p, pair_attr_t fst_attr, pair_attr_t snd_attr ) {
    int rc = pair_set_fst_attr ( p, fst_attr );
    if ( rc == CS_MEM ) return rc;
    rc = pair_set_snd_attr ( p, snd_attr );
    return rc;
}

/* returns a copy ptr with the same value as the first element in the pair or NULL
if memory couldn't be allocated or a value has not been set yet */
void *pair_get_first ( pair p ) {
    return p.fst;
}

/* returns a copy ptr with the same value as the second element in the pair or NULL
if memory couldn't be allocated or a value has not been set yet */
void *pair_get_second ( pair p ) {
    return p.snd;
}

/* returns both values in the first and seconnd ptr given */
void pair_get_both ( pair p, void *fst, void *snd ) {
    if ( fst == NULL || snd == NULL ) return;
    fst = p.fst;
    snd = p.snd;
}

/* prints the first value in the pair using the printer value given */
void pair_print_first ( void *v_p ) {
    pair p = *(pair *) v_p;
    if ( p.fst_attr.print ) p.fst_attr.print ( p.fst );
}

/* prints the second value in the pair using the printer function given */
void pair_print_second ( void *v_p ) {
    pair p = *(pair *) v_p;
    if ( p.snd_attr.print ) p.snd_attr.print ( p.snd );
}

/* prints both values with the given printers */
void pair_print_both ( void *v_p ) {
    pair p = *(pair *) v_p;
    if ( p.fst_attr.print ) p.fst_attr.print ( p.fst );
    if ( p.snd_attr.print ) p.snd_attr.print ( p.snd );
}

/* frees the memory of the elements for pair */
void pair_free ( void *v_p ) {
    pair *p = (pair*) v_p;
    if ( p->fst ) {
        if ( p->fst_attr.fr ) p->fst_attr.fr ( p->fst );
        free ( p->fst );
    }
    if ( p->snd ) {
        if ( p->snd_attr.fr ) p->snd_attr.fr ( p->snd );
        free ( p->snd );
    }
}

/* swaps the two pairs offered only if they have the same dimension for the first and 
second element */
cs_codes pair_swap ( pair *p1, pair *p2 ) {
    if ( p1->fst_attr.size != p2->fst_attr.size || p1->snd_attr.size != p2->snd_attr.size ) return CS_SIZE;
    pair paux;
    pair_init ( &paux, p1->fst_attr, p1->snd_attr );
    pair_set_both_value ( &paux, p1->fst, p1->snd );

    p1->fst_attr = p2->fst_attr, p1->snd_attr = p2->snd_attr;
    pair_set_both_value ( p1, p2->fst, p2->snd );
    p2->fst_attr = paux.fst_attr, p2->snd_attr = paux.snd_attr;
    pair_set_both_value ( p2, paux.fst, paux.snd );
    
    pair_free ( &paux );
    return CS_SUCCESS;
}

/* compares the two pairs by using the compare function given for the first and second 
elemets */
int pair_compare ( pair p1, pair p2, comparer fst_cmp, comparer snd_cmp ) {
    int fst_res = fst_cmp ( p1.fst, p2.fst );
    if ( fst_res == 0 ) return snd_cmp ( p1.snd, p2.snd );
    return fst_res; 
}