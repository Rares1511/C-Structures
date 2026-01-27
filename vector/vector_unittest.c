#include <cs/vector.h>
#include "../include/unittest.h"

// Required by unittest.h
FILE *__DEBUG_OUT = NULL;

// Helper to get a standard int attribute
static inline vector_attr_t get_int_attr() {
    return (vector_attr_t){
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .comp = comp_int_max // standard ascending
    };
}

// 1. Test Initialization
test_res test_vector_init() {
    vector_attr_t attr = get_int_attr();
    vector *vec = vector_init(attr);
    
    if (!vec) return (test_res){(char*)__func__, "Init returned NULL", CS_MEM};
    if (vector_size(*vec) != 0) return (test_res){(char*)__func__, "Size not 0", CS_UNKNOWN};
    
    vector_free(vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 2. Test Push Back and Growth (triggers realloc)
test_res test_vector_push_growth() {
    vector *vec = vector_init(get_int_attr());
    int total = __TEST_SIZE; // Use your defined size

    for (int i = 0; i < total; i++) {
        if (vector_push_back(vec, &i) != CS_SUCCESS) {
            vector_free(vec);
            return (test_res){(char*)__func__, "Push failed", CS_MEM};
        }
    }

    if (vector_size(*vec) != total) {
        vector_free(vec);
        return (test_res){(char*)__func__, "Size mismatch after growth", CS_UNKNOWN};
    }

    // Verify last element
    int last = *(int*)vector_at(*vec, total - 1);
    if (last != total - 1) {
        vector_free(vec);
        return (test_res){(char*)__func__, "Data corruption after growth", CS_ELEM};
    }

    vector_free(vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 3. Test Insert At (Middle, Front, Back)
test_res test_vector_insert_pos() {
    vector *vec = vector_init(get_int_attr());
    int vals[] = {10, 20, 30};
    
    vector_push_back(vec, &vals[0]); // [10]
    vector_push_back(vec, &vals[2]); // [10, 30]
    
    int middle = 15;
    vector_insert_at(vec, &middle, 1); // [10, 15, 30]
    
    if (*(int*)vector_at(*vec, 1) != 15) {
        vector_free(vec);
        return (test_res){(char*)__func__, "Middle insert failed", CS_POS};
    }

    int front = 5;
    vector_insert_at(vec, &front, 0); // [5, 10, 15, 30]
    if (*(int*)vector_at(*vec, 0) != 5) {
        vector_free(vec);
        return (test_res){(char*)__func__, "Front insert failed", CS_POS};
    }

    vector_free(vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 4. Test Erase and Pop
test_res test_vector_remove() {
    vector *vec = vector_init(get_int_attr());
    for (int i = 0; i < 5; i++) vector_push_back(vec, &i); // [0, 1, 2, 3, 4]

    vector_erase(vec, 2); // Remove '2' -> [0, 1, 3, 4]
    if (vector_size(*vec) != 4 || *(int*)vector_at(*vec, 2) != 3) {
        vector_free(vec);
        return (test_res){(char*)__func__, "Erase failed", CS_POS};
    }

    vector_pop_back(vec); // Remove '4' -> [0, 1, 3]
    if (vector_size(*vec) != 3) {
        vector_free(vec);
        return (test_res){(char*)__func__, "Pop failed", CS_POS};
    }

    vector_free(vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 5. Test Sort and Find
test_res test_vector_logic() {
    vector *vec = vector_init(get_int_attr());
    int data[] = {50, 10, 40, 20, 30};
    for (int i = 0; i < 5; i++) vector_push_back(vec, &data[i]);

    vector_sort(vec); // Should be [10, 20, 30, 40, 50]

    int target = 30;
    int idx = vector_find(*vec, &target);
    if (idx != 2) {
        vector_free(vec);
        return (test_res){(char*)__func__, "Find/Sort failed", CS_ELEM};
    }

    vector_free(vec);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 6. Test Swap
test_res test_vector_swap() {
    vector *v1 = vector_init(get_int_attr());
    vector *v2 = vector_init(get_int_attr());
    int a = 1, b = 2;
    
    vector_push_back(v1, &a);
    vector_push_back(v2, &b);

    vector_swap(v1, v2);

    if (*(int*)vector_at(*v1, 0) != 2 || *(int*)vector_at(*v2, 0) != 1) {
        vector_free(v1); vector_free(v2);
        return (test_res){(char*)__func__, "Swap failed", CS_UNKNOWN};
    }

    vector_free(v1);
    vector_free(v2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// MAIN RUNNER
int main(int argc, char **argv) {
    test tests[] = {
        test_vector_init,
        test_vector_push_growth,
        test_vector_insert_pos,
        test_vector_remove,
        test_vector_logic,
        test_vector_swap
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}