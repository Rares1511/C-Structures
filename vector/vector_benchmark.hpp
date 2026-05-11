#pragma once

#include <vector>
#include <algorithm>
#include <benchmark.hpp>

static void *test_vector_insert(void *arg) {
    auto *v = static_cast<std::vector<int>*>(arg);
    const int total = __VECTOR_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        v->push_back(i);
    }
    return v;
}

static void *test_vector_find(void *arg) {
    auto *v = static_cast<std::vector<int>*>(arg);
    int search_value = __VECTOR_STRESS_TEST_SIZE - 1;
    int idx = std::find(v->begin(), v->end(), search_value) - v->begin();
    if (idx != __VECTOR_STRESS_TEST_SIZE - 1) {
        printf("Error: Expected to find %d at index %d but found at index %d\n", search_value, __VECTOR_STRESS_TEST_SIZE - 1, idx);
    }
    return v;
}

static void *test_vector_sort(void *arg) {
    auto *v = static_cast<std::vector<int>*>(arg);
    std::sort(v->begin(), v->end());
    return v;
}

static void *test_vector_delete(void *arg) {
    auto *v = static_cast<std::vector<int>*>(arg);
    const int total = __VECTOR_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        v->pop_back();
    }
    return v;
}

static BenchmarkModule vector_benchmark() {
    static std::vector<int> v;
    return BenchmarkModule{
        "vector",
        &v,
        {
            Test("vector", "insert", test_vector_insert),
            Test("vector", "find",   test_vector_find),
            Test("vector", "sort",   test_vector_sort),
            Test("vector", "delete", test_vector_delete),
        }
    };
}
