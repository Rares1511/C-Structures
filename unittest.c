#include <unittest.h>
#include <valgrind/valgrind.h>
#include <valgrind/memcheck.h>

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

// Unordered associative containers (Hash Table)
#include "unordered_set/unordered_set_unittest.h"
#include "unordered_map/unordered_map_unittest.h"
#include "unordered_multiset/unordered_multiset_unittest.h"
#include "unordered_multimap/unordered_multimap_unittest.h"

// Container adapters
#include "stack/stack_unittest.h"
#include "queue/queue_unittest.h"
#include "priority_queue/priority_queue_unittest.h"
#include "flat_set/flat_set_unittest.h"

// Numeric types
#include "large_number/large_number_unittest.h"

// Utilities
#include "clogger/clogger_unittest.h"

// String module
#include "cstring/cstring_unittest.h"

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

    // Associative containers (Arrays)
    { "vector", vector_tests, sizeof(vector_tests) / sizeof(test) },
    { "deque", deque_tests, sizeof(deque_tests) / sizeof(test) },
    { "list", list_tests, sizeof(list_tests) / sizeof(test) },
    { "forward_list", forward_list_tests, sizeof(forward_list_tests) / sizeof(test) },

    // // Associative containers (RBT)
    { "set", set_tests, sizeof(set_tests) / sizeof(test) },
    { "map", map_tests, sizeof(map_tests) / sizeof(test) },
    { "multiset", multiset_tests, sizeof(multiset_tests) / sizeof(test) },
    { "multimap", multimap_tests, sizeof(multimap_tests) / sizeof(test) },

    // // Unordered associative containers (Hash Table)
    { "unordered_set", unordered_set_tests, sizeof(unordered_set_tests) / sizeof(test) },
    { "unordered_map", unordered_map_tests, sizeof(unordered_map_tests) / sizeof(test) },
    { "unordered_multiset", unordered_multiset_tests, sizeof(unordered_multiset_tests) / sizeof(test) },
    { "unordered_multimap", unordered_multimap_tests, sizeof(unordered_multimap_tests) / sizeof(test) },

    // // Container adapters
    { "stack", stack_tests, sizeof(stack_tests) / sizeof(test) },
    { "queue", queue_tests, sizeof(queue_tests) / sizeof(test) },
    { "priority_queue", priority_queue_tests, sizeof(priority_queue_tests) / sizeof(test) },
    { "flat_set", flat_set_tests, sizeof(flat_set_tests) / sizeof(test) },

    // // Numeric types
    // { "large_number", large_number_tests, sizeof(large_number_tests) / sizeof(test) },

    // Utilities
    { "clogger", clogger_tests, sizeof(clogger_tests) / sizeof(test) },

    // String module
    // { "cstring", cstring_tests, sizeof(cstring_tests) / sizeof(test) },
};

static int num_modules = sizeof(all_modules) / sizeof(module_tests);

void post_operation_time(test_arg *arg, const char *operation, double c_time) {
    for (int i = 0; i < arg->op_time_count; i++) {
        if (strcmp(arg->op_time[i].operation, operation) == 0) {
            arg->op_time[i].c_time = c_time;
            break;
        }
    }
}

void read_operation_time(test_arg *arg, const char *benchmark_file_name, const char *module_name) {
    FILE *file = fopen(benchmark_file_name, "r");
    if (!file) {
        clogger_log((*arg->logger), CLOGGER_ERROR, "Failed to open benchmark file: %s\n", benchmark_file_name);
        return;
    }

    char line[256];

    // Free old operation strings before re-reading
    for (int i = 0; i < arg->op_time_count; i++) {
        free((void*)arg->op_time[i].operation);
    }
    arg->op_time_count = 0;

    while (fgets(line, sizeof(line), file)) {
        char module[64], operation[64];
        double value;
        if (sscanf(line, "%63[^,],%63[^,],%lf", module, operation, &value) == 3) {
            for (int m = 0; m < num_modules; m++) {
                if (strcmp(module_name, module) == 0) {
                    arg->op_time[arg->op_time_count].operation = strdup(operation);
                    arg->op_time[arg->op_time_count].cpp_time = value;
                    arg->op_time[arg->op_time_count].c_time = 0.0; // Will be filled by C benchmark
                    arg->op_time_count++;
                    break;
                }
            }
        }
    }

    fclose(file);
}

int main(int argc, char **argv) {
    int seed = 0, total_success = 0, total_failed = 0, total_tests = 0;
    cparser parser;
    clogger debug_logger, results_logger;
    clogger_options debug_options = { 
        .min_level = CLOGGER_DEBUG, 
        .modes = "a" ,
        .flags = CLOGGER_SHOW_ALL,
        .thread_safe = 0
    };
    clogger_options results_options = { 
        .min_level = CLOGGER_INFO, 
        .modes = "a",
        .flags = CLOGGER_SHOW_NONE,
        .thread_safe = 0
    };
    test_arg arg;

    arg.op_time = (operation_time *)malloc(sizeof(operation_time) * 10); // Allocate for up to 10 operations
    if (!arg.op_time) {
        printf("Failed to allocate memory for operation times.\n");
        return -1;
    }
    arg.op_time_count = 0;

    int seed_default = __UNITTEST_SEED_DEFAULT_VALUE;

    cargs_init(&parser, argc, argv);
    cargs_add_arg(&parser, __UNITTEST_DEBUG_FILE_ARG_NAME, "Path to the debug log file", 0, CARG_TYPE_STRING, NULL);
    cargs_add_arg(&parser, __UNITTEST_RESULTS_FILE_NAME, "Path to the results file", 0, CARG_TYPE_STRING, __UNITTEST_RESULTS_FILE_NAME_VALUE);
    cargs_add_arg(&parser, __UNITTEST_SEED_ARG_NAME, "Random seed for the tests", 0, CARG_TYPE_INT, &seed_default);
    cargs_add_arg(&parser, __UNITTEST_MODULE_ARG_NAME, "Module to test (if not specified, all modules are tested)", 0, CARG_TYPE_STRING, NULL);
    cargs_add_arg(&parser, __UNITTEST_BENCHMARK_FILE_ARG_NAME, "Path to the benchmark CSV file", 0, CARG_TYPE_STRING, __UNITTEST_BENCHMARK_FILE_NAME);
    cargs_parse(&parser);

    const char *debug_file = cargs_get_arg(&parser, __UNITTEST_DEBUG_FILE_ARG_NAME);
    const char *results_file = cargs_get_arg(&parser, __UNITTEST_RESULTS_FILE_NAME);
    const char *benchmark_file = cargs_get_arg(&parser, __UNITTEST_BENCHMARK_FILE_ARG_NAME);
    if (debug_file != NULL) {
        if (clogger_init(&debug_logger, debug_file, debug_options) != CS_SUCCESS) {
            printf("Failed to initialize debug logger for file: %s\n", debug_file);
            return -1;
        }
    } else {
        debug_logger.fp = NULL;
        debug_logger.mutex = NULL;
    }

    if (clogger_init(&results_logger, results_file, results_options) != CS_SUCCESS) {
        printf("Failed to initialize results logger for file: %s\n", results_file);
        clogger_close(&debug_logger);
        cargs_free(&parser);
        return -1;
    }

    seed = *(int*)cargs_get_arg(&parser, __UNITTEST_SEED_ARG_NAME);
    srand(seed);

    const char *module_filter = (const char *)cargs_get_arg(&parser, __UNITTEST_MODULE_ARG_NAME);
    if (module_filter) {
        // Filter modules based on the provided module name
        int found = 0;
        for (int m = 0; m < num_modules; m++) {
            if (strcmp(all_modules[m].name, module_filter) == 0) {
                all_modules[0] = all_modules[m];
                num_modules = 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            clogger_log(debug_logger, CLOGGER_WARNING, "Module '%s' not found.\n", module_filter);
            free(arg.op_time);
            clogger_close(&debug_logger);
            cargs_free(&parser);
            clogger_close(&results_logger);
            return 0;
        }
    }

    // Calculate total tests
    for (int m = 0; m < num_modules; m++) {
        total_tests += all_modules[m].size;
    }

    clogger_log(results_logger, CLOGGER_INFO, "========================================\n");
    clogger_log(results_logger, CLOGGER_INFO, "  UNIFIED UNITTEST RUN\n");
    clogger_log(results_logger, CLOGGER_INFO, "  SEED: %d\n", seed);
    clogger_log(results_logger, CLOGGER_INFO, "  MODULES: %d | TOTAL TESTS: %d\n", num_modules, total_tests);
    clogger_log(results_logger, CLOGGER_INFO, "========================================\n\n");

    // Run tests for each module
    for (int m = 0; m < num_modules; m++) {
        module_tests *mod = &all_modules[m];
        int success = 0, failed = 0;

        clogger_log(results_logger, CLOGGER_INFO, "----------------------------------------\n");
        clogger_log(results_logger, CLOGGER_INFO, "  MODULE: %s (%d tests)\n", mod->name, mod->size);
        clogger_log(results_logger, CLOGGER_INFO, "----------------------------------------\n");

        for (int i = 0; i < mod->size; i++) {
            arg.logger = &debug_logger;
            if (RUNNING_ON_VALGRIND) {
                arg.op_time_count = 0; // Skip benchmark times when running under Valgrind
            } else {
                read_operation_time(&arg, benchmark_file, mod->name);
            }
            
            test_res res = mod->tests[i](&arg);

            char buffer[__MAX_REASON_SIZE];
            const char *dots =
                "..........................................................................";

            strncpy(buffer, res.test_name, sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0';

            size_t len = strlen(buffer);
            size_t max_dots = (__MAX_PRINT_SIZE > len) ? (__MAX_PRINT_SIZE - len) : 0;
            memcpy(buffer + len, dots, max_dots);
            buffer[len + max_dots] = '\0';

            if (res.return_code != CS_SUCCESS) {
                failed++;
                clogger_log(results_logger, CLOGGER_ERROR,
                        "[%2d/%2d] %s" __RED_UNITTEST __BOLD_UNITTEST "[FAIL]" __RESET_UNITTEST
                        "  reason: %s\n",
                        i + 1, mod->size, buffer,
                        res.reason ? res.reason : "(no reason)");
            } else {
                success++;
                clogger_log(results_logger, CLOGGER_INFO,
                        "[%2d/%2d] %s" __GREEN_UNITTEST "[ OK ]" __RESET_UNITTEST "\n",
                        i + 1, mod->size, buffer);
            }
        }

        clogger_log(results_logger, CLOGGER_INFO, "  >> %s: %d passed, %d failed\n\n", mod->name, success, failed);
        total_success += success;
        total_failed += failed;

        if (failed == 0 && !RUNNING_ON_VALGRIND && arg.op_time_count > 0) {
            // Print performance summary for the module if all tests passed
            clogger_log(results_logger, CLOGGER_INFO, "  Performance Summary for %s:\n", mod->name);
            for (int j = 0; j < arg.op_time_count; j++) {
                clogger_log(results_logger, CLOGGER_INFO, "    %-10s: C++ time = %.6f sec, C time = %.6f sec\n",
                    arg.op_time[j].operation, arg.op_time[j].cpp_time, arg.op_time[j].c_time);
            }
            clogger_log(results_logger, CLOGGER_INFO, "\n");
        }

        // Clean up operation time strings after performance summary
        for (int j = 0; j < arg.op_time_count; j++) {
            free((void*)arg.op_time[j].operation);
        }
        arg.op_time_count = 0;
    }

    // Final summary
    clogger_log(results_logger, CLOGGER_INFO, "========================================\n");
    if (total_failed == 0) {
        clogger_log(results_logger, CLOGGER_INFO, __GREEN_UNITTEST __BOLD_UNITTEST
                "FINAL SUMMARY: %d passed, %d failed (total %d)" __RESET_UNITTEST "\n",
                total_success, total_failed, total_tests);
    } else {
        clogger_log(results_logger, CLOGGER_ERROR, __RED_UNITTEST __BOLD_UNITTEST
                "FINAL SUMMARY: %d passed, %d failed (total %d)" __RESET_UNITTEST "\n",
                total_success, total_failed, total_tests);
    }
    clogger_log(results_logger, CLOGGER_INFO, "========================================\n\n");

    free(arg.op_time);
    clogger_close(&debug_logger);
    cargs_free(&parser);
    clogger_close(&results_logger);
    return total_failed > 0 ? 1 : 0;
}