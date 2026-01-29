#include <cs/list.h>

#include <stdlib.h>
#include <string.h>

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * Initializes a list node with the given element
 * @param[in] el    Element that will be added into the node
 * @param[in] size  Size of the element that will be added
 * @param[in] copy  Copy function for the datatype inside the list
 * @return Pointer to the initialized node or NULL if a memory problem occurred
 */
list_node* list_node_init(const void *el, int size, deepcopy copy) {
    list_node *aux = malloc(sizeof(list_node));
    if (!aux)
        return NULL;
    aux->data = malloc(size);
    if (!aux->data) {
        free(aux);
        return NULL;
    }
    if (copy)
        copy(aux->data, el);
    else
        memcpy(aux->data, el, size);
    aux->next = aux;
    aux->prev = aux;
    return aux;
}

/*!
 * Compares two elements using the given comp function or memcmp if comp is NULL
 * @param[in] a     First element to be compared
 * @param[in] b     Second element to be compared
 * @param[in] comp  Comparison function for the datatype inside the list
 * @param[in] size  Size of the elements that will be compared
 * @return Negative value if a < b, 0 if a == b, positive value if a > b
 */
int list_compare(const void *a, const void *b, comparer comp, int size) {
    if (comp)
        return comp((void *)a, (void *)b);
    return memcmp(a, b, size);
}

/*!
 * Helper function for sorting the list using quicksort algorithm
 * @param[in] attr  Attributes for the datatype inside the list
 * @param[in] start Starting node for the sort
 * @param[in] end   Ending node for the sort
 */
void list_sort_helper(list_attr_t attr, list_node *start, list_node *end) {
    if (start == end)
        return;
    void *pivot = end->data;
    list_node *pivot_node = start;

    for (list_node *node = start; node != end; node = node->next) {
        if (list_compare(node->data, pivot, attr.comp, attr.size) <= 0) {
            void* temp = pivot_node->data;
            pivot_node->data = node->data;
            node->data = temp;
            pivot_node = pivot_node->next;
        }
    }

    void* temp = pivot_node->data;
    pivot_node->data = end->data;
    end->data = temp;

    if (pivot_node != start)
        list_sort_helper(attr, start, pivot_node->prev);
    if (pivot_node != end)
        list_sort_helper(attr, pivot_node->next, end);
}

/*!
 * Frees a list node and its data using the given free function
 * @param[in] node  Node that will be freed
 * @param[in] fr    Free function for the datatype inside the list
 */
void list_node_free(list_node *node, freer fr) {
    if (fr)
        fr(node->data);
    free(node->data);
    free(node);
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

list *list_init(list_attr_t attr) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, NULL);
    list *l = malloc(sizeof(list));
    CS_RETURN_IF(l == NULL, NULL);
    l->attr = attr;
    l->meta = malloc(sizeof(metadata_t));
    CS_RETURN_IF(l->meta == NULL, NULL);
    metadata_init(l->meta);
    l->front = NULL;
    return l;
}

cs_codes list_push_front(list *l, const void *el) {
    CS_RETURN_IF(l == NULL || el == NULL, CS_NULL);
    list_node *aux = list_node_init(el, l->attr.size, l->attr.copy);
    CS_RETURN_IF(aux == NULL, CS_MEM);
    if (!list_empty(*l)) {
        aux->prev = l->front->prev;
        l->front->prev->next = aux;
        l->front->prev = aux;
        aux->next = l->front;
    }
    l->front = aux;
    metadata_size_inc(l->meta, 1);
    return CS_SUCCESS;
}

cs_codes list_push_back(list *l, const void *el) {
    CS_RETURN_IF(l == NULL || el == NULL, CS_NULL);
    list_node *aux = list_node_init(el, l->attr.size, l->attr.copy);
    CS_RETURN_IF(aux == NULL, CS_MEM);
    if (!list_empty(*l)) {
        l->front->prev->next = aux;
        aux->prev = l->front->prev;
        aux->next = l->front;
        l->front->prev = aux;
    }
    else
        l->front = aux;
    metadata_size_inc(l->meta, 1);
    return CS_SUCCESS;
}

cs_codes list_pop_front(list *l) {
    CS_RETURN_IF(l == NULL, CS_NULL);
    CS_RETURN_IF(list_empty(*l), CS_EMPTY);
    metadata_size_inc(l->meta, -1);
    if (list_empty(*l)) {
        list_node_free(l->front, l->attr.fr);
        l->front = NULL;
        return CS_SUCCESS;
    }
    list_node *aux = l->front;
    l->front->prev->next = l->front->next;
    l->front->next->prev = l->front->prev;
    l->front = l->front->next;
    list_node_free(aux, l->attr.fr);
    return CS_SUCCESS;
}

cs_codes list_pop_back(list *l) {
    CS_RETURN_IF(l == NULL, CS_NULL);
    CS_RETURN_IF(list_empty(*l), CS_EMPTY);
    metadata_size_inc(l->meta, -1);
    if (list_empty(*l)) {
        list_node_free(l->front, l->attr.fr);
        return CS_SUCCESS;
    }
    list_node *aux = l->front->prev;
    l->front->prev->prev->next = l->front;
    l->front->prev = l->front->prev->prev;
    list_node_free(aux, l->attr.fr);
    return CS_SUCCESS;
}

cs_codes list_erase(list *l, int pos) {
    CS_RETURN_IF(l == NULL, CS_NULL);
    CS_RETURN_IF(list_empty(*l), CS_EMPTY);
    CS_RETURN_IF(pos < 0 || pos >= list_size(*l), CS_POS);

    if (pos == 0)
        return list_pop_front(l);
    if (pos == list_size(*l) - 1)
        return list_pop_back(l);

    list_node *current = l->front;
    for (; pos > 0; pos--, current = current->next);

    current->prev->next = current->next;
    current->next->prev = current->prev;
    list_node_free(current, l->attr.fr);
    metadata_size_inc(l->meta, -1);
    return CS_SUCCESS;
}

void *list_front(list l) {
    CS_RETURN_IF(list_empty(l), NULL);
    return l.front->data;
}

void *list_back(list l) {
    CS_RETURN_IF(list_empty(l), NULL);
    return l.front->prev->data;
}

void list_sort(list *l) {
    CS_RETURN_IF(l == NULL);
    list_sort_helper(l->attr, l->front, l->front->prev);
}

void list_swap(list *l1, list *l2) {
    CS_RETURN_IF(l1 == NULL || l2 == NULL);

    list_attr_t attr = l1->attr;
    list_node *front = l1->front;
    metadata_t *meta = l1->meta;

    l1->attr = l2->attr;
    l1->front = l2->front;
    l1->meta = l2->meta;

    l2->attr = attr;
    l2->front = front;
    l2->meta = meta;
}

void list_clear(list *l) {
    CS_RETURN_IF(l == NULL);
    list_node *node = l->front->next;
    while (node != l->front) {
        list_node *aux = node;
        node = node->next;
        list_node_free(aux, l->attr.fr);
    }
    list_node_free(l->front, l->attr.fr);
    l->front = NULL;
    metadata_size_inc(l->meta, -l->meta->size);
}

void list_print(FILE *stream, void *l_p) {
    CS_RETURN_IF(l_p == NULL || stream == NULL);
    list l = *(list *)l_p;
    CS_RETURN_IF(list_empty(l) || l.attr.print == NULL);
    l.attr.print(stream, l.front->data);
    list_node *node = l.front->next;
    while (node != l.front) {
        l.attr.print(stream, node->data);
        node = node->next;
    }
}

void list_free(void *l_p) {
    CS_RETURN_IF(l_p == NULL);
    list *l = (list *)l_p;
    if (!list_empty(*l)) {
        list_node *node = l->front->next;
        while (node != l->front) {
            list_node *aux = node;
            node = node->next;
            list_node_free(aux, l->attr.fr);
        }
        list_node_free(l->front, l->attr.fr);
    }
    free(l->meta);
    free(l);
}