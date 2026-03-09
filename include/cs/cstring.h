#ifndef __CS_STRING_H__
#define __CS_STRING_H__

#include <cs/universal.h>

typedef struct nfa nfa;
typedef struct dfa dfa;

typedef struct cstring {
    char *_data;
    nfa *_regex_nfa;
    dfa *_regex_dfa;
} cstring;

cs_codes cstring_init(cstring *str, const char *data);
void cstring_free(void *v_str);

#endif