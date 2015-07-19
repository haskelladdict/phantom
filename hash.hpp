// this file implements a number of hash routines on top of openssl
//
// (C) Markus Dittrich, 2015

#ifndef HASH_HPP
#define HASH_HPP

#include <string>

#if defined(__APPLE__)
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#define SHA1 CC_SHA1
#else
#include <openssl/md5.h>
#endif


// md5hash computes the md5 hash of the file at the provided filepath
std::string md5hash(const std::string& path);


#endif
