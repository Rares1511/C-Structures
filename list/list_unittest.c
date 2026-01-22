#include <cs/list.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_list_init() {
    list_attr_t attr = {
        .size = sizeof(int),
        .copy = NULL,
        .comp = NULL,
        .print = print_int,
        .fr = NULL
    };

    list *l = list_init(attr);
    if (l == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "list_init returned NULL",
            .return_code = CS_UNKNOWN
        };
    }

    list_free(l);

    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_list_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}