#include <vector>
#include <string>

#ifndef _STRING_UTILS_
#define _STRING_UTILS_

// Splits a string into substrings using a delimeter
std::vector<std::string> split(std::string s, std::string delim);

// Read any amount of numbers given in a string separated by spaces
std::vector<int> read_numbers(std::string s);

#endif