// this file implements a number of hash routines on top of openssl
//
// (C) Markus Dittrich, 2015

#ifndef HASH_HPP
#define HASH_HPP

#include <string>

#include <openssl/evp.h>

// return the requested (by name) hash of the file at the provided path
std::string hasher(const std::string& digest_name, const std::string& path);

#endif
