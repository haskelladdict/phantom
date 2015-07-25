// this file contains the commandline parser for phantom.
//
// (C) Markus Dittrich, 2015

#ifndef CMDLINE_HPP
#define CMDLINE_HPP

#include <string>


// POD struct for storing commandline options
struct CmdLineOpts {
  int numThreads = 1;            // number of threads to use
  bool compareToRef = false;     // do we want to compare against a reference
  std::string referenceFilePath; // file and if yes, where's the reference file
  std::string rootPath;          // root of directory to work on
};


struct CmdLineOpts parse_cmdline(int argc, char** argv);


#endif
