#include <cs/dfa.h>
#include <cs/nfa.h>

#include <stdlib.h>

cs_codes nfa_to_dfa(nfa *automaton, dfa **result) {
    CS_RETURN_IF(NULL == automaton || NULL == result, CS_NULL);
    *result = (dfa *)malloc(sizeof(dfa));
    CS_RETURN_IF(NULL == *result, CS_MEM);
    vector_attr_t node_attr = {
        .size = sizeof(dfa_node*),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL
    };
    (*result)->nodes = (vector *)malloc(sizeof(vector));
    if (!(*result)->nodes) {
        free(*result);
        return CS_MEM;
    }
    int rc = vector_init((*result)->nodes, node_attr);
    CS_RETURN_IF(CS_SUCCESS != rc, rc);

    (*result)->start = NULL;
    (*result)->end = NULL;

    return CS_SUCCESS;
}

void dfa_free(dfa *automaton) {
    CS_RETURN_IF(NULL == automaton);
    vector_free(automaton->nodes);
    free(automaton->nodes);
    free(automaton);
}