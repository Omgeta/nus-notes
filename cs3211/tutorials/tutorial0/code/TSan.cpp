#include <thread>

int raced;
int result;

int main() {
  std::thread t0([]() { raced = 0; });
  std::thread t1([]() { raced = 1; });
  std::thread t2([]() { result = raced; });

  t0.join();
  t1.join();
  t2.join();

  return result;
}

// Press the "Reload" button in the
// executor tabs on the right
// to rerun the program
// and possibly get different results.

// Top executor is compiled without tsan,
// Bottom executor is compiled with tsan.

// However, godbolt's ThreadSanitizer
// doesn't seem to be very good
// (probably due to the sandbox),
// so try to run this locally!
