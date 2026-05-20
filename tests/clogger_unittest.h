#include <cs/clogger.h>
#include <unittest.h>
#include <unistd.h>

#define __CLOGGER_TEST_FILE "/tmp/clogger_test.log"

// ============================================================================
// clogger_init
// ============================================================================
test_res test_clogger_init(test_arg *arg) {
    unlink(__CLOGGER_TEST_FILE); // Ensure the log file is removed before the test
    clogger_options options = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_ALL,
        .thread_safe = 0
    };
    clogger *logger = UNITTEST_ASSERT(clogger_init(__CLOGGER_TEST_FILE, options), !=, NULL, "Failed to initialize clogger with valid options",
        arg->logger, "clogger_init should return a valid pointer\n");
    clogger_close(logger);
    return SUCCESSFUL_TEST_RES;
}

test_res test_clogger_init_thread_safe(test_arg *arg) {
    unlink(__CLOGGER_TEST_FILE); // Ensure the file is removed before the test
    clogger_options options = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_ALL,
        .thread_safe = 1
    };
    clogger *logger = UNITTEST_ASSERT(clogger_init(__CLOGGER_TEST_FILE, options), !=, NULL, "Failed to initialize thread-safe clogger",
        arg->logger, "clogger_init should return a valid pointer for thread-safe logger\n");
    clogger_close(logger);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// clogger_assert
// ============================================================================
test_res test_clogger_assert(test_arg *arg) {
    unlink(__CLOGGER_TEST_FILE);
    clogger_options options = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_ALL,
        .thread_safe = 0
    };
    clogger *logger = UNITTEST_ASSERT(clogger_init(__CLOGGER_TEST_FILE, options), !=, NULL, "Failed to initialize clogger for assert test",
        arg->logger, "clogger_init should return a valid pointer\n");
    
    int x = 5;
    clogger_assert(logger, x == 5, "x should be equal to 5");
    clogger_assert(logger, x == 10, "x should be equal to 10");

    // Read the log file to verify that the assertion failure message was logged
    char buffer[256];
    FILE *fp = fopen(__CLOGGER_TEST_FILE, "r");
    UNITTEST_ASSERT(fp, !=, NULL, "Failed to open log file for reading",
        arg->logger, "fopen should return a valid file pointer\n");
    int found_assertion_failure = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "Assertion failed: x should be equal to 10") != NULL) {
            found_assertion_failure = 1;
            break;
        }
    }
    fclose(fp);
    UNITTEST_ASSERT(found_assertion_failure, ==, 1, "Assertion failure message was not logged",
        arg->logger, "clogger_assert should log an assertion failure message when the condition is false\n");
    
    clogger_close(logger);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// clogger_log
// ============================================================================
test_res test_clogger_log(test_arg *arg) {
    // Remove existing log file if it exists
    unlink(__CLOGGER_TEST_FILE);
    clogger_options options = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_ALL,
        .thread_safe = 0
    };
    clogger *logger = UNITTEST_ASSERT(clogger_init(__CLOGGER_TEST_FILE, options), !=, NULL, "Failed to initialize clogger for log test",
        arg->logger, "clogger_init should return a valid pointer\n");
    
    clogger_log(logger, CLOGGER_INFO, "This is an info message\n");
    clogger_log(logger, CLOGGER_DEBUG, "This is a debug message\n");
    clogger_log(logger, CLOGGER_WARNING, "This is a warning message\n");
    clogger_log(logger, CLOGGER_ERROR, "This is an error message\n");

    // Read the log file to verify that the messages were logged
    char buffer[256];
    FILE *fp = fopen(__CLOGGER_TEST_FILE, "r");
    UNITTEST_ASSERT(fp, !=, NULL, "Failed to open log file for reading",
        arg->logger, "fopen should return a valid file pointer\n");
    int found_info_message = 0;
    int found_error_message = 0;
    int found_debug_message = 0;
    int found_warning_message = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Read line: %s", buffer);
        if (strstr(buffer, "This is an info message") != NULL) {
            found_info_message = 1;
        }
        if (strstr(buffer, "This is an error message") != NULL) {
            found_error_message = 1;
        }
        if (strstr(buffer, "This is a debug message") != NULL) {
            found_debug_message = 1;
        }
        if (strstr(buffer, "This is a warning message") != NULL) {
            found_warning_message = 1;
        }
    }
    fclose(fp);
    UNITTEST_ASSERT(found_info_message, ==, 1, "Info message was not logged",
        arg->logger, "clogger_log should log info messages when the level is INFO or higher\n");
    UNITTEST_ASSERT(found_error_message, ==, 1, "Error message was not logged",
        arg->logger, "clogger_log should log error messages when the level is ERROR or higher\n");
    UNITTEST_ASSERT(found_debug_message, ==, 1, "Debug message was not logged",
        arg->logger, "clogger_log should log debug messages when the level is DEBUG or higher\n");
    UNITTEST_ASSERT(found_warning_message, ==, 1, "Warning message was not logged",
        arg->logger, "clogger_log should log warning messages when the level is WARNING or higher\n");
    
    clogger_close(logger);
    return SUCCESSFUL_TEST_RES;
}

// ============================================================================
// clogger_print_stacktrace
// ============================================================================
test_res test_clogger_print_stacktrace(test_arg *arg) {
    unlink(__CLOGGER_TEST_FILE); // Ensure the file is removed before the test
    clogger_options options = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_ALL,
        .thread_safe = 0
    };
    clogger *logger = UNITTEST_ASSERT(clogger_init(__CLOGGER_TEST_FILE, options), !=, NULL, "Failed to initialize clogger for stacktrace test",
        arg->logger, "clogger_init should return a valid pointer\n");
    
    clogger_log(logger, CLOGGER_ERROR, "This is an error message that should include a stack trace\n");

    // Read the log file to verify that the stack trace was logged
    char buffer[256];
    FILE *fp = fopen(__CLOGGER_TEST_FILE, "r");
    UNITTEST_ASSERT(fp, !=, NULL, "Failed to open log file for reading",
        arg->logger, "fopen should return a valid file pointer\n");
    int found_stacktrace = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        clogger_log(arg->logger, CLOGGER_DEBUG, "Read line: %s", buffer);
        if (strstr(buffer, "Stack trace:") != NULL) {
            found_stacktrace = 1;
        }
    }
    fclose(fp);
    UNITTEST_ASSERT(found_stacktrace, ==, 1, "Stack trace was not logged",
        arg->logger, "clogger_log should include a stack trace when the level is ERROR or higher\n");
    clogger_close(logger);
    return SUCCESSFUL_TEST_RES;
}

test clogger_tests[] = {
    // clogger_init
    test_clogger_init,
    test_clogger_init_thread_safe,

    // clogger_log
    test_clogger_log,

    // clogger_assert
    test_clogger_assert,

    // clogger_print_stacktrace
    test_clogger_print_stacktrace,
};