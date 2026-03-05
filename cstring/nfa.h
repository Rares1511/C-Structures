#ifndef __CS_NFA_H__
#define __CS_NFA_H__

#include <cs/universal.h>
#include <cs/vector.h>

#define NFA_EPSILON '\0'

typedef struct nfa_node {
    char c;
    struct nfa_node *next1;
    struct nfa_node *next2;
} nfa_node;

typedef struct nfa {
    nfa_node *start;
    nfa_node *end;
    vector *nodes;
} nfa;

cs_codes regex_to_nfa(const char *regex, nfa **automaton, int *pos);
void nfa_free(nfa *automaton);

#endif