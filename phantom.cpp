// phantom is a multithreaded file consistency checker. Given a top level path
// it will either
// 1) print the checksum of all files contained underneath or
// 2) compare the checksums of all files contained underneath with a provided
//    list. All files that differ, are not on the list, or missing are reported.
//
// (C) Markus Dittrich 2015


#include <iostream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "hash.hpp"
#include "parallel_queue.hpp"
#include "util.hpp"


// convenience defs
const int num_workers = 1;

using StringQueue = Pqueue<std::string>;



// add_directory adds the content of the provided directory to the queue
void add_directory(StringQueue& queue, const std::string& path, Printer& print) {

  DIR* dir = opendir(path.c_str());
  if (dir == NULL) {
    print.cerr("opendir failed on " + path);
    return;
  }

  size_t length = dirent_buf_size(dir);
  struct dirent *entry, *end;
  if ((entry = (struct dirent*)malloc(length)) == NULL) {
    error("failed to malloc struct dirent");
  }

  int status;
  while ((status = readdir_r(dir, entry, &end) == 0) && (end != NULL)) {

    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // we use d_type to figure out what type entries are. However, since d_type
    // is not that portable it may be better to use lstat instead.
    if (entry->d_type == DT_DIR || entry->d_type == DT_REG) {
      std::string p(path);
      std::string name(entry->d_name);
      std::string path = concat_filepaths(p, name);
      queue.push(path);
    }
  }
  if (status != 0 && end != NULL) {
    error("add_dir_to_queue(): Failed to parse directory.");
  }
  free(entry);
}


// worker requests items from the queue which are either file or
// directory paths. If item is
// 1) a filepath: computes and prints the hash of the file
// 2) a directory path: adds contained files and directories contained to
//    the queue
void worker(StringQueue& queue, Printer& print) {

  while (!queue.done()) {

    auto path = queue.try_and_wait();
    if (!path) {
      break;
    }

    // check if path is a directory or a file
    struct stat info;
    if (lstat(path->c_str(), &info) < 0) {
      print.cerr("lstat failed on " + *path);
      continue;
    }
    if (S_ISREG(info.st_mode)) {
      print.cout(*path + " = " + md5hash(*path));
    } else if (S_ISDIR(info.st_mode)) {
      add_directory(queue, *path, print);
    }
  }
}



int main(int argc, char** argv) {

  if (argc != 2) {
    usage();
  }

  StringQueue sq(num_workers);
  sq.push(argv[1]);

  Printer pr;
  std::vector<std::thread> threads;
  for (int i=0; i < num_workers; ++i) {
    threads.push_back(std::thread(worker, std::ref(sq), std::ref(pr)));
  }

  // wait for threads to finish
  for (auto& t : threads) {
    t.join();
  }
}
