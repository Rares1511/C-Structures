#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/unittest.h"
#include "../include/vector.h"

test_res test_vector_init() {
    vector v;
    vector_attr_t attr = {sizeof(int), NULL, NULL, NULL, NULL};

    cs_codes rc = vector_init(&v, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_vector_init", .reason = "vector_init failed", .return_code = rc};
    }

    vector_free(&v);
    if (v.vec != NULL) {
        return (test_res){.test_name = "test_vector_init",
                          .reason = "vector_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){.test_name = "test_vector_init", .reason = "none", .return_code = CS_SUCCESS};
}

int main() {
    int i;
    test tests[] = {};
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