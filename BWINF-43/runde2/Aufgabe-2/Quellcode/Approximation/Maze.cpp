#include "Maze.h"
#include "Coordinate.h"

// Implementation of Maze.h

// Define the indices for top, right, bottom and left as well as the offsets in an xy-grid
const int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;
const Coordinate offset[4] = { Coordinate(-1, 0), Coordinate(0, 1),Coordinate(1, 0), Coordinate(0, -1) };

// Constructor initialising all fields of the struct.
// Espacially setting up empty 2-dimensional vectors for holes and walls
Maze::Maze(int rows, int columns, const Coordinate& start, const Coordinate& end) : rows(rows), columns(columns), start(start), end(end), holes(rows, std::vector<bool>(columns, false)) {
    for (int i = 0; i < 4; i++) {
        walls[i] = std::vector<std::vector<bool>>(rows, std::vector<bool>(columns, false));
    }
}

// Function returning a vector of the four fields (Coordinates) one will reach
// when doing one step to top, right, bottom, left, assuming that the given field 
// is not a hole.
std::vector<Coordinate> Maze::get_neighbours(const Coordinate& coordinate) const {
    if (Maze::has_hole(coordinate.row, coordinate.column)) return {};

    std::vector<Coordinate> edges(4);
    
    // For each of the four directions find the next field
    for (int direction = 0; direction < 4; direction++) {
        edges[direction] = Maze::step(coordinate, direction);
    }

    return edges;
}

// Construct the maze corresponding to a maze. That is, the graph having the rows*columns
// Coordinates from the maze as nodes and edges corresponding to moving in one of the four directions
// from each Coordinate
std::vector<std::vector<Coordinate>> Maze::construct_graph() const {
    // Create the adjacency list for the graph descriped above
    int size = Maze::rows * Maze::columns;
    std::vector<std::vector<Coordinate>> adjacency(size);

    for (int row = 0; row < Maze::rows; row++) {
        for (int column = 0; column < Maze::columns; column++) {
            const Coordinate coordinate(row, column);
            adjacency[Maze::to_index(coordinate)] = Maze::get_neighbours(coordinate);
        }
    }

    return adjacency;
}

// Returns the new coordinate when going from certain coordinate in a given direction
Coordinate Maze::step(const Coordinate& coord, int direction) const {
    // If there is a wall in the current direction, the field will not change
    if (coord == Maze::end || Maze::has_wall(direction, coord.row, coord.column)) {
        return coord;
    }

    // If there is no wall in the current direction, the next field will either
    // be the one you will reach by going on step in the current direction, 
    // or the start field, if there is a hole on the later field.
    Coordinate offset_coord = coord + offset[direction];
    Coordinate next_coord = Maze::has_hole(offset_coord.row, offset_coord.column) ? Maze::start : offset_coord;
    return next_coord;
}

// Function converting a coordiante in a maze to an index, 
// mapping each coordinate to a unique index in [0, rows*columns)
int Maze::to_index(const Coordinate& coord) const {
    return coord.row * Maze::columns + coord.column;
}

// Changes the array walls such that it indicates that there is a 
// wall in the certain direction at the given field 
void Maze::set_wall(int direction, int row, int column) {
    walls[direction][row][column] = true;
}

// Changes the holes vector, such that it indicates that there is 
// a hole at the given field
void Maze::set_hole(int row, int column) {
    holes[row][column] = true;
}

// Return weather there is a wall in the given direction on the given field
bool Maze::has_wall(int direction, int row, int column) const {
    return walls[direction][row][column];
}

// Returns weather there is a hole at the given field
bool Maze::has_hole(int row, int column) const {
    return holes[row][column];
}