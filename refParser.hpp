// this file implements a parser for reading in user supplied reference
// hash data to compare against.
//
// (C) Markus Dittrich, 2015

#ifndef REFPARSER_HPP
#define REFPARSER_HPP

#include <string>
#include <unordered_map>

using ReferenceMap = std::unordered_map<std::string, std::string>;


ReferenceMap load_reference_data(const std::string& filePath);


#endif
