#include "../include/stack.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

cs_codes stack_init(stack *s, stack_attr_t attr)
{
    if (attr.size < 0 || attr.size > SIZE_TH)
        return CS_SIZE;
    s->attr = attr;
    s->cap = INIT_CAPACITY;
    s->size = 0;
    s->vec = malloc(s->cap * s->attr.size);
    return CS_SUCCESS;
}

int stack_empty(stack s)
{
    if (s.size == 0)
        return 1;
    return 0;
}

void *stack_top(stack s)
{
    if (stack_empty(s))
        return NULL;
    return (s.vec + (s.size - 1) * s.attr.size);
}

cs_codes stack_push(stack *s, void *el)
{
    if (s->size == s->cap)
    {
        s->vec = realloc(s->vec, (s->cap + INIT_CAPACITY) * s->attr.size);
        if (!s->vec)
            return CS_MEM;
        s->cap += INIT_CAPACITY;
    }
    if (s->attr.cp)
        s->attr.cp(s->vec + s->size * s->attr.size, el);
    else
        memcpy(s->vec + s->size * s->attr.size, el, s->attr.size);
    s->size++;
    return CS_SUCCESS;
}

cs_codes stack_pop(stack *s)
{
    if (stack_empty(*s))
        return CS_EMPTY;
    if (s->attr.fr)
        s->attr.fr(s->vec + (s->size - 1) * s->attr.size);
    s->size--;
    return CS_SUCCESS;
}

void stack_set_attr(stack *s, stack_attr_t attr) { s->attr = attr; }

void stack_set_free(stack *s, freer fr) { s->attr.fr = fr; }

void stack_set_print(stack *s, printer print) { s->attr.print = print; }

void stack_set_copy(stack *s, deepcopy cp) { s->attr.cp = cp; }

void stack_swap(stack *s1, stack *s2)
{
    void *vec = s1->vec;
    stack_attr_t attr = s1->attr;
    int cap = s1->cap;
    int size = s1->size;

    s1->attr = s2->attr;
    s1->cap = s2->cap;
    s1->size = s2->size;
    s1->vec = s2->vec;

    s2->attr = attr;
    s2->cap = cap;
    s2->size = size;
    s2->vec = vec;
}

void stack_clear(stack *s)
{
    if (s->attr.fr)
        for (int i = 0; i < s->size; i++)
            s->attr.fr(s->vec + i * s->attr.size);
    s->size = 0;
}

void stack_free(void *s_p)
{
    stack *s = (stack *)s_p;
    if (s->attr.fr)
        for (int i = 0; i < s->size; i++)
            s->attr.fr(s->vec + i * s->attr.size);
    free(s->vec);
}

void stack_print(void *s_p)
{
    stack s = *(stack *)s_p;
    for (int i = s.size - 1; i >= 0; i--)
        s.attr.print(s.vec + i * s.attr.size);
}