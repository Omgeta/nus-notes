// shared_resource_unsafe.cpp

#include <atomic>
#include <thread>
#include <iostream>
#include <barrier>

std::barrier bar { 4 };

void trade_nvda()
{
    int* foo = new int { 0 };
    // TODO: Insert `delete` for `foo` at appropriate location

    // Predictor threads
    auto predictor_fn = [](int* foo, int prediction) {
        bar.arrive_and_wait();
        *foo = prediction;
    };
    std::thread { predictor_fn, foo, 13300 }.detach();
    std::thread { predictor_fn, foo, 13000 }.detach();

    // Trader threads
    auto trader_fn = [](int* foo) {
        bar.arrive_and_wait();
        while(*foo == 0)
        {
        }
        std::cout << "B NVDA 2 " + std::to_string(*foo) + '\n' << std::flush;
    };
    std::thread { trader_fn, foo }.detach();
    std::thread { trader_fn, foo }.detach();
}

int main()
{
    trade_nvda();
    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });
    trade_nvda();
    std::this_thread::sleep_for(std::chrono::milliseconds { 1 });
    // ...
}
