#include <cs/priority_queue.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_priority_queue_init() {
    priority_queue_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_max,
        .print = NULL,
        .fr = NULL,
        .copy = NULL
    };

    priority_queue *pq = priority_queue_init(attr, CS_PRIORITY_QUEUE_VECTOR);
    if (pq == NULL) {
        return (test_res) {
            .test_name = (char*) __func__,
            .reason = "Failed to initialize priority queue",
            .return_code = CS_UNKNOWN
        };
    }

    priority_queue_free(pq);

    return (test_res) {
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_priority_queue_init,
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}