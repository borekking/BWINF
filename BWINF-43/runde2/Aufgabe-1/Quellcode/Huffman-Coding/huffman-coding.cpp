#include "Node.h"
#include "string_utils.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>  
#include <map>
#include <queue>
#include <chrono>

// Define code letters 
const std::vector<char> code_letters = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// Utility function converting a vector of integers in the range [0, 35] into 
// a string using the code letters.
std::string to_string(const std::vector<int> &vector) {
    std::string string;
    for (int x : vector) {
        string += code_letters[x];
    }
    return string;
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

// DFS for traversing the tree and creating the code.
// Here code_word_vector is the code word created by adding the code letter (index)
// of the childs on the path from the root to the current node.
void dfs(const std::shared_ptr<Node> node, std::vector<int>& code_word_vector, std::map<char32_t, std::string>& code) {
    // Amount of childs
    int childs = (node->children).size();
    
    // Check if the current node is a leaf (ei, has zero childs)
    // and make sure its not a dummy / fake letter having frequency 0
    if (childs == 0 && node->frequency != 0) {
        // Set the resulting code word for the current character
        char32_t character = node->data;
        std::string code_word = to_string(code_word_vector);
        code[character] = code_word;
        return;
    }

    // Otherwise recursively visit all childs of the current node
    for (int i = 0; i < childs; i++) {
        // Change the code word vector according to the index of the current child node
        code_word_vector.push_back(i);

        std::shared_ptr<Node> child = (node->children)[i];
        dfs(child, code_word_vector, code);
        
        code_word_vector.pop_back();
    }
} 

// Function for creating the actual code (ei. a map from characters to their codeword)
// given the root of the huffman tree.
std::map<char32_t, std::string> create_code(const Node& root) {
    std::map<char32_t, std::string> code;

    std::vector<int> code_word_vector; // Initally empty code word 
    dfs(std::make_shared<Node>(root), code_word_vector, code);

    return code;
}

// Function for creating the huffman tree, given a vector of letters and the amount 
// of code letters r.
Node create_tree(const std::vector<std::pair<int, char32_t>>& letters, int r) {
    // Priority queue sorting the Nodes by their frequency, such that the Node
    // with lowest frequency will be at top
    std::priority_queue<Node> queue;

    // Initally add all letters to the priority queue
    for (std::pair<int, char32_t> letter : letters) {
        int frequency = letter.first;
        char32_t character = letter.second;
        Node node(character, frequency);
        queue.push(node);
    }

    // Add dummy letters (frequency 0) in order to achieve queue.size() - 1 = 0 (mod r-1)
    while ((r-1 > 1) && (queue.size() % (r-1) != 1)) {
        Node node(0, 0);
        queue.push(node);
    }

    while (queue.size() > 1) {
        Node node(0, 0);

        // Add the r nodes with lowest frequency currently in the queue to the current node
        for (int i = 0; i < r; i++) {
            std::shared_ptr<Node> child = std::make_shared<Node>(queue.top());
            queue.pop();
            node.add_child(child);
        }

        queue.push(node);
    }

    // The last node in the queue is the root of the whole tree
    Node root = queue.top();
    queue.pop();
    return root;
}

int solve() {
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

    // Start to measure time
    auto start_time = std::chrono::high_resolution_clock::now();

    // Count letter frequencies
    std::map<char32_t, int> frequencies;

    for (char32_t c : input) {
        frequencies[c]++;
    }

    // Convert map of frequencies to vector of pairs of the form (amount, letter)
    int n = frequencies.size();
    std::vector<std::pair<int, char32_t>> letters = get_letters(frequencies);

    // --- Actual Algorithm ---
    // Create the huffman tree 
    Node root = create_tree(letters, r);

    // Create a code book from the huffman tree
    std::map<char32_t, std::string> code = create_code(root);

    // Compute the encoded text using the computed code
    std::string encoded_text = "";

    for (char32_t c : input) {
        encoded_text += code[c];
    }

    // Compute duration
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // --- Outputs ---
    // Output the letters with their frequency, their code words, and their costs (=code word length)
    for (int i = 0; i < n; i++) {
        int frequency = letters[i].first;
        char32_t ch = letters[i].second;

        std::string code_word = code[ch];
        int code_word_cost = code_word.size();

        std::u32string ch_string(1, ch);
        std::cout << to_UTF8(ch_string) << " " << frequency << " " << code_word << " " << code_word_cost << std::endl;
    }

    // Output the encoded text and its length (=costs)
    std::cout << encoded_text << std::endl;

    int total_costs = encoded_text.size();
    std::cout << total_costs << std::endl;

    // Print duration
    std::cout << "Program ran for " << elapsed.count() << " seconds.\n";
    return 0;
}

// This version is for part a, using huffman coding
int main() {
    // --- Get input and output files ---
    std::string input_file_name;
    std::string output_filename;

    std::cout << "Input input file's name: ";
    std::cin >> input_file_name;

    std::cout << "Input output file's name: ";
    std::cin >> output_filename;

    freopen(input_file_name.c_str(), "r", stdin);
    freopen(output_filename.c_str(), "w", stdout);

    solve();
}