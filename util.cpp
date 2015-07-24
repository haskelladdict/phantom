// helper functions for file system manipulation
//
// (C) Markus Dittrich, 2015

#include <cstddef>
#include <cstring>
#include <unistd.h>

#include <iostream>

#include "util.hpp"


// add_directory adds the content of the provided directory to the queue
void add_directory(StringQueue& queue, const std::string& path, Printer& print) {

  try {
    Dir dir(path);

    size_t length = dirent_buf_size(dir.get());
    struct dirent *entry, *end;
    if ((entry = (struct dirent*)malloc(length)) == NULL) {
      error("failed to malloc struct dirent");
    }

    int status;
    while ((status = readdir_r(dir.get(), entry, &end) == 0) && (end != NULL)) {

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
  } catch (FailedDirAccess e) {
    std::cerr << e.what() << "\n";
    return;
  }
}


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
  std::cout << "phantom v" << version << " (C) Markus Dittrich, 2015\n\n"
            << "usage(): phantom [options] <root path>" << "\n\n"
            << "options:\n"
            << "\t -n, --num_threads <int>  number of parallel threads used for\n"
            << "\t                          execution of program" << "\n"
            << "\t -c, --compare <path>     path to file with phantom has output\n"
            << "\t                          from a previous run. In this case phanton\n"
            << "\t                          will list all files that are missing, new or\n"
            << "\t                          different from the previous run.\n"
            << "\t -h, --help               this message\n\n"
            << std::endl;
  exit(1);
}


// error wrapper
void error(const std::string& msg) {
  std::cout << "ERROR: " << msg << std::endl;
  exit(1);
}
