#include <benchmark.hpp>

#include <unordered_set>

static void *test_unordered_multiset_insert(void *arg) {
    auto *s = static_cast<std::unordered_multiset<int>*>(arg);
    const int total = __UNORDERED_MULTISET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->insert(i);
    }
    return s;
}

static void *test_unordered_multiset_find(void *arg) {
    auto *s = static_cast<std::unordered_multiset<int>*>(arg);
    const int total = __UNORDERED_MULTISET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->find(i);
    }
    return s;
}

static void *test_unordered_multiset_delete(void *arg) {
    auto *s = static_cast<std::unordered_multiset<int>*>(arg);
    const int total = __UNORDERED_MULTISET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->erase(i);
    }
    return s;
}

static BenchmarkModule unordered_multiset_benchmark() {
    static std::unordered_multiset<int> s;
    return BenchmarkModule{
        "unordered_multiset",
        &s,
        {
            Test("unordered_multiset", "insert", test_unordered_multiset_insert),
            Test("unordered_multiset", "find",   test_unordered_multiset_find),
            Test("unordered_multiset", "delete", test_unordered_multiset_delete),
        }
    };
}