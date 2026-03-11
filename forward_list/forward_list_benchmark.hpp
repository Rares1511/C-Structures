#pragma once

#include <forward_list>

#include <benchmark.hpp>

static void *test_forward_list_insert(void *arg) {
    auto *l = static_cast<std::forward_list<int>*>(arg);
    const int total = __FORWARD_LIST_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        l->push_front(i);
    }
    return l;
}

static void *test_forward_list_find(void *arg) {
    auto *l = static_cast<std::forward_list<int>*>(arg);
    int value_to_find = __FORWARD_LIST_STRESS_TEST_SIZE / 2; // Find the middle value
    std::find(l->begin(), l->end(), value_to_find);
    return l;
}

static void *test_forward_list_delete(void *arg) {
    auto *l = static_cast<std::forward_list<int>*>(arg);
    const int total = __FORWARD_LIST_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        l->pop_front();
    }
    return l;
}

static BenchmarkModule forward_list_benchmark() {
    static std::forward_list<int> l;
    return BenchmarkModule{
        "forward_list",
        &l,
        {
            Test("forward_list", "insert", test_forward_list_insert),
            Test("forward_list", "find", test_forward_list_find),
            Test("forward_list", "delete", test_forward_list_delete),
        }
    };
}