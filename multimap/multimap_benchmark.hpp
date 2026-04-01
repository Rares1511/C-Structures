#include <benchmark.hpp>

#include <map>

static void *test_multimap_insert(void *arg) {
    std::multimap<int, std::string> *mm = static_cast<std::multimap<int, std::string> *>(arg);
    std::string value;
    int total = __MULTIMAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        value = "StressValue_" + std::to_string(i);
        mm->insert({i, value});
    }
    return mm;
}

static void *test_multimap_find(void *arg) {
    std::multimap<int, std::string> *mm = static_cast<std::multimap<int, std::string> *>(arg);
    int total = __MULTIMAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        mm->find(i);
    }
    return mm;
}

static void *test_multimap_erase(void *arg) {
    std::multimap<int, std::string> *mm = static_cast<std::multimap<int, std::string> *>(arg);
    int total = __MULTIMAP_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        mm->erase(i);
    }
    return mm;
}

static BenchmarkModule multimap_benchmark() {
    static std::multimap<int, std::string> mm;
        return BenchmarkModule{
        "multimap",
        &mm,
        {
            Test("multimap", "insert", test_multimap_insert),
            Test("multimap", "find", test_multimap_find),
            Test("multimap", "erase", test_multimap_erase),
        }
    };
};