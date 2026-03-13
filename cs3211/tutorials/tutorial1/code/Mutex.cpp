#include <iostream>
#include <thread>
#include <mutex>


int counter;
std::mutex mut;

int main()
{
    auto t0 = std::thread([]() { 
        std::scoped_lock lk{mut};
        ++counter; 
    });
    auto t1 = std::thread([]() { 
        std::scoped_lock lk{mut};
        ++counter; 
    });

    t0.join();
    t1.join();

    std::cout << counter << std::endl;
}
