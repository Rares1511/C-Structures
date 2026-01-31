#include <unittest.h>
#include <valgrind/valgrind.h>
#include <valgrind/memcheck.h>

FILE *__DEBUG_OUT = NULL;

// ============================================================================
// Include all module unittest headers
// Each header defines: <module>_tests[] array and <module>_tests_size
// ============================================================================

#include "pair/pair_unittest.h"

// Associative containers (Arrays)
#include "vector/vector_unittest.h"
#include "deque/deque_unittest.h"
#include "list/list_unittest.h"
#include "forward_list/forward_list_unittest.h"

// Associative containers (RBT)
#include "set/set_unittest.h"
#include "map/map_unittest.h"
#include "multiset/multiset_unittest.h"
#include "multimap/multimap_unittest.h"

// ============================================================================
// Module registry - list all modules to test
// ============================================================================

typedef struct {
    const char *name;
    test *tests;
    int size;
} module_tests;

static module_tests all_modules[] = {
    // Extra modules
    { "pair", pair_tests, sizeof(pair_tests) / sizeof(test) },

    // // Associative containers (Arrays)
    // { "vector", vector_tests, sizeof(vector_tests) / sizeof(test) },
    // { "deque", deque_tests, sizeof(deque_tests) / sizeof(test) },
    // { "list", list_tests, sizeof(list_tests) / sizeof(test) },
    // { "forward_list", forward_list_tests, sizeof(forward_list_tests) / sizeof(test) },

    // // Associative containers (RBT)
    // { "set", set_tests, sizeof(set_tests) / sizeof(test) },
    // { "map", map_tests, sizeof(map_tests) / sizeof(test) },
    // { "multiset", multiset_tests, sizeof(multiset_tests) / sizeof(test) },
    // { "multimap", multimap_tests, sizeof(multimap_tests) / sizeof(test) },
};

static int num_modules = sizeof(all_modules) / sizeof(module_tests);

int main(int argc, char **argv) {
    int seed = 0, total_success = 0, total_failed = 0, total_tests = 0;
    cparser parser;

    int seed_default = __UNITTEST_SEED_DEFAULT_VALUE;

    cargs_init(&parser, argc, argv);
    cargs_add_arg(&parser, __UNITTEST_DEBUG_FILE_ARG_NAME, "Path to the debug log file", 0, CARG_TYPE_STRING, NULL);
    cargs_add_arg(&parser, __UNITTEST_SEED_ARG_NAME, "Random seed for the tests", 0, CARG_TYPE_INT, &seed_default);
    cargs_parse(&parser);

    const char *debug_file = cargs_get_arg(&parser, __UNITTEST_DEBUG_FILE_ARG_NAME);
    int close_file = 0;
    if (debug_file != NULL) {
        __DEBUG_OUT = fopen(debug_file, "a");
        if (__DEBUG_OUT == NULL) {
            printf("Failed to open debug file: %s\n", debug_file);
            return -1;
        }
        close_file = 1;
    } else {
        __DEBUG_OUT = stdout;
    }

    seed = *(int*)cargs_get_arg(&parser, __UNITTEST_SEED_ARG_NAME);
    srand(seed);

    // Calculate total tests
    for (int m = 0; m < num_modules; m++) {
        total_tests += all_modules[m].size;
    }

    fprintf(__DEBUG_OUT, "========================================\n");
    fprintf(__DEBUG_OUT, "  UNIFIED UNITTEST RUN\n");
    fprintf(__DEBUG_OUT, "  SEED: %d\n", seed);
    fprintf(__DEBUG_OUT, "  MODULES: %d | TOTAL TESTS: %d\n", num_modules, total_tests);
    fprintf(__DEBUG_OUT, "========================================\n\n");

    // Run tests for each module
    for (int m = 0; m < num_modules; m++) {
        module_tests *mod = &all_modules[m];
        int success = 0, failed = 0;

        fprintf(__DEBUG_OUT, "----------------------------------------\n");
        fprintf(__DEBUG_OUT, "  MODULE: %s (%d tests)\n", mod->name, mod->size);
        fprintf(__DEBUG_OUT, "----------------------------------------\n");

        for (int i = 0; i < mod->size; i++) {
            test_res res = mod->tests[i]();

            char buffer[__MAX_REASON_SIZE];
            const char *dots =
                "..........................................................................";

            strncpy(buffer, res.test_name, sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';

            size_t len = strlen(buffer);
            size_t max_dots = (__MAX_PRINT_SIZE > len) ? (__MAX_PRINT_SIZE - len) : 0;
            strncat(buffer, dots, max_dots);

            if (res.return_code != CS_SUCCESS) {
                failed++;
                fprintf(__DEBUG_OUT,
                        "[%2d/%2d] %s" __RED_UNITTEST __BOLD_UNITTEST "[FAIL]" __RESET_UNITTEST
                        "  reason: %s\n",
                        i + 1, mod->size, buffer,
                        res.reason ? res.reason : "(no reason)");
            } else {
                success++;
                fprintf(__DEBUG_OUT,
                        "[%2d/%2d] %s" __GREEN_UNITTEST "[ OK ]" __RESET_UNITTEST "\n",
                        i + 1, mod->size, buffer);
            }
        }

        fprintf(__DEBUG_OUT, "  >> %s: %d passed, %d failed\n\n", mod->name, success, failed);
        total_success += success;
        total_failed += failed;
    }

    // Final summary
    fprintf(__DEBUG_OUT, "========================================\n");
    if (total_failed == 0) {
        fprintf(__DEBUG_OUT, __GREEN_UNITTEST __BOLD_UNITTEST
                "FINAL SUMMARY: %d passed, %d failed (total %d)\n" __RESET_UNITTEST,
                total_success, total_failed, total_tests);
    } else {
        fprintf(__DEBUG_OUT, __RED_UNITTEST __BOLD_UNITTEST
                "FINAL SUMMARY: %d passed, %d failed (total %d)\n" __RESET_UNITTEST,
                total_success, total_failed, total_tests);
    }
    fprintf(__DEBUG_OUT, "========================================\n\n");

    if (close_file) {
        fclose(__DEBUG_OUT);
    }
    cargs_free(&parser);
    return total_failed > 0 ? 1 : 0;
}