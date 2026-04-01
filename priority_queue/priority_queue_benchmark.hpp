#include <benchmark.hpp>

#include <queue>

static void *test_priority_queue_insert(void *arg) {
    std::priority_queue<int> *pq = static_cast<std::priority_queue<int>*>(arg);
    const int total = __PRIORITY_QUEUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        pq->push(i);
    }
    return pq;
}

static void *test_priority_queue_delete(void *arg) {
    std::priority_queue<int> *pq = static_cast<std::priority_queue<int>*>(arg);
    const int total = __PRIORITY_QUEUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        pq->pop();
    }
    return pq;
}

static BenchmarkModule priority_queue_benchmark() {
    static std::priority_queue<int> pq;
    return BenchmarkModule{
        "priority_queue",
        &pq,
        {
            Test("priority_queue", "insert", test_priority_queue_insert),
            Test("priority_queue", "delete", test_priority_queue_delete),
        }
    };
}