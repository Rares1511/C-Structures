#include "../include/heap.h"
#include "../include/unittest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

test_res test_heap_init() {
    heap h;
    heap_attr_t attr = {sizeof(int), NULL, NULL, NULL, NULL};
    cs_codes res;

    res = heap_init(&h, attr);
    if (res != CS_SUCCESS) {
        return (test_res){
            .reason = "heap init failed", .test_name = "HEAP_INIT", .return_code = res};
    }

    heap_free(&h);
    if (h.vec != NULL) {
        return (test_res){
            .reason = "heap free failed", .test_name = "HEAP_INIT", .return_code = CS_MEM};
    }

    return (test_res){.reason = "heap init success", .test_name = "HEAP_INIT", .return_code = res};
}

test_res test_heap_push() {
    heap h;
    heap_attr_t attr = {sizeof(int), NULL, NULL, NULL, NULL};
    cs_codes res;

    res = heap_init(&h, attr);
    if (res != CS_SUCCESS) {
        return (test_res){
            .reason = "heap init failed", .test_name = "HEAP_PUSH", .return_code = res};
    }

    int el = 5;
    res = heap_push(&h, &el);
    if (res != CS_SUCCESS) {
        return (test_res){
            .reason = "heap push failed", .test_name = "HEAP_PUSH", .return_code = res};
    }

    int *top = malloc(sizeof(int));
    heap_top(h, top);
    if (*top != el) {
        return (test_res){
            .reason = "heap top failed", .test_name = "HEAP_PUSH", .return_code = CS_UNKNOWN};
    }
    free(top);

    heap_free(&h);
    if (h.vec != NULL) {
        return (test_res){
            .reason = "heap free failed", .test_name = "HEAP_PUSH", .return_code = CS_MEM};
    }

    return (test_res){.reason = "heap push success", .test_name = "HEAP_PUSH", .return_code = res};
}

int main() {
    int i;
    test tests[] = {
        test_heap_init,
        test_heap_push,
    };
    test_res res;

    for (i = 0; i < HEAP_TEST_SIZE && i < (int)(sizeof(tests) / sizeof(test)); i++) {
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