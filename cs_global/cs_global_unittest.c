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

    return (test_res){
        .test_name = "cs_global_init_test", .reason = "none", .return_code = CS_SUCCESS};
}

int main() {
    int i;
    test tests[] = {cs_global_init_test};
    test_res res;

    for (i = 0; i < MATRIX_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++) {
        res = tests[i]();
        char buffer[1024];
        strcpy(buffer, res.test_name);
        strncat(buffer,
                "..........................................................................",
                MAX_PRINT_SIZE - strlen(res.test_name));
        if (res.return_code != CS_SUCCESS) {
            printf("%sFAILED: %s\n", buffer, res.reason);
            exit(-(int)(sizeof(tests) / sizeof(test) - i));
        } else {
            printf("%sSUCCESS: %d/%d\n", buffer, i + 1, (int)(sizeof(tests) / sizeof(test)));
        }
    }
    return 0;
}