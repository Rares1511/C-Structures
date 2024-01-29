#include "string.h"

/* initializes the string variable and returns a ptr to it or NULL if the memory 
allocation failed */
string *string_init ( ) {
    string *s = malloc ( sizeof ( string ) );
    if ( !s ) return NULL;
    s->len = 0;
    s->size = 1024;
    s->str = malloc ( s->size );
    if ( !s->str ) return NULL;
    return s;
}

/* creates a copy of the string offered and returns a pointer to it */
string *string_copy ( string *str ) {
    string *aux = string_init ( );
    string_assign_str ( aux, str );
    return aux;
} 

/* pushes the character at the end of the string */
enum return_codes string_push_back ( string *s, char ch ) {
    if ( s->len == s->size ) {
        s->size += 1024;
        s->str = realloc ( s->str, s->size );
        if ( !s->str ) return MEMORY_PROBLEM;
    }
    s->str[s->len++] = ch;
    return SUCCESSFUL_ADDITION;
}

/* deletes the last character */
enum return_codes string_pop_back ( string *s ) {
    if ( s->len == 0 ) return WRONG_POSITION;
    if ( s->len < s->size / 3 ) {
        s->size /= 2;
        s->str = realloc ( s->str, s->size );
        if ( !s->str ) return MEMORY_PROBLEM;
    }
    s->len--;
    return SUCCESSFUL_DELETION;
}

/* deletes len characters from the position given */
enum return_codes string_erase ( string *s, size_t p, size_t l ) {
    if ( p >= s->len ) return WRONG_POSITION;
    for ( size_t i = p; i < s->len; i++ ) {
        if ( i + l < s->len )
            s->str[i] = s->str[i + l];
        else
            s->str[i] = 0;
    }
    if ( p + l > s->len )
        s->len += ( p + l - s->len );
    s->len -= l;
    return SUCCESSFUL_DELETION;
}

/* appends the char array to the string offered */
enum return_codes string_append_chr ( string *s, char *ch_arr, size_t len ) {
    int rc;
    for ( size_t i = 0; i < len; i++ ) {
        rc = string_push_back ( s, ch_arr[i] );
        if ( rc == MEMORY_PROBLEM ) return rc;
    }
    return SUCCESSFUL_ADDITION;
}

/* appends the second string into the first */
enum return_codes string_append_str ( string *s1, string *s2 ) {
    return string_append_chr ( s1, s2->str, s2->len );
}

/* inserts the first len characters of the char array in the string from the position offered */
enum return_codes string_insert_chr ( string *s, char *arr, size_t pos, size_t len ) {
    if ( pos >= s->len ) return WRONG_POSITION;
    if ( s->len + len > s->size ) {
        s->size += len + INIT_CAPACITY;
        s->str = realloc ( s->str, s->size );
        if ( !s->str ) return MEMORY_PROBLEM;
    }
    for ( size_t i = s->len - 1; i > pos; i-- ) 
        s->str[i + len] = s->str[i];
    s->str[pos + len] = s->str[pos];
    for ( size_t i = 0; i < len; i++ )
        s->str[pos + i] = arr[i];
    s->len += len;
    return SUCCESSFUL_ADDITION;
}

/* inserts the second string into the first one from the position given */
enum return_codes string_insert_str ( string *s1, string *s2, size_t pos ) {
    return string_insert_chr ( s1, s2->str, pos, s2->len );
}

/* replaces the value of the string with the char array */
enum return_codes string_assign_chr ( string *s, char *arr, size_t len ) {
    if ( len > s->size ) {
        s->size = len + INIT_CAPACITY;
        s->str = realloc ( s->str, s->size );
        if ( !s->str ) return MEMORY_PROBLEM;
    }
    for ( size_t i = 0; i < len; i++ )
        s->str[i] = arr[i];
    s->len = len;
    return SUCCESSFUL_REPLACEMENT;
}

/* replaces the value of the first string with the value of the second string */
enum return_codes string_assign_str ( string *s1, string *s2 ) {
    return string_assign_chr ( s1, s2->str, s2->len );
}

/* replaces the text of s1 with the text of the charr array given starting from pos */
enum return_codes string_replace_chr ( string *s, char *arr, size_t pos, size_t len ) {
    if ( pos >= s->len ) return WRONG_POSITION; 
    for ( size_t i = 0; i < len && pos + i < s->len; i++ )
        s->str[pos + i] = arr[i];
    return SUCCESSFUL_REPLACEMENT;
}

/* replaces the text of s1 with the text of s2 starting from pos */
enum return_codes string_replace_str ( string *s1, string *s2, size_t pos ) {
    return string_replace_chr ( s1, s2->str, pos, s2->len );
}

/* returns the character from the string offered at the position given */
char string_at ( string *s, size_t pos ) {
    if ( pos >= s->len ) return 0;
    return s->str[pos];
}

/* returns the last character of the string */
char string_back ( string *s ) { return string_at ( s, s->len - 1 ); }

/* returns the first character of the string */
char string_front ( string *s ) { return string_at ( s, 0 ); }

/* returns the position at which the char array is found within the string
starting from the position offered */
long string_find_chr ( string *s, char *arr, size_t pos, size_t len ) {
    if ( len > s->len || pos > s->len ) return -1;
    for ( size_t i = pos; i < s->len - len; i++ ) {
        char found = 0;
        for ( size_t j = 0; j < len && found == 0; j++ )
            if ( s->str[i + j] != arr[j] )
                found = 1;
        if ( found == 0 )
            return i;
    }
    return -1;
}

/* returns the position at which the second string is found within the first string
starting from the position offered */
long string_find_str ( string *s1, string *s2, size_t pos ) {
    return string_find_chr ( s1, s2->str, pos, s2->len );
}

/* returns the last position at which the char array is found within the string
starting from the position offered */
long string_rfind_chr ( string *s, char *arr, size_t pos, size_t len ) {
    if ( len > s->len || pos > s->len ) return -1;
    for ( size_t i = s->len - len - 1; i >= pos; i-- ) {
        char found = 0;
        for ( size_t j = 0; j < len && found == 0; j++ )
            if ( s->str[i + j] != arr[j] )
                found = 1;
        if ( found == 0 )
            return i;
    }
    return -1;
}

/* returns the last position at which the second string is found within the first string
starting from the position offered */
long string_rfind_str ( string *s1, string *s2, size_t pos ) {
    return string_rfind_chr ( s1, s2->str, pos, s2->len );
}

/* finds the first occurance of one of the characters of the char array in the
string starting from the given position */
long string_find_first_of_chr ( string *s, char *arr, size_t pos, size_t len, enum find_match m ) {
    if ( pos >= s->len ) return -1;
    for ( size_t i = pos; i < s->len; i++ )
        for ( size_t j = 0; j < len; j++ )
            if ( ( s->str[i] == arr[j] ) == m ) 
                return i;
    return -1;
}

/* finds the first occurance of one of the characters of second string in the
first string starting from the given position */
long string_find_first_of_str ( string *s1, string *s2, long pos, enum find_match m ) {
    return string_find_first_of_chr ( s1, s2->str, pos, s2->len, m );
}

/* find the last occurance of one of the characters in the char array present in
the string starting from the given position */
long string_find_last_of_chr ( string *s, char *arr, size_t pos, size_t len, enum find_match m ) {
    if ( pos >= s->len ) return -1;
    for ( size_t i = s->len - 1; i >= pos; i-- ) {
        for ( size_t j = 0; j < len; j++ )
            if ( ( s->str[i] == arr[j] ) == m ) 
                return i;
        if ( i == 0 )
            return -1;
    }
    return -1;
}

/* find the last occurance of one of the characters in the second string present in
the first string starting from the given position */
long string_find_last_of_str ( string *s1, string *s2, long pos, enum find_match m ) {
    return string_find_last_of_chr ( s1, s2->str, pos, s2->len, m );
}

/* compares the two strings given */
int string_compare ( string *s1, string *s2 ) {
    for ( size_t i = 0; i < s1->len || i < s2->len; i++ ) {
        if ( s1->str[i] != s2->str[i] )
            return s1->str[i] - s2->str[i];
    }
    return 0;
}

/* swaps the two strings */
void string_swap ( string *s1, string *s2 ) {
    universal_swap ( s1, s2, sizeof ( string ) );
}

/* prints the string stored in the string variable given */
void string_print ( string *s ) {
    if ( s->str )
        for ( size_t i = 0; i < s->len; i++ )
            printf ( "%c", s->str[i] );
    else
        printf ( "NULL" );
}