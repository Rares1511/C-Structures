#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "../include/map.h"
#include "../include/unittest.h"

test_res test_map_init() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL};

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_init", .reason = "map_init failed", .return_code = rc};
    }

    map_free(&m);
    if (m.root != NULL) {
        return (test_res){.test_name = "test_map_init",
                          .reason = "map_free failed",
                          .return_code = CS_UNKNOWN};
    }

    return (test_res){.test_name = "test_map_init", .reason = "none", .return_code = CS_SUCCESS};
};

test_res test_map_insert() {
    map m;
    map_attr_t key_attr = {sizeof(int), NULL, print_int, NULL, NULL};
    map_attr_t val_attr = {sizeof(int), NULL, print_int, NULL, NULL};
    int keys[] = {5, 3, 7, 2, 4, 6, 8};
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    cs_codes rc = map_init(&m, key_attr, val_attr);
    if (rc != CS_SUCCESS) {
        return (test_res){
            .test_name = "test_map_insert", .reason = "map_init failed", .return_code = rc};
    }

    for (int i = 0; i < 2; i++) {
        rc = map_insert(&m, &keys[i], &values[i]);
        if (rc != CS_SUCCESS) {
            return (test_res){
                .test_name = "test_map_insert", .reason = "map_insert failed", .return_code = rc};
        }
    }

    map_print(&m);

    map_free(&m);
    return (test_res){.test_name = "test_map_insert", .reason = "none", .return_code = CS_SUCCESS};
}

int main() {
    test tests[] = {
        test_map_init,
        test_map_insert,
    };

    unittest(tests, sizeof(tests) / sizeof(test));
    return 0;
}