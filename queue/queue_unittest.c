#include <cs/queue.h>

#include "../include/unittest.h"

FILE *__DEBUG_OUT = NULL;

test_res test_queue_init() {
    queue_attr attr = {
        .size = sizeof(int),
        .fr = NULL,
        .print = NULL,
        .copy = NULL,
        .comp = NULL
    };

    queue *q_deque = queue_init(attr, CS_QUEUE_DEFAULT);
    if (q_deque == NULL) {
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize queue with default type",
            .return_code = CS_UNKNOWN
        };
    }

    queue *q_list = queue_init(attr, CS_QUEUE_LIST);
    if (q_list == NULL) {
        queue_free(q_deque);
        return (test_res){
            .test_name = (char*) __func__,
            .reason = "Failed to initialize queue with list type",
            .return_code = CS_UNKNOWN
        };
    }

    queue_free(q_list);
    queue_free(q_deque);
    return (test_res){
        .test_name = (char*) __func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_queue_init
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}