// ParallelQueue is an implementation of a thread safe queue based on
// C++'s new concurrency primitives.
//
// (C) Markus Dittrich 2015

#ifndef PARALLEL_QUEUE_HPP
#define PARALLEL_QUEUE_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>


template <typename T>
class Pqueue {


public:

  using size_type = typename std::queue<T>::size_type;

  Pqueue(int num_threads) : num_threads_(num_threads) {};

  Pqueue(const Pqueue& pq) {
    std::lock_guard<std::mutex> lg(mx_);
    queue_ = pq.queue_;
  }

  Pqueue& operator=(const Pqueue& pq) = delete;

  size_type size() const {
    std::lock_guard<std::mutex> lg(mx_);
    return queue_.size();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lg(mx_);
    return queue_.empty();
  }

  void push(const T& elem) {
    std::lock_guard<std::mutex> lg(mx_);
    queue_.push(elem);
    queue_ready_.notify_one();
  }

  std::unique_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lg(mx_);
    if (queue_.empty()) {
      return nullptr;
    }
    auto elem(std::make_unique<T>(queue_.front()));
    queue_.pop();
    return elem;
  }

  std::unique_ptr<T> try_and_wait() {
    std::unique_lock<std::mutex> ul(mx_);

    // once all active threads are waiting no more elements will enter the queue
    // and we need to tear down. To do so, set done to true and wake up all
    // sleeping threads.
    ++num_waiting_;
    if (num_waiting_ == num_threads_) {
      done_ = true;
      queue_ready_.notify_all();
    }
    while (queue_.empty()) {
      // only continue waiting if we are not done yet
      if (done_) {
        return nullptr;
      }
      queue_ready_.wait(ul);
    }
    --num_waiting_;

    auto elem(std::make_unique<T>(queue_.front()));
    queue_.pop();
    return elem;
  }

  bool done() const {
    std::lock_guard<std::mutex> lg(mx_);
    return done_;
  }

private:

  std::queue<T> queue_;
  mutable std::mutex mx_;
  std::condition_variable queue_ready_;

  // variable to determine when queue can terminate
  int num_threads_;
  int num_waiting_ = 0;
  bool done_ = false;
};


#endif
