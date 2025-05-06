#include "Node.h"
#include "common.h"

#include <map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <chrono>

// Create a tree which will later generate the n least cost code words
Node create_tree(const std::vector<int>& costs, int r, int n) {
    // For each Node in the queue we know that it has some potential child such that the results costs 
    // are that of the integer in the pair.
    // That child will always be at index node.children.size.

    Node root(-1, 0);
    auto pointer = std::make_shared<Node>(root);

    std::priority_queue<std::pair<int, std::shared_ptr<Node>>,
                        std::vector<std::pair<int, std::shared_ptr<Node>>>,
                        std::greater<std::pair<int, std::shared_ptr<Node>>>> queue;

    // Add the root with the cost of its first child to the queue
    queue.push({costs[0], pointer});

    // Counter will store the amount of nodes in the tree, 
    // ei. the amount of code words created by the tree
    int counter = 1;

    while (counter < n) {
        // Get the current node
        int cost = queue.top().first;
        std::shared_ptr<Node> node = queue.top().second;
        queue.pop();

        // Get the index and cost of the next child
        int index = node->children.size();
        int letter = index;
        int letter_cost = costs[letter];

        // Create the child and add it to the current node; then add the child to the queue
        // Also increase the counter because one code word was just created
        auto child = std::make_shared<Node>(letter, letter_cost);
        node->add_child(child);
        queue.push({cost + costs[0], child});
        counter++;

        // If there is a potential child left, add node to queue again
        if (node->children.size() < r) {
            int next_index = node->children.size();
            queue.push({cost - letter_cost + costs[next_index], node});
        }
    }

    // Return the root
    return *pointer;
}

// DFS for traversing the tree and creating the code.
// Here code_word_vector is the code word created by adding the code letter (index)
// of the childs on the path from the root to the current node.
void dfs(const std::shared_ptr<Node> node, std::vector<int>& code_word_vector, std::vector<std::string>& code_words) {
    int childs = node->children.size();
    
    // Add the current code word to list of code words
    std::string code_word = to_string(code_word_vector);
    code_words.push_back(code_word);

    // Otherwise recursively visit all childs of the current node
    for (int i = 0; i < childs; i++) {
        code_word_vector.push_back(i);
        
        std::shared_ptr<Node> child = (node->children)[i];
        dfs(child, code_word_vector, code_words);

        code_word_vector.pop_back();
    }
} 

// Function for creating a list of code words given the root of the tree
std::vector<std::string> create_words(Node& root) {
    std::vector<std::string> code_words;

    std::vector<int> code_word_vector; // Initally empty code word 
    dfs(std::make_shared<Node>(root), code_word_vector, code_words);

    return code_words;
}

// Encode some positive number k with a prefix free code consisting of 
// 0's and 1's.
std::string encode(int k) {
    std::string code = "";
    int bits = (k == 0) ? 0 : (32 - __builtin_clz(k));

    for (int i = 0; i < bits; i++) {
        if (k & (1 << i)) { // 1 at i-th position (right to left) of k
            code = "10" + code;
        } else {
            code = "01" + code;
        }
    }

    code = code + "00";
    return code;
} 

// Given the n least cost words, create prefix free code words by 
// adding some prefix to each word based on its length
std::vector<std::string> create_code_words(const std::vector<std::string>& words) {
    std::vector<std::string> code_words;

    for (int i = 0; i < (int) words.size(); i++) {
        // Get the current code words and its length
        std::string code_word = words[i];
        int length = code_word.size();

        // Create the prefix based on length
        std::string prefix = encode(length);
        std::string new_code_word = prefix + code_word;

        // Add the new prefix free code word
        code_words.push_back(new_code_word);
    }

    return code_words;
}

// Create n code words given the letters and costs of the code letters
std::vector<std::string> implementation(const std::vector<std::pair<int, char32_t>>& letters, const std::vector<int>& costs) {
    int r = costs.size();
    int n = letters.size();

    // Create the n least cost code words
    Node root = create_tree(costs, r, n);
    std::vector<std::string> words = create_words(root);

    // Create an prefix free code using the code words
    std::vector<std::string> code_words = create_code_words(words);

    return code_words;
}

// Approximation ii)
// First create the n shortest code words,
// then make them prefix free by adding some prefix based on their length
int main() {
    solve(implementation);
    return 0;
}