#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include "Graph.h" 

namespace amos {
class AMOs {
public:
    static void generate(const Graph &G, std::vector<std::set<int>> &A, std::vector<std::vector<int>> &amos, 
                         std::vector<int> to = {}, std::vector<int> membership = {}) {
        if (membership.empty()) {
            membership.resize(G.n + 1000, -1);
            for (size_t i = 0; i < A.size(); ++i) {
                for (int w : A[i]) {
                    membership[w] = i;
                }
            }
        }

        int n = G.n;
        if (to.size() == n) {
            amos.push_back(std::vector<int>(to));
            return;
        }

        // Find the last non-empty subset in A
        int i = A.size() - 1;
        while (i >= 0 && A[i].empty()) {
            --i;
        }

        if (i < 0) return; // No valid subsets left

        int v = *A[i].begin();
        int x = v;
        std::set<int> R;

        while (true) {
            A[i].erase(x);
            membership[x] = -1;
            to.push_back(x);

            // Update neighbors of x
            std::set<int> x_neighbors = G.get_neighbors(x);
            for (int w : x_neighbors) {
                if (std::find(to.begin(), to.end(), w) != to.end()) continue;

                int j = membership[w];
                A[j].erase(w);
                membership[w] = j + 1;
                
                if (j + 1 >= static_cast<int>(A.size()) || A[j + 1].empty()) {
                    A.push_back(set<int>());
                }

                A[j + 1].insert(w);
            }

            generate(G, A, amos, to, membership);

            // Revert changes for neighbors of x
            for (int w : x_neighbors) {
                if (std::find(to.begin(), to.end(), w) != to.end()) continue;

                int j = membership[w];
                A[j].erase(w);
                membership[w] = j - 1;
                A[j - 1].insert(w);
            }

            A[i].insert(x);
            membership[x] = i;
            to.pop_back();

            if (x == v) {
                R = reachable_in_subset(G, v, A[i]);
                R.erase(v);
            }

            if (R.empty()) break;
            x = *R.begin();
            R.erase(R.begin());
        }
    }

private:
    static std::set<int> reachable_in_subset(const Graph &G, int v, const std::set<int> &subset) {
        std::set<int> visited;
        std::stack<int> stack;
        stack.push(v);

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            if (visited.count(current)) continue;
            visited.insert(current);

            for (int neighbor : G.get_neighbors(current)) {
                if (subset.count(neighbor) && !visited.count(neighbor)) {
                    stack.push(neighbor);
                }
            }
        }

        return visited;
    }
};
}