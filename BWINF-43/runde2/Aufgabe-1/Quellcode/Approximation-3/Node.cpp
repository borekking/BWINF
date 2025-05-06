#include "Node.h"

// Implementation of Node.h

Node::Node(int counter, int letter, int cost) : counter(counter), letter(letter), cost(cost), children() {}

// Add a child to the childrens
void Node::add_child(const std::shared_ptr<Node> child) {
    children.push_back(child);
}

// Comparison for Nodes
bool Node::operator<(const Node& other) const {
    return cost > other.cost; 
}