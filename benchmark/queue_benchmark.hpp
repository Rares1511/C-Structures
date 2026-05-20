#include <benchmark.hpp>

#include <queue>

static void *test_queue_push(void *arg) {
    std::queue<int> *q = static_cast<std::queue<int>*>(arg);
    const int total = __QUEUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        q->push(i);
    }
    return q;
}

static void *test_queue_pop(void *arg) {
    std::queue<int> *q = static_cast<std::queue<int>*>(arg);
    const int total = __QUEUE_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        q->pop();
    }
    return q;
}

static BenchmarkModule queue_benchmark() {
    static std::queue<int> q;
    return BenchmarkModule{
        "queue",
        &q,
        {
            Test("queue", "push", test_queue_push),
            Test("queue", "pop",  test_queue_pop),
        }
    };
}