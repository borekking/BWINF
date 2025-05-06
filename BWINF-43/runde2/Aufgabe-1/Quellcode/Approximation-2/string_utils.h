#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

// Utility functions for working with special characters

std::u32string to_UTF32(const std::string&);

std:: string to_UTF8(const std::u32string&);

void getline_u32(std::istream&, std::u32string&);

#endif