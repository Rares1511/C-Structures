#include <cs/unordered_map.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_unordered_map_init() {
    unordered_map_attr_t key_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map_attr_t value_attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .comp = NULL
    };

    unordered_map *umap = unordered_map_init(key_attr, value_attr, NULL, 16);

    if (umap == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize unordered_map",
            .return_code = CS_UNKNOWN
        };
    }

    unordered_map_free(umap);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_unordered_map_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}