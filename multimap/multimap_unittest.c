#include <cs/multimap.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_multimap_init() {
    multimap_attr_t key_attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = NULL,
        .comp = NULL
    };

    multimap_attr_t value_attr = {
        .size = sizeof(char),
        .fr = NULL,
        .copy = NULL,
        .print = NULL,
        .comp = NULL
    };

    multimap *mm = multimap_init(key_attr, value_attr);
    if (mm == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize multimap",
            .return_code = CS_UNKNOWN
        };
    }

    multimap_free(mm);
    
    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_multimap_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}