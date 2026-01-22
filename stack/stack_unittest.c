#include <cs/stack.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_stack_init() {
    stack_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .fr = NULL,
        .comp = NULL,
        .print = print_int
    };

    stack *s = stack_init(CS_STACK_ARRAY, attr);
    if (s == NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "Failed to initialize stack",
            .return_code = CS_ELEM
        };
    }

    stack_free(s);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_stack_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}