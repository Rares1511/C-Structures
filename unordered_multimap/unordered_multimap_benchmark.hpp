#include <benchmark.hpp>

#include <map>

static void *test_unordered_multimap_insert(void *arg) {
    std::multimap<int, int> *mm = static_cast<std::multimap<int, int> *>(arg);
    int total = __MULTIMAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        mm->insert({i, i});
    }
    return mm;
}

static void *test_unordered_multimap_find(void *arg) {
    std::multimap<int, int> *mm = static_cast<std::multimap<int, int> *>(arg);
    int total = __MULTIMAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        mm->find(i);
    }
    return mm;
}

static void *test_unordered_multimap_erase(void *arg) {
    std::multimap<int, int> *mm = static_cast<std::multimap<int, int> *>(arg);
    int total = __MULTIMAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        mm->erase(i);
    }
    return mm;
}

static BenchmarkModule unordered_multimap_benchmark() {
    static std::multimap<int, int> mm;
    return BenchmarkModule{
        "unordered_multimap",
        &mm,
        {
            Test("unordered_multimap", "insert", test_unordered_multimap_insert),
            Test("unordered_multimap", "find",   test_unordered_multimap_find),
            Test("unordered_multimap", "delete", test_unordered_multimap_erase),
        }
    };
}