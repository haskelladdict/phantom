// Stats keeps track of runtime statistics (number of files parsed,
// number of bytes read, etc.)
//
// (C) Markus Dittrich 2015

#ifndef STATS_HPP
#define STATS_HPP

#include <sys/stat.h>

#include <chrono>
#include <mutex>
#include <thread>


class Stats {

public:

  Stats(const std::chrono::time_point<std::chrono::system_clock>& t) :
    startTime_(t) {};


  long long num_files() const {
    std::lock_guard<std::mutex> lg(mx_);
    return num_files_;
  }


  long long num_bytes() const {
    std::lock_guard<std::mutex> lg(mx_);
    return num_bytes_;
  }


  void add(off_t size) {
    std::lock_guard<std::mutex> lg(mx_);
    ++num_files_;
    num_bytes_ += size;
  }

  std::chrono::time_point<std::chrono::system_clock> startTime() const {
    std::lock_guard<std::mutex> lg(mx_);
    return startTime_;
  }


private:

  mutable std::mutex mx_;

  long long num_files_ = 0;
  long long num_bytes_ = 0;

  std::chrono::time_point<std::chrono::system_clock> startTime_;
};

#endif
