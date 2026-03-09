#include <cs/nfa.h>

#include <stdlib.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


nfa_node* nfa_node_init(const char c) {
    nfa_node *node = (nfa_node *)malloc(sizeof(nfa_node));
    if (!node) {
        return NULL;
    }
    node->c = c;
    node->next1 = NULL;
    node->next2 = NULL;
    return node;
}

void nfa_node_free(void *v_node) {
    nfa_node *node = *(nfa_node **)v_node;
    CS_RETURN_IF(NULL == node);
    free(node);
}

nfa* nfa_init(const char c) {
    nfa *automaton = (nfa *)malloc(sizeof(nfa));
    CS_RETURN_IF(NULL == automaton, NULL);
    vector_attr_t node_attr = {
        .size = sizeof(nfa_node*),
        .comp = NULL,
        .copy = NULL,
        .fr = nfa_node_free,
        .print = NULL
    };
    automaton->nodes = (vector *)malloc(sizeof(vector));
    if (!automaton->nodes) {
        free(automaton);
        return NULL;
    }
    CS_RETURN_IF(CS_SUCCESS != vector_init(automaton->nodes, node_attr), NULL);
    automaton->start = nfa_node_init(c);
    if (!automaton->start) {
        free(automaton);
        return NULL;
    }
    if (c == NFA_EPSILON) {
        automaton->end = automaton->start;
        if (vector_push_back(automaton->nodes, &automaton->start) != CS_SUCCESS) {
            free(automaton->start);
            free(automaton->nodes);
            free(automaton);
            return NULL;
        }
        return automaton;
    }
    automaton->end = nfa_node_init(NFA_EPSILON);
    if (!automaton->end) {
        free(automaton->start);
        free(automaton->nodes);
        free(automaton);
        return NULL;
    }
    automaton->start->next1 = automaton->end;
    if (vector_push_back(automaton->nodes, &automaton->start) != CS_SUCCESS ||
        vector_push_back(automaton->nodes, &automaton->end) != CS_SUCCESS) {
        free(automaton->start);
        free(automaton->end);
        free(automaton->nodes);
        free(automaton);
        return NULL;
    }
    return automaton;
}

nfa* nfa_concat(nfa *a, nfa *b) {
    a->end->c = NFA_EPSILON;
    a->end->next1 = b->start;
    a->end = b->end;
    for (int i = 0; i < b->nodes->size; i++) {
        nfa_node *node = *(nfa_node **)vector_at((*b->nodes), i);
        vector_push_back(a->nodes, &node);
    }
    free(b->nodes->vec);
    free(b->nodes);
    free(b);
    return a;
}

nfa* nfa_union(nfa *a, nfa *b) {
    nfa *automaton = nfa_init(NFA_EPSILON);
    if (!automaton) {
        return NULL;
    }
    automaton->start->next1 = a->start;
    automaton->start->next2 = b->start;
    automaton->end = nfa_node_init(NFA_EPSILON);
    if (!automaton->end) {
        nfa_free(automaton);
        return NULL;
    }
    a->end->c = NFA_EPSILON;
    a->end->next1 = automaton->end;
    b->end->c = NFA_EPSILON;
    b->end->next1 = automaton->end;
    for (int i = 0; i < a->nodes->size; i++) {
        nfa_node *node = *(nfa_node **)vector_at((*a->nodes), i);
        vector_push_back(automaton->nodes, &node);
    }
    for (int i = 0; i < b->nodes->size; i++) {
        nfa_node *node = *(nfa_node **)vector_at((*b->nodes), i);
        vector_push_back(automaton->nodes, &node);
    }
    free(a->nodes->vec);
    free(b->nodes->vec);
    free(a->nodes);
    free(b->nodes);
    free(a);
    free(b);
    return automaton;
}

nfa* nfa_star(nfa *a) {
    nfa *automaton = nfa_init(NFA_EPSILON);
    if (!automaton) {
        return NULL;
    }
    automaton->start->next1 = a->start;
    automaton->start->next2 = automaton->end;
    a->end->c = NFA_EPSILON;
    a->end->next1 = a->start;
    a->end->next2 = automaton->end;
    for (int i = 0; i < a->nodes->size; i++) {
        nfa_node *node = *(nfa_node **)vector_at((*a->nodes), i);
        vector_push_back(automaton->nodes, &node);
    }
    free(a->nodes->vec);
    free(a->nodes);
    free(a);
    return automaton;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes regex_to_nfa(const char *regex, nfa **result, int *pos) {
    *result = nfa_init(NFA_EPSILON);
    CS_RETURN_IF(NULL == *result, CS_MEM);
    for (; regex[*pos] != '\0'; (*pos)++) {
        switch (regex[*pos]) {
            case '*':
                *result = nfa_star(*result);
                CS_RETURN_IF(NULL == *result, CS_MEM);
                break;
            case '|':
                nfa *next_nfa;
                (*pos)++;
                int rc = regex_to_nfa(regex, &next_nfa, pos);
                CS_RETURN_IF(rc != CS_SUCCESS, rc);
                *result = nfa_union(*result, next_nfa);
                break;
            case '(':
                nfa *group_nfa;
                (*pos)++;
                int rc_group = regex_to_nfa(regex, &group_nfa, pos);
                CS_RETURN_IF(rc_group != CS_SUCCESS, rc_group);
                *result = nfa_concat(*result, group_nfa);
                break;
            case ')':
                (*pos)++;
                return CS_SUCCESS;
                break;
            default:
                nfa *char_nfa = nfa_init(regex[*pos]);
                CS_RETURN_IF(NULL == char_nfa, CS_MEM);
                *result = nfa_concat(*result, char_nfa);
                break;
        }

        CS_RETURN_IF(NULL == (*result), CS_MEM);
    }
    return CS_SUCCESS;
}

void nfa_free(nfa *automaton) {
    CS_RETURN_IF(NULL == automaton);
    vector_free(automaton->nodes);
    free(automaton->nodes);
    free(automaton);
}