#include <stdlib.h>
#include <string.h>

#include "../include/pair.h"
#include "../include/universal.h"

cs_codes pair_init ( pair *p, pair_attr_t fst_attr, pair_attr_t snd_attr ) {
    if ( fst_attr.size < 0 || snd_attr.size < 0 || fst_attr.size > SIZE_TH || snd_attr.size > SIZE_TH ) return CS_SIZE;
    p->fst = NULL;
    p->snd = NULL;
    p->fst_attr = fst_attr;
    p->snd_attr = snd_attr;
    return CS_SUCCESS;
}

cs_codes pair_set_fst_value ( pair *p, void *el ) {
    if ( !p->fst ) {
        p->fst = malloc ( p->fst_attr.size );
        if ( !p->fst ) return CS_MEM;
        memcpy ( p->fst, el, p->fst_attr.size );
        return CS_SUCCESS;
    }
    if ( p->fst_attr.fr ) p->fst_attr.fr ( p->fst );
    memcpy ( p->fst, el, p->fst_attr.size );
    return CS_SUCCESS;
}

cs_codes pair_set_snd_value ( pair *p, void *el ) {
    if ( !p->snd ) {
        p->snd = malloc ( p->snd_attr.size );
        if ( !p->snd ) return CS_MEM;
        memcpy ( p->snd, el, p->snd_attr.size );
        return CS_SUCCESS;
    }
    if ( p->snd_attr.fr ) p->snd_attr.fr ( p->snd );
    memcpy ( p->snd, el, p->snd_attr.size );
    return CS_SUCCESS;
}

cs_codes pair_set_both_value ( pair *p, void *fst, void *snd ) {
    int rc = pair_set_fst_value ( p, fst );
    if ( rc == CS_MEM ) return rc;
    rc = pair_set_snd_value ( p, snd );
    if ( rc == CS_MEM ) return rc;
    return CS_SUCCESS;
}

void pair_set_fst_free ( pair *p, freer fst_free ) { p->fst_attr.fr = fst_free; }

void pair_set_snd_free ( pair *p, freer snd_free ) { p->snd_attr.fr = snd_free; }

void pair_set_both_free ( pair *p, freer fst_free, freer snd_free ) {
    p->fst_attr.fr = fst_free;
    p->snd_attr.fr = snd_free;
}

void pair_set_fst_print ( pair *p, printer fst_print ) { p->fst_attr.print = fst_print; }

void pair_set_snd_print ( pair *p, printer snd_print ) { p->snd_attr.print = snd_print; }

void pair_set_both_print ( pair *p, printer fst_print, printer snd_print ) {
    p->fst_attr.print = fst_print;
    p->snd_attr.print = snd_print;
}

void *pair_get_first ( pair p ) { return p.fst; }

void *pair_get_second ( pair p ) { return p.snd; }

void pair_get_both ( pair p, void *fst, void *snd ) {
    if ( fst == NULL || snd == NULL ) return;
    fst = p.fst;
    snd = p.snd;
}

void pair_print_first ( void *v_p ) {
    pair p = *(pair *) v_p;
    if ( p.fst_attr.print ) p.fst_attr.print ( p.fst );
}

void pair_print_second ( void *v_p ) {
    pair p = *(pair *) v_p;
    if ( p.snd_attr.print ) p.snd_attr.print ( p.snd );
}

void pair_print_both ( void *v_p ) {
    pair p = *(pair *) v_p;
    if ( p.fst_attr.print ) p.fst_attr.print ( p.fst );
    if ( p.snd_attr.print ) p.snd_attr.print ( p.snd );
}

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

int pair_compare ( pair p1, pair p2 ) {
    if ( p1.fst_attr.size != p2.fst_attr.size || p1.snd_attr.size != p2.snd_attr.size ) return 0;
    if ( !p1.fst_attr.comp || !p1.snd_attr.comp || !p2.fst_attr.comp || !p2.snd_attr.comp ) return 0;
    int fst_res = ( p1.fst_attr.comp ( p1.fst, p2.fst ) + p2.fst_attr.comp ( p1.fst, p2.fst ) ) / 2;
    if ( fst_res == 0 ) return ( p1.snd_attr.comp ( p1.snd, p2.snd ) + p2.snd_attr.comp ( p1.snd, p2.snd ) ) / 2;
    return fst_res; 
}