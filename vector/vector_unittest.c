#include <cs/vector.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_vector_init() {
    vector_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .comp = comp_int_min
    };

    vector *vec = vector_init(attr);
    if (vec == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "vector_init returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    vector_free(vec);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_vector_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}