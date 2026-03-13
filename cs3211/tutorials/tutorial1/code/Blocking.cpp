#include <chrono>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <semaphore>
#include <thread>

struct Job {
  int id;
  int count;
};

// Unbounded queue with enqueue, non-blocking and blocking dequeue
// Use semaphores to signal blocked consumers
class JobQueue3 {
  std::queue<Job> jobs;
  std::mutex mut;
  std::counting_semaphore<> count;

 public:
  JobQueue3() : jobs{}, mut{}, count{0} {}

  void enqueue(Job job) {
    std::unique_lock lk{mut};
    jobs.push(job);
    count.release();
  }

  std::optional<Job> try_dequeue() {
    if (!count.try_acquire())
      return std::nullopt;
    else {
      std::unique_lock lk{mut};
      Job j = jobs.front();
      jobs.pop();
      return j;
    }
  }

  Job dequeue() {
    count.acquire();
    std::unique_lock lk{mut};
    Job j = jobs.front();
    jobs.pop();
    return j;
  }
};

int main() {
  JobQueue3 queue;

  std::thread t1{[&queue]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    queue.enqueue(Job{420, 420});
  }};
  std::thread t2{[&queue]() {
    Job job = queue.dequeue();
    std::cout << "This should print 420: " << job.id << std::endl;
  }};

  t1.join();
  t2.join();

  return 0;
}
