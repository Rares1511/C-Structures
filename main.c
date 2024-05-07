#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/vector.h"

struct painting
{
    int count;
    char *name;
    int *paints;
};

void print_painting(void *el)
{
    struct painting p = *(struct painting *)el;
    printf("%s with %d paints:\n", p.name, p.count);
    for (int i = 0; i < p.count; i++)
        printf("%d ", p.paints[i]);
    printf("\n");
}

void free_painting(void *el)
{
    struct painting p = *(struct painting *)el;
    free(p.name);
    free(p.paints);
}

void copy_painting(void *dest, const void *src)
{
    struct painting *dest_p = (struct painting *)dest;
    struct painting *src_p = (struct painting *)src;
    dest_p->count = src_p->count;
    dest_p->name = malloc(strlen(src_p->name) + 1);
    dest_p->paints = malloc(dest_p->count * sizeof(int));
    strcpy(dest_p->name, src_p->name);
    for (int i = 0; i < dest_p->count; i++)
        dest_p->paints[i] = src_p->paints[i];
}

int main()
{
    clock_t start = clock();

    vector vec;
    vector_attr_t attr = {sizeof(struct painting), free_painting, print_painting, copy_painting, NULL};
    struct painting p;

    p.count = 5;
    p.name = malloc(10 * sizeof(char));
    p.paints = malloc(p.count * sizeof(int));

    vector_init(&vec, attr);

    strcpy(p.name, "Catalina\0");
    for (int i = 0; i < p.count; i++)
        p.paints[i] = p.count - i;
    vector_push_back(&vec, &p);

    strcpy(p.name, "Rares\0");
    for (int i = 0; i < p.count; i++)
        p.paints[i] = i + 1;
    vector_insert_at(&vec, &p, 0);

    strcpy(p.name, "Doru\0");
    for (int i = 0; i < p.count; i++)
        p.paints[i] = p.count - i / 2;
    vector_push_back(&vec, &p);

    vector_print(&vec);

    vector_free(&vec);
    free_painting(&p);

    double time_taken = 1.0 * (clock() - start) / CLOCKS_PER_SEC;
    printf("programmed ran in %f\n", time_taken);
    return 0;
}