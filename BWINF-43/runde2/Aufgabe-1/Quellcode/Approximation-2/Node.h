#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>  

// Node used in tree generating lowest cost code words
struct Node {
    // The code letter
    int letter;

    // The cost of the code letter
    int cost;

    // Pointers to child nodes
    std::vector<std::shared_ptr<Node>> children; 

    Node(int letter, int cost);

    // Add a child to the childrens
    void add_child(const std::shared_ptr<Node> child);

    // Comparison for Nodes
    bool operator<(const Node& other) const;

};

#endif 