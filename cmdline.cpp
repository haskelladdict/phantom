// this file contains the commandline parser for phantom
//
// (C) Markus Dittrich, 2015

#include <getopt.h>

#include <iostream>

#include "cmdline.hpp"
#include "util.hpp"


// long_options for getopt_long command line parsing
static struct option long_options[] = {
  {"num_threads", required_argument, NULL, 'n'},
  {"compare", required_argument, NULL, 'c'},
  {"help", no_argument, NULL, 'h'},
  {NULL, 0, NULL, 0}
};


// parse_cmdline parses any provided command line arguments and uses this
// information to populate a CmdLinOpts struct.
struct CmdLineOpts parse_cmdline(int argc, char** argv) {

  struct CmdLineOpts cmdOpts;

  int c;
  long nthreads;
  while ((c = getopt_long (argc, argv, "n:c:h", long_options, NULL)) != -1) {

    switch(c) {
      case 'n':
        nthreads = strtol(optarg, NULL, 10);
        if (nthreads == 0) {
          error("incorrect number of threads specified on command line");
        }
        cmdOpts.numThreads = nthreads;
        break;

      case 'c':
        cmdOpts.compareToRef = true;
        cmdOpts.referenceFilePath = optarg;
        break;

      case 'h':
      default :
        usage();
    }
  }

  if (argc == optind) {
    usage();
  }
  cmdOpts.rootPath = argv[optind];

  return cmdOpts;
}
