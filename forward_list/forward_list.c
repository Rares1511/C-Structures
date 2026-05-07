#include <cs/forward_list.h>

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
inline int forward_list_compare(const void *a, const void *b, comparer comp, int size) {
    if (comp)
        return comp((void *)a, (void *)b);
    return memcmp(a, b, size);
}

/*!
 * Merges two sorted linked lists into one sorted list using the provided comparison function.
 * @param a First sorted linked list.
 * @param b Second sorted linked list.
 * @param attr Attributes for the elements in the lists, used for comparison and copying.
 * @return Pointer to the head of the merged sorted linked list.
 */
forward_list_node* merge_iterative(forward_list_node* a, forward_list_node* b, elem_attr_t attr) {
    forward_list_node dummy;
    forward_list_node *tail = &dummy;
    size_t sz = attr.size;

    while (a && b) {
        if (forward_list_compare(a->data, b->data, attr.comp, sz) <= 0) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;

    return dummy.next;
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
#pragma endregion

cs_codes forward_list_init(forward_list *list, elem_attr_t attr) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    CS_RETURN_IF(list == NULL, CS_NULL);

    list->head = NULL;
    list->tail = NULL;
    list->attr = attr;
    list->size = 0;

    list->header.magic = CS_FORWARD_LIST_MAGIC;
    list->header.type = CS_FORWARD_LIST_TYPE;
    return CS_SUCCESS;
}

int forward_list_find(forward_list *list, const void* data) {
    CS_RETURN_IF(data == NULL, -1);
    CS_RETURN_IF(list == NULL, -1);
    CS_RETURN_IF(list->header.magic != CS_FORWARD_LIST_MAGIC, -1);
    CS_RETURN_IF(list->size == 0, -1);

    forward_list_node* current = list->head;
    comparer comp = list->attr.comp;
    for (int pos = 0; pos < list->size; pos++, current = current->next) {
        if (comp && comp(current->data, data) == 0)
            return pos;
        else if (!comp && memcmp(current->data, data, list->attr.size) == 0)
            return pos;
    }

    return -1;
}

void forward_list_sort(forward_list *list) {
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC || list->size < 2);

    forward_list_node *bins[32] = {NULL}; 
    forward_list_node *curr = list->head;
    forward_list_node *next_node;

    while (curr) {
        next_node = curr->next;
        curr->next = NULL; // Critical: Isolate the node
        
        int i = 0;
        while (i < 31 && bins[i] != NULL) {
            curr = merge_iterative(bins[i], curr, list->attr);
            bins[i] = NULL;
            i++;
        }
        bins[i] = curr;
        curr = next_node;
    }

    forward_list_node *result = NULL;
    for (int i = 0; i < 32; i++) {
        if (bins[i]) { // Minor optimization: only merge if bin is not empty
            result = merge_iterative(bins[i], result, list->attr);
        }
    }

    // Update Head
    list->head = result;

    // Optional: Update Tail if your structure maintains one
    if (result) {
        forward_list_node *t = result;
        while (t->next) t = t->next;
        list->tail = t; // Ensure your struct's tail is correct
        t->next = NULL; // Ensure the list is NULL-terminated
    }
}

void forward_list_swap(forward_list* list1, forward_list* list2) {
    CS_RETURN_IF(list1 == NULL || list2 == NULL || list1->header.magic != CS_FORWARD_LIST_MAGIC || list2->header.magic != CS_FORWARD_LIST_MAGIC);

    cs_header_t temp_header = list1->header;
    forward_list_node* temp_head = list1->head;
    forward_list_node* temp_tail = list1->tail;
    int temp_size = list1->size;
    elem_attr_t temp_attr = list1->attr;

    list1->header = list2->header;
    list1->tail = list2->tail;
    list1->head = list2->head;
    list1->size = list2->size;
    list1->attr = list2->attr;

    list2->header = temp_header;
    list2->head = temp_head;
    list2->tail = temp_tail;
    list2->size = temp_size;
    list2->attr = temp_attr;
}

void forward_list_clear(forward_list* list){
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC);

    forward_list_node* current = list->head;
    forward_list_node* next_node;
    freer fr = list->attr.fr;

    while (current) {
        next_node = current->next;
        if (fr)
            fr(current->data);
        free(current);
        current = next_node;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void forward_list_print(FILE *stream, const void *v_l) {
    CS_RETURN_IF(stream == NULL || v_l == NULL);
    forward_list* list = (forward_list*)v_l;
    CS_RETURN_IF(list == NULL || list->header.magic != CS_FORWARD_LIST_MAGIC || list->attr.print == NULL || list->size == 0);

    forward_list_node* current = list->head;
    while (current) {
        list->attr.print(stream, current->data);
        current = current->next;
    }
}

void forward_list_free(void *v_l) {
    CS_RETURN_IF(v_l == NULL);
    forward_list* list = (forward_list*)v_l;
    CS_RETURN_IF(list->header.magic != CS_FORWARD_LIST_MAGIC);
    if (list->size > 0) {
        freer fr = list->attr.fr;
        forward_list_node* current = list->head;
        forward_list_node* next_node;
        while (current) {
            next_node = current->next;
            if (fr)
                fr(current->data);
            free(current);
            current = next_node;
        }
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->header.magic = 0; // Invalidate the list
}