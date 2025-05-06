#include "string_utils.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>  
#include <map>
#include <queue>
#include <chrono>
#include <functional>

// Define code letters 
const std::vector<char> code_letters = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// Create a map mapping the code letters to their index in the code letters vector
void setup_code_letter_map();

// Utility function converting a vector of integers in the range [0, 35] into 
// a string using the vector code_letters.
std::string to_string(const std::vector<int> &vector);

// Function computing the costs for some code word, where the code word is given 
// as a string and we have costs for each index of the code words
int get_costs(const std::string& code, const std::vector<int>& costs);

// Function converting a map of letter frequencies to a vector containg 
// pairs (amount, letter) for each letter in the given map
std::vector<std::pair<int, char32_t>> get_letters(const std::map<char32_t, int>& frequencies);

// Given a vector of code words, return a vector of pairs consisting of code word costs and code words
std::vector<std::pair<int, std::string>> get_code_words_and_costs(std::vector<std::string>& code_words, std::vector<int>& costs);

// Create the actual code (ei., map from chars to strings) given the letters and code words
std::map<char32_t, std::string> create_code(std::vector<std::pair<int, char32_t>>& letters, std::vector<std::pair<int, std::string>>& code_words);

// Given a function for converting the letter vector and the costs to some code words, 
// read in the statement, create code words, create an optimal code based on the code words and 
// output the code and the encoded text
void solve(std::function<std::vector<std::string>(std::vector<std::pair<int, char32_t>>, std::vector<int>)> get_code_words);