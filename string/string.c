#include "../include/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cs_codes string_init ( string *s ) {
    s->cap = INIT_CAPACITY;
    s->size = 0;
    s->str = malloc ( INIT_CAPACITY );
    if ( !s->str ) return CS_MEM;
    return CS_SUCCESS;
}

cs_codes string_push_back ( string *s, char ch ) {
    if ( s->size == s->cap ) {
        s->str = realloc ( s->str, s->cap + INIT_CAPACITY );
        if ( !s->str ) return CS_MEM;
        s->cap += INIT_CAPACITY;
    }
    s->str[s->size++] = ch;
    return CS_SUCCESS;
}

cs_codes string_pop_back ( string *s ) {
    if ( s->size == 0 ) return CS_EMPTY;
    s->str[--s->size] = 0;
    return CS_SUCCESS;
}

cs_codes string_append_str ( string *s1, string s2 ) {
    if ( s1->size + s2.size > s1->cap ) {
        s1->cap = s1->size + s2.size + INIT_CAPACITY;
        s1->str = realloc ( s1->str, s1->cap );
        if ( !s1->str ) return CS_MEM;
    }
    for ( int i = 0; i < s2.size; i++ )
        s1->str[s1->size++] = s2.str[i];
    return CS_SUCCESS;
}

cs_codes string_append_chr ( string *s, char *arr, int size ) {
    if ( s->size + size > s->cap ) {
        s->cap = s->size + size + INIT_CAPACITY;
        s->str = realloc ( s->str, s->cap );
        if ( !s->str ) return CS_MEM;
    }
    for ( int i = 0; i < size; i++ )
        s->str[s->size++] = arr[i];
    return CS_SUCCESS;
}

cs_codes string_assign_str ( string *s1, string s2, int start, int count ) {
    if ( s2.size > s1->cap ) {
        s1->cap = s2.size + INIT_CAPACITY;
        s1->str = realloc ( s1->str, s1->cap );
        if ( !s1->str ) return CS_MEM;
    }
    s1->size = 0;
    for ( int i = 0; i < count; i++ )
        s1->str[s1->size++] = s2.str[start + i];
    return CS_SUCCESS;
}

cs_codes string_assign_chr ( string *s, char *arr, int size ) {
    if ( size > s->cap ) {
        s->cap = size + INIT_CAPACITY;
        s->str = realloc ( s->str, s->cap );
        if ( !s->str ) return CS_MEM;
    }
    s->size = 0;
    for ( int i = 0; i < size; i++ )
        s->str[s->size++] = arr[i];
    return CS_SUCCESS;
}

cs_codes string_erase ( string *s, int start, int count ) {
    if ( start < 0 || start + count > s->size ) return CS_SIZE;
    for ( int i = start; i < s->size - count; i++ )
        s->str[i] = s->str[i + count];
    s->size -= count;
    return CS_SUCCESS;
}

cs_codes string_replace_str ( string *s1, string s2, int start, int count ) {
    if ( start < 0 || start + count > s1->size || count > s2.size ) return CS_SIZE;
    for ( int i = 0; i < count; i++ )
        s1->str[start + i] = s2.str[i];
    return CS_SUCCESS;
}

cs_codes string_replace_chr ( string *s, char *arr, int start, int count ) {
    if ( start < 0 || start + count > s->size || count > strlen ( arr ) ) return CS_SIZE;
    for ( int i = 0; i < count; i++ )
        s->str[start + i] = arr[i];
    return CS_SUCCESS;
}

cs_codes string_insert_str ( string *s1, string s2, int start, int count ) {
    if ( start < 0 || start > s1->size || count > s2.size ) return CS_SIZE;
    if ( s1->size + count > s1->cap ) {
        s1->cap = s1->size + count + INIT_CAPACITY;
        s1->str = realloc ( s1->str, s1->cap );
        if ( !s1->str ) return CS_MEM;
    }
    for ( int i = s1->size - 1; i >= start; i-- )
        s1->str[i + count] = s1->str[i];
    for ( int i = 0; i < count; i++ )
        s1->str[start + i] = s2.str[i];
    return CS_SUCCESS;
}

cs_codes string_insert_chr ( string *s, char *arr, int start, int count ) {
    if ( start < 0 || start > s->size || count > strlen ( arr ) ) return CS_SIZE;
    if ( s->size + count > s->cap ) {
        s->cap = s->size + count + INIT_CAPACITY;
        s->str = realloc ( s->str, s->cap );
        if ( !s->str ) return CS_MEM;
    }
    for ( int i = s->size - 1; i >= start; i++ )
        s->str[i + count] = s->str[i];
    for ( int i = 0; i < count; i++ )
        s->str[start + i] = arr[i];
    return CS_SUCCESS;
}

cs_codes string_copy ( string *dst, string src, int start, int count ) {
    dst->size = 0;
    return string_insert_str ( dst, src, start, count );
}

int string_find_str ( string s1, string s2 ) {
    for ( int i = 0; i < s1.size - s2.size; i++ ) {
        int match = 1;
        for ( int j = 0; j < s2.size; j++ )
            if ( s2.str[j] != s1.str[i + j] ) {
                match = 0;
                break;
            }
        if ( match == 1 ) return i;
    }
    return -1;
}

int string_find_chr ( string s, char *arr, int size ) {
    for ( int i = 0; i < s.size - size; i++ ) {
        int match = 1;
        for ( int j = 0; j < size; j++ )
            if ( arr[j] != s.str[i + j] ) {
                match = 0;
                break;
            }
        if ( match == 1 ) return i;
    }
    return -1;
}

int string_rfind_str ( string s1, string s2 ) {
    for ( int i = s1.size - s2.size - 1; i >= 0; i-- ) {
        int match = 1;
        for ( int j = 0; j < s2.size; j++ )
            if ( s2.str[j] != s1.str[i + j] ) {
                match = 0;
                break;
            }
        if ( match == 1 ) return i;
    }
    return -1;
}

int string_rfind_chr ( string s, char *arr, int size ) {
    for ( int i = s.size - size - 1; i >= 0; i++ ) {
        int match = 1;
        for ( int j = 0; j < size; j++ )
            if ( arr[j] != s.str[i + j] ) {
                match = 0;
                break;
            }
        if ( match == 1 ) return i;
    }
    return -1;
}

int string_find_first_of_str ( string s1, string s2 ) {
    for ( int i = 0; i < s1.size; i++ )
        for ( int j = 0; j < s2.size; j++ )
            if ( s1.str[i] == s2.str[j] )
                return i;
    return -1;
}

int string_find_first_of_chr ( string s, char *arr, int size ) {
    for ( int i = 0; i < s.size; i++ )
        for ( int j = 0; j < size; j++ )
            if ( s.str[i] == arr[j] ) 
                return i;
    return -1;
}

int string_find_last_of_str ( string s1, string s2 ) {
    for ( int i = s1.size - 1; i >= 0; i-- )
        for ( int j = 0; j < s2.size; j++ )
            if ( s1.str[i] == s2.str[j] )
                return i;
    return -1;
}

int string_find_last_of_chr ( string s, char *arr, int size ) {
    for ( int i = s.size - 1; i >= 0; i-- )
        for ( int j = 0; j < size; j++ )
            if ( s.str[i] == arr[j] ) 
                return i;
    return -1;
}

int string_find_first_not_of_str ( string s1, string s2 ) {
    for ( int i = 0; i < s1.size; i++ ) {
        int match = 0;
        for ( int j = 0; j < s2.size && match == 0; j++ )
            if ( s1.str[i] == s2.str[j] )
                match = 1;
        if ( match == 0 ) return i;
    }
    return -1;
}

int string_find_first_not_of_chr ( string s, char *arr, int size ) {
    for ( int i = 0; i < s.size; i++ ) {
        int match = 0;
        for ( int j = 0; j < size && match == 0; j++ )
            if ( s.str[i] == arr[j] )
                match = 1;
        if ( match == 0 ) return i;
    }
    return -1;
}

int string_find_last_not_of_str ( string s1, string s2 ) {
    for ( int i = s1.size - 1; i >= 0; i-- ) {
        int match = 0;
        for ( int j = 0; j < s2.size && match == 0; j++ )
            if ( s1.str[i] == s2.str[j] )
                match = 1;
        if ( match == 0 ) return i;
    }
    return -1;
}

int string_find_last_not_of_chr ( string s, char *arr, int size ) {
    for ( int i = s.size - 1; i >= 0; i-- ) {
         int match = 0;
        for ( int j = 0; j < size && match == 0; j++ )
            if ( s.str[i] == arr[j] )
                match = 1;
        if ( match == 0 ) return i;
    }
    return -1;
}

int string_compare ( string s1, string s2 ) {
    for ( int i = 0; i < s1.size && i < s2.size; i++ ) {
        if ( s1.str[i] != s2.str[i] ) return s1.str[i] - s2.str[i];
    }
    if ( s2.size > s1.size ) return -s2.str[s1.size];
    else if ( s1.size > s2.size ) return s1.str[s2.size];
    else return 0;
}

char string_front ( string s ) {  if ( s.size == 0 ) return 0; return s.str[0]; }

char string_back ( string s ) { if ( s.size == 0 ) return 0; return s.str[s.size - 1]; }

char string_at ( string s, int pos ) {
    if ( pos < 0 || pos > s.size - 1 ) return 0;
    return s.str[pos];
}

void string_swap ( string *s1, string *s2 ) {
    int cap = s1->cap;
    int size = s1->size;
    char* str = s1->str;

    s1->cap = s2->cap;
    s1->size = s2->size;
    s1->str = s2->str;

    s2->cap = cap;
    s2->size = size;
    s2->str = str;
}

void string_free ( string *s ) { free ( s->str ); }

void string_print ( string s ) {
    for ( int i = 0; i < s.size; i++ )
        printf ( "%c", s.str[i] );
}