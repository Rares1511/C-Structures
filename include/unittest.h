#define MAX_REASON_SIZE 1024
#define MAX_PRINT_SIZE 75

#define MAP_TEST_SIZE 20
#define DEQUE_TEST_SIZE 20
#define HASH_TABLE_TEST_SIZE 20
#define HEAP_TEST_SIZE 20
#define MATRIX_TEST_SIZE 20
#define VECTOR_TEST_SIZE 20

typedef struct test_res {
    char *test_name;
    char *reason;
    int return_code;
} test_res;

struct painting {
    char *name;
    int year, noPaints;
    int *paints;
};

typedef struct test_res (*test)();

void print_painting(void *v_p) {
    struct painting *p = (struct painting *)v_p;
    printf("Name: %s, Year: %d, NoPaints: %d, Paints: ", p->name, p->year, p->noPaints);
    for (int i = 0; i < p->noPaints; i++) {
        printf("%d ", p->paints[i]);
    }
    printf("\n");
}

void copy_painting(void *dest, const void *src) {
    struct painting *p_dest = (struct painting *)dest;
    struct painting *p_src = (struct painting *)src;
    p_dest->name = malloc(strlen(p_src->name) + 1);
    strcpy(p_dest->name, p_src->name);
    p_dest->year = p_src->year;
    p_dest->noPaints = p_src->noPaints;
    p_dest->paints = malloc(p_src->noPaints * sizeof(int));
    memcpy(p_dest->paints, p_src->paints, p_src->noPaints * sizeof(int));
}

void free_painting(void *v_p) {
    struct painting *p = (struct painting *)v_p;
    free(p->name);
    free(p->paints);
}

void print_int(void *el) { printf("%d ", *(int *)el); }

void unittest(test *tests, int size) {
    int i, success = 0;
    test_res res;

    for (i = 0; i < size; i++) {
        res = tests[i]();
        char buffer[1024];
        strcpy(buffer, res.test_name);
        strncat(buffer,
                "..........................................................................",
                MAX_PRINT_SIZE - strlen(res.test_name));
        if (res.return_code != CS_SUCCESS) {
            printf("%sFAILED: %s\n", buffer, res.reason);
            exit(-(int)(size / sizeof(test) - i));
        } else {
            printf("%sSUCCESS: %d/%d\n", buffer, i + 1, size);
            success++;
        }
    }
    printf("Total: %d/%d\n", success, i);
}