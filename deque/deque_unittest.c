#include <cs/deque.h>
#include "../include/unittest.h"

// Required by unittest.h
FILE *__DEBUG_OUT = NULL;

// Helper to get standard int attributes
static inline deque_attr_t get_int_attr() {
    return (deque_attr_t){
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .comp = comp_int_max
    };
}

// 1. Test Initialization
test_res test_deque_init() {
    deque_attr_t attr = get_int_attr();
    deque *dq = deque_init(attr);

    if (!dq) return (test_res){(char*)__func__, "Init returned NULL", CS_MEM};
    if (deque_size(*dq) != 0) return (test_res){(char*)__func__, "Initial size not 0", CS_UNKNOWN};

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 2. Test Bidirectional Growth (Pushing past block boundaries)
test_res test_deque_push_both_sides() {
    deque *dq = deque_init(get_int_attr());
    int total = DEQUE_BLOCK_SIZE * 3; // Ensure we fill multiple blocks

    // Push to back: 0, 1, 2...
    for (int i = 0; i < total; i++) {
        deque_push_back(dq, &i);
    }
    // Push to front: -1, -2, -3...
    for (int i = 1; i <= total; i++) {
        int val = -i;
        deque_push_front(dq, &val);
    }

    if (deque_size(*dq) != total * 2) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Size mismatch after bidirectional push", CS_UNKNOWN};
    }

    // Check boundaries
    int front_val = *(int*)deque_front(*dq);
    int back_val = *(int*)deque_back(*dq);

    if (front_val != -total || back_val != total - 1) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Data corruption at deque ends", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 3. Test Random Access (Cross-block indexing)
test_res test_deque_at_indexing() {
    deque *dq = deque_init(get_int_attr());
    int total = 200; // Larger than 2 blocks

    for (int i = 0; i < total; i++) {
        deque_push_back(dq, &i);
    }

    // Test middle access
    int target_idx = 150;
    int *val = (int*)deque_at(*dq, target_idx);
    if (!val || *val != 150) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Random access failed at block transition", CS_POS};
    }

    // Test out of bounds
    if (deque_at(*dq, 500) != NULL) {
        deque_free(dq);
        return (test_res){(char*)__func__, "OOB index did not return NULL", CS_POS};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 4. Test Insertion and Erasure
test_res test_deque_edit_at() {
    deque *dq = deque_init(get_int_attr());
    for (int i = 0; i < 10; i++) deque_push_back(dq, &i); // [0..9]

    int new_val = 99;
    deque_insert_at(dq, &new_val, 5); // Insert in middle
    
    if (*(int*)deque_at(*dq, 5) != 99 || deque_size(*dq) != 11) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Middle insertion failed", CS_POS};
    }

    deque_erase(dq, 5); // Back to [0..9]
    if (*(int*)deque_at(*dq, 5) != 5 || deque_size(*dq) != 10) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Middle erasure failed", CS_POS};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 5. Test Clear and Reuse
test_res test_deque_clear() {
    deque *dq = deque_init(get_int_attr());
    for (int i = 0; i < 50; i++) deque_push_back(dq, &i);

    deque_clear(dq);
    if (deque_size(*dq) != 0 || !deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Clear did not reset size", CS_UNKNOWN};
    }

    // Verify reuse
    int val = 42;
    deque_push_back(dq, &val);
    if (*(int*)deque_front(*dq) != 42) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Reuse after clear failed", CS_ELEM};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 6. Test Popping Logic (Memory freeing of empty blocks)
test_res test_deque_pop_logic() {
    deque *dq = deque_init(get_int_attr());
    int total = DEQUE_BLOCK_SIZE * 2;
    for (int i = 0; i < total; i++) deque_push_back(dq, &i);

    // Pop everything from the front
    for (int i = 0; i < total; i++) {
        if (deque_pop_front(dq) != CS_SUCCESS) {
            deque_free(dq);
            return (test_res){(char*)__func__, "Pop front failed", CS_EMPTY};
        }
    }

    if (!deque_empty(*dq)) {
        deque_free(dq);
        return (test_res){(char*)__func__, "Deque not empty after pops", CS_UNKNOWN};
    }

    deque_free(dq);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// MAIN RUNNER
int main(int argc, char **argv) {
    test tests[] = {
        test_deque_init,
        test_deque_push_both_sides,
        test_deque_at_indexing,
        test_deque_edit_at,
        test_deque_clear,
        test_deque_pop_logic
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}