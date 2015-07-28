// this file implements a number of hash routines on top of openssl
//
// (C) Markus Dittrich, 2015

#include <string>

#include "hash.hpp"
#include "util.hpp"


// return the requested (by name) hash of the file at the provided path
std::string hasher(const std::string& digest_name, const std::string& path) {

  const EVP_MD *md = EVP_get_digestbyname(digest_name.c_str());
  if (!md) {
    error("hash function " + digest_name + " not known");
  }

  try {
    File file(path);

    EVP_MD_CTX c;
    EVP_MD_CTX_init(&c);

    if (!EVP_DigestInit_ex(&c, md, NULL)) {
      error("hash(): Failed to initalize digest.");
    }

    char buffer[512];
    size_t nread;
    while ((nread = fread(buffer, 1, sizeof(buffer), file.get())) > 0) {
      if(!EVP_DigestUpdate(&c, buffer, nread)) {
        error("hash(): Failed to update hash");
      }
    }

    unsigned int length = 0;
    unsigned char digest[EVP_MAX_MD_SIZE];
    if (!EVP_DigestFinal_ex(&c, digest, &length)) {
      error("hash(): Failed to finalize the hash");
    }

    if (!EVP_MD_CTX_cleanup(&c)) {
      error("hash(): Failed to cleanup hash structure");
    }

    std::string hash(2*length, '0');
    for (unsigned int n = 0; n < length; ++n) {
      snprintf(&(hash[n*2]), length*2, "%02x", (unsigned int)digest[n]);
    }
    return hash;

  } catch (FailedFileAccess e) {
    std::cerr << e.what() << "\n";
    return std::string();
  }
}
