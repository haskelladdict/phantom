// Pmap is a thin layer on top of a std::unordered map to provide concurrent
// insertion and element access.
// NOTE: This is *not* a full fledged concurrent map implementation just good
// enough for what we need within phantom.
//
// (C) Markus Dittrich 2015

#ifndef PARALLEL_MAP_HPP
#define PARALLEL_MAP_HPP

#include <mutex>
#include <unordered_map>


template <typename T1, typename T2>
class Pmap {

public:

  using size_type = typename std::unordered_map<T1, T2>::size_type;
  using const_iter_type = typename std::unordered_map<T1,T2>::const_iterator;

  Pmap() {};

  Pmap(const Pmap& pm) = delete;
  Pmap& operator=(const Pmap& pm) = delete;

  size_type size() const {
    std::lock_guard<std::mutex> lg(mx_);
    return map_.size();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lg(mx_);
    return map_.empty();
  }

  T2& operator[](const T1& key) {
    std::lock_guard<std::mutex> lg(mx_);
    return map_[key];
  }

  const T2& at(const T1& key) const {
    std::lock_guard<std::mutex> lg(mx_);
    return map_.at(key);
  }

  const_iter_type find(const T1& key) const {
    std::lock_guard<std::mutex> lg(mx_);
    return map_.find(key);
  }

  const_iter_type end() const noexcept {
    return map_.end();
  }

private:

  std::unordered_map<T1, T2> map_;
  mutable std::mutex mx_;
};

using StringMap = Pmap<std::string, int>;


#endif
