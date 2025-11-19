#define MAX_REASON_SIZE 1024
#define MAX_PRINT_SIZE 75

#define MAP_TEST_SIZE 20
#define DEQUE_TEST_SIZE 20
#define HASH_TABLE_TEST_SIZE 20
#define HEAP_TEST_SIZE 20
#define MATRIX_TEST_SIZE 20
#define VECTOR_TEST_SIZE 20

#pragma once
#include <stdio.h>

extern FILE *DEBUG_OUT;

#define GREEN_UNITTEST  "\033[32m"
#define RED_UNITTEST    "\033[31m"
#define RESET_UNITTEST  "\033[0m"
#define BOLD_UNITTEST   "\033[1m"

#ifdef DEBUG
#define DEBUG_PRINT(...) do { \
    if (DEBUG_OUT) { \
        fprintf(DEBUG_OUT, "[%s:%d:%s] ", __FILE__, __LINE__, __func__); \
        fprintf(DEBUG_OUT, __VA_ARGS__); \
        fflush(DEBUG_OUT); \
    } \
} while (0)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif

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

void print_int(FILE *stream, void *el) { fprintf(stream, "%d", *(int *)el); }

void unittest(test *tests, int size) {
    int i;
    int success = 0;
    int failed = 0;
    test_res res;

    fprintf(DEBUG_OUT, "\n========================================\n");
    fprintf(DEBUG_OUT, "  UNITTEST RUN (total tests: %d)\n", size);
    fprintf(DEBUG_OUT, "========================================\n");

    for (i = 0; i < size; i++) {
        res = tests[i]();

        char buffer[1024];
        const char *dots =
            "..........................................................................";

        strncpy(buffer, res.test_name, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        size_t len = strlen(buffer);
        size_t max_dots = (MAX_PRINT_SIZE > len) ? (MAX_PRINT_SIZE - len) : 0;
        strncat(buffer, dots, max_dots);

        if (res.return_code != CS_SUCCESS) {
            /* FAILURE */
            failed++;
            fprintf(DEBUG_OUT,
                    "[%2d/%2d] %s" RED_UNITTEST BOLD_UNITTEST "[FAIL]" RESET_UNITTEST
                    "  reason: %s\n",
                    i + 1, size, buffer,
                    res.reason ? res.reason : "(no reason)");

        } else {
            /* SUCCESS */
            success++;
            fprintf(DEBUG_OUT,
                    "[%2d/%2d] %s" GREEN_UNITTEST "[ OK ]" RESET_UNITTEST
                    "  SUCCESS: %d/%d\n",
                    i + 1, size, buffer, success, size);
        }
    }

    fprintf(DEBUG_OUT, "----------------------------------------\n");

    /* Color summary depending on fail/success */
    if (failed == 0) {
        fprintf(DEBUG_OUT, GREEN_UNITTEST BOLD_UNITTEST
                "SUMMARY: %d passed, %d failed (total %d)\n" RESET_UNITTEST,
                success, failed, size);
    } else {
        fprintf(DEBUG_OUT, RED_UNITTEST BOLD_UNITTEST
                "SUMMARY: %d passed, %d failed (total %d)\n" RESET_UNITTEST,
                success, failed, size);
    }

    fprintf(DEBUG_OUT, "========================================\n\n");

    /* Makefile sees non-zero = failing unittest */
    if (failed > 0) {
        exit(EXIT_FAILURE);
    }
}