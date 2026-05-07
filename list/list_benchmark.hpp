#pragma once

#include <list>

#include <benchmark.hpp>

static void *test_list_insert_back(void *arg) {
    auto *l = static_cast<std::list<int>*>(arg);
    const int total = __LIST_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        l->push_back(i);
    }
    return l;
}

static void *test_list_find(void *arg) {
    auto *l = static_cast<std::list<int>*>(arg);
    int value_to_find = __LIST_STRESS_TEST_SIZE / 2; // Find the middle value
    int result = std::find(l->begin(), l->end(), value_to_find) != l->end() ? 1 : 0;
    if (result == 0) {
        printf("Should not happen: value %d not found in list\n", value_to_find);
    }
    return l;
}

static void *test_list_insert_front(void *arg) {
    auto *l = static_cast<std::list<int>*>(arg);
    const int total = __LIST_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        l->push_front(i);
    }
    return l;
}

static void *test_list_delete_front(void *arg) {
    auto *l = static_cast<std::list<int>*>(arg);
    const int total = __LIST_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        l->pop_front();
    }
    return l;
}

static void *test_list_delete_back(void *arg) {
    auto *l = static_cast<std::list<int>*>(arg);
    const int total = __LIST_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        l->pop_back();
    }
    return l;
}

static void *test_list_sort(void *arg) {
    auto *l = static_cast<std::list<int>*>(arg);
    l->sort();
    return l;
}

static BenchmarkModule list_benchmark() {
    static std::list<int> l;
    return BenchmarkModule{
        "list",
        &l,
        {
            Test("list", "insert_back", test_list_insert_back),
            Test("list", "find", test_list_find),
            Test("list", "delete_back", test_list_delete_back),
            Test("list", "insert_front", test_list_insert_front),
            Test("list", "sort", test_list_sort),
            Test("list", "delete_front", test_list_delete_front),
        }
    };
}