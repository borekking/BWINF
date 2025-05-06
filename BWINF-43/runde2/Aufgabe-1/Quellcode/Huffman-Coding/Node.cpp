#include "Node.h"

// Implementation of Node.h

Node::Node(char32_t value, int frequency) : data(value), frequency(frequency), children() {}

// Function adding a child to this node
void Node::add_child(const std::shared_ptr<Node> child) {
    children.push_back(child);
    frequency += child->frequency;
}

// Comparison for Nodes
bool Node::operator<(const Node& other) const {
    return frequency > other.frequency; 
}