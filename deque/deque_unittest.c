#include <cs/deque.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_deque_init() {
    deque_attr_t attr = {
        .size = sizeof(int),
        .print = print_int,
        .fr = NULL,
        .comp = NULL,
        .copy = NULL
    };
    deque *dq = deque_init(attr);
    if (dq == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize deque",
            .return_code = CS_UNKNOWN
        };
    };

    deque_free(dq);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_deque_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}