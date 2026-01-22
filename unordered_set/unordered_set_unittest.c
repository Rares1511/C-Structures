#include <cs/unordered_set.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_unordered_set_init() {
    unordered_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .fr = NULL,
        .copy = NULL,
        .print = print_int
    };

    unordered_set *uset = unordered_set_init(attr, NULL, 10);

    if (uset == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered set",
            .return_code = CS_UNKNOWN
        };
    }

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_unordered_set_init,
    };
    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
};