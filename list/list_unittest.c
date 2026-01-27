#include <cs/list.h>
#include "../include/unittest.h"

// Required by unittest.h
FILE *__DEBUG_OUT = NULL;

// Helper to get standard int attributes
static inline list_attr_t get_int_attr() {
    return (list_attr_t){
        .size = sizeof(int),
        .fr = NULL,
        .copy = NULL,
        .print = print_int,
        .comp = comp_int_max
    };
}

// 1. Test Initialization
test_res test_list_init() {
    list_attr_t attr = get_int_attr();
    list *l = list_init(attr);

    if (!l) return (test_res){(char*)__func__, "Init returned NULL", CS_MEM};
    if (l->front != NULL || list_size(*l) != 0) {
        list_free(l);
        return (test_res){(char*)__func__, "Initial state invalid", CS_UNKNOWN};
    }

    list_free(l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 2. Test Circularity (Front and Back links)
test_res test_list_circularity() {
    list *l = list_init(get_int_attr());
    int a = 10, b = 20, c = 30;

    list_push_back(l, &a); // [10]
    list_push_back(l, &b); // [10, 20]
    list_push_front(l, &c); // [30, 10, 20]

    // Check circular links
    // front->prev should be 20, front->prev->next should be 30
    if (*(int*)list_front(*l) != 30 || *(int*)list_back(*l) != 20) {
        list_free(l);
        return (test_res){(char*)__func__, "Front/Back data mismatch", CS_ELEM};
    }

    if (l->front->prev->next != l->front || l->front->next->prev != l->front) {
        list_free(l);
        return (test_res){(char*)__func__, "Circular pointers broken", CS_UNKNOWN};
    }

    list_free(l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 3. Test Sequential Popping (Testing transitions to empty)
test_res test_list_pop_behavior() {
    list *l = list_init(get_int_attr());
    int vals[] = {1, 2, 3};
    for(int i=0; i<3; i++) list_push_back(l, &vals[i]);

    list_pop_front(l); // [2, 3]
    if (*(int*)list_front(*l) != 2 || list_size(*l) != 2) {
        list_free(l);
        return (test_res){(char*)__func__, "Pop front failed", CS_UNKNOWN};
    }

    list_pop_back(l); // [2]
    if (*(int*)list_back(*l) != 2 || list_size(*l) != 1) {
        list_free(l);
        return (test_res){(char*)__func__, "Pop back failed", CS_UNKNOWN};
    }

    list_pop_front(l); // Empty
    if (!list_empty(*l) || l->front != NULL) {
        list_free(l);
        return (test_res){(char*)__func__, "Final pop didn't NULL front", CS_UNKNOWN};
    }

    list_free(l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 4. Test Sort (Quicksort on linked nodes)
test_res test_list_sort() {
    list *l = list_init(get_int_attr());
    int data[] = {5, 1, 4, 2, 3};
    for(int i=0; i<5; i++) list_push_back(l, &data[i]);

    list_sort(l); // Should be [1, 2, 3, 4, 5]

    list_node *curr = l->front;
    for(int i=1; i<=5; i++) {
        if (*(int*)curr->data != i) {
            list_free(l);
            return (test_res){(char*)__func__, "Sort order incorrect", CS_ELEM};
        }
        curr = curr->next;
    }

    list_free(l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 5. Test Erase at Position
test_res test_list_erase() {
    list *l = list_init(get_int_attr());
    int vals[] = {100, 200, 300, 400};
    for(int i=0; i<4; i++) list_push_back(l, &vals[i]);

    list_erase(l, 2); // Erase 300 -> [100, 200, 400]
    
    if (list_size(*l) != 3 || *(int*)l->front->next->next->data != 400) {
        list_free(l);
        return (test_res){(char*)__func__, "Middle erase failed", CS_POS};
    }

    list_erase(l, 0); // Erase 100 -> [200, 400]
    if (*(int*)list_front(*l) != 200) {
        list_free(l);
        return (test_res){(char*)__func__, "Front erase failed", CS_POS};
    }

    list_free(l);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// 6. Test Swap and Clear
test_res test_list_utility() {
    list *l1 = list_init(get_int_attr());
    list *l2 = list_init(get_int_attr());
    int val1 = 1, val2 = 2;

    list_push_back(l1, &val1);
    list_push_back(l2, &val2);

    list_swap(l1, l2);
    if (*(int*)list_front(*l1) != 2) {
        list_free(l1); list_free(l2);
        return (test_res){(char*)__func__, "Swap failed", CS_UNKNOWN};
    }

    list_clear(l1);
    if (!list_empty(*l1)) {
        list_free(l1); list_free(l2);
        return (test_res){(char*)__func__, "Clear failed", CS_UNKNOWN};
    }

    list_free(l1);
    list_free(l2);
    return (test_res){(char*)__func__, NULL, CS_SUCCESS};
}

// MAIN RUNNER
int main(int argc, char **argv) {
    test tests[] = {
        test_list_init,
        test_list_circularity,
        test_list_pop_behavior,
        test_list_sort,
        test_list_erase,
        test_list_utility
    };

    unittest(tests, sizeof(tests) / sizeof(test), argc, argv);
    return 0;
}