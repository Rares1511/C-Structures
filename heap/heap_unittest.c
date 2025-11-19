#include <cs/heap.h>

#include "../include/unittest.h"

FILE *DEBUG_OUT = NULL;

test_res test_heap_init() {
    heap h;
    heap_attr_t attr = {
        .size = sizeof(int),
        .comp = NULL,
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .stream = DEBUG_OUT
    };

    int rc = heap_init(&h, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_heap_init",
            .reason = "heap_init returned error code",
            .return_code = rc
        };
    }

    if (h.size != 0 || h.cap != INIT_CAPACITY || h.vec == NULL) {
        return (test_res){
            .test_name = "test_heap_init",
            .reason = "heap not initialized correctly",
            .return_code = CS_UNKNOWN
        };
    }

    heap_free(&h);

    return (test_res){
        .test_name = "test_heap_init",
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_heap_init,
    };
    
    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}