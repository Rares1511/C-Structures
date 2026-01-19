#include <cs/unordered_set.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_unordered_set_init() {
    unordered_set uset;
    unordered_set_attr_t attr = { 
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .size = sizeof(int),
        .print = print_int
    };
    cs_codes rc = unordered_set_init(&uset, attr, NULL, 10);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered_set",
            .return_code = rc
        };
    }

    if (uset.ht == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Hash table inside unordered_set is NULL after initialization",
            .return_code = CS_ELEM
        };
    }

    unordered_set_free(&uset);

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