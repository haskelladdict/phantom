// worker describes the thread worker function
//
// (C) Markus Dittrich 2015


#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/stat.h>

#include "hash.hpp"
#include "parallel_map.hpp"
#include "refParser.hpp"
#include "util.hpp"
#include "worker.hpp"


static void compare_to_reference(const std::string& path, const std::string& hash,
  const Printer& printer, RefData& rd);


// worker requests items from the queue which are either file or
// directory paths. If item is
// 1) a filepath: computes and prints the hash of the file
// 2) a directory path: adds contained files and directories contained to
//    the queue
void worker(StringQueue& fileQueue, const Printer& printer, RefData& rd,
  Stats& stats, CmdLineOpts& opts) {

  // if we receive a non-empty refMap we compare against it
  bool compare = false;
  if (!rd.refMap.empty()) {
    compare = true;
  }

  while (!fileQueue.done()) {
    auto path = fileQueue.try_and_wait();
    if (path.empty()) {
      break;
    }

    // check if path is a directory or a file
    struct stat info;
    if (lstat(path.c_str(), &info) < 0) {
      printer.cerr("lstat failed on " + path);
      continue;
    }
    if (S_ISREG(info.st_mode)) {
      std::string hash = hasher(opts.hashMethod, path);
      if (opts.collectStats) {
        stats.add(info.st_size);
      }
      if (compare) {
        compare_to_reference(path, hash, printer, rd);
      } else {
        printer.cout(opts.hashMethod + " , " + path + " , " + hash);
      }
    } else if (S_ISDIR(info.st_mode)) {
      add_directory(fileQueue, path, printer);
    }
  }
}


// compare_to_reference is a short helper function for checking if a file is
// in the reference data set and if yes if the hash matches. Otherwise prints
// an error message.
static void compare_to_reference(const std::string& path, const std::string& hash,
  const Printer& printer, RefData& rd) {

  rd.fileMap[path] = 1;
  auto r = rd.refMap.find(path);
  if (r != rd.refMap.end()) {
    if (r->second != hash) {
      printer.cout("hash differs    :  " + path + "  found(" + hash
        + ") expected(" + r->second + ")");
    }
  } else {
    printer.cout("extra file      :  " + path + " with hash(" + hash + ")");
  }
}
