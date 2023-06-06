#include "universal.h"

void universal_swap ( void *el1, void *el2, size_t len ) {
    unsigned char *p = el1, *q = el2, tmp;
    for ( size_t i = 0; i != len; i++ ) {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

int universal_compare ( void *el1, void *el2, size_t dim ) {
    unsigned char *p = el1, *q = el2;
    for ( size_t i = 0; i != dim; i++ ) {
        if ( p[i] < q[i] ) 
            return -1;
        else if ( p[i] > q[i] )
            return 1;
    }
    return 0;
}