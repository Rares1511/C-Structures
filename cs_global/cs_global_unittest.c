#include <cs_global.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/unittest.h"

test_res cs_global_init_test() {
    if (cs_global_add_entry(CS_DEQUE, NULL) != CS_SUCCESS) {
        return (test_res){.test_name = "cs_global_init_test",
                          .reason = "cs_global_add_entry failed",
                          .return_code = CS_UNKNOWN};
    }

    printf("cs_global_size: %d\n", cs_global_size());

    return (test_res){
        .test_name = "cs_global_init_test", .reason = "none", .return_code = CS_SUCCESS};
}

int main() {
    test tests[] = {cs_global_init_test};

    unittest(tests, sizeof(tests) / sizeof(test));

    return 0;
}