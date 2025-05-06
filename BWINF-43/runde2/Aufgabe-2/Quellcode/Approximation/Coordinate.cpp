#include "Coordinate.h"

// Implementation of Coordinate.h

Coordinate::Coordinate() {
    Coordinate(0, 0);
}

Coordinate::Coordinate(int row, int column) : row(row), column(column) { }

// Function beforming elementwise addtion, returning a new Coordinate object
Coordinate Coordinate::operator+(const Coordinate &other) const {
    return {row + other.row, column + other.column};
}

// Function beforming elementwise subtraction, returning a new Coordinate object
Coordinate Coordinate::operator-(const Coordinate &other) const {
    return {row - other.row, column - other.column};
}

// Function comparing Cordinates, such that first row will be compared, and for 
// equality of row valiues, column value will be compared
bool Coordinate::operator<(const Coordinate& other) const {
    if (row == other.row) {
        return column < other.column;
    }
    return row < other.row; 
}

// Simple == implementation, returning weather both Coordinates have the some row and column values
bool Coordinate::operator==(const Coordinate& other) const {
    return row == other.row && column == other.column;
}

// Simple != implemenation based on the == implementation
bool Coordinate::operator!=(const Coordinate& other) const {
    return !(*this == other);
}

// Function writing the object to some outputstream
std::ostream& operator<<(std::ostream& os, const Coordinate& coord) {
    return os << coord.row << " " << coord.column;
}