#include "Coordinate.h"
#include "Maze.h"

#include <queue>
#include <chrono>
#include <algorithm>
#include <tuple>

// Const for infinity in shortest path
const int INFTY = 1e9;

// Define the indices for up, right, bottom and left as well as the offsets in an xy-grid
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

// Function converting a coordiante in a maze to an index, 
// mapping each coordinate to a unique index in [0, rows*columns)
int to_index(const Coordinate& coord) {
    return coord.row * columns + coord.column;
}

// Given the graph of a maze as its adjacency list, as well as a starting coorindate,
// compute a vector booleans indicating weather or not you can reach some coordinate from the given 
// starting coordiante in the given maze
std::vector<bool> get_visable_coordinates(const Coordinate& starting_field, const std::vector<std::vector<Coordinate>>& adjacency) {
    // Perform a default breath-first search
    std::vector<bool> visit(rows * columns, 0);
    std::queue<Coordinate> queue;
    queue.push(starting_field); 

    // Repeat until the queue is empty
    while (queue.size() > 0) {
        // Get the current coordinate from the queue, remove it from the queue and get its index
        Coordinate node = queue.front();
        queue.pop();
        int index = to_index(node);

        // If coordinate (node) was already visited, dont visit it again, otherwise mark it as visited
        if (visit[index]) continue;
        visit[index] = true;

        // Add all neighbours of the current coordinate (node) to the queue
        for (Coordinate next : adjacency[index]) {
            queue.push(next);
        }
    }

    return visit;
}

// Compute the single-source shortest path for the given unweighted graph using a simple bfs,
// writing the results, e.i., distances and predecessor for each node to given vectors.
// Assuming dist vector has initally all values set to infinity 
void shortest_path(std::vector<int>& dist, std::vector<Coordinate>& last, const Coordinate& starting_field, const std::vector<std::vector<Coordinate>>& adjacency) {
    // Perform a default breath-first search
    // The queue contains the the current cost, current node and predecessor node 
    std::queue<std::tuple<int, Coordinate, Coordinate>> queue;
    queue.push({0, starting_field, starting_field}); 

    // Repeat until the queue is empty
    while (queue.size() > 0) {
        // Get the current node from the queue, remove it from the queue and compute its index
        int distance;
        Coordinate node, parent;
        std::tie(distance, node, parent) = queue.front();
        queue.pop();
        int index = to_index(node);

        // If coordinate (node) was already visited (by checking if dist value is already set),
        // dont visit it again, otherwise set the values of dist and last 
        if (dist[index] != INFTY) continue;
        dist[index] = distance;
        last[index] = parent;

        // Add all neighbours of the current coordinate (node) to the queue
        for (Coordinate next : adjacency[index]) {
            queue.push({distance+1, next, node});
        }
    }
}

// Given the last vector computed by a shortest paths algorithm, compute the path from start to end.
// Last contains for each coordinate (node of the graph) its predecessor.
std::vector<Coordinate> construct_path(const Coordinate& starting_field, const Coordinate& ending_field, const std::vector<Coordinate>& last) {
    // Return a path of length one if start = end
    if (starting_field == ending_field) {
        return {starting_field};
    }

    // Iteratively constructing the path backwords, starting at end
    std::vector<Coordinate> path;
    Coordinate node = ending_field;

    do {
        path.push_back(node);
        int index = to_index(node);
        node = last[index];
    } while (node != starting_field);

    // Finally add the start node
    path.push_back(starting_field);

    // Reverse the path to achieve the correct order (from start to end)
    std::reverse(path.begin(), path.end());
    return path;
}

// Given some path in the maze, compute the sequence of directions creating it
std::vector<int> construct_directions(const std::vector<Coordinate>& path) {
    int n = path.size();
    if (n <= 1) return {};

    std::vector<int> directions(n-1);

    for (int i = 0; i < n-1; i++) {
        Coordinate node1 = path[i];
        Coordinate node2 = path[i+1];

        // Find out which directions to apply in order to get from node1 to node2 in the maze
        for (int direction = 0; direction < 4; direction++) {
            if (node1 + offset[direction] == node2) {
                directions[i] = direction;
            } 
        }
    }
    
    return directions;
}

// Apply a sequence of directions to some starting field in a given maze
Coordinate apply_directions(const Coordinate& starting_field, const std::vector<std::vector<Coordinate>>& adjacency, const std::vector<int>& directions) {
    Coordinate node = starting_field;

    for (int d : directions) {
        // Make sure to stop moving if the end field is reached
        if (node == end) {
            break;
        }

        // Move to the next field
        int index = to_index(node);
        node = adjacency[index][d];
    }

    return node;
}

// Given the graphs of the two mazes, combine the shortest paths from start to end in 
// both mazes to one path working for both mazes.
std::vector<int> combine_paths(const std::vector<std::vector<Coordinate>>& adjacency1, const std::vector<std::vector<Coordinate>>& adjacency2) {
    int size = rows * columns;

    // Compute the shortest path from start to end in the first maze
    std::vector<int> dist1(size, INFTY);
    std::vector<Coordinate> last1(size);
    shortest_path(dist1, last1, start, adjacency1);

    // Create the path and the resulting list of directions from the shortest path in the first maze
    std::vector<Coordinate> path1 = construct_path(start, end, last1);
    std::vector<int> directions1 = construct_directions(path1);

    // Apply the list of directions of the shortest maze in first maze two the 
    Coordinate node_second = apply_directions(start, adjacency2, directions1);

    // Compute the shortest path from the just computed node_second to end in second maze
    std::vector<int> dist2(size, INFTY);
    std::vector<Coordinate> last2(size);
    shortest_path(dist2, last2, node_second, adjacency2);
    
    // Create the path and the resulting list of directions from the shortest path in the second maze
    std::vector<Coordinate> path2 = construct_path(node_second, end, last2);
    std::vector<int> directions2 = construct_directions(path2);
    
    // Compute the final sequnce of directions working for both mazes by combining the two computed
    // list of directions
    std::vector<int> directions = directions1;
    for (int direction : directions2) {
        directions.push_back(direction);
    }

    return directions;
}

// Given the graph of a maze, some start field and a vector of directions, 
// computes the path (ei., vector of Coordinates) resulting from applying the directions
// to the maze, starting at some start field, and stopping if end field was already reached
std::vector<Coordinate> get_path(const std::vector<std::vector<Coordinate>>& adjacency, const Coordinate& starting_field, const std::vector<int>& directions) {
    std::vector<Coordinate> path;
    Coordinate node = starting_field;

    for (int d : directions) {
        path.push_back(node);

        // Make sure to not change current field (node) if its already the end field
        if (node != end) {
            int index = to_index(node);
            node = adjacency[index][d];
        }
    }

    path.push_back(node);
    return path;
}

void solve() {
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

    // Create the graph of both mazes
    const std::vector<std::vector<Coordinate>> adjacency1 = maze1.construct_graph();
    const std::vector<std::vector<Coordinate>> adjacency2 = maze2.construct_graph();

    // Check weather the end field is reachable from the start field in both mazes
    const std::vector<bool> visable1 = get_visable_coordinates(start, adjacency1);
    bool is_possible_1 = visable1[to_index(end)];

    const std::vector<bool> visable2 = get_visable_coordinates(start, adjacency2);
    bool is_possible_2 = visable2[to_index(end)];

    // If the start and end fields are not connected in both mazes, stop the program
    if (!(is_possible_1 && is_possible_2)) {
        // Compute the duration    
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;

        std::cout << "Impossible: " << is_possible_1 << " " << is_possible_2 << std::endl;

        // Print duration
        std::cout << "Program ran for " << std::fixed << elapsed.count() << " seconds.\n";
        return;
    }

    // Otherwise run the approximation algorithm in both possible orders
    std::vector<int> directions1 = combine_paths(adjacency1, adjacency2);
    std::vector<int> directions2 = combine_paths(adjacency2, adjacency1);

    // Make sure directions1 has fewer amount if directions, because directions1 will be printed
    if (directions2.size() < directions1.size()) {
        std::swap(directions1, directions2);
    }
    
    // --- Output Results ---
    // Compute the duration    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Possible!" << std::endl;

    // Print amount of moves
    std::cout << "Amount of moves: " << directions1.size() << std::endl;

    // Print moves 
    std::cout << "Moves:" << std::endl;

    for (int direction : directions1) {
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

    // Print path through both mazes
    const std::vector<Coordinate> path1 = get_path(adjacency1, start, directions1);
    const std::vector<Coordinate> path2 = get_path(adjacency2, start, directions1);

    for (int i = 0; i < (int) path1.size(); i++) {
        std::cout << path1[i] << " " << path2[i] << std::endl;
    }

    // Print duration
    std::cout << "Program ran for " << std::fixed << elapsed.count() << " seconds.\n";
}

// Main function of the approximation algorithm.
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