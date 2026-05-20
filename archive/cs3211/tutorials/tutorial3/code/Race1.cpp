// shared_ptr_race-1.cpp

#include <thread>
#include <memory>
#include <cstdio>

int main()
{
    std::shared_ptr<int> ptr = std::make_shared<int>(0);

    auto reader = std::thread([](std::shared_ptr<int> ptr) {
        for(int i = 0; i < 100; i++)
            printf("%d ", *ptr);
        printf("\n");
    }, ptr);

    auto writer = std::thread([](std::shared_ptr<int> ptr) {
        for(int i = 0; i < 100; i++)
            *ptr = i;
    }, ptr);

    reader.join();
    writer.join();
}
