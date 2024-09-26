#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <queue>
#include "matrix_bool.h"
#include "max_cliques.h"

// 1. simple all max cliques algorithm 
std::set<std::set<int>> max_cliques(int n, matrix_bool &mat) {
    std::set<std::set<int>> all;
    std::queue<std::set<int>> q;

    for (int i = 0; i < n; i++) {
        q.push({i});
    }

    while (q.size()) {
        // Current clique
        std::set<int> top = q.front();
        q.pop();

        bool max = true;

        for (int i = 0; i < n; i++) {
            // Node i already in clique
            if (top.find(i) != top.end()) continue;

            // Check if adding v results in a clique;
            bool clique = true;

            for (int v : top) {
                if (!mat(v, i)) {
                    clique = false;
                    break;
                }
            }

            // If new, bigger clique: Add to queue
            if (clique) {
                max = false; // top cannot be a max. clique
                std::set<int> new_clique = top;
                new_clique.insert(i);
                q.push(new_clique);
            }
        }

        // Add to all if it is a max. clique
        if (max) {
            all.insert(top);
        }
    }

    return all;
}