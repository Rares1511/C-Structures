#include "deque_internal.h"

deque_node_t *deque_node_init(void *data, int size, deepcopy cp) {
    deque_node_t *node = malloc(sizeof(deque_node_t));
    if (!node)
        return NULL;
    node->data = malloc(size);
    if (!node->data) {
        free(node);
        return NULL;
    }
    if (cp)
        cp(node->data, data);
    else
        memcpy(node->data, data, size);
    node->next = NULL;
    node->prev = NULL;
    return node;
}