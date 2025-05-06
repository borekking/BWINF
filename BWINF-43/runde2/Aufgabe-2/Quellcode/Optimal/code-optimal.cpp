#include "Coordinate.h"
#include "Maze.h"
#include "Node.h"

#include <queue>
#include <chrono>
#include <algorithm>
#include <tuple>
#include <set>

// Const for infinity in shortest path
const int INFTY = 1e9;

// Define the indices for up, right, bottom and left as well as the offsets in an row, column based Coordinate
const int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;
const Coordinate offset[4] = { Coordinate(-1, 0), Coordinate(0, 1),Coordinate(1, 0), Coordinate(0, -1) };

// Globally store the amount of rows and columns in both mazes
int rows, columns;

// Globally store start and end fields
Coordinate start, end;

// Function for reading one maze in the given format, and returning a Maze object
Maze read_input(int rows, int columns) {
    // Create a new empty Maze
    Maze maze(rows, columns, start, end); 
    
    // Read all right wall
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns-1; column++) {
            // Check weather the current field has a wall to the right
            // Correspondingly set the right wall at that field and the left wall at the field on to the right
            int x;
            std::cin >> x;
            bool has_wall = (x == 1);

            if (has_wall) {
                maze.set_wall(RIGHT, row, column);
                maze.set_wall(LEFT, row, column+1);
            }
        }
    }

    // Set the most left wall in every row
    for (int row = 0; row < rows; row++) {
        maze.set_wall(LEFT, row, 0);
    }

    // Set the most right wall in every row
    for (int row = 0; row < rows; row++) {
        maze.set_wall(RIGHT, row, columns-1);
    }

    // Read all bottom walls
    for (int row = 0; row < rows-1; row++) {
        for (int column = 0; column < columns; column++) {
            // Check weather the current field has a wall at the bottom
            // Correspondingly set the down wall at that field and the top wall at the field below
            int x;
            std::cin >> x;
            bool has_wall = x;

            if (has_wall) {
                maze.set_wall(DOWN, row, column);
                maze.set_wall(UP, row+1, column);
            }
       }
    }

    // Set all top walls in the first row
    for (int column = 0; column < maze.columns; column++) {
        maze.set_wall(UP, 0, column);
    }

    // Set all bottom walls in the last row
    for (int column = 0; column < maze.columns; column++) {
        maze.set_wall(DOWN, rows-1, column);
    }

    // Read the amount of holes
    int amount;
    std::cin >> amount;

    // Read and set all holes
    for (int i = 0; i < amount; i++) {      
        // First one is the column, second one is the row
        int column, row;
        std::cin >> column >> row;
        maze.set_hole(row, column);
    }

    return maze;
}

// Function converting a Coordinate in the maze to some index, 
// such that there will be a bijective mapping
int to_index(Coordinate& first, Coordinate& second) {
    int i1 = first.row * columns + first.column;
    int i2 = second.row * columns + second.column;
    return i1 * rows*columns + i2;
}

// Given a coordinate of each of the two mazes, computed the four possible next 
// states depending on the choosen direction.
std::vector<Node> get_edges(const Coordinate& first, const Coordinate& second, const Maze& maze1, const Maze& maze2) {
    // If one of the coordinates is a field with a hole, we can ignore (because we can never land here)
    if (maze1.has_hole(first.row, first.column)) return {};
    if (maze2.has_hole(second.row, second.column)) return {};

    // Get the four neighbours of the given coordinates per maze
    std::vector<Coordinate> neighbours1 = maze1.get_neighbours(first);
    std::vector<Coordinate> neighbours2 = maze2.get_neighbours(second);

    // Combine the two vectors of neighbours 
    std::vector<Node> reach(4);

    for (int i = 0; i < 4; i++) {
        Node node = Node(neighbours1[i], neighbours2[i]);
        reach[i] = node;
    }

    return reach;
}

// Compute the single-source shortest path for a unweighted graph using a simple bfs,
// writing the results, e.i., distances and predecessor for each node to given vectors.
// Assuming dist vector has initally all values set to infinity 
void shortest_path(const Node& starting_node, const Maze& maze1, const Maze& maze2, std::vector<int>& dist, std::vector<Node>& last) {
    // Perform a default breath-first search
    // The queue contains the the current cost, current node and predecessor node 
    std::queue<std::tuple<int, Node, Node>> queue;
    queue.push({0, starting_node, starting_node});
    
    // Repeat until the queue is empty
    while (queue.size() > 0) {
        // Get the current node from the queue, remove it from the queue and compute its index
        int distance;
        Node node, parent;
        std::tie(distance, node, parent) = queue.front();
        queue.pop();
        int index = to_index(node.first, node.second);

        // If node was already visited (by checking if dist value is already set),
        // dont visit it again, otherwise set the values of dist and last 
        if (dist[index] != INFTY) continue;
        dist[index] = distance;
        last[index] = parent;

        // Add all neighbours of the current node to the queue
        for (Node next : get_edges(node.first, node.second, maze1, maze2)) {
            queue.push({distance+1, next, node});
        }
    }
}

// Given the last vector computed by a shortest paths algorithm, compute the path from start to end.
// Last contains for each coordinate (node of the graph) its predecessor.
std::vector<Node> construct_path(const Node& start_node, const Node& end_node, std::vector<Node>& last) {
    // Return a path of length one if start = end
    if (start_node == end_node) {
        return {start_node};
    }

    // Iteratively constructing the path backwords, starting at end
    std::vector<Node> path;
    Node node = end_node;

    do {
        path.push_back(node);
        int index = to_index(node.first, node.second);
        node = last[index];
    } while (node != start_node);

    // Finally add the start node
    path.push_back(start_node);

    // Reverse the path to achieve the correct order (from start to end)
    reverse(path.begin(), path.end());
    return path;
}

// Given some path in both mazes, compute the sequence of directions creating it
std::vector<int> construct_directions(const std::vector<Node>& path, const Maze& maze1, const Maze& maze2) {
    int n = path.size();
    std::vector<int> directions(n-1);

    for (int i = 0; i < n-1; i++) {
        Node node1 = path[i];
        Node node2 = path[i+1];

        // Find out which directions to apply in order to get from node1 to node2 in the mazes
        for (int direction = 0; direction < 4; direction++) {
            if ((maze1.step(node1.first, direction) == node2.first) && (maze2.step(node1.second, direction) == node2.second)) {
                directions[i] = direction;
            } 
        }
    }
    
    return directions;
}

int solve() {
    // --- Read input ---
    std::cin >> columns >> rows;

    // Setup start and end fields
    start = Coordinate(0, 0);
    end = Coordinate(rows-1, columns-1);

    // Read both mazes
    const Maze maze1 = read_input(rows, columns);
    const Maze maze2 = read_input(rows, columns);

    // --- Actual algorithm ---
    auto start_time = std::chrono::high_resolution_clock::now();

    // Find the shortest path 
    int size = rows*columns*rows*columns;
    std::vector<int> dist(size, INFTY);
    std::vector<Node> last(size);
    shortest_path(Node(start, start), maze1, maze2, dist, last);

    // Construct the actual path from last vector
    std::vector<Node> path = construct_path(Node(start, start), Node(end, end), last);

    // Construct the directions creating the path
    std::vector<int> directions = construct_directions(path, maze1, maze2);
    
    // --- Output Results ---
    // Record the end time
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    
    // Print amount of directions
    std::cout << "Amount of moves: " << directions.size() << std::endl;

    // Print moves 
    std::cout << "Moves:" << std::endl;
    
    for (int direction : directions) {
        if (direction == UP) {
            std::cout << "U "; 
        } else if (direction == RIGHT) {
            std::cout << "R ";
        } else if (direction == DOWN) {
            std::cout << "D ";
        } else if (direction == LEFT) {
            std::cout << "L ";
        }
    }
    std::cout << std::endl;

    for (Node node : path) {
        std::cout << node << std::endl;
    }
    
    std::cout << "Program ran for " << std::fixed << elapsed.count() << " seconds.\n";
    return 0;
}

// Main function of optimal algorithm.
// Assuming that some solutions exists!
int main() {
    std::string input_file;
    std::string output_file;

    std::cout << "Input input file name:" << std::endl;
    std::cin >> input_file;

    std::cout << "Input output file name:" << std::endl;
    std::cin >> output_file;

    freopen(input_file.c_str(), "r", stdin);
    freopen(output_file.c_str(), "w", stdout);

    solve();
    return 0;
}