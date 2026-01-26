#include <cs/unordered_multiset.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_unordered_multiset_init() {
    unordered_multiset_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .fr = NULL,
        .print = NULL,
    };

    unordered_multiset *umset = unordered_multiset_init(attr, NULL, 10);
    if (umset == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered_multiset",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multiset_free(umset);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_unordered_multiset_init,
    };
    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}