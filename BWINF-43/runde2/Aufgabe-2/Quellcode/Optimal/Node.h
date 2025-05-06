#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "Coordinate.h"

// Struct for storing two coordiantes in one place, providing
// the usual comparision operations
struct Node {
    Coordinate first;
    Coordinate second;

    Node();
    Node(const Coordinate& first, const Coordinate& second);

    bool operator<(const Node& other) const;
    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Node& node);
};

#endif 
