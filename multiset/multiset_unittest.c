#include <cs/multiset.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_multiset_init() {
    multiset_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = NULL,
        .comp = NULL
    };

    multiset *ms = multiset_init(attr);
    if (ms == NULL) {
        return (test_res) {
            .test_name = (char *)__func__,
            .reason = "multiset_init returned NULL",
            .return_code = CS_UNKNOWN,
        };
    }

    multiset_free(ms);

    return (test_res) {
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_multiset_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}