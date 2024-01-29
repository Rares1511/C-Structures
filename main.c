#include "universal/universal.h"
#include "pair/pair.h"
#include <stddef.h>

struct human {
    float height;
    int age;
};

void human_print ( void* el ) {
    struct human h = *(struct human*) el;
    printf ( "human: height: %f, age: %d\n", h.height, h.age );
}

void print_int_1 ( void *el ) {
    int a = *(int *) el;
    printf ( "%d ", a );
}

void print_int_2 ( void *el ) {
    int a = *(int *) el;
    printf ( "%d\t", a );
}

int main ( ) {

    pair p1, p2;
    pair_attr_t fst_attr, snd_attr;
    int a, b;

    fst_attr.fr = NULL, fst_attr.print = print_int_1, fst_attr.size = sizeof ( int );
    snd_attr.fr = NULL, snd_attr.print = print_int_2, snd_attr.size = sizeof ( int );

    pair_init ( &p1, fst_attr, fst_attr );
    pair_init ( &p2, snd_attr, snd_attr );

    a = 69, b = 70;
    pair_set_both_value ( &p1, &a, &b );

    a = 36, b = 23;
    pair_set_both_value ( &p2, &a, &b );

    printf ( "before:\n" );
    pair_print_both ( &p1 );
    printf ( "\n" );
    pair_print_both ( &p2 );
    printf ( "\n" );

    pair_swap ( &p1, &p2 );

    printf ( "after:\n" );
    pair_print_both ( &p1 );
    printf ( "\n" );
    pair_print_both ( &p2 );
    printf ( "\n" );

    pair_free ( &p1 );
    pair_free ( &p2 );
    return 0;    
}