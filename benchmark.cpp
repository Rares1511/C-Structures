#include <benchmark.hpp>

// Associative containers (Arrays)
#include <vector/vector_benchmark.hpp>
#include <deque/deque_benchmark.hpp>
#include <list/list_benchmark.hpp>
#include <forward_list/forward_list_benchmark.hpp>

// Associative containers (RBT)
#include <set/set_benchmark.hpp>
#include <map/map_benchmark.hpp>
#include <multiset/multiset_benchmark.hpp>
#include <multimap/multimap_benchmark.hpp>

// Unordered associative containers (Hash Table)
#include <unordered_set/unordered_set_benchmark.hpp>
#include <unordered_map/unordered_map_benchmark.hpp>
#include <unordered_multiset/unordered_multiset_benchmark.hpp>
#include <unordered_multimap/unordered_multimap_benchmark.hpp>

// Container adapters
#include <stack/stack_benchmark.hpp>
#include <queue/queue_benchmark.hpp>
#include <priority_queue/priority_queue_benchmark.hpp>
// #include <flat_set/flat_set_benchmark.hpp> // Not supported in C++17, only in C++23

int main() {
    // Run benchmarks and write results to CSV file only if CSV file doesn't exist
    if (FILE *file = fopen("logs/benchmark.csv", "r")) {
        printf("Benchmark CSV file already exists. Skipping benchmark run.\n");
        fclose(file);
        return 0;
    }
    TestRunner runner;
    runner.add_module(vector_benchmark());
    runner.add_module(deque_benchmark());
    runner.add_module(list_benchmark());
    runner.add_module(forward_list_benchmark());
    runner.add_module(set_benchmark());
    runner.add_module(map_benchmark());
    runner.add_module(multiset_benchmark());
    runner.add_module(multimap_benchmark());
    runner.add_module(unordered_set_benchmark());
    runner.add_module(unordered_map_benchmark());
    runner.add_module(unordered_multiset_benchmark());
    runner.add_module(unordered_multimap_benchmark());
    runner.add_module(stack_benchmark());
    runner.add_module(queue_benchmark());
    runner.add_module(priority_queue_benchmark());
    // runner.add_module(flat_set_benchmark());
    return runner.run("logs/benchmark.csv");
}