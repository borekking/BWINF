#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>  

// Node in the huffman tree
struct Node {
    // The character associated with a leaf; 0 if its not a leaf
    char32_t data;

    // This will be the sum of frequencies of subtree
    int frequency;

    // Pointers to child nodes
    std::vector<std::shared_ptr<Node>> children; 

    Node(char32_t value, int frequency);

    // Function adding a child to this node
    void add_child(const std::shared_ptr<Node> child);

    // Comparison for Nodes
    bool operator<(const Node& other) const;

};

#endif 