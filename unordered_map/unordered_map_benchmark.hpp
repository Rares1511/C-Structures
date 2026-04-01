#include <benchmark.hpp>

#include <map>

static void *test_unordered_map_insert(void *arg) {
    auto *m = static_cast<std::unordered_map<int, std::string>*>(arg);
    const int total = __UNORDERED_MAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        m->emplace(i, "StressVal_" + std::to_string(i));
    }
    return m;
}

static void *test_unordered_map_find(void *arg) {
    auto *m = static_cast<std::unordered_map<int, std::string>*>(arg);
    const int total = __UNORDERED_MAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        m->find(i);
    }
    return m;
}

static void *test_unordered_map_delete(void *arg) {
    auto *m = static_cast<std::unordered_map<int, std::string>*>(arg);
    const int total = __UNORDERED_MAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        m->erase(i);
    }
    return m;
}

static BenchmarkModule unordered_map_benchmark() {
    static std::unordered_map<int, std::string> m;
    return BenchmarkModule{
        "unordered_map",
        &m,
        {
            Test("unordered_map", "insert", test_unordered_map_insert),
            Test("unordered_map", "find",   test_unordered_map_find),
            Test("unordered_map", "delete", test_unordered_map_delete),
        }
    };
}