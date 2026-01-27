#define __MAX_REASON_SIZE 1024
#define __MAX_PRINT_SIZE 75

#define __TEST_SIZE 1000
#define __VALUE_RANGE 10000

#define __UNITTEST_DEBUG_FILE_ARG_NAME "--debug-file"
#define __UNITTEST_DEBUG_FILE_DEFAULT_VALUE "unittest_log.ansi"

#define __UNITTEST_SEED_ARG_NAME "--seed"
#define __UNITTEST_SEED_DEFAULT_VALUE 42

#define __UNITTEST_MODULE_ARG_NAME "--module"

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cs/universal.h>
#include <cs/cargs.h>

extern FILE *__DEBUG_OUT;

#define __GREEN_UNITTEST  "\033[32m"
#define __RED_UNITTEST    "\033[31m"
#define __RESET_UNITTEST  "\033[0m"
#define __BOLD_UNITTEST   "\033[1m"

#ifdef DEBUG
#define DEBUG_PRINT(...) do { \
    if (__DEBUG_OUT) { \
        fprintf(__DEBUG_OUT, "[%s:%d:%s] ", __FILE__, __LINE__, __func__); \
        fprintf(__DEBUG_OUT, __VA_ARGS__); \
        fflush(__DEBUG_OUT); \
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

static inline void print_int(FILE *stream, const void *el) { fprintf(stream, "%d", *(int *)el); }

static inline int comp_int_min(const void *a, const void *b) { return *(int *)b - *(int *)a; }
static inline int comp_int_max(const void *a, const void *b) { return *(int *)a - *(int *)b; }

static inline void unittest(test *tests, int size, int argc, char **argv) {
    int i, seed, success = 0, failed = 0;
    test_res res;
    cparser parser;

    int seed_default = __UNITTEST_SEED_DEFAULT_VALUE;

    cargs_init(&parser, argc, argv);
    cargs_add_arg(&parser, __UNITTEST_DEBUG_FILE_ARG_NAME, "Path to the debug log file", 0, CARG_TYPE_STRING, __UNITTEST_DEBUG_FILE_DEFAULT_VALUE);
    cargs_add_arg(&parser, __UNITTEST_SEED_ARG_NAME, "Random seed for the tests", 0, CARG_TYPE_INT, &seed_default);
    cargs_add_arg(&parser, __UNITTEST_MODULE_ARG_NAME, "Module to run tests for", 1, CARG_TYPE_STRING, NULL);
    cargs_parse(&parser);

    const char *debug_file = cargs_get_arg(&parser, __UNITTEST_DEBUG_FILE_ARG_NAME);
    __DEBUG_OUT = fopen(debug_file, "a");
    if (__DEBUG_OUT == NULL) {
        printf("Failed to open debug file: %s\n", debug_file);
        return;
    }

    seed = *(int*)cargs_get_arg(&parser, __UNITTEST_SEED_ARG_NAME);
    srand(seed);

    const char *module_name = (const char*)cargs_get_arg(&parser, __UNITTEST_MODULE_ARG_NAME);

    fprintf(__DEBUG_OUT, "========================================\n");
    fprintf(__DEBUG_OUT, "  MODULE: %s\n", module_name);
    fprintf(__DEBUG_OUT, "  SEED: %d\n", seed);
    fprintf(__DEBUG_OUT, "  UNITTEST RUN (total tests: %d)\n", size);
    fprintf(__DEBUG_OUT, "========================================\n");

    for (i = 0; i < size; i++) {
        res = tests[i]();

        char buffer[__MAX_REASON_SIZE];
        const char *dots =
            "..........................................................................";

        strncpy(buffer, res.test_name, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';

        size_t len = strlen(buffer);
        size_t max_dots = (__MAX_PRINT_SIZE > len) ? (__MAX_PRINT_SIZE - len) : 0;
        strncat(buffer, dots, max_dots);

        if (res.return_code != CS_SUCCESS) {
            /* FAILURE */
            failed++;
            fprintf(__DEBUG_OUT,
                    "[%2d/%2d] %s" __RED_UNITTEST __BOLD_UNITTEST "[FAIL]" __RESET_UNITTEST
                    "  reason: %s\n",
                    i + 1, size, buffer,
                    res.reason ? res.reason : "(no reason)");

        } else {
            /* SUCCESS */
            success++;
            fprintf(__DEBUG_OUT,
                    "[%2d/%2d] %s" __GREEN_UNITTEST "[ OK ]" __RESET_UNITTEST
                    "  SUCCESS: %d/%d\n",
                    i + 1, size, buffer, success, size);
        }
    }

    /* Color summary depending on fail/success */
    if (failed == 0) {
        fprintf(__DEBUG_OUT, __GREEN_UNITTEST __BOLD_UNITTEST
                "SUMMARY: %d passed, %d failed (total %d)\n" __RESET_UNITTEST,
                success, failed, size);
    } else {
        fprintf(__DEBUG_OUT, __RED_UNITTEST __BOLD_UNITTEST
                "SUMMARY: %d passed, %d failed (total %d)\n" __RESET_UNITTEST,
                success, failed, size);
    }

    fprintf(__DEBUG_OUT, "========================================\n\n");

    fclose(__DEBUG_OUT);
    cargs_free(&parser);
}