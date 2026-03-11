#pragma once

#include <map>
#include <string>
#include <benchmark.hpp>

static void *test_map_insert(void *arg) {
    auto *m = static_cast<std::map<int, std::string>*>(arg);
    const int total = __MAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        m->emplace(i, "StressVal_" + std::to_string(i));
    }
    return m;
}

static void *test_map_find(void *arg) {
    auto *m = static_cast<std::map<int, std::string>*>(arg);
    const int total = __MAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        m->find(i);
    }
    return m;
}

static void *test_map_delete(void *arg) {
    auto *m = static_cast<std::map<int, std::string>*>(arg);
    const int total = __MAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        m->erase(i);
    }
    return m;
}

static BenchmarkModule map_benchmark() {
    static std::map<int, std::string> m;
    return BenchmarkModule{
        "map",
        &m,
        {
            Test("map", "insert", test_map_insert),
            Test("map", "find",   test_map_find),
            Test("map", "delete", test_map_delete),
        }
    };
}
