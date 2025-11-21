#include <cs/heap.h>

#include "../include/unittest.h"

#define TEST_SIZE 10
#define VALUE_SIZE 10000

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
            .test_name = (char *)__func__,
            .reason = "heap_init returned error code",
            .return_code = rc
        };
    }

    if (h.size != 0 || h.cap != INIT_CAPACITY || h.vec == NULL) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "heap not initialized correctly",
            .return_code = CS_UNKNOWN
        };
    }

    heap_free(&h);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_heap_push() {
    heap h;
    heap_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_min,
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .stream = DEBUG_OUT
    };
    int value, min_value;

    int rc = heap_init(&h, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "heap_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % VALUE_SIZE;
        rc = heap_push(&h, &value);
        if (rc != CS_SUCCESS) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_push returned error code",
                .return_code = rc
            };
        }

        if (h.size == 1 || value < min_value) {
            min_value = value;
        }

        int *top = (int *)heap_top(h);
        if (top == NULL) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_top returned NULL",
                .return_code = CS_UNKNOWN
            };
        }

        if (*top != min_value) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap property violated after heap_push",
                .return_code = CS_UNKNOWN
            };
        }
    }

    heap_free(&h);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_heap_pop() {
    heap h;
    heap_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_min,
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .stream = DEBUG_OUT
    };
    int value, prev_value = -1;

    int rc = heap_init(&h, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "heap_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % VALUE_SIZE;
        rc = heap_push(&h, &value);
        if (rc != CS_SUCCESS) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_push returned error code",
                .return_code = rc
            };
        }
    }

    while (h.size > 0) {
        int *top = (int *)heap_top(h);
        if (top == NULL) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_top returned NULL",
                .return_code = CS_UNKNOWN
            };
        }

        if (prev_value != -1 && *top < prev_value) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap property violated after heap_pop",
                .return_code = CS_UNKNOWN
            };
        }

        prev_value = *top;
        heap_pop(&h);
    }

    heap_free(&h);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_heap_swap() {
    heap h1, h2;
    heap_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_min,
        .copy = NULL,
        .fr = NULL,
        .print = NULL,
        .stream = DEBUG_OUT
    };
    int fr1[VALUE_SIZE], fr2[VALUE_SIZE];
    for (int i = 0; i < VALUE_SIZE; i++) {
        fr1[i] = 0;
        fr2[i] = 0;
    }

    int rc = heap_init(&h1, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "heap_init for h1 returned error code",
            .return_code = rc
        };
    }

    rc = heap_init(&h2, attr);
    if (rc != CS_SUCCESS) {
        heap_free(&h1); 
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "heap_init for h2 returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        int value1 = rand() % VALUE_SIZE;
        int value2 = rand() % VALUE_SIZE;
        rc = heap_push(&h1, &value1);
        if (rc != CS_SUCCESS) {
            heap_free(&h1);
            heap_free(&h2);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_push for h1 returned error code",
                .return_code = rc
            };
        }
        rc = heap_push(&h2, &value2);
        if (rc != CS_SUCCESS) {
            heap_free(&h1);
            heap_free(&h2);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_push for h2 returned error code",
                .return_code = rc
            };
        }
        fr1[value1]++;
        fr2[value2]++;
    }

    heap_swap(&h1, &h2);

    if (h1.size != TEST_SIZE || h2.size != TEST_SIZE) {
        heap_free(&h1);
        heap_free(&h2);
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "heap_swap did not swap sizes correctly",
            .return_code = CS_UNKNOWN
        };
    }

    while (h1.size > 0) {
        int *top = (int *)heap_top(h1);
        fr2[*top]--;
        heap_pop(&h1);
    }

    while (h2.size > 0) {
        int *top = (int *)heap_top(h2);
        fr1[*top]--;
        heap_pop(&h2);
    }

    for (int i = 0; i < VALUE_SIZE; i++) {
        if (fr1[i] != 0 || fr2[i] != 0) {
            heap_free(&h1);
            heap_free(&h2);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_swap did not swap elements correctly",
                .return_code = CS_UNKNOWN
            };
        }
    }

    heap_free(&h1);
    heap_free(&h2);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

test_res test_heap_set_comp() {
    heap h;
    heap_attr_t attr = {
        .size = sizeof(int),
        .comp = comp_int_min,
        .copy = NULL,
        .fr = NULL,
        .print = print_int,
        .stream = DEBUG_OUT
    };
    int value;

    int rc = heap_init(&h, attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = (char *)__func__,
            .reason = "heap_init returned error code",
            .return_code = rc
        };
    }

    for (int i = 0; i < TEST_SIZE; i++) {
        value = rand() % VALUE_SIZE;
        rc = heap_push(&h, &value);
        if (rc != CS_SUCCESS) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_push returned error code",
                .return_code = rc
            };
        }
    }

    heap_set_comp(&h, comp_int_max);

    int prev_value = -1;
    while (h.size > 0) {
        int *top = (int *)heap_top(h);
        if (top == NULL) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap_top returned NULL",
                .return_code = CS_UNKNOWN
            };
        }

        if (prev_value != -1 && *top > prev_value) {
            heap_free(&h);
            return (test_res){
                .test_name = (char *)__func__,
                .reason = "heap property violated after heap_set_comp",
                .return_code = CS_UNKNOWN
            };
        }
        prev_value = *top;
        heap_pop(&h);
    }

    heap_free(&h);

    return (test_res){
        .test_name = (char *)__func__,
        .reason = NULL,
        .return_code = CS_SUCCESS
    };
}

int main(int argc, char **argv) {
    test tests[] = {
        test_heap_init,
        test_heap_push,
        test_heap_pop,
        test_heap_swap,
        test_heap_set_comp,
    };
    
    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}