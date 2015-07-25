// worker describes the thread worker function
//
// (C) Markus Dittrich 2015

#ifndef WORKER_HPP
#define WORKER_HPP

#include <iostream>
#include <string>

#include "parallel_map.hpp"
#include "refParser.hpp"


struct RefData {
  ReferenceMap refMap;    // reference map of files and hashes to compare to
  StringMap fileMap;      // map of all files found (so missing items can be identified)
};

// worker requests items from the queue which are either file or
// directory paths. If item is
// 1) a filepath: computes and prints the hash of the file
// 2) a directory path: adds contained files and directories contained to
//    the queue
void worker(StringQueue& queue, const Printer& print, RefData& rd);

#endif



