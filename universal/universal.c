#include "universal.h"

/* swaps two memory zones considering they are both of size len */
void universal_swap ( void *el1, void *el2, size_t len ) {
    unsigned char *p = el1, *q = el2, tmp;
    for ( size_t i = 0; i != len; i++ ) {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

/* compares two memory zones bytes by bytes considering they are both of size len */
int universal_compare ( const void *el1, const void *el2, size_t len ) {
    const unsigned char *p = el1, *q = el2;
    for ( size_t i = 0; i != len; i++ ) {
        if ( p[i] < q[i] ) 
            return 1;
        else if ( p[i] > q[i] )
            return -1;
    }
    return 0;
}