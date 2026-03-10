#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>

#include <benchmark.h>

int main() {
    using Clock = std::chrono::steady_clock;
    using Seconds = std::chrono::duration<double>;

    std::vector<int> v;
    const int total = __VECTOR_STRESS_TEST_SIZE;

    double elapsed_time = 0.0;

    std::cout << std::fixed << std::setprecision(9);

    // INSERT benchmark (push_back)
    auto start = Clock::now();
    for (int i = 0; i < total; i++) {
        v.push_back(i);
    }
    auto end = Clock::now();
    elapsed_time = Seconds(end - start).count();

    std::cout << "vector,insert," << elapsed_time << "\n";

    // FIND benchmark (std::find)
    start = Clock::now();
    int search_value = total - 1;
    auto it = std::find(v.begin(), v.end(), search_value);
    end = Clock::now();
    elapsed_time = Seconds(end - start).count();
    std::cout << "vector,find," << elapsed_time << "\n";

    // DELETE benchmark (pop_back)
    start = Clock::now();
    for (int i = 0; i < total; i++) {
        v.pop_back();
    }
    end = Clock::now();
    elapsed_time = Seconds(end - start).count();

    std::cout << "vector,delete," << elapsed_time << "\n";

    return 0;
}