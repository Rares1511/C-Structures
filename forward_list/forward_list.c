#include <cs/forward_list.h>

#include <stdlib.h>
#include <string.h>

// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                      START OF HELPER FUNCTIONS SECTION                                     ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


/*! 
 * Initializes a forward list node with the given data.
 * @param data Pointer to the data to store in the node.
 * @param copy Function pointer to a deepcopy function for the data type.
 * @param data_size Size of the data type.
 * @return Pointer to the initialized forward list node, or NULL on failure.
 */
forward_list_node* forward_list_node_init(const void* data, deepcopy copy, size_t data_size){
    forward_list_node* node = (forward_list_node*)malloc(sizeof(forward_list_node));
    if (!node) return NULL;

    node->data = malloc(data_size);
    if (!node->data){
        free(node);
        return NULL;
    }
    if (copy)
        copy(node->data, data);
    else
        memcpy(node->data, data, data_size);
    node->next = NULL;

    return node;
}

/*! 
 * Frees a forward list node and its data.
 * @param node Pointer to the forward list node to free.
 * @param fr Function pointer to a freer function for the data type.
 */
void forward_list_node_free(forward_list_node* node, freer fr){
    if (!node) return;
    if (fr)
        fr(node->data);
    free(node->data);
    free(node);
}


// ╔════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
// ║                                       END OF HELPER FUNCTIONS SECTION                                      ║
// ╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝


forward_list *forward_list_init(forward_list_attr_t attr) {
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, NULL);
    forward_list* list = (forward_list*)malloc(sizeof(forward_list));
    CS_RETURN_IF(list == NULL, NULL);

    list->head = NULL;
    list->attr = attr;
    list->meta = malloc(sizeof(metadata_t));
    CS_RETURN_IF(list->meta == NULL, NULL);
    metadata_init(list->meta);

    return list;
}

cs_codes forward_list_push_front(forward_list* list, const void* data) {
    CS_RETURN_IF(list == NULL || data == NULL, CS_NULL);

    forward_list_node* new_node = forward_list_node_init(data, list->attr.copy, list->attr.size);
    CS_RETURN_IF(new_node == NULL, CS_MEM);

    new_node->next = list->head;
    list->head = new_node;
    metadata_size_inc(list->meta, 1);

    return CS_SUCCESS;
}

cs_codes forward_list_pop_front(forward_list* list) {
    CS_RETURN_IF(list == NULL, CS_NULL);
    CS_RETURN_IF(forward_list_empty(*list), CS_EMPTY);

    forward_list_node* temp = list->head;
    list->head = list->head->next;

    forward_list_node_free(temp, list->attr.fr);
    metadata_size_inc(list->meta, -1);

    return CS_SUCCESS;
}

void* forward_list_front(forward_list list) {
    CS_RETURN_IF(forward_list_empty(list), NULL);
    return list.head->data;
}

void forward_list_swap(forward_list* list1, forward_list* list2) {
    CS_RETURN_IF(list1 == NULL || list2 == NULL);

    forward_list_node* temp_head = list1->head;
    metadata_t *temp_meta = list1->meta;
    forward_list_attr_t temp_attr = list1->attr;

    list1->head = list2->head;
    list1->meta = list2->meta;
    list1->attr = list2->attr;

    list2->head = temp_head;
    list2->meta = temp_meta;
    list2->attr = temp_attr;
}

void forward_list_clear(forward_list* list){
    CS_RETURN_IF(list == NULL);

    forward_list_node* current = list->head;
    forward_list_node* next_node;

    while (current) {
        next_node = current->next;
        forward_list_node_free(current, list->attr.fr);
        current = next_node;
    }

    list->head = NULL;
    metadata_size_inc(list->meta, -forward_list_size(*list));
}

void forward_list_print(FILE *stream, const void *v_l) {
    CS_RETURN_IF(stream == NULL || v_l == NULL);
    forward_list* list = (forward_list*)v_l;
    CS_RETURN_IF(list == NULL);
    CS_RETURN_IF(forward_list_empty(*list));
    CS_RETURN_IF(list->attr.print == NULL);

    forward_list_node* current = list->head;
    while (current) {
        list->attr.print(stream, current->data);
        current = current->next;
    }
}

void forward_list_free(void *v_l) {
    CS_RETURN_IF(v_l == NULL);
    forward_list* list = (forward_list*)v_l;
    forward_list_node* current = list->head;
    forward_list_node* next_node;
    while (current) {
        next_node = current->next;
        forward_list_node_free(current, list->attr.fr);
        current = next_node;
    }
    free(list->meta);
    free(list);
}