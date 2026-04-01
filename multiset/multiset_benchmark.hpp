#include <benchmark.hpp>

#include <set>

static void *test_multiset_insert(void *arg) {
    std::multiset<int> *ms = static_cast<std::multiset<int> *>(arg);
    int total = __MULTISET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        ms->insert(i);
    }
    return ms;
}

static void *test_multiset_find(void *arg) {
    std::multiset<int> *ms = static_cast<std::multiset<int> *>(arg);
    int total = __MULTISET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        ms->find(i);
    }
    return ms;
}

static void *test_multiset_erase(void *arg) {
    std::multiset<int> *ms = static_cast<std::multiset<int> *>(arg);
    int total = __MULTISET_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        ms->erase(i);
    }
    return ms;
}

static BenchmarkModule multiset_benchmark() {
    static std::multiset<int> ms;
    return BenchmarkModule{
        "multiset",
        &ms,
        {
            Test("multiset", "insert", test_multiset_insert),
            Test("multiset", "find", test_multiset_find),
            Test("multiset", "erase", test_multiset_erase),
        }
    };
};