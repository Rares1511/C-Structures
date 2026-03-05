#include <cs/cstring.h>
#include <unittest.h>

// ============================================================================
// cstring_init
// ============================================================================
test_res test_cstring_init(test_arg *arg) {
    cstring str;
    const char *test_data = "Hello, World!";
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing string with data: '%s'\n", test_data);
    cs_codes result = cstring_init(&str, test_data);
    if (result != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Cstring init failed", CS_MEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring initialized with data: '%s'\n", str._data);
    if (!str._data || strcmp(str._data, test_data) != 0) {
        cstring_free(&str);
        return (test_res){(char*)__func__, "Cstring data mismatch after init", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring data verified successfully\n");
    cstring_free(&str);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// cstring_init with empty string
// ============================================================================
test_res test_cstring_init_empty(test_arg *arg) {
    cstring str;
    const char *test_data = "";
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing string with empty data\n");
    cs_codes result = cstring_init(&str, test_data);
    if (result != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Cstring init failed for empty string", CS_MEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring initialized with empty data\n");
    if (!str._data || strcmp(str._data, test_data) != 0) {
        cstring_free(&str);
        return (test_res){(char*)__func__, "Cstring data mismatch after init with empty string", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring empty data verified successfully\n");
    cstring_free(&str);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// cstring_init regex init and free
// ============================================================================
test_res test_cstring_init_regex(test_arg *arg) {
    cstring str;
    const char *test_data = "ab";
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing string with regex data: '%s'\n", test_data);
    cs_codes result = cstring_init(&str, test_data);
    if (result != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Cstring init failed for regex string", CS_MEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring initialized with regex data: '%s'\n", str._data);
    if (!str._regex_nfa) {
        cstring_free(&str);
        return (test_res){(char*)__func__, "Cstring regex NFA initialization failed", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring regex NFA initialized successfully\n");
    cstring_free(&str);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// cstring_init regex init union
// ============================================================================
test_res test_cstring_init_regex_union(test_arg *arg) {
    cstring str;
    const char *test_data = "a|b";
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing string with regex data: '%s'\n", test_data);
    cs_codes result = cstring_init(&str, test_data);
    if (result != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Cstring init failed for regex string", CS_MEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring initialized with regex data: '%s'\n", str._data);
    if (!str._regex_nfa) {
        cstring_free(&str);
        return (test_res){(char*)__func__, "Cstring regex NFA initialization failed", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring regex NFA initialized successfully\n");
    cstring_free(&str);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// cstring_init regex init star
// ============================================================================
test_res test_cstring_init_regex_star(test_arg *arg) {
    cstring str;
    const char *test_data = "a*";
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing string with regex data: '%s'\n", test_data);
    cs_codes result = cstring_init(&str, test_data);
    if (result != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Cstring init failed for regex string", CS_MEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring initialized with regex data: '%s'\n", str._data);
    if (!str._regex_nfa) {
        cstring_free(&str);
        return (test_res){(char*)__func__, "Cstring regex NFA initialization failed", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring regex NFA initialized successfully\n");
    cstring_free(&str);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// ============================================================================
// cstring_init regex init with parentheses
// ============================================================================
test_res test_cstring_init_regex_paran(test_arg *arg) {
    cstring str;
    const char *test_data = "(ab)*";
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Initializing string with regex data: '%s'\n", test_data);
    cs_codes result = cstring_init(&str, test_data);
    if (result != CS_SUCCESS) {
        return (test_res){(char*)__func__, "Cstring init failed for regex string", CS_MEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring initialized with regex data: '%s'\n", str._data);
    if (!str._regex_nfa) {
        cstring_free(&str);
        return (test_res){(char*)__func__, "Cstring regex NFA initialization failed", CS_ELEM};
    }
    clogger_log(*arg->logger, CLOGGER_DEBUG, "Cstring regex NFA initialized successfully\n");
    cstring_free(&str);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

test cstring_tests[] = {
    // cstring_init
    test_cstring_init,
    test_cstring_init_empty,

    // cstring_init regex
    test_cstring_init_regex,
    test_cstring_init_regex_union,
    test_cstring_init_regex_star,
    test_cstring_init_regex_paran
};