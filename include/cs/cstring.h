#ifndef __CS_STRING_H__
#define __CS_STRING_H__

#include <cs/universal.h>

typedef struct nfa nfa;

typedef struct cstring {
    char *_data;
    nfa *_regex_nfa;
} cstring;

cs_codes cstring_init(cstring *str, const char *data);
void cstring_free(void *v_str);

#endif