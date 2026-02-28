#include <cs/clogger.h>
#include <unittest.h>
#include <unistd.h>

#define __CLOGGER_TEST_FILE "/tmp/clogger_test.log"

// Helper to read file contents
static char *read_test_log_file(void) {
    FILE *fp = fopen(__CLOGGER_TEST_FILE, "r");
    if (!fp) return NULL;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *content = malloc(size + 1);
    if (content) {
        fread(content, 1, size, fp);
        content[size] = '\0';
    }
    fclose(fp);
    return content;
}

// Helper to cleanup test file
static void cleanup_test_log_file(void) {
    unlink(__CLOGGER_TEST_FILE);
}

// ============================================================================
// clogger_init tests
// ============================================================================

test_res test_clogger_init_basic(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    cs_codes rc = clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    if (rc != CS_SUCCESS) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Init failed", rc};
    }
    
    if (logger.fp == NULL) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "File pointer is NULL after init", CS_NULL};
    }
    
    if (logger.mutex != NULL) {
        clogger_close(&logger);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Mutex should be NULL when thread_safe=0", CS_UNKNOWN};
    }
    
    clogger_close(&logger);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_init_null_logger(test_arg *arg) {
    (void)arg;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    cs_codes rc = clogger_init(NULL, __CLOGGER_TEST_FILE, opts);
    if (rc != CS_NULL) {
        return (test_res){(char*)__func__, "Should return CS_NULL for NULL logger", rc};
    }
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_init_null_filename(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    cs_codes rc = clogger_init(&logger, NULL, opts);
    if (rc != CS_NULL) {
        return (test_res){(char*)__func__, "Should return CS_NULL for NULL filename", rc};
    }
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_init_null_modes(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = NULL,
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    cs_codes rc = clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    if (rc != CS_NULL) {
        return (test_res){(char*)__func__, "Should return CS_NULL for NULL modes", rc};
    }
    
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_init_thread_safe(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 1
    };
    
    cs_codes rc = clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    if (rc != CS_SUCCESS) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Init with thread_safe failed", rc};
    }
    
    if (logger.mutex == NULL) {
        clogger_close(&logger);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Mutex should not be NULL when thread_safe=1", CS_NULL};
    }
    
    clogger_close(&logger);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// clogger_log tests
// ============================================================================

test_res test_clogger_log_basic(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "Test message\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    if (strstr(content, "[INFO") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Log level not found in output", CS_ELEM};
    }
    
    if (strstr(content, "Test message") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Log message not found in output", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_log_levels(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_DEBUG, "Debug msg\n");
    clogger_log(logger, CLOGGER_INFO, "Info msg\n");
    clogger_log(logger, CLOGGER_WARNING, "Warning msg\n");
    clogger_log(logger, CLOGGER_ERROR, "Error msg\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    if (strstr(content, "[DEBUG") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "DEBUG level not found", CS_ELEM};
    }
    
    if (strstr(content, "[INFO") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "INFO level not found", CS_ELEM};
    }
    
    if (strstr(content, "[WARNING") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "WARNING level not found", CS_ELEM};
    }
    
    if (strstr(content, "[ERROR") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "ERROR level not found", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_log_min_level_filter(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_WARNING,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_DEBUG, "Debug msg\n");
    clogger_log(logger, CLOGGER_INFO, "Info msg\n");
    clogger_log(logger, CLOGGER_WARNING, "Warning msg\n");
    clogger_log(logger, CLOGGER_ERROR, "Error msg\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // DEBUG and INFO should NOT be present
    if (strstr(content, "Debug msg") != NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "DEBUG should be filtered out", CS_ELEM};
    }
    
    if (strstr(content, "Info msg") != NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "INFO should be filtered out", CS_ELEM};
    }
    
    // WARNING and ERROR should be present
    if (strstr(content, "Warning msg") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "WARNING should be present", CS_ELEM};
    }
    
    if (strstr(content, "Error msg") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "ERROR should be present", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// clogger_flags tests
// ============================================================================

test_res test_clogger_flags_none(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_NONE,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "Simple message\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // With SHOW_NONE, should only have the message
    if (strstr(content, "[INFO") != NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Level should not be shown with SHOW_NONE", CS_ELEM};
    }
    
    if (strstr(content, "Simple message") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Message should still be present", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_flags_time(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_TIME,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "Timed message\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Should contain time format HH:MM:SS
    if (strstr(content, ":") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Time format not found", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_flags_func(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_FUNC,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "Func message\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Should contain function name with ()
    if (strstr(content, "test_clogger_flags_func()") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Function name not found in output", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_flags_line(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LINE,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    int expected_line = __LINE__ + 1;
    clogger_log(logger, CLOGGER_INFO, "Line message\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Should contain line number
    char line_str[16];
    snprintf(line_str, sizeof(line_str), "%d ", expected_line);
    if (strstr(content, line_str) == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Line number not found in output", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_flags_all(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_ALL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "Full message\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Should contain level, time (colons), function
    if (strstr(content, "[INFO") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Level not found with SHOW_ALL", CS_ELEM};
    }
    
    if (strstr(content, ":") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Time not found with SHOW_ALL", CS_ELEM};
    }
    
    if (strstr(content, "test_clogger_flags_all()") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Function not found with SHOW_ALL", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Stack trace tests
// ============================================================================

test_res test_clogger_stacktrace_on_error(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL | CLOGGER_SHOW_STACKTRACE,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_ERROR, "Error with trace\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Should contain stack trace marker
    if (strstr(content, "Stack trace:") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Stack trace not found on ERROR", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_no_stacktrace_on_warning(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL | CLOGGER_SHOW_STACKTRACE,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_WARNING, "Warning without trace\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Stack trace should NOT be present for WARNING level
    if (strstr(content, "Stack trace:") != NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Stack trace should not appear on WARNING", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_no_stacktrace_without_flag(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,  // No CLOGGER_SHOW_STACKTRACE
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_ERROR, "Error without trace flag\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Stack trace should NOT be present without the flag
    if (strstr(content, "Stack trace:") != NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Stack trace should not appear without flag", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// clogger_close tests
// ============================================================================

test_res test_clogger_close_basic(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_close(&logger);
    
    if (logger.fp != NULL) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "File pointer should be NULL after close", CS_UNKNOWN};
    }
    
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_close_thread_safe(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 1
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_close(&logger);
    
    if (logger.fp != NULL) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "File pointer should be NULL after close", CS_UNKNOWN};
    }
    
    if (logger.mutex != NULL) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Mutex should be NULL after close", CS_UNKNOWN};
    }
    
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_close_null(test_arg *arg) {
    (void)arg;
    // Should not crash when closing NULL
    clogger_close(NULL);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// clogger_assert tests
// ============================================================================

test_res test_clogger_assert_pass(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_assert(logger, 1 == 1, "This should not appear");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Assertion passed, so nothing should be logged
    if (strstr(content, "Assertion failed") != NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Assertion passed but message was logged", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test_res test_clogger_assert_fail(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_assert(logger, 1 == 0, "Expected failure message");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Assertion failed, should be logged
    if (strstr(content, "Assertion failed") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Assertion failed but message not logged", CS_ELEM};
    }
    
    if (strstr(content, "Expected failure message") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Assertion message not found", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Append mode test
// ============================================================================

test_res test_clogger_append_mode(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    // First write
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "First message\n");
    clogger_close(&logger);
    
    // Second write with append mode
    opts.modes = "a";
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "Second message\n");
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    // Both messages should be present
    if (strstr(content, "First message") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "First message not found in appended file", CS_ELEM};
    }
    
    if (strstr(content, "Second message") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Second message not found in appended file", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Formatted output test
// ============================================================================

test_res test_clogger_formatted_output(test_arg *arg) {
    (void)arg;
    clogger logger;
    clogger_options opts = {
        .min_level = CLOGGER_DEBUG,
        .modes = "w",
        .flags = CLOGGER_SHOW_LEVEL,
        .thread_safe = 0
    };
    
    clogger_init(&logger, __CLOGGER_TEST_FILE, opts);
    clogger_log(logger, CLOGGER_INFO, "Int: %d, String: %s, Float: %.2f\n", 42, "test", 3.14);
    clogger_close(&logger);
    
    char *content = read_test_log_file();
    if (!content) {
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Could not read log file", CS_MEM};
    }
    
    if (strstr(content, "Int: 42") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Integer formatting failed", CS_ELEM};
    }
    
    if (strstr(content, "String: test") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "String formatting failed", CS_ELEM};
    }
    
    if (strstr(content, "Float: 3.14") == NULL) {
        free(content);
        cleanup_test_log_file();
        return (test_res){(char*)__func__, "Float formatting failed", CS_ELEM};
    }
    
    free(content);
    cleanup_test_log_file();
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// Test registry
// ============================================================================

test clogger_tests[] = {
    // clogger_init tests
    test_clogger_init_basic,
    test_clogger_init_null_logger,
    test_clogger_init_null_filename,
    test_clogger_init_null_modes,
    test_clogger_init_thread_safe,
    
    // clogger_log tests
    test_clogger_log_basic,
    test_clogger_log_levels,
    test_clogger_log_min_level_filter,
    
    // clogger_flags tests
    test_clogger_flags_none,
    test_clogger_flags_time,
    test_clogger_flags_func,
    test_clogger_flags_line,
    test_clogger_flags_all,
    
    // Stack trace tests
    test_clogger_stacktrace_on_error,
    test_clogger_no_stacktrace_on_warning,
    test_clogger_no_stacktrace_without_flag,
    
    // clogger_close tests
    test_clogger_close_basic,
    test_clogger_close_thread_safe,
    test_clogger_close_null,
    
    // clogger_assert tests
    test_clogger_assert_pass,
    test_clogger_assert_fail,
    
    // Mode and formatting tests
    test_clogger_append_mode,
    test_clogger_formatted_output,
};
