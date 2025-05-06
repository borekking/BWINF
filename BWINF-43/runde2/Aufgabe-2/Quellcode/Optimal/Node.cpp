#include "Node.h"

// Implemenation of Node.h 

Node::Node() {
    Coordinate zero(0, 0);
    Node(zero, zero);
}

Node::Node(const Coordinate& first, const Coordinate& second) : first(first), second(second) { }

// Implementation of < comparision with another Node based on comparison of Nodes
bool Node::operator<(const Node& other) const {
    if (first == other.first)
        return second < other.second; 
    return first < other.first; 
}

// Basic implementation of == based on comparison of Nodes
bool Node::operator==(const Node& other) const {
    return first == other.first && second == other.second;
}

// Basic implementation of != using the implementation of ==
bool Node::operator!=(const Node& other) const {
    return !(*this == other); 
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
    return os << node.first << " " << node.second;
}
