#include <cs/flat_set.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_flat_set_init() {
    flat_set_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL
    };

    flat_set *fs = flat_set_init(attr, CS_FLAT_SET_VECTOR);
    if (fs == NULL) {
        return (test_res) {
            .test_name = (char*) __func__,
            .reason = "flat_set_init returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    flat_set_free(fs);

    return (test_res) {
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_flat_set_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
} 