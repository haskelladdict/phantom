// this file implements a number of hash routines on top of openssl
//
// (C) Markus Dittrich, 2015

#include <string>

#include "hash.hpp"
#include "util.hpp"


// md5hash computes the md5 hash of the file at the provided filepath
std::string md5hash(const std::string& path) {

  FILE *fp = fopen(path.c_str(), "r");
  if (fp == NULL) {
    return std::string();
  }

  MD5_CTX c;
  if (!MD5_Init(&c)) {
    fclose(fp);
    error("md5hash(): Failed to initialize md5 hash structure");
  }

  char buffer[512];
  size_t nread;
  while ((nread = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
    if(!MD5_Update(&c, buffer, nread)) {
      fclose(fp);
      error("md5hash(): Failed to update md5 hash");
    }
  }

  unsigned char digest[16];
  if (!MD5_Final(digest, &c)) {
    fclose(fp);
    error("md5hash(): Failed to compute md5 hash");
  }

  std::string hash(32, '0');
  for (int n = 0; n < 16; ++n) {
    snprintf(&(hash[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
  }
  fclose(fp);
  return hash;
}
