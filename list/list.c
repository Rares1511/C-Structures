#include <cs/list.h>

#pragma region Helper Functions
// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*!
 * Compares two elements using the given comp function or memcmp if comp is NULL
 * @param[in] a     First element to be compared
 * @param[in] b     Second element to be compared
 * @param[in] comp  Comparison function for the datatype inside the list
 * @param[in] size  Size of the elements that will be compared
 * @return Negative value if a < b, 0 if a == b, positive value if a > b
 */
inline int list_compare(const void *a, const void *b, comparer comp, int size) {
    if (comp)
        return comp((void *)a, (void *)b);
    return memcmp(a, b, size);
}

/*!
 * Merges two sorted lists into one sorted list using the given attributes for comparison
 * @param[in] a     First sorted list to be merged
 * @param[in] b     Second sorted list to be merged
 * @param[in] attr  Attributes for the elements in the lists (used for comparison)
 * @return Pointer to the head of the merged sorted list
 */
list_node* merge_iterative(list_node* a, list_node* b, elem_attr_t attr) {
    list_node dummy;
    list_node *tail = &dummy;
    size_t sz = attr.size;

    while (a && b) {
        if (list_compare(a->data, b->data, attr.comp, sz) <= 0) {
            tail->next = a;
            a->prev = tail;
            a = a->next;
        } else {
            tail->next = b;
            b->prev = tail;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    if (tail->next) tail->next->prev = tail;

    return dummy.next;
}

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                        END OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes list_init(list *l, elem_attr_t attr) {
    CS_RETURN_IF(NULL == l, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    l->attr = attr;
    l->size = 0;
    l->front = NULL;

    l->header.magic = CS_LIST_MAGIC;
    l->header.type = CS_LIST_TYPE;
    return CS_SUCCESS;
}

cs_codes list_erase(list *l, int pos) {
    CS_RETURN_IF(l == NULL, CS_NULL);
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, CS_UNINITIALIZED);
    CS_RETURN_IF(l->size == 0, CS_EMPTY);
    CS_RETURN_IF(pos < 0 || pos >= l->size, CS_POS);

    if (pos == 0)
        return list_pop_front(l);
    if (pos == l->size - 1)
        return list_pop_back(l);

    list_node *current = l->front;
    for (; pos > 0; pos--, current = current->next);

    current->prev->next = current->next;
    current->next->prev = current->prev;
    freer fr = l->attr.fr;
    if (fr)
        fr(current->data);
    free(current);
    l->size--;
    return CS_SUCCESS;
}

int list_find(list *l, const void *el) {
    CS_RETURN_IF(el == NULL, -1);
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC, -1);
    CS_RETURN_IF(l->size == 0, -1);

    list_node *current = l->front;
    comparer comp = l->attr.comp;
    int elem_size = l->attr.size;
    for (int pos = 0; pos < l->size; pos++, current = current->next) {
        if (list_compare(current->data, el, comp, elem_size) == 0)
            return pos;
    }
    return -1;
}

void list_sort(list *l) {
    CS_RETURN_IF(l == NULL || l->header.magic != CS_LIST_MAGIC || l->size < 2);

    // 1. Break Circularity
    list_node *head = l->front;
    l->front->prev->next = NULL; 
    head->prev = NULL;

    // 2. Iterative Merge (Bottom-Up)
    // We use a small array of bins to merge lists of size 2^i
    list_node *bins[32] = {NULL}; // Supports up to 2^32 elements
    list_node *curr = head;
    list_node *next_node;

    while (curr) {
        next_node = curr->next;
        curr->next = curr->prev = NULL;
        
        int i = 0;
        while (i < 31 && bins[i] != NULL) {
            curr = merge_iterative(bins[i], curr, l->attr);
            bins[i] = NULL;
            i++;
        }
        bins[i] = curr;
        curr = next_node;
    }

    // 3. Final Merge of all bins
    list_node *result = NULL;
    for (int i = 0; i < 32; i++) {
        result = merge_iterative(bins[i], result, l->attr);
    }

    // 4. Re-establish Circularity
    l->front = result;
    list_node *tail = result;
    while (tail->next) tail = tail->next;
    
    tail->next = l->front;
    l->front->prev = tail;
}

void list_swap(list *l1, list *l2) {
    CS_RETURN_IF(l1 == NULL || l2 == NULL || l1->header.magic != CS_LIST_MAGIC || l2->header.magic != CS_LIST_MAGIC);

    elem_attr_t attr = l1->attr;
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
    CS_RETURN_IF(l == NULL || l->header.magic != CS_LIST_MAGIC);
    list_node *node = l->front->next;
    freer fr = l->attr.fr;
    while (node != l->front) {
        list_node *aux = node;
        node = node->next;
        if (fr)
            fr(aux->data);
        free(aux);
    }
    if (fr)
        fr(l->front->data);
    free(l->front);
    l->front = NULL;
    l->size = 0;
}

void list_print(FILE *stream, void *l_p) {
    CS_RETURN_IF(l_p == NULL || stream == NULL);
    list l = *(list *)l_p;
    CS_RETURN_IF(l.header.magic != CS_LIST_MAGIC || l.size == 0 || l.attr.print == NULL);
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
    CS_RETURN_IF(l->header.magic != CS_LIST_MAGIC);
    freer fr = l->attr.fr;
    if (l->size != 0) {
        list_node *node = l->front->next;
        while (node != l->front) {
            list_node *aux = node;
            node = node->next;
            if (fr)
                fr(aux->data);
            free(aux);
        }
        if (fr)
            fr(l->front->data);
        free(l->front);
    }
}