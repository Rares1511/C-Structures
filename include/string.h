#ifndef STRING_H
#define STRING_H

#include "universal.h"

typedef struct string {
    char* str;   /*!< the string contained in the data structure */
    int   cap;   /*!< current maximum capacity of the string */
    int   size;  /*!< current size of the string */
} string;

/*!
 * Initializes the string structure
 * @param[out] s  String structure that will be initialized
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful initialization
 */
cs_codes string_init ( string *s );

/*!
 * Pushes the character at the back of the string
 * @param[out] s  String structure in which the character will be added
 * @param[in] ch  Character to be added
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes string_push_back ( string *s, char ch );

/*!
 * Pops the last element in the string if it's not empty
 * @param[out] s  String structure that will have its last element deleted
 * @return CS_EMPTY if the string is empty or CS_SUCCESS for a successful deletion
 */
cs_codes string_pop_back ( string *s );

/*!
 * Appends the second string offered at the end of the first string
 * @param[out] dst  String in which the other one will be inserted
 * @param[in]  src  String that will be inserted
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes string_append_str ( string *dst, string src );

/*!
 * Appends the char array given at the end of the string structure
 * @param[out] dst   String in which the character array will be inserted into
 * @param[in]  src   Array of characters that will be inserted
 * @param[in]  size  Number of characters to be appended from the array
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes string_append_chr ( string *dst, char *src, int size );

/*!
 * Assigns the value of the second string to the first string
 * @param[out] dst    Destionation string in which the characters will be assigned
 * @param[in]  src    Source string for the characters to be assigned
 * @param[in]  start  Starting position from which the characters will be copied from
 * @param[in]  count  Number of characters to assign in the destination string 
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes string_assign_str ( string *dst, string src, int start, int count );

/*!
 * Assignes the value of the character array to the given string structure
 * @param[out] dst   Destionation string in which the characters will be assigned
 * @param[in]  src   Source array of characters
 * @param[in]  size  Number of characters to be used in the assignation
 * @return CS_MEM if a memory problem occurred or CS_SUCCESS for a successful addition
 */
cs_codes string_assign_chr ( string *dst, char *src, int size );

/*!
 * Erases a number of characters from the string structure
 * @param[out] s      String from which the characters will be deleted
 * @param[in]  start  Starting position from which count characters will be deleted
 * @param[in]  count  Number of characters to be deleted
 * @return CS_SIZE if given a wrong size to delete or CS_SUCCESS for a successful deletion
 */
cs_codes string_erase ( string *s, int start, int count );

/*!
 * Replaces a number of characters with the first count characters of the src string
 * @param[out] dst    Destination string that will have its characters replaced
 * @param[in]  src    Source string from which the value of new characters will be taken
 * @param[in]  start  Starting position for characters to be replaced
 * @param[in]  count  Number of characters to be replaced
 * @return CS_SIZE if given a wrong size to replace characters or CS_SUCCESS for a successful
 * replacement
 */
cs_codes string_replace_str ( string *dst, string src, int start, int count );

/*!
 * Replaces a number of characters with the first count characters of the array given
 * @param[out] dst    Destination string that will have its characters replaced
 * @param[in]  src    Source array of characters that will replace characters in the string
 * @param[in]  start  Starting position in the string to start replacing characters
 * @param[in]  count  Number of characters to be replaced
 * @return CS_SIZE if given a wrong size to replace characters or CS_SUCCESS for a successful
 * replacement
 */
cs_codes string_replace_chr ( string *dst, char *src, int start, int count );

/*!
 * Inserts a number of characters into the destionation string from the source destionation
 * @param[out] dst    Destionation string in which the new characters will be inserted to
 * @param[in]  src    Source string that will have its first count characters inserted in the 
 * destionation string
 * @param[in]  start  Starting position to start inserting new characters from the source string
 * @param[in]  count  Number of characters to be inserted
 * @return CS_SIZE if given a wrong size to replace characters or CS_SUCCESS for a successful
 * replacement
 */
cs_codes string_insert_str ( string *dst, string src, int start, int count );

/*!
 * Inserts a number of characters into the destionation string from the character array
 * @param[out] dst    Destionation string in which the new characters will be inserted to
 * @param[in]  src    Source array that will have its first count characerts inserted in the 
 * destionation string
 * @param[in]  start  Starting position to start inserting new characters from the source array
 * @param[in]  count  Number of characters to be inserted
 * @return CS_SIZE if given a wrong size to replace characters or CS_SUCCESS for a successful
 * replacement
 */
cs_codes string_insert_chr ( string *dst, char *src, int start, int count );

/*!
 * Makes a copy of the source string into the destionation string 
 * @param[out] dst    Destionation string to place the copy
 * @param[in]  src    Source string to use for the copy
 * @param[in]  start  Starting position to start copying characters
 * @param[in]  count  Number of charcters to use in the copy
 * @return CS_SIZE if given a wrong size to replace characters or CS_SUCCESS for a successful
 * replacement
 */
cs_codes string_copy ( string *dst, string src, int start, int count );

/*!
 * Finds the first apparition of the second string in the first one
 * @param[in] s1  String in which the second one is searched
 * @param[in] s2  String that is searched
 * @return -1 if the second string is not found or the starting position of the first
 * apparition
 */
int string_find_str ( string s1, string s2 );

/*!
 * Finds the first apparition of the character array in the string
 * @param[in] s     String in which the array is searched
 * @param[in] arr   Character array that is searched
 * @param[in] size  Size of the character array
 * @return -1 if the array is not found or the starting position of the first apparition
 */
int string_find_chr ( string s, char *arr, int size );

/*!
 * Finds the last apparition of the second string in the first string
 * @param[in] s1  String in which the second one is searched
 * @param[in] s2  String that is searched
 * @return -1 if the string is not found or the starting position of the last apparition
 */
int string_rfind_str ( string s1, string s2 );

/*!
 * Finds the last apparition of the character array in the string
 * @param[in] s     String in which the array will be searched
 * @param[in] arr   Array to be searched
 * @param[in] size  Number of characters to be used from the array
 * @return -1 if the array is not found or the starting position of the last apparition
 */
int string_rfind_chr ( string s, char *arr, int size );

/*!
 * Finds the first apparition of a character from the second string in the first string
 * @param[in] s1  String in which the characters from s2 will be searched
 * @param[in] s2  String whose character will be searched
 * @return -1 if s1 has no characters from s2 or the first position of a character from s2
 */
int string_find_first_of_str ( string s1, string s2 );

/*!
 * Finds the first apparition of a character from the array in the string
 * @param[in] s     String in which the characters from the array will be searched
 * @param[in] arr   Array whose characters will be searched
 * @param[in] size  Number of characters to be used from the array
 * @return -1 if the string has no characters from the array or the first position of a character
 * from the array in the string
 */
int string_find_first_of_chr ( string s, char *arr, int size );

/*!
 * Finds the last apparition of a character from s2 in s1
 * @param[in] s1  String in which the characters from the second one will be searched
 * @param[in] s2  String whose characters will be searched
 * @return -1 if s1 has no characters from s2 or the last apparition of a character from s2
 */
int string_find_last_of_str ( string s1, string s2 );

/*!
 * Finds the last apparition of a character from the array in the string
 * @param[in] s     String in which the characters from the array will be searched
 * @param[in] arr   Array whose characters will be searched
 * @param[in] size  Number of characters to be used from the array
 * @return -1 if no character from the array is in the string or the last apparition of a character
 * from the array in the string
 */
int string_find_last_of_chr ( string s, char *arr, int size );

/*!
 * Finds the first apparition of a character that is not in s2 from s1
 * @param[in] s1  String in which the apparition of non-s2 characters will be searched
 * @param[in] s2  String whose characters will be searched
 * @return -1 if s1 contains only characters from s2 or the first apparition of a character that is
 * not in s2 in s1
 */
int string_find_first_not_of_str ( string s1, string s2 );

/*!
 * Finds the first apparition of a character that is not in the array in the string
 * @param[in] s     String whose characters will be compared to the characters of the array
 * @param[in] arr   Array of characters that will be searched in the string
 * @param[in] size  Number of characters to be used from the array
 * @return -1 if the string has only characters from the array or the first position of a character
 * that is not in the array in the string
 */
int string_find_first_not_of_chr ( string s, char *arr, int size );

/*!
 * Finds the last apparition of a character that is not from s2 in s1
 * @param[in] s1  String whose characters will be compared to the second string characters
 * @param[in] s2  String whose characters will be searched in the first string
 * @return -1 if s1 contains only characters from s2 or the last position of a character that is not
 * from s2 in s1
 */
int string_find_last_not_of_str ( string s1, string s2 );

/*!
 * Finds the last apparition of a character that is not from the array in th string
 * @param[in] s     String in which the characters that will be compared to the character array
 * @param[in] arr   Array of characters to be searched in the string
 * @param[in] size  Numbers of characters to be used in the array
 * @return -1 if the string has only characters from the array or the last apparition of a character
 * that is not from the array in the string
 */
int string_find_last_not_of_chr ( string s, char *arr, int size );

/*!
 * Compares the two strings
 * @param[in] s1,s2  The two strings that will be compared
 * @return The difference between the two different characters from the strings or 0
 * if they are the same 
 */
int string_compare ( string s1, string s2 );

/*!
 * Gives the character at the front of the string 
 * @param[in] s  String whose first character will be accessed
 * @return The first character of the string or 0 if the string is empty
 */
char string_front ( string s );

/*!
 * Gives the character at the back of the string 
 * @param[in] s  String whose last character will be accessed
 * @return The last character of the string or 0 if the string is empty
 */
char string_back ( string s );

/*!
 * Gives the character at the position given from the string
 * @param[in] s    String whose character at the position given will be accessed
 * @param[in] pos  Position of the character that will be accessed
 * @return The character at the position given or 0 if the position given is outside the
 * string
 */
char string_at ( string s, int pos );

/*!
 * Frees the memory that the string structure uses
 * @param[in] s  String structure whose memory will be freed
 */
void string_free ( string *s );

/*!
 * Prints the string
 * @param[in] s  String that will be printed
 */
void string_print ( string s );

#endif