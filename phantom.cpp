// phantom is a multithreaded file consistency checker. Given a top level path
// it will either
// 1) print the checksum of all files contained underneath or
// 2) compare the checksums of all files contained underneath with a provided
//    list. All files that differ, are not on the list, or missing are reported.
//
// (C) Markus Dittrich 2015


#include <iostream>
#include <vector>

#include "cmdline.hpp"
#include "hash.hpp"
#include "refParser.hpp"
#include "util.hpp"
#include "worker.hpp"


int main(int argc, char** argv) {

  if (argc <= 1) {
    usage();
  }
  auto cmdlOpts = parse_cmdline(argc, argv);
  RefData ref;
  if (cmdlOpts.compareToRef) {
    ref.refMap = load_reference_data(cmdlOpts.referenceFilePath);
    if (ref.refMap.empty()) {
      error("Failed to parse reference data file");
    }
  }

  // initialize queue with root path
  StringQueue sq(cmdlOpts.numThreads);
  sq.push(cmdlOpts.rootPath);

  Printer pr;
  std::vector<std::thread> threads;
  for (int i=0; i < cmdlOpts.numThreads; ++i) {
    threads.push_back(std::thread(worker, std::ref(sq), std::ref(pr),
      std::ref(ref)));
  }

  // wait for threads to finish
  for (auto& t : threads) {
    t.join();
  }

  // check for disappeared files
  for (const auto& e : ref.refMap) {
    if (ref.fileMap.find(e.first) == ref.fileMap.end()) {
      std::cout << "file disappeared:  " << e.first << "\n";
    }
  }
}
