#include <benchmark.hpp>

// Associative containers (Arrays)
#include <vector/vector_benchmark.hpp>
#include <deque/deque_benchmark.hpp>
#include <list/list_benchmark.hpp>
#include <forward_list/forward_list_benchmark.hpp>

// Associative containers (RBT)
#include <set/set_benchmark.hpp>
#include <map/map_benchmark.hpp>

int main() {
    TestRunner runner;
    runner.add_module(vector_benchmark());
    runner.add_module(deque_benchmark());
    runner.add_module(list_benchmark());
    runner.add_module(forward_list_benchmark());
    runner.add_module(set_benchmark());
    runner.add_module(map_benchmark());
    return runner.run();
}