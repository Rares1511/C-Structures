#include <benchmark.hpp>

#include <flat_set>

static void *test_flat_set_insert(void *arg) {
    std::flat_set<int> *s = static_cast<std::flat_set<int>*>(arg);
    const int total = __FLAT_SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->insert(i);
    }
    return s;
}

static void *test_flat_set_find(void *arg) {
    std::flat_set<int> *s = static_cast<std::flat_set<int>*>(arg);
    const int total = __FLAT_SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->find(i);
    }
    return s;
}

static void *test_flat_set_delete(void *arg) {
    std::flat_set<int> *s = static_cast<std::flat_set<int>*>(arg);
    const int total = __FLAT_SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->erase(i);
    }
    return s;
}

static BenchmarkModule flat_set_benchmark() {
    static std::flat_set<int> s;
    return BenchmarkModule{
        "flat_set",
        &s,
        {
            Test("flat_set", "insert", test_flat_set_insert),
            Test("flat_set", "find",   test_flat_set_find),
            Test("flat_set", "delete", test_flat_set_delete),
        }
    };
}