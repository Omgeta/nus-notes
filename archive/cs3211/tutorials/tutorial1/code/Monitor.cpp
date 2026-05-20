#include <chrono>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <condition_variable>
#include <thread>

struct Job {
  int id;
  int count;
};

// Unbounded queue with enqueue, non-blocking and blocking dequeue
class JobQueue4 {
  std::queue<Job> jobs;
  std::mutex mut;
  std::condition_variable cond;

 public:
  JobQueue4() : jobs{}, mut{}, cond{} {}

  void enqueue(Job job) {
    {
      std::unique_lock lock{mut};
      jobs.push(job);
    }

    cond.notify_one();
  }
    
  std::optional<Job> try_dequeue() {
    std::unique_lock lock{mut};

    if (jobs.empty()) {
      return std::nullopt;
    } else {
      // Unfortunately, there's no pop + return in one API call
      // (yet?)
      Job job = jobs.front();
      jobs.pop();

      return job;
    }
  }

  Job dequeue() {
    std::unique_lock lock{mut};
    
    while (jobs.empty()) {
        cond.wait(lock);
    }
    // Alternatively, this is exactly the same code
    // cond.wait(lock, [this]() { return !jobs.empty(); });
    
    Job job = jobs.front();
    jobs.pop();
    
    return job;
  }
};

int main() {
  JobQueue4 queue;

  std::thread t1{[&queue]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    queue.enqueue(Job{420, 420});
  }};
  std::thread t2{[&queue]() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::optional<Job> job = queue.try_dequeue();
    if (job) {
        std::cout << "This should print 420: " << job->id << std::endl;
    } else {
        std::cout << "Did not get a job" << std::endl;
    }
  }};

  t1.join();
  t2.join();

  return 0;
}
