#include <cs/forward_list.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_forward_list_init() {
    forward_list_attr_t attr = {
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .comp = NULL
    };

    forward_list* list = forward_list_init(attr);
    if (list == NULL) {
        return (test_res) {
            .test_name = (char*) __func__,
            .reason = "forward_list_init returned NULL",
            .return_code = CS_UNKNOWN
        };
    };

    forward_list_free(list);

    return (test_res) {
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char** argv) {
    test tests[] = {
        test_forward_list_init,
    };

    unittest(tests, sizeof(tests)/sizeof(test), argc, argv);
    return 0;
}