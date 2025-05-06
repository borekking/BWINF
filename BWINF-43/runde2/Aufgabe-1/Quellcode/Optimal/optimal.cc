#include "ortools/linear_solver/linear_solver.h"
#include "common.h"
#include "string_utils.h"

#include <codecvt>
#include <string>
#include <locale>
#include <istream> 
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>  
#include <map>
#include <queue>
#include <chrono>

// The ILP Program
// Returns { _, a_1, a_2, ... , a_m } where a_j is the amount of code words of cost j
// Note that m is the maximum code word cost and everything will be 1-index.
std::vector<int> solve_ilp(int m, const std::vector<std::pair<int, char32_t>>& letters, const std::vector<int>& costs) {
    // amount of code letters
    int r = costs.size(); 

    // Amount of letters (ei. amount of needed codewords)
    int n = letters.size();

    // Create a new ilp solver using SCIP
    operations_research::MPSolver solver("Karp-ILP", operations_research::MPSolver::SCIP_MIXED_INTEGER_PROGRAMMING);

    // Setup a time limit and the tolerence gap 
    solver.set_time_limit(60 * 1000); // 60 seconds
    solver.SetSolverSpecificParametersAsString("limits/gap=0"); // 0 percent

    // 1. Define variables
    // 1.1 y_{ij}, e.i., if the i-th letter has a code word with cost j (binary)
    std::vector<std::vector<operations_research::MPVariable*>> vector_y(n+1, std::vector<operations_research::MPVariable*>(m+1));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            vector_y[i][j] = solver.MakeIntVar(0.0, 1.0, "y-" + std::to_string(i) + "-" + std::to_string(j));
        }
    }

    // 1.2 b_j. ei., amount of real prefixes of cost j
    std::vector<operations_research::MPVariable*> vector_b(m+1);

    for (int j = 1; j <= m; j++) {
        vector_b[j] = solver.MakeIntVar(0.0, solver.infinity(), "b-" + std::to_string(j));
    }
    // ---

    // 2. Constraints
    // 2.1 Make sure that each letter has exactly one code words 
    for (int i = 1; i <= n; i++) {
        operations_research::MPConstraint* constraint = solver.MakeRowConstraint(1, 1);
        for (int j = 1; j <= m; j++) {
            constraint->SetCoefficient(vector_y[i][j], 1.0);
        }
    }

    // 2.2 Karps inequality: For each j: sum_{i=1}^n y_{ij} + b_j <= sum_{k=1}^r b_{j-c_k}
    for (int j = 1; j <= m; j++) {
        // Count the occurrences of j-costs[k-1] over all k
        std::vector<int> counter(m+1, 0);

        for (int k = 1; k <= r; k++) {
            if (j-costs[k-1] < 0) {
                continue;
            }
            counter[j-costs[k-1]]++; 
        }

        // By definition b_0 = 1, hence RHS is counter[0]
        int RHS = counter[0];
        operations_research::MPConstraint* constraint = solver.MakeRowConstraint(-solver.infinity(), RHS);

        // Add the y_{ij} for (i=1,...,n) to LHS
        for (int i = 1; i <= n; i++) {
            constraint->SetCoefficient(vector_y[i][j], 1.0);
        }

        // Subtruct counter[k] * b_k from LHS for k=1,...,m
        for (int k = 1; k <= m; k++) {
            if (counter[k] > 0) {
                constraint->SetCoefficient(vector_b[k], -counter[k]);
            }
        }

        // Add b_j to LHS
        constraint->SetCoefficient(vector_b[j], 1.0);
    }
    // ---

    // 3. Objective
    operations_research::MPObjective* objective = solver.MutableObjective();

    // Set the objective to the total cost 
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            objective->SetCoefficient(vector_y[i][j], letters[i-1].first * j);
        }
    }

    objective->SetMinimization();
    // ---

    // Solve the integer linear program
    operations_research::MPSolver::ResultStatus result_status = solver.Solve();

    // Print basic results 
    if (result_status == operations_research::MPSolver::OPTIMAL) {
        std::cout << "Status: " << result_status << std::endl;
        std::cout << "Optimal solution found!:" << std::endl;
        std::cout << "Min value = " << objective->Value() << std::endl;

        // Construct a_j using y_{ij}
        std::vector<int> vector_a(m+1);

        for (int j = 1; j <= m; j++) {
            int sum = 0;
            for (int i = 1; i <= n; i++) {
                sum += vector_y[i][j]->solution_value();
            }
            vector_a[j] = sum;
        }

        return vector_a;
    } else {
        std::cout << "No optimal solution found: " << result_status << std::endl;
        return {};
    }
}

// Function constructing the code words given the vector a_j, where a_j is the amount 
// of code words having cost j, 1 <= j <= m
std::vector<std::string> construct_code_words(const std::vector<int>& vector_a, const std::vector<int>& costs) {
    int m = vector_a.size()-1; // maximum code word cost
    int r = costs.size(); // amount of code letters

    // Remaining code words to create (we still have to create h[j] code words of cost j)
    std::vector<int> vector_h = vector_a;

    // Keep track of the amount of non-zero elements of h; 
    // Stop creating new code words if counter is 0
    int counter = 0;
    for (int j = 1; j <= m; j++) {
        if (vector_h[j] != 0) {
            counter++;
        }
    }

    // Actual set of code words
    std::vector<std::string> code_words;

    // Queue will store the next code words to add
    std::queue<std::string> queue;
    queue.push("");

    while (counter > 0) {
        // Pop current word and its costs from the queue 
        std::string string = queue.front();
        queue.pop();
        int cost = get_costs(string, costs);

        // If h[cost] > 0 (assuming 1 <= cost <= m), add this word to the code words
        if (cost <= m && vector_h[cost] > 0) {
            code_words.push_back(string);
            vector_h[cost]--;

            if (vector_h[cost] == 0) {
                counter--;
            }
        } else {
            // Otherwise push the r words with prefix string to the queue
            for (int k = 1; k <= r; k++) {
                queue.push(string + code_letters[k-1]);
            }
        }
    }

    return code_words;
}

// Create n code words given the letters and costs of the code letters
std::vector<std::string> implementation(const std::vector<std::pair<int, char32_t>>& letters, const std::vector<int>& costs) {
    int m = 200;
    int r = costs.size();
    int n = letters.size();

    // Create the values a_j, where a_j is the amount of code words having cost j
    std::vector<int> vector_a = solve_ilp(m, letters, costs);

    if (vector_a.size() == 0) {
        return {};
    }

    // Construct the code words from the vector a
    std::vector<std::string> code_words = construct_code_words(vector_a, costs);
    return code_words;
}

// Optimal solutions using Karp's ILP
int main() {
    solve(implementation);
    return 0;
}