// helper functions for file system manipulation
//
// (C) Markus Dittrich, 2015

#include <iostream>

#include <cstddef>
#include <unistd.h>

#include "util.hpp"


// helper function to compute the buf size required for dirent for
// the particular filesystem in use. Code adapted from
// http://womble.decadent.org.uk/readdir_r-advisory.html
size_t dirent_buf_size(DIR* dirp) {
  long name_max = fpathconf(dirfd(dirp), _PC_NAME_MAX);
  if (name_max == -1) {
    error("fpathconf failed");
  }
  size_t name_end = (size_t)offsetof(struct dirent, d_name) + name_max + 1;
  return (name_end > sizeof(struct dirent) ? name_end : sizeof(struct dirent));
}


// concat_filepaths concatenates two filepaths into one single path
std::string concat_filepaths(std::string& s1, const std::string& s2) {
  if (s1.length() == 0) {
    return s1;
  }
  // remove trailing '/'s from s1
  size_t i = s1.find_last_not_of("/");
  return s1.substr(0, i+1) + "/" + s2;
}


// File is a thin wrapper class for managing C style filepointers
File::File(const std::string& fileName) {
  fp_ = fopen(fileName.c_str(), "r");
  if (fp_ == NULL) {
    throw FailedFileAccess(fileName);
  }
}


File::~File() {
  fclose(fp_);
}


FILE* File::get() {
  return fp_;
}


// Dir is a thin wrapper class for managing C style directory pointers
Dir::Dir(const std::string& dirName) {
  dp_ = opendir(dirName.c_str());
  if (dp_ == NULL) {
    throw FailedDirAccess(dirName);
  }
}


Dir::~Dir() {
  closedir(dp_);
}


DIR* Dir::get() {
  return dp_;
}


// simple usage message
void usage() {
  std::cout << "usage(): phantom <path>" << std::endl;
  exit(1);
}


// error wrapper
void error(const std::string& msg) {
  std::cout << "ERROR: " << msg << std::endl;
  exit(1);
}
