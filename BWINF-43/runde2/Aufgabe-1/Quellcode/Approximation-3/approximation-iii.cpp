#include "Node.h"
#include "common.h"

#include <map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <chrono>

// Create the n least cost code words not containing the terminal sequence 0...0 (K times)
Node create_tree(const std::vector<int>& costs, int K, int r, int n) {
    // For each Node in the queue we know that it has some potential child such that the results costs 
    // are that of the integer in the pair.
    // That child will always be at index node.children.size.

    Node root(0, -1, 0);
    auto pointer = std::make_shared<Node>(root);

    std::priority_queue<std::pair<int, std::shared_ptr<Node>>,
                        std::vector<std::pair<int, std::shared_ptr<Node>>>,
                        std::greater<std::pair<int, std::shared_ptr<Node>>>> queue;

    queue.push({costs[0], pointer});

    int counter = 1;

    while (counter < n) {
        // Pop the least cost node from the queue
        int cost = queue.top().first;
        std::shared_ptr<Node> node = queue.top().second;
        queue.pop();

        // Get the index of the next child of node and its letter cost
        int index = node->children.size();
        int letter = index;
        int letter_cost = costs[letter];

        // Create a child note, setting counter of zeros corresponding to letter and node (parent)
        auto child = std::make_shared<Node>((letter == 0) ? (node->counter + 1) : 0, letter, letter_cost);

        // Make sure we will never have K consecutive zeros
        if (letter == 0 && node->counter == K-1) {
            node->add_child(nullptr);
        } else {    
            // Otherwise add the child to the children of node and add it to the queue
            node->add_child(child);
            queue.push({cost + costs[0], child});
        }
        
        // Dont count as a word if it ends in zero
        if (letter > 0) {
            counter++;
        }

        // If there is a potential child left, add node to queue again
        if (node->children.size() < r) {
            int next_index = node->children.size();
            queue.push({cost - letter_cost + costs[next_index], node});
        }
    }

    return *pointer;
}

// DFS for traversing the tree and creating the code.
// Here code_word_vector is the code word created by adding the code letter (index)
// of the childs on the path from the root to the current node.
void dfs(const std::shared_ptr<Node> node, std::vector<int>& code_word_vector, std::vector<std::string>& words) {
    // Dont consider nodes being null (which are the node where we would have K consecutive zeros)
    if (node == nullptr) {
        return;
    }

    int childs = node->children.size();
    
    // Only add the code word if it does not end in 0
    if (node->counter == 0) {
        std::string code_word = to_string(code_word_vector);
        words.push_back(code_word);
    }

    // Recursively visit all childs of the current node
    for (int i = 0; i < childs; i++) {
        code_word_vector.push_back(i);
        
        std::shared_ptr<Node> child = (node->children)[i];
        dfs(child, code_word_vector, words);

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

// Create code words using the n least cost words (not containing terminal sequence 0...0, K zeros; and not ending in 0)
// by adding the terminal sequence to their end 
std::vector<std::string> create_code_words(int K, std::vector<std::string>& words) {
    std::vector<std::string> code_words;
    std::string suffix = "";

    for (int i = 0; i < K; i++) {
        suffix += "0";
    }

    for (int i = 0; i < (int) words.size(); i++) {
        std::string word = words[i];
        code_words.push_back(word + suffix);
    }

    return code_words;
}

// Create n code words given the letters and costs of the code letters
std::vector<std::string> implementation(const std::vector<std::pair<int, char32_t>>& letters, const std::vector<int>& costs) {
    int K = 2;
    int r = costs.size();
    int n = letters.size();

    // Create tree from which the words are being generated
    Node root = create_tree(costs, K, r, n);

    // Get all words from the created tree
    std::vector<std::string> words = create_words(root);

    // Create an prefix free code using the code words
    std::vector<std::string> code_words = create_code_words(K, words);

    return code_words;
}

// Approximation iii)
// First create the n shortest code words, not containing terminal sequence 00 (K=2), and not ending with 0,
// then make them prefix free by adding terminal sequnce 00 in the end of each word
int main() {
    solve(implementation);
    return 0;
}