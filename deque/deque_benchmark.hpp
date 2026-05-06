#pragma once

#include <deque>
#include <algorithm>
#include <benchmark.hpp>

static void *test_deque_insert_back(void *arg) {
    auto *d = static_cast<std::deque<int>*>(arg);
    const int total = __DEQUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        d->push_back(i);
    }
    return d;
}

static void *test_deque_delete_back(void *arg) {
    auto *d = static_cast<std::deque<int>*>(arg);
    const int total = __DEQUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        d->pop_back();
    }
    return d;
}

static void *test_deque_insert_front(void *arg) {
    auto *d = static_cast<std::deque<int>*>(arg);
    const int total = __DEQUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        d->push_front(i);
    }
    return d;
}

static void *test_deque_delete_front(void *arg) {
    auto *d = static_cast<std::deque<int>*>(arg);
    const int total = __DEQUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        d->pop_front();
    }
    return d;
}

static BenchmarkModule deque_benchmark() {
    static std::deque<int> d;
    return BenchmarkModule{
        "deque",
        &d,
        {
            Test("deque", "insert_back", test_deque_insert_back),
            Test("deque", "delete_back", test_deque_delete_back),
            Test("deque", "insert_front", test_deque_insert_front),
            Test("deque", "delete_front", test_deque_delete_front),
        }
    };
}