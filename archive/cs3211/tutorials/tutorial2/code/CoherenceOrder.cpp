#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <latch>

std::atomic<int> a { 0 };
constexpr int LOOPS = 10;

using stdmo = std::memory_order;

// constexpr auto LOAD_MO = stdmo::seq_cst;
// constexpr auto STORE_MO = stdmo::seq_cst;

// constexpr auto LOAD_MO = stdmo::acquire;
// constexpr auto STORE_MO = stdmo::release;

constexpr auto LOAD_MO = stdmo::relaxed;
constexpr auto STORE_MO = stdmo::relaxed;

// Ensure that all threads are
// started before moving on
std::latch latch { 4 };

void microsleep()
{
    std::this_thread::sleep_for(std::chrono::microseconds(1500));
}

void store_even()
{
    latch.arrive_and_wait();
    for(int i = 0; i < LOOPS; i += 2)
    {
        a.store(i, STORE_MO);
        microsleep();
    }
}

void store_odd()
{
    latch.arrive_and_wait();
    for(int i = 1; i < LOOPS; i += 2)
    {
        a.store(i, STORE_MO);
        microsleep();
    }
}

void load_fn(std::vector<int>* v)
{
    latch.arrive_and_wait();
    for(int i = 0; i < LOOPS; ++i)
    {
        v->push_back(a.load(LOAD_MO));
        microsleep();
    }
}

int main()
{
    std::vector<int> v3, v4;
    auto t1 = std::thread { store_even };
    auto t2 = std::thread { store_odd };
    auto t3 = std::thread { load_fn, &v3 };
    auto t4 = std::thread { load_fn, &v4 };

    t1.join(), t2.join(), t3.join(), t4.join();

    std::cout << "Values of 'a' seen by thread 3:\n";
    for(int i : v3)
        std::cout << i << " ";
    std::cout << '\n';
    std::cout << "Values of 'a' seen by thread 4:\n";
    for(int i : v4)
        std::cout << i << " ";
    std::cout << '\n';
}
