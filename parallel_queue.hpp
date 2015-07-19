// ParallelQueue is an implementation of a thread safe queue based on
// C++'s new concurrency primitives.
//
// (C) Markus Dittrich 2015

#ifndef PARALLEL_QUEUE_HPP
#define PARALLEL_QUEUE_HPP

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>


template <typename T>
class Pqueue {


public:

  Pqueue() {};

  Pqueue(const Pqueue& pq) {
    std::lock_guard<std::mutex> lg(mx_);
    queue_ = pq.queue_;
    std::cout << "copying queue" << std::endl;
  }




private:

  std::queue<T> queue_;
  std::mutex mx_;
  std::condition_variable queue_ready_;

};


#endif
