#include "../include/large_number.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static inline cs_codes check_memory ( large_number *ln ) {
    if ( ln->size == ln->cap ) {
        ln->cap += ln->cap / 2;
        ln->vec = realloc ( ln->vec, ln->cap );
        if ( !ln->vec ) return CS_MEM;
    }
    return CS_SUCCESS;
}

cs_codes large_number_init ( large_number *ln, ln_type type, ... ) {
    ln->size = 0;
    ln->cap = 100;
    ln->aux = NULL;
    if ( type != LN_NUM ) {
        ln->vec = malloc ( ln->cap );
        if ( !ln->vec ) return CS_MEM;
    }
    if ( type == LN_NULL )
        ln->vec[ln->size++] = 0;
    else {
        va_list arg;
        va_start ( arg, 1 );
        if ( type == LN_NUM ) {
            large_number src = va_arg ( arg, large_number );
            ln->cap = src.cap;
            ln->vec = malloc ( ln->cap );
            if ( !ln->vec ) return CS_MEM;
            for ( int i = 0; i < src.size; i++ )
                ln->vec[i] = src.vec[i];
            ln->size = src.size;
        }
        else if ( type == LN_SCALE ) {
            long scale = va_arg ( arg, long );
            for ( scale = scale; scale != 0; scale /= 10 )
                ln->vec[ln->size++] = scale % 10;
        }
        va_end ( arg );
    }
    return CS_SUCCESS;
}

cs_codes large_number_assign ( large_number *ln, ln_type type, ... ) {
    if ( type == LN_NULL ) {
        ln->vec[0] = 0;
        ln->size = 0;
        return CS_SUCCESS;
    }
    va_list arg;
    va_start ( arg, 1 );
    if ( type == LN_SCALE ) {
        long scale = va_arg ( arg, long );
        int count = 0;
        for ( long copy = scale; copy != 0; copy /= 10 )
            count++;
        if ( count > ln->cap ) {
            ln->cap = count * 3 / 2;
            ln->vec = realloc ( ln->vec, ln->cap );
            if ( !ln->vec ) return CS_MEM;
        }
        for ( int i = 0; scale != 0; scale /= 10, i++ )
            ln->vec[i] = scale % 10;
        ln->size = count;
    }
    else if ( type == LN_NUM ) {
        large_number src = va_arg ( arg, large_number );
        if ( src.size > ln->cap ) {
            ln->cap = src.cap;
            ln->vec = realloc ( ln->vec, ln->cap );
            if ( !ln->vec ) return CS_MEM;
        }
        ln->size = src.size;
        for ( int i = 0; i < src.size; i++ )
            ln->vec[i] = src.vec[i];
    }
    return CS_SUCCESS;
}

cs_codes large_number_add ( large_number *dst, ln_type type, ... ) {
    if ( type == LN_NULL ) return CS_SUCCESS;
    va_list arg;
    va_start ( arg, 1 );
    int temp = 0;
    if ( type == LN_NUM ) {
        large_number src = va_arg ( arg, large_number );
        for ( int i = 0; i < dst->size && i < src.size; i++ ) {
            dst->vec[i] += src.vec[i] + temp;
            temp = dst->vec[i] / 10;
            dst->vec[i] %= 10;
        }
        while ( dst->size < src.size || temp != 0 ) {
            if ( check_memory ( dst ) == CS_MEM ) return CS_MEM;
            dst->vec[dst->size] = temp;
            if ( dst->size < src.size )
                dst->vec[dst->size] += src.vec[dst->size];
            temp = dst->vec[dst->size] / 10;
            dst->vec[dst->size] %= 10;
            dst->size++;
        }
    }
    else if ( type == LN_SCALE ) {
        long scale = va_arg ( arg, long );
        for ( int i = 0; i < dst->size && scale != 0; i++, scale /= 10 ) {
            dst->vec[i] += scale % 10 + temp;
            temp = dst->vec[i] / 10;
            dst->vec[i] %= 10;  
        }
        while ( scale != 0 || temp != 0 ) {
            if ( check_memory ( dst ) == CS_MEM ) return CS_MEM;
            dst->vec[dst->size] = temp + scale % 10;
            temp = dst->vec[dst->size] / 10;
            dst->vec[dst->size] %= 10;  
            scale /= 10;
            dst->size++;
        }
    }
    return CS_SUCCESS;
}

cs_codes large_number_mul_helper ( large_number *dst, long scale, int offset ) {
    long temp = 0;
    for ( int i = 0; i < dst->size; i++ ) {
        long prod = temp + dst->vec[i] * scale;
        dst->vec[i] = prod % 10;
        temp = prod / 10;
    }
    while ( temp != 0 ) {
        if ( check_memory ( dst ) == CS_MEM ) return CS_MEM;
        dst->vec[dst->size] = temp % 10;
        temp /= 10;
        dst->size++;
    }
    if ( offset == 0 ) return CS_SUCCESS;
    if ( dst->size + offset > dst->cap ) {
        dst->cap = dst->size + offset;
        dst->vec = realloc ( dst->vec, dst->cap );
        if ( !dst->vec ) return CS_MEM;
    }
    for ( int i = dst->size - 1; i >= 0; i-- )
        dst->vec[i + offset] = dst->vec[i];
    for ( int i = 0; i < offset; i++ )
        dst->vec[i] = 0;
    dst->size += offset;
    return CS_SUCCESS;
}

cs_codes large_number_mul ( large_number *dst, ln_type type, ... ) {
    if ( type == LN_NULL ) return CS_SUCCESS;
    va_list arg;
    va_start ( arg, 1 );
    if ( type == LN_SCALE )
        large_number_mul_helper ( dst, va_arg ( arg, long ), 0 );
    else if ( type == LN_NUM ) {
        large_number src = va_arg ( arg, large_number );
        large_number aux_prod;
        if ( large_number_init ( &aux_prod, LN_NULL ) == CS_MEM ) return CS_MEM;
        if ( !dst->aux ) {
            dst->aux = malloc ( sizeof ( large_number ) );
            if ( !dst->aux ) return CS_MEM;
            if ( large_number_init ( dst->aux, LN_NULL ) == CS_MEM ) return CS_MEM;
        }
        int start = src.size - 1;
        while ( start >= 0 ) {
            long scale = 0;
            int end;
            for ( end = start; end >= 0 && start - end < 18; end-- ) {
                scale = scale * 10 + src.vec[end];
            }
            large_number_assign ( dst->aux, LN_NUM, *dst );
            large_number_mul_helper ( dst->aux, scale, end + 1 );
            large_number_add ( &aux_prod, LN_NUM, *(dst->aux) );
            while ( end >= 0 && src.vec[end] == 0 )
                end--;
            start = end;
        }
        large_number_swap ( dst, &aux_prod );
        large_number_free ( &aux_prod );
    }
    va_end ( arg );
    return CS_SUCCESS;
}

cs_codes large_number_div_helper ( large_number *dst, long scale, int offset ) {
    
}

cs_codes large_number_div ( large_number *dst, ln_type type, ... ) {
    if ( type == LN_NULL ) return CS_SUCCESS;
    va_list arg;
    va_start ( arg, 1 );
    if ( type == LN_SCALE ) {

    }
    else if ( type == LN_NUM ) {

    }
}

cs_codes large_number_sum ( large_number *sum, int  count, ... ) {
    sum->size = 0;
    sum->vec[0] = 0;
    va_list args;
    va_start ( args, count );
    for ( int i = 0; i < count; i++ ) {
        int rc = large_number_add ( sum, LN_NUM, va_arg ( args, large_number ) );
        if ( rc == CS_MEM ) return rc;
    }
    va_end ( args );
    return CS_SUCCESS;
}

cs_codes large_number_prod ( large_number *prod, int count, ... ) {
    prod->size = 1;
    prod->vec[0] = 1;
    va_list args;
    va_start ( args, count );
    for ( int i = 0; i < count; i++ ) {
        int rc = large_number_mul ( prod, LN_NUM, va_arg ( args, large_number ) );
        if ( rc == CS_MEM ) return rc;
    }
    va_end ( args );
    return CS_SUCCESS;
}

cs_codes large_number_exp ( large_number *ln, ln_type type, ... ) {
    if ( type == LN_NULL ) return CS_SUCCESS;
    va_list arg;
    va_start ( arg, 1 );
    large_number pow;
    if ( large_number_init ( &pow, LN_SCALE, 1 ) == CS_MEM ) return CS_MEM;
    if ( type == LN_SCALE ) {
        long scale = va_arg ( arg, long );
        while ( scale ) {
            if ( scale & 1 ) {
                large_number_mul ( &pow, LN_NUM, *ln );
            }
            large_number_mul ( ln, LN_NUM, *ln );
            scale = ( scale >> 1 );
        }
    }
    else if ( type == LN_NUM ) {
        large_number exp = va_arg ( arg, large_number );
        while ( exp.size != 0 ) {
            if ( exp.vec[0] & 1 )
                large_number_mul ( &pow, LN_NUM, *ln );
            large_number_mul ( ln, LN_NUM, *ln );
        }
    }
    large_number_swap ( ln, &pow );
    large_number_free ( &pow );
    return CS_SUCCESS;
}

void large_number_swap ( large_number *ln1, large_number *ln2 ) {
    int cap = ln1->cap;
    int size = ln1->size;
    char* vec = ln1->vec;

    ln1->cap = ln2->cap;
    ln1->size = ln2->size;
    ln1->vec = ln2->vec;

    ln2->cap = cap;
    ln2->size = size;
    ln2->vec = vec;
}

void large_number_free ( large_number *ln ) { 
    if ( ln->aux ) {
        free ( ln->aux->vec );
        free ( ln->aux ); 
    }
    free ( ln->vec ); 
}

void large_number_print ( large_number ln ) {
    for ( int i = ln.size - 1; i >= 0; i-- ) {
        printf ( "%d", ln.vec[i] );
        if ( i % 3 == 0 && i != 0 )
            printf ( "," );
    }
    printf ( "\n" );
}