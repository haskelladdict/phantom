// helper functions for file system manipulation
//
// (C) Markus Dittrich, 2015

#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <dirent.h>

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
