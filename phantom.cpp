// phantom is a multithreaded file consistency checker. Given a top level path
// it will either
// 1) print the checksum of all files contained underneath or
// 2) compare the checksums of all files contained underneath with a provided
//    list. All files that differ, are not on the list, or missing are reported.
//
// (C) Markus Dittrich 2015


#include <iostream>
#include <string>


#include "parallel_queue.hpp"



int main() {

  std::cout << "hello world" << std::endl;
  Pqueue<std::string> q;
  Pqueue<std::string> p(q);

}
