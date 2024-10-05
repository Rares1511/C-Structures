#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "../include/deque.h"
#include "../include/unittest.h"

struct painting
{
    char *name;
    int year, noPaints;
    int *paints;
};

void print_painting(void *v_p)
{
    struct painting *p = (struct painting *)v_p;
    printf("Name: %s, Year: %d, NoPaints: %d, Paints: ", p->name, p->year, p->noPaints);
    for (int i = 0; i < p->noPaints; i++)
    {
        printf("%d ", p->paints[i]);
    }
    printf("\n");
}

void copy_painting(void *dest, const void *src)
{
    struct painting *p_dest = (struct painting *)dest;
    struct painting *p_src = (struct painting *)src;
    p_dest->name = malloc(strlen(p_src->name) + 1);
    strcpy(p_dest->name, p_src->name);
    p_dest->year = p_src->year;
    p_dest->noPaints = p_src->noPaints;
    p_dest->paints = malloc(p_src->noPaints * sizeof(int));
    memcpy(p_dest->paints, p_src->paints, p_src->noPaints * sizeof(int));
}

void free_painting(void *v_p)
{
    struct painting *p = (struct painting *)v_p;
    free(p->name);
    free(p->paints);
}

void print_int(void *el)
{
    printf("%d ", *(int *)el);
}

bool check_values(deque dq, int deque_values[], int size)
{
    if (dq.size != size)
    {
        return false;
    }
    deque_node_t *aux = dq.front;
    int i = 0;
    while (aux != NULL)
    {
        if (*(int *)aux->data != deque_values[i])
        {
            return false;
        }
        i++;
        aux = aux->next;
    }
    return true;
}

test_res test_deque_init()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_INIT", .reason = "deque_init failed", .return_code = res};
    }
    if (dq.size != 0 || dq.front != NULL || dq.back != NULL || dq.attr.size != sizeof(int) || dq.attr.copy != NULL)
    {
        return (test_res){.test_name = "TEST_DEQUE_INIT", .reason = "deque_init failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_INIT", .reason = "deque_init success", .return_code = CS_SUCCESS};
}

test_res test_deque_init_wrong_attr()
{
    deque dq;
    deque_attr_t attr_neg = {-1, NULL, NULL, print_int};
    deque_attr_t attr_max = {SIZE_TH + 1, NULL, NULL, print_int};
    cs_codes res = deque_init(&dq, attr_neg);
    if (res != CS_SIZE)
    {
        return (test_res){.test_name = "TEST_DEQUE_INIT_WRONG_ATTR", .reason = "deque_init was not failed though a negative size was given", .return_code = res};
    }

    res = deque_init(&dq, attr_max);
    if (res != CS_SIZE)
    {
        return (test_res){.test_name = "TEST_DEQUE_INIT_WRONG_ATTR", .reason = "deque_init was not failed though a size bigger than the threshold was given", .return_code = res};
    }
    return (test_res){.test_name = "TEST_DEQUE_INIT_WRONG_ATTR", .reason = "deque_init failed correctly", .return_code = CS_SUCCESS};
}

test_res test_deque_push_back()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    cs_codes res = deque_init(&dq, attr);
    int deque_values[] = {1, 2, 3, 4, 5};
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_PUSH_BACK", .reason = "deque_init failed", .return_code = res};
    }
    for (int i = 0; i < (int)(sizeof(deque_values) / sizeof(int)); i++)
    {
        res = deque_push_back(&dq, &deque_values[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_PUSH_BACK", .reason = "deque_push_back failed", .return_code = res};
        }
    }
    if (check_values(dq, deque_values, (int)(sizeof(deque_values) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_PUSH_BACK", .reason = "deque_push_back failed", .return_code = CS_UNKNOWN};
    }

    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_PUSH_BACK", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_PUSH_BACK", .reason = "deque_push_back success", .return_code = CS_SUCCESS};
}

test_res test_deque_push_front()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    cs_codes res = deque_init(&dq, attr);
    int deque_values[] = {1, 2, 3, 4, 5}, reversed_values[] = {5, 4, 3, 2, 1};
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_PUSH_FRONT", .reason = "deque_init failed", .return_code = res};
    }
    for (int i = 0; i < (int)(sizeof(deque_values) / sizeof(int)); i++)
    {
        res = deque_push_front(&dq, &deque_values[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_PUSH_FRONT", .reason = "deque_push_front failed", .return_code = res};
        }
    }
    if (check_values(dq, reversed_values, (int)(sizeof(reversed_values) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_PUSH_FRONT", .reason = "deque_push_front failed", .return_code = CS_UNKNOWN};
    }

    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_PUSH_FRONT", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_PUSH_FRONT", .reason = "deque_push_front success", .return_code = CS_SUCCESS};
}

test_res test_deque_pop_back()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    int deque_values[] = {1, 2, 3, 4, 5}, saved_size, deque_values_delete[] = {1, 2, 3, 4};
    deque_node_t *saved_back;
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "deque_init failed", .return_code = res};
    }
    for (int i = 0; i < (int)(sizeof(deque_values) / sizeof(int)); i++)
    {
        res = deque_push_back(&dq, &deque_values[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "deque_push_back failed", .return_code = res};
        }
    }
    if (check_values(dq, deque_values, (int)(sizeof(deque_values) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "values were not pushed back correctly", .return_code = CS_UNKNOWN};
    }
    saved_size = dq.size;
    saved_back = dq.back;
    res = deque_pop_back(&dq);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "deque_pop_back failed", .return_code = res};
    }
    if (dq.size != saved_size - 1 || dq.back == saved_back)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "deque_pop_back failed", .return_code = CS_UNKNOWN};
    }
    if (check_values(dq, deque_values_delete, (int)(sizeof(deque_values_delete) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "values were not popped back correctly", .return_code = CS_UNKNOWN};
    }

    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_POP_BACK", .reason = "deque_pop_back success", .return_code = CS_SUCCESS};
}

test_res test_deque_pop_front()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    int deque_values[] = {1, 2, 3, 4, 5}, saved_size, deque_values_delete[] = {2, 3, 4, 5};
    deque_node_t *saved_front;
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "deque_init failed", .return_code = res};
    }
    for (int i = 0; i < (int)(sizeof(deque_values) / sizeof(int)); i++)
    {
        res = deque_push_back(&dq, &deque_values[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "deque_push_back failed", .return_code = res};
        }
    }
    if (check_values(dq, deque_values, (int)(sizeof(deque_values) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "values were not pushed back correctly", .return_code = CS_UNKNOWN};
    }
    saved_size = dq.size;
    saved_front = dq.front;
    res = deque_pop_front(&dq);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "deque_pop_front failed", .return_code = res};
    }
    if (dq.size != saved_size - 1 || dq.front == saved_front)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "deque_pop_front failed", .return_code = CS_UNKNOWN};
    }
    if (check_values(dq, deque_values_delete, (int)(sizeof(deque_values_delete) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "values were not popped front correctly", .return_code = CS_UNKNOWN};
    }

    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_POP_FRONT", .reason = "deque_pop_front success", .return_code = CS_SUCCESS};
}

test_res test_deque_front()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    int deque_values[] = {1, 2, 3, 4, 5};
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_FRONT", .reason = "deque_init failed", .return_code = res};
    }
    for (int i = 0; i < (int)(sizeof(deque_values) / sizeof(int)); i++)
    {
        res = deque_push_back(&dq, &deque_values[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_FRONT", .reason = "deque_push_back failed", .return_code = res};
        }
    }
    if (check_values(dq, deque_values, (int)(sizeof(deque_values) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_FRONT", .reason = "values were not pushed back correctly", .return_code = CS_UNKNOWN};
    }
    int *front = deque_front(dq);
    if (front == NULL || *front != deque_values[0])
    {
        return (test_res){.test_name = "TEST_DEQUE_FRONT", .reason = "deque_front failed", .return_code = CS_UNKNOWN};
    }

    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_FRONT", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_FRONT", .reason = "deque_front success", .return_code = CS_SUCCESS};
}

test_res test_deque_back()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    int deque_values[] = {1, 2, 3, 4, 5};
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_BACK", .reason = "deque_init failed", .return_code = res};
    }
    for (int i = 0; i < (int)(sizeof(deque_values) / sizeof(int)); i++)
    {
        res = deque_push_back(&dq, &deque_values[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_BACK", .reason = "deque_push_back failed", .return_code = res};
        }
    }
    if (check_values(dq, deque_values, (int)(sizeof(deque_values) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_BACK", .reason = "values were not pushed back correctly", .return_code = CS_UNKNOWN};
    }
    int *back = deque_back(dq);
    if (back == NULL || *back != deque_values[(int)(sizeof(deque_values) / sizeof(int)) - 1])
    {
        return (test_res){.test_name = "TEST_DEQUE_BACK", .reason = "deque_back failed", .return_code = CS_UNKNOWN};
    }

    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_BACK", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_BACK", .reason = "deque_back success", .return_code = CS_SUCCESS};
}

test_res test_deque_swap()
{
    deque dq1, dq2;
    deque_attr_t attr = {sizeof(int), NULL, NULL, print_int};
    int deque_values1[] = {1, 2, 3, 4, 5}, deque_values2[] = {6, 7, 8, 9, 10, 11, 12};

    // Initialize dq1 and dq2
    cs_codes res = deque_init(&dq1, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_init failed", .return_code = res};
    }
    res = deque_init(&dq2, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_init failed", .return_code = res};
    }

    // Push values to dq1
    for (int i = 0; i < (int)(sizeof(deque_values1) / sizeof(int)); i++)
    {
        res = deque_push_back(&dq1, &deque_values1[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_push_back failed", .return_code = res};
        }
    }
    if (check_values(dq1, deque_values1, (int)(sizeof(deque_values1) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "values were not pushed back correctly", .return_code = CS_UNKNOWN};
    }

    // Push values to dq2
    for (int i = 0; i < (int)(sizeof(deque_values2) / sizeof(int)); i++)
    {
        res = deque_push_back(&dq2, &deque_values2[i]);
        if (res != CS_SUCCESS)
        {
            return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_push_back failed", .return_code = res};
        }
    }
    if (check_values(dq2, deque_values2, (int)(sizeof(deque_values2) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "values were not pushed back correctly", .return_code = CS_UNKNOWN};
    }

    // Swap dq1 and dq2
    deque_swap(&dq1, &dq2);
    if (dq1.size != (int)(sizeof(deque_values2) / sizeof(int)) || dq2.size != (int)(sizeof(deque_values1) / sizeof(int)))
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_swap failed", .return_code = CS_UNKNOWN};
    }
    if (check_values(dq1, deque_values2, (int)(sizeof(deque_values2) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "values were not swapped correctly", .return_code = CS_UNKNOWN};
    }
    if (check_values(dq2, deque_values1, (int)(sizeof(deque_values1) / sizeof(int))) == false)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "values were not swapped correctly", .return_code = CS_UNKNOWN};
    }

    // Free dq1
    deque_free(&dq1);
    if (dq1.front != NULL || dq1.back != NULL || dq1.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_free for 1st deque failed", .return_code = CS_UNKNOWN};
    }

    // Free dq2
    deque_free(&dq2);
    if (dq1.front != NULL || dq1.back != NULL || dq1.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_free for 2nd deque failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_SWAP", .reason = "deque_swap success", .return_code = CS_SUCCESS};
}

test_res test_deque_copy()
{
    deque dq;
    deque_attr_t attr = {sizeof(struct painting), free_painting, copy_painting, NULL};

    // Initialize dq
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "deque_init failed", .return_code = res};
    }

    // Push a struct to dq
    struct painting p = {"Mona Lisa", 1503, 3, (int[]){1, 2, 3}};
    res = deque_push_back(&dq, &p);
    if (res != CS_SUCCESS || dq.size != 1 || dq.front == NULL || dq.back == NULL)
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "deque_push_back failed", .return_code = res};
    }

    // Compare the struct in dq and the struct p
    struct painting *p_f = (struct painting *)deque_front(dq);
    if (strcmp(p_f->name, p.name) != 0 || p_f->year != p.year || p_f->noPaints != p.noPaints || memcmp(p_f->paints, p.paints, p.noPaints * sizeof(int)))
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "struct was not copied correctly", .return_code = CS_UNKNOWN};
    }

    // Free memory of dq
    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }

    return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "deque_copy success", .return_code = CS_SUCCESS};
}

test_res test_deque_set_attr()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, NULL};
    deque_attr_t new_attr = {sizeof(struct painting), free_painting, copy_painting, print_painting};

    // Initialize dq
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_ATTR", .reason = "deque_init failed", .return_code = res};
    }

    // Set new attributes for dq
    deque_set_attr(&dq, new_attr);
    if (dq.attr.size != new_attr.size || dq.attr.fr != new_attr.fr || dq.attr.copy != new_attr.copy || dq.attr.print != new_attr.print)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_ATTR", .reason = "deque_set_attr failed", .return_code = CS_UNKNOWN};
    }

    // Free memory of dq
    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_ATTR", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_SET_ATTR", .reason = "deque_set_attr success", .return_code = CS_SUCCESS};
}

test_res test_deque_set_attr_wrong()
{
    deque dq;
    deque_attr_t attr = {sizeof(int), NULL, NULL, NULL};
    deque_attr_t new_attr = {sizeof(struct painting), free_painting, copy_painting, print_painting};

    // Initialize dq
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_ATTR_WRONG", .reason = "deque_init failed", .return_code = res};
    }

    int el = 5;
    res = deque_push_back(&dq, &el);
    if (res != CS_SUCCESS || dq.size != 1 || dq.front == NULL || dq.back == NULL)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_ATTR_WRONG", .reason = "deque_push_back failed", .return_code = res};
    }

    // Set new attributes for dq
    deque_set_attr(&dq, new_attr);
    if (dq.attr.size == new_attr.size || dq.attr.fr == new_attr.fr || dq.attr.copy == new_attr.copy || dq.attr.print == new_attr.print)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_ATTR_WRONG", .reason = "deque_set_attr failed", .return_code = CS_UNKNOWN};
    }

    // Free memory of dq
    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_ATTR_WRONG", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_SET_ATTR_WRONG", .reason = "deque_set_attr success", .return_code = CS_SUCCESS};
}

test_res test_deque_set_free()
{
    deque dq;
    deque_attr_t attr = {sizeof(struct painting), NULL, copy_painting, NULL};
    freer new_fr = free_painting;

    // Initialize dq
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_FREE", .reason = "deque_init failed", .return_code = res};
    }

    struct painting p = {"Mona Lisa", 1503, 3, (int[]){1, 2, 3}};
    res = deque_push_back(&dq, &p);
    if (res != CS_SUCCESS || dq.size != 1 || dq.front == NULL || dq.back == NULL)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_FREE", .reason = "deque_push_back failed", .return_code = res};
    }

    // Compare the struct in dq and the struct p
    struct painting *p_f = (struct painting *)deque_front(dq);
    if (strcmp(p_f->name, p.name) != 0 || p_f->year != p.year || p_f->noPaints != p.noPaints || memcmp(p_f->paints, p.paints, p.noPaints * sizeof(int)))
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "struct was not copied correctly", .return_code = CS_UNKNOWN};
    }

    // Set new free function for dq
    deque_set_free(&dq, new_fr);
    if (dq.attr.fr != new_fr)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_FREE", .reason = "deque_set_free failed", .return_code = CS_UNKNOWN};
    }

    // Free memory of dq
    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_FREE", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_SET_FREE", .reason = "deque_set_free success", .return_code = CS_SUCCESS};
}

test_res test_deque_set_print()
{
    deque dq;
    deque_attr_t attr = {sizeof(struct painting), free_painting, copy_painting, NULL};
    printer new_print = print_painting;

    // Initialize dq
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_PRINT", .reason = "deque_init failed", .return_code = res};
    }

    struct painting p = {"Mona Lisa", 1503, 3, (int[]){1, 2, 3}};
    res = deque_push_back(&dq, &p);
    if (res != CS_SUCCESS || dq.size != 1 || dq.front == NULL || dq.back == NULL)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_PRINT", .reason = "deque_push_back failed", .return_code = res};
    }

    // Compare the struct in dq and the struct p
    struct painting *p_f = (struct painting *)deque_front(dq);
    if (strcmp(p_f->name, p.name) != 0 || p_f->year != p.year || p_f->noPaints != p.noPaints || memcmp(p_f->paints, p.paints, p.noPaints * sizeof(int)))
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "struct was not copied correctly", .return_code = CS_UNKNOWN};
    }

    // Set new print function for dq
    deque_set_print(&dq, new_print);
    if (dq.attr.print != new_print)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_PRINT", .reason = "deque_set_print failed", .return_code = CS_UNKNOWN};
    }

    // Free memory of dq
    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_PRINT", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_SET_PRINT", .reason = "deque_set_print success", .return_code = CS_SUCCESS};
}

test_res test_deque_set_copy()
{
    deque dq;
    deque_attr_t attr = {sizeof(struct painting), free_painting, NULL, NULL};
    deepcopy new_copy = copy_painting;

    // Initialize dq
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY", .reason = "deque_init failed", .return_code = res};
    }

    // Set new copy function for dq
    deque_set_copy(&dq, new_copy);
    if (dq.attr.copy != new_copy)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY", .reason = "deque_set_copy failed", .return_code = CS_UNKNOWN};
    }

    struct painting p = {"Mona Lisa", 1503, 3, (int[]){1, 2, 3}};
    res = deque_push_back(&dq, &p);
    if (res != CS_SUCCESS || dq.size != 1 || dq.front == NULL || dq.back == NULL)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY", .reason = "deque_push_back failed", .return_code = res};
    }

    // Compare the struct in dq and the struct p
    struct painting *p_f = (struct painting *)deque_front(dq);
    if (strcmp(p_f->name, p.name) != 0 || p_f->year != p.year || p_f->noPaints != p.noPaints || memcmp(p_f->paints, p.paints, p.noPaints * sizeof(int)))
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "struct was not copied correctly", .return_code = CS_UNKNOWN};
    }

    // Free memory of dq
    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_SET_COPY", .reason = "deque_set_copy success", .return_code = CS_SUCCESS};
}

test_res test_deque_set_copy_wrong()
{
    deque dq;
    deque_attr_t attr = {sizeof(struct painting), free_painting, copy_painting, NULL};
    deepcopy new_copy = NULL;

    // Initialize dq
    cs_codes res = deque_init(&dq, attr);
    if (res != CS_SUCCESS)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY_WRONG", .reason = "deque_init failed", .return_code = res};
    }

    struct painting p = {"Mona Lisa", 1503, 3, (int[]){1, 2, 3}};
    res = deque_push_back(&dq, &p);
    if (res != CS_SUCCESS || dq.size != 1 || dq.front == NULL || dq.back == NULL)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY_WRONG", .reason = "deque_push_back failed", .return_code = res};
    }

    // Compare the struct in dq and the struct p
    struct painting *p_f = (struct painting *)deque_front(dq);
    if (strcmp(p_f->name, p.name) != 0 || p_f->year != p.year || p_f->noPaints != p.noPaints || memcmp(p_f->paints, p.paints, p.noPaints * sizeof(int)))
    {
        return (test_res){.test_name = "TEST_DEQUE_COPY", .reason = "struct was not copied correctly", .return_code = CS_UNKNOWN};
    }

    // Set new copy function for dq
    deque_set_copy(&dq, new_copy);
    if (dq.attr.copy == new_copy)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY_WRONG", .reason = "deque_set_copy failed", .return_code = CS_UNKNOWN};
    }

    // Free memory of dq
    deque_free(&dq);
    if (dq.front != NULL || dq.back != NULL || dq.size != 0)
    {
        return (test_res){.test_name = "TEST_DEQUE_SET_COPY_WRONG", .reason = "deque_free failed", .return_code = CS_UNKNOWN};
    }
    return (test_res){.test_name = "TEST_DEQUE_SET_COPY_WRONG", .reason = "deque_set_copy success", .return_code = CS_SUCCESS};
}

int main()
{
    int i;
    test tests[] = {
        test_deque_init,
        test_deque_init_wrong_attr,
        test_deque_push_back,
        test_deque_push_front,
        test_deque_pop_back,
        test_deque_pop_front,
        test_deque_front,
        test_deque_back,
        test_deque_swap,
        test_deque_copy,
        test_deque_set_attr,
        test_deque_set_attr_wrong,
        test_deque_set_free,
        test_deque_set_print,
        test_deque_set_copy,
        test_deque_set_copy_wrong,
    };
    test_res res;

    for (i = 0; i < DEQUE_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++)
    {
        res = tests[i]();
        char buffer[1024];
        strcpy(buffer, res.test_name);
        strncat(buffer, "..................................................................................................", MAX_PRINT_SIZE - strlen(res.test_name));
        if (res.return_code != CS_SUCCESS)
        {
            printf("%sFAILED: %s\n", buffer, res.reason);
            exit(-(int)(sizeof(tests) / sizeof(test) - i));
        }
        else
        {
            printf("%sSUCCESS: %d/%d\n", buffer, i + 1, (int)(sizeof(tests) / sizeof(test)));
        }
    }

    return 0;
}