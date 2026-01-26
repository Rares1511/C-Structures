#include <cs/unordered_multimap.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_unordered_multimap_init() {
    unordered_multimap_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL,
    };

    unordered_multimap_attr_t value_attr = {
        .size = sizeof(char),
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .comp = NULL,
    };

    unordered_multimap *umap = unordered_multimap_init(key_attr, value_attr, NULL, 10);
    if (umap == NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize unordered_multimap",
            .return_code = CS_UNKNOWN,
        };
    }

    unordered_multimap_free(umap);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS,
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_unordered_multimap_init,
    };

    unittest(tests, sizeof(tests) / sizeof(tests[0]), argc, argv);
    return 0;
}