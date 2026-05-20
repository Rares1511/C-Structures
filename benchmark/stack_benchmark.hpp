#include <benchmark.hpp>

#include <stack>

static void *test_stack_push(void *arg) {
    std::stack<int> *s = static_cast<std::stack<int>*>(arg);
    const int total = __STACK_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->push(i);
    }
    return s;
}

static void *test_stack_pop(void *arg) {
    std::stack<int> *s = static_cast<std::stack<int>*>(arg);
    const int total = __STACK_STRESS_TEST_SIZE;
    for (int i = 0; i < total; i++) {
        s->pop();
    }
    return s;
}

static BenchmarkModule stack_benchmark() {
    static std::stack<int> s;
    return BenchmarkModule{
        "stack",
        &s,
        {
            Test("stack", "push", test_stack_push),
            Test("stack", "pop",  test_stack_pop),
        }
    };
}