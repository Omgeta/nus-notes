// shared_ptr_race-2.cpp

#include <memory>
#include <thread>
#include <cstdio>
#include <functional>

int main()
{
    std::shared_ptr<int> ptr;

    auto reader = std::thread([](std::shared_ptr<int>& ptr) {
        while(ptr == nullptr)
            ;
        printf("%d\n", *ptr);

    }, std::ref(ptr));

    auto writer = std::thread([](std::shared_ptr<int>& ptr) {
        for(int i = 0; i < 100; i++)
            ptr = std::make_shared<int>(i);

    }, std::ref(ptr));

    reader.join();
    writer.join();
}
