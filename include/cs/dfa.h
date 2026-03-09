#ifndef __CS_DFA_H__
#define __CS_DFA_H__

#include <cs/universal.h>
#include <cs/vector.h>

typedef struct nfa nfa;

typedef struct dfa_node {
    char c;
} dfa_node;

typedef struct dfa {
    vector *nodes;
    dfa_node *start;
    dfa_node *end;
} dfa;

cs_codes nfa_to_dfa(nfa *automaton, dfa **result);
void dfa_free(dfa *automaton);

#endif