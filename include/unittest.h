#define MAX_REASON_SIZE 1024
#define MAX_PRINT_SIZE 75

#define TEST_SIZE 1000
#define VALUE_RANGE 10000

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct test_res (*test)();

static inline void print_int(FILE *stream, void *el) { fprintf(stream, "%d", *(int *)el); }

static inline int comp_int_min(const void *a, const void *b) { return *(int *)b - *(int *)a; }
static inline int comp_int_max(const void *a, const void *b) { return *(int *)a - *(int *)b; }

static inline void unittest(test *tests, int size, int argc, char **argv) {
    int i, seed, success = 0, failed = 0;
    test_res res;

    if (argc < 3) {
        printf("Usage: %s [debug_file] [seed]\n", argv[0]);
        return;
    } 

    DEBUG_OUT = fopen(argv[1], "a");
    if (DEBUG_OUT == NULL) {
        fprintf(DEBUG_OUT, "Failed to open debug file: %s\n", argv[1]);
        return;
    }

    seed = atoi(argv[2]);
    srand(seed);

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

    fclose(DEBUG_OUT);
}