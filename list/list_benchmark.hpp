#pragma once

#include <list>

#include <benchmark.hpp>

static void *test_list_insert(void *arg) {
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
    std::find(l->begin(), l->end(), value_to_find);
    return l;
}

static void *test_list_delete(void *arg) {
    auto *l = static_cast<std::list<int>*>(arg);
    const int total = __LIST_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        l->pop_back();
    }
    return l;
}

static BenchmarkModule list_benchmark() {
    static std::list<int> l;
    return BenchmarkModule{
        "list",
        &l,
        {
            Test("list", "insert", test_list_insert),
            Test("list", "find", test_list_find),
            Test("list", "delete", test_list_delete),
        }
    };
}