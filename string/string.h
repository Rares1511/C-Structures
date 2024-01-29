#ifndef STRING_H
#define STRING_H

#include "../universal/universal.h"

typedef struct string {
    char *str;
    size_t len, size;
} string;

#define string_length(s) s->len
#define string_size(s) s->size
#define string_chr_array(s) s->str
#define string_empty(s) s->len == 0

enum find_match {
    NO_MATCH, MATCH
};

string *string_init ( );
string *string_copy ( string *str );
enum return_codes string_push_back ( string *str, char ch );
enum return_codes string_pop_back ( string *str );
enum return_codes string_erase ( string *str, size_t pos, size_t len );
enum return_codes string_append_str ( string *dest, string *str );
enum return_codes string_append_chr ( string *dest, char *char_array, size_t len );
enum return_codes string_insert_str ( string *dest, string *str, size_t pos );
enum return_codes string_insert_chr ( string *dest, char *char_array, size_t pos, size_t len );
enum return_codes string_assign_str ( string *dest, string *source );
enum return_codes string_assign_chr ( string *dest, char *source_array, size_t len );
enum return_codes string_replace_str ( string *str1, string *str2, size_t pos );
enum return_codes string_replace_chr ( string *str, char *char_array, size_t pos, size_t len );
char string_at ( string *str, size_t position );
char string_front ( string *str );
char string_back ( string *str );
long string_find_str ( string *str1, string *str2, size_t pos );
long string_find_chr ( string *str, char *char_array, size_t pos, size_t len );
long string_rfind_str ( string *str1, string *str2, size_t pos );
long string_rfind_chr ( string *str, char *char_array, size_t pos, size_t len );
long string_find_first_of_str ( string *str1, string *str2, long pos, enum find_match match );
long string_find_first_of_chr ( string *str, char *arr, size_t pos, size_t len, enum find_match match );
long string_find_last_of_str ( string *str1, string *str2, long pos, enum find_match match );
long string_find_last_of_chr ( string *str, char *arr, size_t pos, size_t len, enum find_match match );
int string_compare ( string *str1, string *str2 );
void string_print ( string *str );
void string_swap ( string *str1, string *str2 );

#endif