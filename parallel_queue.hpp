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

  Pqueue() {};

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
      return std::make_unique<T>();
    }
    auto elem(std::make_unique<T>(queue_.front()));
    queue_.pop();
    return elem;
  }

  std::unique_ptr<T> try_and_wait() {
    std::unique_lock<std::mutex> ul(mx_);
    queue_ready_.wait(ul, [this] { return !queue_.empty();});
    auto elem(std::make_unique<T>(queue_.front()));
    queue_.pop();
    return elem;
  }


private:

  std::queue<T> queue_;
  mutable std::mutex mx_;
  std::condition_variable queue_ready_;

};


#endif
