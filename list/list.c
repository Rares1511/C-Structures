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
list_node* list_node_init(void *el, int size, deepcopy copy) {
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

cs_codes list_init(list *l, list_attr_t attr) {
    if (attr.size < 0 || attr.size > SIZE_TH)
        return CS_SIZE;
    l->attr = attr;
    l->size = 0;
    l->front = NULL;
    return CS_SUCCESS;
}

cs_codes list_push_front(list *l, void *el) {
    list_node *aux = list_node_init(el, l->attr.size, l->attr.copy);
    if (!aux)
        return CS_MEM;
    if (l->size != 0) {
        aux->prev = l->front->prev;
        l->front->prev->next = aux;
        l->front->prev = aux;
        aux->next = l->front;
    }
    l->front = aux;
    l->size++;
    return CS_SUCCESS;
}

cs_codes list_push_back(list *l, void *el) {
    list_node *aux = list_node_init(el, l->attr.size, l->attr.copy);
    if (!aux)
        return CS_MEM;
    if (l->size != 0) {
        l->front->prev->next = aux;
        aux->prev = l->front->prev;
        aux->next = l->front;
        l->front->prev = aux;
    }
    else
        l->front = aux;
    l->size++;
    return CS_SUCCESS;
}

cs_codes list_pop_front(list *l) {
    if (l->size == 0)
        return CS_EMPTY;
    l->size--;
    if (l->size == 0) {
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
    if (l->size == 0)
        return CS_EMPTY;
    l->size--;
    if (l->size == 0) {
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
    if (pos < 0 || pos >= l->size)
        return CS_SIZE;

    if (l->size == 1 || pos == 0) {
        list_node *aux = l->front;
        l->front = l->front->next;
        list_node_free(aux, l->attr.fr);
        l->size--;
        if (l->size == 0)
            l->front = NULL;
        return CS_SUCCESS;
    }

    list_node *current = l->front;
    for (int i = 0; i < pos; i++, current = current->next);

    current->prev->next = current->next;
    current->next->prev = current->prev;
    list_node_free(current, l->attr.fr);
    l->size--;
    return CS_SUCCESS;
}

void *list_front(list l) {
    if (l.size == 0)
        return NULL;
    return l.front->data;
}

void *list_back(list l) {
    if (l.size == 0)
        return NULL;
    return l.front->prev->data;
}

cs_codes list_sort(list *l) {
    list_sort_helper(l->attr, l->front, l->front->prev);
    return CS_SUCCESS;
}

void list_swap(list *l1, list *l2) {
    list_attr_t attr = l1->attr;
    list_node *front = l1->front;
    int size = l1->size;

    l1->attr = l2->attr;
    l1->front = l2->front;
    l1->size = l2->size;

    l2->attr = attr;
    l2->front = front;
    l2->size = size;
}

void list_clear(list *l) {
    list_free(l);
    l->front = NULL;
    l->size = 0;
}

void list_free(void *l_p) {
    list *l = (list *)l_p;
    if (l->size == 0)
        return;
    list_node *node = l->front->next;
    while (node != l->front) {
        list_node *aux = node;
        node = node->next;
        list_node_free(aux, l->attr.fr);
    }
    list_node_free(l->front, l->attr.fr);
}

void list_print(void *l_p) {
    list l = *(list *)l_p;
    if (!l.attr.print || l.size == 0)
        return;
    l.attr.print(l.attr.stream, l.front->data);
    list_node *node = l.front->next;
    while (node != l.front) {
        l.attr.print(l.attr.stream, node->data);
        node = node->next;
    }
}