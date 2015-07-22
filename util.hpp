// helper functions for file system manipulation
//
// (C) Markus Dittrich, 2015

#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdio>
#include <dirent.h>

#include <iostream>
#include <mutex>
#include <string>



// File is a thin wrapper class for managing C style filepointers
class File {

public:

  File(const std::string& fileName);
  ~File();

  bool init();

  FILE* fp();


private:

  std::string fileName_;
  FILE *fp_;

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
