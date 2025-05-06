#ifndef MAZE_H
#define MAZE_H

#include "Coordinate.h"

#include <iostream>
#include <vector>
#include <array>

// Struct storing a maze, containing 2-dimensional vector of holes, and
// 4 2-dimensional boolean vectors in an array, containing the wall
struct Maze {
    public:
        const int rows;
        const int columns;

    private:
        // Each 2-dimensional vector has values 0 or 1, such that 
        // vector[i][j] is 1 if there is a wall in the certain direction
        // at the field in row i and column j
        std::array<std::vector<std::vector<bool>>, 4> walls;

        // 2-dimensional vector having values 0 or 1, such that 
        // holes[i][j] is 1 if there is a hole at the field in row i and column j
        std::vector<std::vector<bool>> holes;
    
        // Start and end fields of the maze
        const Coordinate start;
        const Coordinate end;

    public:
        Maze(int rows, int columns, const Coordinate& start, const Coordinate& end);

        // Function returning a vector of the given node's four neighbours 
        std::vector<Coordinate> get_neighbours(const Coordinate& node) const;

        // Constructs a graph from the maze
        std::vector<std::vector<Coordinate>> construct_graph() const; 

        // Returns the new coordinate when going from certain coordinate in a given direction
        Coordinate step(const Coordinate& coord, int direction) const;

        // Changes the array walls such that it indicates that there is a 
        // wall in the certain direction at the given field 
        void set_wall(int direction, int row, int column);

        // Changes the holes vector, such that it indicates that there is 
        // a hole at the given field
        void set_hole(int row, int column);

        // Return weather there is a wall in the given direction on the given field
        bool has_wall(int direction, int row, int column) const;

        // Returns weather there is a hole at the given field
        bool has_hole(int row, int column) const;

        // Converts an coordinate of the maze to its index
        int to_index(const Coordinate& coord) const;

};

#endif