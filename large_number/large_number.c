#include "large_number.h"

int8_t push_value ( large_number *ln, int8_t digit ) {
    if ( ln->size == ln->cap ) {
        ln->cap += LARGE_NUMBER_INIT_CAP;
        ln->arr = realloc ( ln->arr, ln->cap );
        if ( !ln->arr ) return -1;
    }
    ln->arr[ln->size++] = digit % 10;
    return digit / 10;
}

int8_t set_value ( large_number *ln, int8_t digit, size_t pos ) {
    if ( pos == ln->size ) return push_value ( ln, digit );
    if ( pos > ln->size ) return -2;
    ln->arr[pos] = digit % 10;
    return digit / 10;
}

int8_t add_value ( large_number *ln, int8_t digit, size_t pos ) {
    if ( pos == ln->size ) return push_value ( ln, digit );
    if ( pos > ln->size ) return -2;
    ln->arr[pos] += digit;
    int8_t temp = ln->arr[pos] / 10;
    ln->arr[pos] %= 10;
    return temp;
}

/* initiliazes the structure for a large number or returns NULL if there has been a problem */
large_number* large_number_init ( int64_t val ) {
    large_number *ln = malloc ( LARGE_NUMBER_SIZE );
    if ( !ln ) return NULL;
    ln->cap = LARGE_NUMBER_INIT_CAP;
    ln->arr = malloc ( ln->cap );
    if ( !ln->arr ) {
        free ( ln );
        return NULL;
    }
    if ( val == 0 ) {
        ln->size = 1;
        ln->arr[0] = 0;
        return ln;
    }
    ln->size = 0;
    while ( val > 0 ) {
        ln->arr[ln->size++] = val % 10;
        val /= 10;
    }
    return ln;
}

/* adds a small number to the large number structure, returns sum of the two if the 
addition was successful or NULL if there has been a problem with allocating memory */
enum return_codes large_number_add_scale ( large_number* ln, int64_t num, large_number *sum ) {
    large_number *aux = large_number_init ( num );
    if ( !aux ) return MEMORY_PROBLEM;
    enum return_codes rc = large_number_add ( ln, aux, sum );
    large_number_free ( aux );
    return rc;
}

/* adds another large number to the large number structure, returns sum of the two if the 
addition was successful or NULL if there has been a problem with allocating memory */
enum return_codes large_number_add ( large_number *ln1, large_number *ln2, large_number *sum ) {
    int8_t temp = 0;
    size_t i;
    for ( i = 0; i < ln1->size && i < ln2->size; i++ )
        temp = set_value ( sum, ln1->arr[i] + ln2->arr[i] + temp, i );
    for ( i = i; i < ln1->size; i++ )
        temp = set_value ( sum, ln1->arr[i] + temp, i );
    for ( i = i; i < ln2->size; i++ )
        temp = set_value ( sum, ln2->arr[i] + temp, i );
    for ( i = i; temp > 0; i++ )
        temp = set_value ( sum, temp, i );
    return SUCCESSFUL_ADDITION;
}

enum return_codes large_number_mul_scale ( large_number *ln, int64_t scale, large_number *prod ) {
    large_number *aux = large_number_init ( scale );
    if ( !aux ) return MEMORY_PROBLEM;
    enum return_codes rc = large_number_mul ( ln, aux, prod );
    large_number_free ( aux );
    return rc;
}

enum return_codes large_number_mul ( large_number *ln1, large_number *ln2, large_number *prod ) {
    large_number *aux = large_number_init ( 0 );
    for ( size_t i = 0; i < ln1->size; i++ ) {
        size_t j, count = 0;
        int8_t temp = 0;
        for ( j = 0; j < ln2->size; j++ )
            temp = add_value ( aux, temp + ln1->arr[i] * ln2->arr[j], i + j );
        while ( temp > 0 ) {
            temp = add_value ( aux, temp, i + j + count );
            count++;
        }
    }
    large_number_swap ( aux, prod );
    large_number_free ( aux );
    return SUCCESSFUL_ADDITION;
}

/* swaps the two large number structures */
void large_number_swap ( large_number *ln1, large_number *ln2 ) {
    universal_swap ( ln1, ln2, LARGE_NUMBER_SIZE );
}

/* frees the memory that the large number structure uses */
void large_number_free ( large_number *ln ) {
    free ( ln->arr );
    free ( ln );
}

/* prints the number saved in the structure */
void large_number_print ( large_number *ln ) {
    if ( ln->size == 0 ) return;
    for ( size_t i = ln->size - 1; i > 0; i-- )
        printf ( "%d", ln->arr[i] ); 
    printf ( "%d\n", ln->arr[0] );
}