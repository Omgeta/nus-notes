#include <chrono>
#include <iostream>
#include <string>
#include <thread>

std::string hello = "Hello";

void print_hello() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << hello << std::endl;
}

struct Lambda {
  std::string name;

  void operator()() const {
    std::cout << name << std::endl;
  }
};

int main() {
  std::string world("World");

  std::thread thread1(print_hello);
  std::thread thread2(
      [](std::string name) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        name += "!";
        std::cout << name << std::endl;
      },
      world);
  std::thread thread3(Lambda{"Bienvenue!"});

  thread1.join();
  thread2.join();
  thread3.join();

  return 0;
}

// Output after 1 second: Hello
// Output after 2 seconds: World!
