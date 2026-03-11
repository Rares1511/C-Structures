#pragma once

#include <deque>
#include <algorithm>
#include <benchmark.hpp>

static void *test_deque_insert(void *arg) {
    auto *d = static_cast<std::deque<int>*>(arg);
    const int total = __DEQUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        d->push_back(i);
    }
    return d;
}

static void *test_deque_delete(void *arg) {
    auto *d = static_cast<std::deque<int>*>(arg);
    const int total = __DEQUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        d->pop_back();
    }
    return d;
}

static BenchmarkModule deque_benchmark() {
    static std::deque<int> d;
    return BenchmarkModule{
        "deque",
        &d,
        {
            Test("deque", "insert", test_deque_insert),
            Test("deque", "delete", test_deque_delete),
        }
    };
}