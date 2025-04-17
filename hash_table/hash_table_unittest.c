#include "../include/unittest.h"
#include "../include/universal.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int i;
    test tests[] = {};
    test_res res;

    for (i = 0; i < DEQUE_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++) {
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