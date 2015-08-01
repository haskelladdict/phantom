// helper functions for file system manipulation
//
// (C) Markus Dittrich, 2015

#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdio>
#include <dirent.h>

#include <exception>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>

#include "parallel_queue.hpp"


const std::string version = "0.2";


// forward declarations
class Stats;


// custom exception class for failed file access (e.g. due to improper permissions)
class FailedFileAccess : public std::runtime_error {

public:

  FailedFileAccess(const std::string& msg)
    : runtime_error("Failed to access file: " + msg) {}
};


// custom exception class for failed directory access (e.g. due to improper permissions)
class FailedDirAccess : public std::runtime_error {

public:

  FailedDirAccess(const std::string& msg)
    : runtime_error("Failed to access directory: " + msg) {}
};


// File is a thin wrapper class for managing C style filepointers
class File {

public:

  File(const std::string& fileName);
  ~File();

  FILE* get();

private:

  FILE *fp_;
};


// Dir is a thin wrapper class for managing C style directory pointers
class Dir {

public:

  Dir(const std::string& dirName);
  ~Dir();

  DIR* get();

private:

  DIR *dp_;
};


// Printer is a helper class for serializing stdout and stderr
class Printer {

public:

  void cout(const std::string& msg) const {
    std::lock_guard<std::mutex> lg(mx_);
    std::cout << msg << "\n";
  }

  void cerr(const std::string& msg) const {
    std::lock_guard<std::mutex> lg(mx_);
    std::cerr << msg << "\n";
  }

private:

  mutable std::mutex mx_;

};


// add_directory adds the content of the provided directory to the queue
void add_directory(StringQueue& queue, const std::string& path,
  const Printer& print);


// helper function to compute the buf size required for dirent for
// the particular filesystem in use. Code used mostly verbatim from
// http://womble.decadent.org.uk/readdir_r-advisory.html
// Returns 0 on failure.
size_t dirent_buf_size(DIR * dirp);


// concat_filepaths concatenates two filepaths into one single path
std::string concat_filepaths(std::string& s1, const std::string& s2);


// simple usage message
void usage();


// error wrapper
void error(const std::string& msg);


// print final file/data statistics to stdout
void print_stats(const Stats& stats);


// convert a std::chrono::time_point to a human readable string
std::string time_point_to_c_time(const std::chrono::system_clock::time_point& tp);

#endif
