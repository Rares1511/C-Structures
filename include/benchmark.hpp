#ifndef __CS_BENCHMARK_HPP__
#define __CS_BENCHMARK_HPP__

#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <benchmark.h>

class Test {
public:
    std::string module;
    std::string name;
    std::function<void*(void*)> func;
    void *result;

    Test(const std::string &module, const std::string &name, std::function<void*(void*)> func)
        : module(module), name(name), func(func), result(nullptr) {}

    double run(void *arg) {
        using Clock = std::chrono::steady_clock;
        using Seconds = std::chrono::duration<double>;
        auto start = Clock::now();
        result = func(arg);
        auto end = Clock::now();
        return Seconds(end - start).count();
    }
};

struct BenchmarkModule {
    std::string name;
    void *init_arg;
    std::vector<Test> tests;
};

class TestRunner {
public:
    std::vector<BenchmarkModule> modules;

    void add_module(BenchmarkModule mod) {
        modules.push_back(std::move(mod));
    }

    int run(const std::string &output_file) {
        std::ofstream ofs(output_file);
        if (!ofs.is_open()) {
            std::cerr << "Failed to open benchmark output file: " << output_file << "\n";
            return 1;
        }
        ofs << std::fixed << std::setprecision(9);
        ofs << "module,operation,value\n";

        for (auto &mod : modules) {
            void *arg = mod.init_arg;
            for (auto &test : mod.tests) {
                double elapsed = test.run(arg);
                ofs << test.module << "," << test.name << "," << elapsed << "\n";
                arg = test.result;
            }
        }

        ofs.close();
        return 0;
    }
};

#endif // __CS_BENCHMARK_HPP__