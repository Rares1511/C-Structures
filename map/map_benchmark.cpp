#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <cstdint>

#include <benchmark.h>

struct TestAddress {
    std::string street;
    std::string city;
    int zip_code;
};

struct TestContact {
    std::string email;
    std::string phone;
    int priority;
};

struct TestStruct {
    int id;
    std::string name;
    double score;
    TestAddress address;
    std::vector<TestContact> contacts;
    std::vector<int> tags;
    unsigned char flags;
};

static TestStruct create_test_struct(int i) {
    TestStruct val;
    val.id = i;
    val.name = "StressVal_" + std::to_string(i);
    val.score = static_cast<double>(i * 3);

    val.address.street = "Main Street " + std::to_string(i);
    val.address.city = "Bucharest";
    val.address.zip_code = 10000 + (i % 90000);

    val.contacts.push_back({
        "user" + std::to_string(i) + "@mail.com",
        "0712345678",
        i % 10
    });

    val.contacts.push_back({
        "backup" + std::to_string(i) + "@mail.com",
        "0723456789",
        (i + 1) % 10
    });

    val.tags.push_back(i);
    val.tags.push_back(i * 2);
    val.tags.push_back(i * 3);

    val.flags = static_cast<unsigned char>(i % 256);

    return val;
}

int main() {
    using Clock = std::chrono::steady_clock;
    using Seconds = std::chrono::duration<double>;

    std::map<int, TestStruct> m;
    const int total = __MAP_STRESS_TEST_SIZE;

    double elapsed_time = 0.0;
    double avg_time = 0.0;

    // INSERT benchmark
    auto start = Clock::now();
    for (int i = 0; i < total; i++) {
        TestStruct val = create_test_struct(i);
        auto [it, inserted] = m.emplace(i, std::move(val));
        if (!inserted) {
            std::cerr << "Insert failed at key " << i << "\n";
            return 1;
        }
    }
    auto end = Clock::now();
    elapsed_time = Seconds(end - start).count();
    avg_time = elapsed_time / total;

    std::cout << std::fixed << std::setprecision(9);
    std::cout << "map,insert,time," << elapsed_time << "\n";
    std::cout << "map,insert,avg_time," << avg_time << "\n";

    // FIND benchmark
    start = Clock::now();
    for (int i = 0; i < total; i++) {
        auto it = m.find(i);
        if (it == m.end()) {
            std::cerr << "Find failed at key " << i << "\n";
            return 1;
        }
        if (it->second.id != i) {
            std::cerr << "Find returned wrong value at key " << i << "\n";
            return 1;
        }
    }
    end = Clock::now();
    elapsed_time = Seconds(end - start).count();
    avg_time = elapsed_time / total;

    std::cout << "map,find,time," << elapsed_time << "\n";
    std::cout << "map,find,avg_time," << avg_time << "\n";

    // DELETE benchmark
    start = Clock::now();
    for (int i = 0; i < total; i++) {
        std::size_t erased = m.erase(i);
        if (erased != 1) {
            std::cerr << "Delete failed at key " << i << "\n";
            return 1;
        }
    }
    end = Clock::now();
    elapsed_time = Seconds(end - start).count();
    avg_time = elapsed_time / total;

    std::cout << "map,delete,time," << elapsed_time << "\n";
    std::cout << "map,delete,avg_time," << avg_time << "\n";

    return 0;
}