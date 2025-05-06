#include "string_utils.h"
#include "common.h"

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

// This map will point from a character to its index in the code letters vector
std::map<char, int> code_letter_map;

// Create a map mapping the code letters to their index in the code letters vector
void setup_code_letter_map() {
    for (int i = 0; i < (int) code_letters.size(); i++) {
        char character = code_letters[i];
        code_letter_map[character] = i;
    }
}

// Utility function converting a vector of integers in the range [0, 35] into 
// a string using the vector code_letters.
std::string to_string(const std::vector<int>& vector) {
    std::string string;

    for (int x : vector) {
        string += code_letters[x];
    }

    return string;
}

// Function computing the costs for some code word, where the code word is given 
// as a string and we have costs for each index of the code words
int get_costs(std::string& code, std::vector<int>& costs) {
    int cost = 0;

    for (char character : code) {
        int index = code_letter_map[character];
        cost += costs[index];
    }

    return cost;
}   

// Function converting a map of letter frequencies to a vector containg 
// pairs (amount, letter) for each letter in the given map
std::vector<std::pair<int, char32_t>> get_letters(const std::map<char32_t, int>& frequencies) {
    std::vector<std::pair<int, char32_t>> letters;

    for (auto p : frequencies) {
        char32_t ch = p.first;
        int frequency = p.second;
        letters.push_back({frequency, ch});
    }

    return letters;
}

// Given a vector of code words, return a vector of pairs consisting of code word costs and code words
std::vector<std::pair<int, std::string>> get_code_words_and_costs(std::vector<std::string>& code_words, std::vector<int>& costs) {
    // Find costs for each code word
    std::vector<std::pair<int, std::string>> costs_vector;

    for (int i = 0; i < code_words.size(); i++) {
        std::string code_word = code_words[i];
        int cost = get_costs(code_word, costs);
        costs_vector.push_back({cost, code_word});
    }

    return costs_vector;
}

// Create the actual code (ei., map from chars to strings) given the letters and code words
std::map<char32_t, std::string> create_code(std::vector<std::pair<int, char32_t>>& letters, std::vector<std::pair<int, std::string>>& code_words) {
    // Sort the letters by their frequencies (biggest first)
    std::sort(letters.begin(), letters.end());
    std::reverse(letters.begin(), letters.end());

    // Sort the code words by their costs (smallest first)
    std::sort(code_words.begin(), code_words.end());

    // => Now, the letter at index i will have code word at index i
    std::map<char32_t, std::string> code;

    for (int i = 0; i < letters.size(); i++) {
        char32_t character = letters[i].second;
        std::string code_word = code_words[i].second;
        code[character] = code_word;
    }

    return code;
}

// Given a function for converting the letter vector and the costs to some code words, 
// read in the statement, create code words, create an optimal code based on the code words and 
// output the code and the encoded text
void solve(std::function<std::vector<std::string>(std::vector<std::pair<int, char32_t>>, std::vector<int>)> get_code_words) {
    // --- Get input and output files ---
    std::string input_file_name;
    std::string output_filename;

    std::cout << "Input input file's name: ";
    std::cin >> input_file_name;

    std::cout << "Input output file's name: ";
    std::cin >> output_filename;

    freopen(input_file_name.c_str(), "r", stdin);
    freopen(output_filename.c_str(), "w", stdout);

    // --- Read input ---
    int r;
    std::cin >> r;

    std::vector<int> costs(r);
    for (int i = 0; i < r; i++) {
        std::cin >> costs[i];
    }

    std::u32string input;
    getline_u32(std::cin, input);
    getline_u32(std::cin, input);
    
    // Count letter frequencies
    std::map<char32_t, int> frequencies;

    for (char32_t c : input) {
        frequencies[c]++;
    }

    // Convert map of frequencies to vector of pairs of the form (amount, letter)
    int n = frequencies.size();
    std::vector<std::pair<int, char32_t>> letters = get_letters(frequencies);

    // Setup a map from code letters (chars) to their index 
    setup_code_letter_map();

    // Start to measure time
    auto start_time = std::chrono::high_resolution_clock::now();

    // DEPENDS ON IMPLEMENTATION
    std::vector<std::string> code_words = get_code_words(letters, costs); 

    // Convert the code words 
    std::vector<std::pair<int, std::string>> code_vector = get_code_words_and_costs(code_words, costs);

    // Compute the code 
    std::map<char32_t, std::string> code = create_code(letters, code_vector);

    // Compute the encoded text using the computed code
    std::string encoded_text = "";

    for (char32_t c : input) {
        encoded_text += code[c];
    }

    // Calcuate duration
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // --- Outputs ---
    // Output the letters with their frequency, their code words, and their costs
    // Compute the total costs
    int total_costs = 0;

    for (int i = 0; i < n; i++) {
        int frequency = letters[i].first;
        char32_t ch = letters[i].second;

        std::string code_word = code[ch];
        int code_word_cost = get_costs(code_word, costs);

        total_costs += frequency * code_word_cost;

        std::u32string ch_string(1, ch);
        std::cout << to_UTF8(ch_string) << " " << frequency << " " << code_word << " " << code_word_cost << std::endl;
    }

    // Output the encoded text and its costs
    std::cout << encoded_text << std::endl;
    std::cout << total_costs << std::endl;

    // Print duration
    std::cout << "Program ran for " << elapsed.count() << " seconds.\n";
}