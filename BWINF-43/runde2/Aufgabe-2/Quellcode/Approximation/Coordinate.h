#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>

// Struct storing two integer variables used for fields in the maze.
// Espacially enabling operations like addtion, subtraction and comparison.
struct Coordinate {
    int row;
    int column;

    Coordinate();
    Coordinate(int row, int column);

    Coordinate operator+(const Coordinate &other) const;
    Coordinate operator-(const Coordinate &other) const;

    bool operator<(const Coordinate& other) const;
    bool operator==(const Coordinate& other) const;
    bool operator!=(const Coordinate& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Coordinate& coord);
};

#endif 