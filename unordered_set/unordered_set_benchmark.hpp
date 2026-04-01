#include <benchmark.hpp>

#include <unordered_set>

static void *test_unordered_set_insert(void *arg) {
    std::unordered_set<int> *s = static_cast<std::unordered_set<int>*>(arg);
    const int total = __UNORDERED_SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->insert(i);
    }
    return s;
}

static void *test_unordered_set_find(void *arg) {
    std::unordered_set<int> *s = static_cast<std::unordered_set<int>*>(arg);
    for (int i = 0; i < __UNORDERED_SET_STRESS_TEST_SIZE; i++) {
        s->find(i);
    }
    return s;
}

static void *test_unordered_set_delete(void *arg) {
    std::unordered_set<int> *s = static_cast<std::unordered_set<int>*>(arg);
    const int total = __UNORDERED_SET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->erase(i);
    }
    return s;
}

static BenchmarkModule unordered_set_benchmark() {
    static std::unordered_set<int> s;
    return BenchmarkModule{
        "unordered_set",
        &s,
        {
            Test("unordered_set", "insert", test_unordered_set_insert),
            Test("unordered_set", "find",   test_unordered_set_find),
            Test("unordered_set", "delete", test_unordered_set_delete),
        }
    };
}