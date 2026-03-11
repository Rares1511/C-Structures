#pragma once

#include <set>
#include <benchmark.hpp>

static void *test_set_insert(void *arg) {
    auto *s = static_cast<std::set<int>*>(arg);
    const int total = __SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->insert(i);
    }
    return s;
}

static void *test_set_find(void *arg) {
    auto *s = static_cast<std::set<int>*>(arg);
    const int total = __SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->find(i);
    }
    return s;
}

static void *test_set_delete(void *arg) {
    auto *s = static_cast<std::set<int>*>(arg);
    const int total = __SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->erase(i);
    }
    return s;
}

static BenchmarkModule set_benchmark() {
    static std::set<int> s;
    return BenchmarkModule{
        "set",
        &s,
        {
            Test("set", "insert", test_set_insert),
            Test("set", "find",   test_set_find),
            Test("set", "delete", test_set_delete),
        }
    };
}