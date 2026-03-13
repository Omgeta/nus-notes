
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

// Declarations for mutex version
int counter = 0;
std::mutex m;
// Declarations for atomic version
// TODO: add atomic implementation here
std::atomic<int> atomic_counter{0};

constexpr int LOOPS = 100000;
namespace sc = std::chrono;

void test1()
{
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < LOOPS; i++)
    {
        auto lk = std::unique_lock{m};
        counter++;
    }

    auto stop = sc::high_resolution_clock::now();
    std::cout                                                        //
        << "One lock version thread took "                           //
        << sc::duration_cast<sc::microseconds>(stop - start).count() //
        << " us\n";
}

void test2()
{
    auto start = sc::high_resolution_clock::now();
    for(int i = 0; i < LOOPS; i++)
    {
        atomic_counter.fetch_add(1);
    }

    auto stop = sc::high_resolution_clock::now();
    std::cout                                                        //
        << "One atomic version thread took "                         //
        << sc::duration_cast<sc::microseconds>(stop - start).count() //
        << " us\n";
}

int main()
{
    {
        auto t1_1 = std::jthread { test1 };
        auto t1_2 = std::jthread { test1 };
    }
    {
        auto t2_1 = std::jthread { test2 };
        auto t2_2 = std::jthread { test2 };
    }
}

