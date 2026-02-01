#include <cs/stack.h>
#include <cs/vector.h>
#include <cs/deque.h>
#include <cs/list.h>

#include <stdlib.h>

cs_codes stack_init(stack *s, stack_type type, stack_attr_t attr) {
    CS_RETURN_IF(NULL == s, CS_NULL);
    CS_RETURN_IF(attr.size <= 0 || attr.size > SIZE_TH, CS_SIZE);
    s->type = type;
    switch(type) {
        case CS_STACK_ARRAY:
            s->container = malloc(sizeof(vector));
            CS_RETURN_IF(NULL == s->container, CS_MEM);
            return vector_init(s->container, attr);
        case CS_STACK_DEQUE:
            s->container = malloc(sizeof(deque));
            CS_RETURN_IF(NULL == s->container, CS_MEM);
            return deque_init(s->container, attr);
        case CS_STACK_LIST:
            s->container = malloc(sizeof(list));
            CS_RETURN_IF(NULL == s->container, CS_MEM);
            return list_init(s->container, attr);
        default:
            free(s->container);
            return CS_ELEM;
    }
    return CS_SUCCESS;
}

cs_codes stack_push(stack *s, const void *el) {
    CS_RETURN_IF(NULL == s || NULL == el, CS_ELEM);
    switch(s->type) {
        case CS_STACK_ARRAY:
            return vector_push_back(s->container, el);
        case CS_STACK_DEQUE:
            return deque_push_back(s->container, el);
        case CS_STACK_LIST:
            return list_push_back(s->container, el);
        default:
            return CS_ELEM;
    }
}

cs_codes stack_pop(stack *s) {
    CS_RETURN_IF(NULL == s, CS_ELEM);
    switch(s->type) {
        case CS_STACK_ARRAY:
            return vector_pop_back(s->container);
        case CS_STACK_DEQUE:
            return deque_pop_back(s->container);
        case CS_STACK_LIST:
            return list_pop_back(s->container);
        default:
            return CS_ELEM;
    }
}

void* stack_top(const stack *s) {
    CS_RETURN_IF(NULL == s, NULL);
    switch(s->type) {
        case CS_STACK_ARRAY:
            return vector_at(*(vector *)(s->container), vector_size(*(vector *)(s->container)) - 1);
        case CS_STACK_DEQUE:
            return deque_back(*(deque *)(s->container));
        case CS_STACK_LIST:
            return list_back(*(list *)(s->container));
        default:
            return NULL;
    }
}

int stack_size(const stack *s) {
    CS_RETURN_IF(NULL == s, -1);
    switch(s->type) {
        case CS_STACK_ARRAY:
            return vector_size(*(vector *)(s->container));
        case CS_STACK_DEQUE:
            return deque_size(*(deque *)(s->container));
        case CS_STACK_LIST:
            return list_size(*(list *)(s->container));
        default:
            return -1;
    }
}

int stack_empty(const stack *s) {
    CS_RETURN_IF(NULL == s, -1);
    switch(s->type) {
        case CS_STACK_ARRAY:
            return vector_empty(*(vector *)(s->container));
        case CS_STACK_DEQUE:
            return deque_empty(*(deque *)(s->container));
        case CS_STACK_LIST:
            return list_empty(*(list *)(s->container));
        default:
            return -1;
    }
}

void stack_clear(stack *s) {
    CS_RETURN_IF(NULL == s);
    switch(s->type) {
        case CS_STACK_ARRAY:
            vector_clear(s->container);
            break;
        case CS_STACK_DEQUE:
            deque_clear(s->container);
            break;
        case CS_STACK_LIST:
            list_clear(s->container);
            break;
        default:
            break;
    }
}

void stack_print(FILE *stream, void *v_s) {
    if (v_s == NULL) {
        return;
    }
    stack *s = (stack *)v_s;
    switch(s->type) {
        case CS_STACK_ARRAY:
            vector_print(stream, s->container);
            break;
        case CS_STACK_DEQUE:
            deque_print(stream, s->container);
            break;
        case CS_STACK_LIST:
            list_print(stream, s->container);
            break;
        default:
            break;
    }
}

void stack_free(void *v_s) {
    CS_RETURN_IF(NULL == v_s);
    stack *s = (stack *)v_s;
    switch (s->type) {
    case CS_STACK_ARRAY:
        vector_free(s->container);
        break;
    case CS_STACK_DEQUE:
        deque_free(s->container);
        break;
    case CS_STACK_LIST:
        list_free(s->container);
        break;
    default:
        break;
    }
    free(s->container);
}