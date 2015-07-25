// this file implements a parser for reading in user supplied reference
// hash data to compare against.
//
// (C) Markus Dittrich, 2015

#include <iostream>
#include <fstream>
#include <vector>

#include "refParser.hpp"

using VecString = std::vector<std::string>;

static bool insert_line(const std::string& line, ReferenceMap& info);
static VecString split(const std::string& s, const std::string& delim);


// load_reference_data parses a reference data set at filePath expected to be
// in phantom style output format. It returns an unordered map from filepaths
// to hash values
ReferenceMap load_reference_data(const std::string& filePath) {

  std::ifstream refFile(filePath);
  if (!refFile) {
    return ReferenceMap();
  }

  std::string line;
  ReferenceMap outMap;
  while (getline(refFile, line)) {
    if (!insert_line(line, outMap)) {
      return ReferenceMap();
    }
  }

  return outMap;
}


// insert_line parses a single line of a reference hash file and inserts it into
// the reference map data structure. The line is expected to be in csv format
// of the form:  <hash type>,  <file path>,  <file hash>
bool insert_line(const std::string& line, ReferenceMap& map) {
  auto result = split(line, ", ");
  if (result.size() != 3) {
    return false;
  }
  map[result[1]] = result[2];
  return true;
}


// split is a helper function splitting string s into substring at each instance
// of any character contained in the delimiter string
VecString split(const std::string& s, const std::string& delim) {
  size_t j = 0;
  std::vector<std::string> out;
  while ((j = s.find_first_not_of(delim, j)) != std::string::npos) {
    size_t i = s.find_first_of(delim, j);
    if (i == std::string::npos) {
      out.push_back(s.substr(j, s.length()-j));
      break;
    }
    out.push_back(s.substr(j,i-j));
    j = i;
  }
  return out;
}










