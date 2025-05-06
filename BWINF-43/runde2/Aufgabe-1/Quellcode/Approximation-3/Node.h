#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>  

// Node in the tree used to generate least cost words not containing 
// (0...0) and not ending in 0.
struct Node {
    // The amount of consecutive zeros when going from node to root (including this node)
    int counter; 

    // Index of the letter this node has
    int letter;

    // The cost of the code letter
    int cost;

    // Pointers to child nodes
    std::vector<std::shared_ptr<Node>> children; // Pointers to child nodes

    Node(int counter, int letter, int cost);

    // Add a child to the childrens
    void add_child(std::shared_ptr<Node> child);

    // Comparison for Nodes
    bool operator<(const Node& other) const;

};

#endif 