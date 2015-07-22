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

#endif
