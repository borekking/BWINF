#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <math.h>
#include <queue>
#include <chrono>

#include "matrix_int.h"
#include "matrix_bool.h"
#include "string_utils.h"
#include "max_cliques.h"

const int left = 1;
const int right = 3;

// Find the next box or return empty box if there is none
matrix_int next(int types, int styles, matrix_int &clothes, std::vector<std::set<int>> &cliques) {
    for (std::set<int> Q : cliques) {
        matrix_int box(types, styles, 0);
        bool b = true;

        for (int i = 0; i < types; i++) {
            int s = 0;
            for (int q : Q) {
                int a = clothes(i, q);
                int val = std::min(a, right - s);
                box(i, q) = val;
                s += val;
            }
            if (s < left) {
                b = false;
                break;
            }
        }

        if (b) {
            return box;
        }
    }
    
    matrix_int nul(0, 0, 0);
    return nul;
}

std::vector<matrix_int> solve(int types, int styles, matrix_int &clothes, std::vector<std::set<int>> &cliques) {
    std::vector<matrix_int> boxes;
    matrix_int X(types, styles, 0);
    matrix_int clothes_left = clothes;
    bool b = true;

    while (b) {
        X = next(types, styles, clothes_left, cliques);

        if (X.get_cols() == 0) {
            b = false;
        } else {
            boxes.push_back(X);
            clothes_left = clothes_left - X;
        }
    }

    return boxes;
}

// Return the files lines
std::vector<std::string> get_lines() {
    std::vector<std::string> lines;
    std::string line;

    while (getline(std::cin, line)) {
        lines.push_back(line);
    }

    return lines;
}

int main() {
    // Get input file
    std::cout << "Enter input file: ";
    std::string file;
    std::cin >> file;
    freopen(file.c_str(), "r", stdin); 

    // Read whole file
    std::vector<std::string> lines = get_lines();

    // Read ampunt of types and styles
    std::vector<int> pair1 = read_numbers(lines[0]);
    int types = pair1[0], styles = pair1[1];

    // Read relation of styles as matrix
    matrix_bool styles_matrix(styles, styles, false); 
    for (int i = 0; i < styles; i++) {
        styles_matrix(i, i) = 1;
    }

    int index = 1;
    while (!lines[index].empty()) {
        std::vector<int> pair = read_numbers(lines[index]);
        int x = pair[0], y = pair[1];
        x--; y--;
        styles_matrix(x, y) = true;
        styles_matrix(y, x) = true;
        index++;
    }

    // Read matrix A of clothes; clothes[x][y] = l -> cloth of type x exists l times in style y
    matrix_int clothes(types, styles, 0);
    index++;

    while (index < (int) lines.size() && !lines[index].empty()) {
        std::vector<int> triple = read_numbers(lines[index]);
        int x = triple[0], y = triple[1], amount = triple[2];
        x--; y--;
        clothes(x, y) = amount;
        index++;
    }
    
    // Find all cliques
    std::set<std::set<int>> all_max_cliques = max_cliques(styles, styles_matrix);
    std::vector<std::set<int>> cliques;

    for (std::set<int> clique : all_max_cliques) {
        cliques.push_back(clique);
    }
    
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<matrix_int> matrices = solve(types, styles, clothes, cliques);

    auto end  = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " microseconds" << std::endl;

    // Output results:
    std::cout << "Amount: " << matrices.size() << std::endl;
    int counter = 0;

    for (matrix_int m : matrices) {
        std::cout << std::endl;
        for (int i = 0; i < types; i++) {
            for (int j = 0; j < styles; j++) {
                counter += m(i, j);
                std::cout << m(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "Used Clothes: " << counter << std::endl;
    return 0;
}
