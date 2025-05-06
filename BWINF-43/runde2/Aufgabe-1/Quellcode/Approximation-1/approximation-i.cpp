#include "Node.h"
#include "common.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>  
#include <map>
#include <queue>
#include <chrono>

// DFS for traversing the tree and creating the code words.
// Here code_word_vector is the code word created by adding the code letter (index)
// of the childs on the path from the root to the current node.
void dfs(const std::shared_ptr<Node> node, std::vector<int>& code_word_vector, std::vector<std::string>& code_words) {
    // Amount of childs
    int childs = node->children.size();
    
    // Check if the current node is a leaf (ei, has zero childs)
    // and make sure its not a dummy / fake letter having frequency 0
    if (childs == 0 && node->frequency != 0) {
        // Add the resulting code word to the list of codewords
        std::string code_word = to_string(code_word_vector);
        code_words.push_back(code_word);
        return;
    }

    // Otherwise recursively visit all childs of the current node
    for (int i = 0; i < childs; i++) {
        // Change the code word vector according to the index of the current child node
        code_word_vector.push_back(i);

        std::shared_ptr<Node> child = (node->children)[i];
        dfs(child, code_word_vector, code_words);
        
        code_word_vector.pop_back();
    }
} 

// Function for creating a list of code words given the root of the huffman tree.
std::vector<std::string> create_code_words(const Node& root) {
    std::vector<std::string> code_words;

    std::vector<int> code_word_vector; // Initally empty code word 
    dfs(std::make_shared<Node>(root), code_word_vector, code_words);

    return code_words;
}

// Create the huffman tree
Node create_tree(const std::vector<std::pair<int, char32_t>>& letters, int r) {
    // Priority queue sorting the Nodes by their frequency, such that the Node
    // with lowest frequency will be at top
    std::priority_queue<Node> queue;

    // Initally add all letters to the priority queue
    for (auto p : letters) {
        int frequency = p.first;
        Node node(frequency);
        queue.push(node);
    }

    // Add dummy letters (frequency 0) in order to have queue.size() - 1 = 0 (mod r-1)
    while ((r-1 > 1) && (queue.size() % (r-1) != 1)) {
        Node node(0);
        queue.push(node);
    }

    while (queue.size() > 1) {
        Node node(0);
        
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

// Create n code words given the letters and costs of the code letters
std::vector<std::string> implementation(const std::vector<std::pair<int, char32_t>>& letters, const std::vector<int>& costs) {
    int r = costs.size();

    // Create the tree
    Node root = create_tree(letters, r);

    // Create a list of code words from the huffman tree
    std::vector<std::string> code_words = create_code_words(root);
    return code_words;
}

// Approximation i)
// Use the huffman tree to create a prefix free code based on the letter
// frequency, then sort by their costs, and assings code words to letters 
// accordingly
int main() {
    solve(implementation);
    return 0;
}