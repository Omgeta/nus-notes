#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

std::atomic<int> t1_ctr = 0;
std::atomic<int> t2_ctr = 0;

constexpr int LOOPS = 10000000;

// constexpr auto loadOrder = std::memory_order_seq_cst;
// constexpr auto storeOrder = std::memory_order_seq_cst;

constexpr auto loadOrder = std::memory_order_acquire;
constexpr auto storeOrder = std::memory_order_release;

// constexpr auto loadOrder = std::memory_order_relaxed;
//constexpr auto storeOrder = std::memory_order_relaxed;

void t1()
{
    for(int i = 0; i <= LOOPS; i++)
        t1_ctr.store(i, storeOrder);
}

void t2()
{
    for(int j = 0; j <= LOOPS; j++)
        t2_ctr.store(j, storeOrder);
}
void t3()
{
    namespace chrono = std::chrono;
    auto start = std::chrono::high_resolution_clock::now();
    // Wait for either thread to finish getting to their max value
    while(t1_ctr.load(loadOrder) < LOOPS && t2_ctr.load(loadOrder) < LOOPS)
        ;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout                                                                //
        << "T3 saw either T1 or T2 finish in "                               //
        << chrono::duration_cast<chrono::milliseconds>(stop - start).count() //
        << "ms\n";
}

int main()
{
    auto th1 = std::jthread { t1 };
    auto th2 = std::jthread { t2 };
    auto th3 = std::jthread { t3 };

    return 0;
}
