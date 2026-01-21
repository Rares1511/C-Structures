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


cs_codes forward_list_init(forward_list* list, forward_list_attr_t attr){
    if (!list) return CS_ELEM;
    if (attr.size <= 0 || attr.size > SIZE_TH) return CS_SIZE;

    list->head = NULL;
    list->attr = attr;
    metadata_init(&list->meta, 1);

    return CS_SUCCESS;
}

cs_codes forward_list_push_front(forward_list* list, const void* data){
    if (!metadata_is_init(list->meta)) return CS_NULL;
    if (!list) return CS_NULL;
    if (!data) return CS_ELEM;

    forward_list_node* new_node = forward_list_node_init(data, list->attr.copy, list->attr.size);
    if (!new_node) return CS_MEM;

    new_node->next = list->head;
    list->head = new_node;
    metadata_size_inc(&list->meta, 1);

    return CS_SUCCESS;
}

cs_codes forward_list_pop_front(forward_list* list){
    if (!list) return CS_ELEM;
    if (forward_list_empty(*list)) return CS_EMPTY;

    forward_list_node* temp = list->head;
    list->head = list->head->next;

    forward_list_node_free(temp, list->attr.fr);
    metadata_size_inc(&list->meta, -1);

    return CS_SUCCESS;
}

void* forward_list_front (forward_list list){
    if (forward_list_empty(list)) return NULL;
    return list.head->data;
}

void forward_list_swap(forward_list* list1, forward_list* list2){
    if (!list1 || !list2) return;
    if (!metadata_is_init(list1->meta) || !metadata_is_init(list2->meta)) return;

    forward_list_node* temp_head = list1->head;
    metadata_t temp_meta = list1->meta;
    forward_list_attr_t temp_attr = list1->attr;

    list1->head = list2->head;
    list1->meta = list2->meta;
    list1->attr = list2->attr;

    list2->head = temp_head;
    list2->meta = temp_meta;
    list2->attr = temp_attr;
}

void forward_list_clear(forward_list* list){
    if (!list) return;
    if (!metadata_is_init(list->meta)) return;

    forward_list_node* current = list->head;
    forward_list_node* next_node;

    while (current) {
        next_node = current->next;
        forward_list_node_free(current, list->attr.fr);
        current = next_node;
    }

    list->head = NULL;
    metadata_init(&list->meta, 0);
}

void forward_list_print(FILE *stream, const void *v_l) {
    forward_list* list = (forward_list*)v_l;
    if (!list) return;
    if (!list->attr.print) return;
    if (!metadata_is_init(list->meta)) return;
    if (forward_list_empty(*list)) return;

    forward_list_node* current = list->head;
    while (current) {
        list->attr.print(stream, current->data);
        current = current->next;
    }
}

void forward_list_free(void *v_l) {
    forward_list* list = (forward_list*)v_l;
    if(!list) return;
    forward_list_clear(list);
}