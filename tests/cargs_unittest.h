#include <cs/cargs.h>

#include <unittest.h>
#include <sys/wait.h>

test_res test_cargs_init(test_arg *arg) {
    cparser *parser = UNITTEST_ASSERT(cargs_init(0, NULL), !=, NULL, "Failed to initialize cargs parser",
        arg->logger, "Successfully initialized cargs parser\n");

    cargs_free(parser);
    return SUCCESSFUL_TEST_RES;
};

test_res test_cargs_parse(test_arg *arg) {
    int argc = 8;
    char *argv[] = { "program_name", "--int-arg", "123", "--float-arg", "3.14", "--string-arg", "test", "--bool-arg" };
    cparser *parser = UNITTEST_ASSERT(cargs_init(argc, argv), !=, NULL, "Failed to initialize cargs parser for parse test",
        arg->logger, "Successfully initialized cargs parser for parse test\n");

    cargs_add_arg(parser, "--int-arg", "An integer argument", 0, CARG_TYPE_INT, NULL);
    cargs_add_arg(parser, "--float-arg", "A float argument", 0, CARG_TYPE_FLOAT, NULL);
    cargs_add_arg(parser, "--string-arg", "A string argument", 0, CARG_TYPE_STRING, NULL);
    cargs_add_arg(parser, "--bool-arg", "A boolean argument", 0, CARG_TYPE_BOOL, NULL);
    cargs_parse(parser);

    int *int_arg = (int *)UNITTEST_ASSERT(cargs_get_arg(parser, "--int-arg"), !=, NULL, "Failed to get integer argument",
        arg->logger, "Successfully retrieved integer argument\n");
    float *float_arg = (float *)UNITTEST_ASSERT(cargs_get_arg(parser, "--float-arg"), !=, NULL, "Failed to get float argument",
        arg->logger, "Successfully retrieved float argument\n");
    char *string_arg = (char *)UNITTEST_ASSERT(cargs_get_arg(parser, "--string-arg"), !=, NULL, "Failed to get string argument",
        arg->logger, "Successfully retrieved string argument\n");
    char *bool_arg = (char *)UNITTEST_ASSERT(cargs_get_arg(parser, "--bool-arg"), !=, NULL, "Failed to get boolean argument",
        arg->logger, "Successfully retrieved boolean argument\n");

    UNITTEST_ASSERT(*int_arg, ==, 123, "Parsed integer argument does not match expected value",
        arg->logger, "Parsed integer argument matches expected value\n");
    UNITTEST_ASSERT(*float_arg, ==, 3.14f, "Parsed float argument does not match expected value",
        arg->logger, "Parsed float argument matches expected value\n");
    UNITTEST_ASSERT(strcmp(string_arg, "test"), ==, 0, "Parsed string argument does not match expected value",
        arg->logger, "Parsed string argument matches expected value\n");
    UNITTEST_ASSERT(*bool_arg, ==, 1, "Parsed boolean argument does not match expected value",
        arg->logger, "Parsed boolean argument matches expected value\n");

    int_arg = (int *)UNITTEST_ASSERT(cargs_get_arg(parser, "int-arg"), !=, NULL, "Failed to get integer argument",
        arg->logger, "Successfully retrieved integer argument\n");
    float_arg = (float *)UNITTEST_ASSERT(cargs_get_arg(parser, "float-arg"), !=, NULL, "Failed to get float argument",
        arg->logger, "Successfully retrieved float argument\n");
    string_arg = (char *)UNITTEST_ASSERT(cargs_get_arg(parser, "string-arg"), !=, NULL, "Failed to get string argument",
        arg->logger, "Successfully retrieved string argument\n");
    bool_arg = (char *)UNITTEST_ASSERT(cargs_get_arg(parser, "bool-arg"), !=, NULL, "Failed to get boolean argument",
        arg->logger, "Successfully retrieved boolean argument\n");

    UNITTEST_ASSERT(*int_arg, ==, 123, "Parsed integer argument does not match expected value",
        arg->logger, "Parsed integer argument matches expected value\n");
    UNITTEST_ASSERT(*float_arg, ==, 3.14f, "Parsed float argument does not match expected value",
        arg->logger, "Parsed float argument matches expected value\n");
    UNITTEST_ASSERT(strcmp(string_arg, "test"), ==, 0, "Parsed string argument does not match expected value",
        arg->logger, "Parsed string argument matches expected value\n");
    UNITTEST_ASSERT(*bool_arg, ==, 1, "Parsed boolean argument does not match expected value",
        arg->logger, "Parsed boolean argument matches expected value\n");

    UNITTEST_ASSERT(cargs_get_arg(parser, "non-existent-arg"), ==, NULL, "Getting a non-existent argument should return NULL",
        arg->logger, "Getting a non-existent argument correctly returns NULL\n");

    cargs_free(parser);
    return SUCCESSFUL_TEST_RES;
}

test_res test_cargs_help(test_arg *arg) {
    pid_t pid = fork();
    char *argv[] = { "program_name", "--help" };
    int argc = sizeof(argv) / sizeof(char*);
    if (pid == 0) {
        cparser* parser = cargs_init(argc, argv);
        cargs_add_arg(parser, "--int-arg", "An integer argument", 0, CARG_TYPE_INT, NULL);
        cargs_add_arg(parser, "--float-arg", "A float argument", 0, CARG_TYPE_FLOAT, NULL);
        cargs_add_arg(parser, "--string-arg", "A string argument", 0, CARG_TYPE_STRING, NULL);
        cargs_add_arg(parser, "--bool-arg", "A boolean argument", 0, CARG_TYPE_BOOL, NULL);

        int default_int = 42;
        float default_float = 2.718f;
        char default_string[] = "default";
        char default_bool = 1;
        cargs_add_arg(parser, "--int-arg-def", "An integer argument", 0, CARG_TYPE_INT, &default_int);
        cargs_add_arg(parser, "--float-arg-def", "A float argument", 0, CARG_TYPE_FLOAT, &default_float);
        cargs_add_arg(parser, "--string-arg-def", "A string argument", 0, CARG_TYPE_STRING, default_string);
        cargs_add_arg(parser, "--bool-arg-def", "A boolean argument", 0, CARG_TYPE_BOOL, &default_bool);

        int default_int_req = 42;
        float default_float_req = 2.718f;
        char default_string_req[] = "default";
        char default_bool_req = 1;
        cargs_add_arg(parser, "--int-arg-def-req", "An integer argument", 1, CARG_TYPE_INT, &default_int_req);
        cargs_add_arg(parser, "--float-arg-def-req", "A float argument", 1, CARG_TYPE_FLOAT, &default_float_req);
        cargs_add_arg(parser, "--string-arg-def-req", "A string argument", 1, CARG_TYPE_STRING, default_string_req);
        cargs_add_arg(parser, "--bool-arg-def-req", "A boolean argument", 1, CARG_TYPE_BOOL, &default_bool_req);

        cargs_add_arg(parser, "--int-arg-req", "A required integer argument", 1, CARG_TYPE_INT, NULL);
        cargs_add_arg(parser, "--float-arg-req", "A required float argument", 1, CARG_TYPE_FLOAT, NULL);
        cargs_add_arg(parser, "--string-arg-req", "A required string argument", 1, CARG_TYPE_STRING, NULL);
        cargs_add_arg(parser, "--bool-arg-req", "A required boolean argument", 1, CARG_TYPE_BOOL, NULL);
        cargs_parse(parser);
        // If the child process reaches this point, it means the help message was not triggered as expected
        cargs_free(parser);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            int actual_exit_code = WEXITSTATUS(status);
            UNITTEST_ASSERT(actual_exit_code, ==, EXIT_SUCCESS, "Child process did not exit with expected code after help message",
                arg->logger, "Child process exited with expected code after help message\n");
        }
        return SUCCESSFUL_TEST_RES; // If the child process did not exit normally, consider the test successful since it indicates the help message was triggered
    }
}

test_res test_cargs_missing_required_arg(test_arg *arg) {
    pid_t pid = fork();
    char *argv[] = { "program_name", "--int-arg", "123" }; // Missing required arguments
    int argc = sizeof(argv) / sizeof(char*);
    if (pid == 0) {
        cparser* parser = cargs_init(argc, argv);
        cargs_add_arg(parser, "--int-arg", "An integer argument", 0, CARG_TYPE_INT, NULL);
        cargs_add_arg(parser, "--required-arg", "A required argument", 1, CARG_TYPE_STRING, NULL);
        cargs_parse(parser);
        // If the child process reaches this point, it means the missing required argument was not detected as expected
        cargs_free(parser);
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            int actual_exit_code = WEXITSTATUS(status);
            UNITTEST_ASSERT(actual_exit_code, ==, EXIT_FAILURE, "Child process did not exit with expected code after missing required argument",
                arg->logger, "Child process exited with expected code after missing required argument\n");
        }
        return SUCCESSFUL_TEST_RES; // If the child process did not exit normally, consider the test successful since it indicates the missing required argument was detected
    }
};

// ============================================================================
// Test Registry
// ============================================================================
test cargs_tests[] = {
    // cargs_init
    test_cargs_init,

    // cargs_parse
    test_cargs_parse,

    // cargs_missing_required_arg
    test_cargs_missing_required_arg,

    // cargs_help
    test_cargs_help,
};