// phantom is a multithreaded file consistency checker. Given a top level path
// it will either
// 1) print the checksum of all files contained underneath or
// 2) compare the checksums of all files contained underneath with a provided
//    list. All files that differ, are not on the list, or missing are reported.
//
// (C) Markus Dittrich 2015

#include <openssl/evp.h>

#include <chrono>
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

  // initialize OpenSSL
  // NOTE: Adding all digests may lead to large static executables
  OpenSSL_add_all_digests();

  auto cmdlOpts = parse_cmdline(argc, argv);
  RefData refData;
  if (cmdlOpts.compareToRef) {
    refData.refMap = load_reference_data(cmdlOpts.referenceFilePath);
    if (refData.refMap.empty()) {
      error("Failed to parse reference data file");
    }
  }

  // initialize queue with root path
  StringQueue fileQueue(cmdlOpts.numThreads);
  fileQueue.push(cmdlOpts.rootPath);

  Printer printer;
  Stats stats(std::chrono::system_clock::now());
  std::vector<std::thread> threads;
  for (int i=0; i < cmdlOpts.numThreads; ++i) {
    threads.push_back(std::thread(worker, std::ref(fileQueue), std::ref(printer),
      std::ref(refData), std::ref(stats), std::ref(cmdlOpts)));
  }

  // wait for threads to finish
  for (auto& t : threads) {
    t.join();
  }

  // check for disappeared files
  for (const auto& e : refData.refMap) {
    if (refData.fileMap.find(e.first) == refData.fileMap.end()) {
      std::cout << "file disappeared:  " << e.first << "\n";
    }
  }

  // print final statistics
  if (cmdlOpts.collectStats) {
    print_stats(stats);
  }

  // cleanup openssl
  EVP_cleanup();
}
