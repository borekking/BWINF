#include <vector>
#include <string>
#include "string_utils.h"

// Splits a string into substrings using a delimeter
std::vector<std::string> split(std::string s, std::string delim) {
    std::vector<std::string> all;
    std::string tok;

    auto it = s.find(delim);

    while (it != std::string::npos) {
        tok = s.substr(0, it);
        all.push_back(tok);
        s.erase(0, it + delim.size());
        it = s.find(delim);
    }

    all.push_back(s);
    return all;
}

// Read any amount of numbers given in a string separated by spaces
std::vector<int> read_numbers(std::string s) {
    std::vector<std::string> splited = split(s, " ");
    std::vector<int> numbers;

    for (std::string sp : splited) {
        int x = std::stoi(sp);
        numbers.push_back(x);
    }

    return numbers;
}