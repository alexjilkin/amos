#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <queue>
#include <algorithm> 

class Enumerator {
public:
    // Constructor: Takes number of vertices and the adjacency list of the graph
    Enumerator(int k, const std::vector<std::vector<int>> &adj)
        : k_(k), adj_(adj) {
        A.resize(k_);
        posInA.resize(k_, 0);

        // Initially, all vertices are in A[0]
        for (int v = 0; v < k_; v++) {
            A[0].insert(v);
        }
    }

    // Start the enumeration process
    void enumerate() {
        if ((int)tau.size() == k_) {
            outputAMO(tau);
            return;
        }

        // Find the highest non-empty set in A
        int i = -1;
        for (int idx = k_ - 1; idx >= 0; idx--) {
            if (!A[idx].empty()) {
                i = idx;
                break;
            }
        }
        if (i < 0) return;

        int v = *(A[i].begin());
        std::vector<int> R = getReachable(v, i);

        // Ensure `v` is the first vertex in R
        if (!R.empty() && R[0] != v) {
            for (int k = 0; k < (int)R.size(); k++) {
                if (R[k] == v) {
                    std::swap(R[k], R[0]);
                    break;
                }
            }
        }

        while (!R.empty()) {
            int x = R.back();
            R.pop_back();

            A[i].erase(x);
            tau.push_back(x);
            posInA[x] = -1;

            // Adjust neighbors of `x`
            for (int w : adj_[x]) {
                if (!inTau(w)) {
                    int j = posInA[w];
                    if (j >= 0 && j < k_ - 1) {
                        A[j].erase(w);
                        A[j + 1].insert(w);
                        posInA[w] = j + 1;
                    }
                }
            }

            // Recurse
            enumerate();

            // Undo changes for neighbors of `x`
            for (int w : adj_[x]) {
                if (!inTau(w)) {
                    int j = posInA[w];
                    if (j > 0) {
                        A[j].erase(w);
                        A[j - 1].insert(w);
                        posInA[w] = j - 1;
                    }
                }
            }

            // Undo changes for `x`
            A[i].insert(x);
            tau.pop_back();
            posInA[x] = i;

            // Recompute R if x == v
            if (x == v) {
                std::vector<int> newR = getReachable(v, i);
                newR.erase(std::remove(newR.begin(), newR.end(), v), newR.end());
                for (int y : newR) {
                    if (!inTau(y)) {
                        R.push_back(y);
                    }
                }
            }
        }
    }

private:
    int k_;
    const std::vector<std::vector<int>> &adj_;
    std::vector<std::unordered_set<int>> A;
    std::vector<int> posInA;
    std::vector<int> tau;

    // Find all vertices reachable from `start` in the subgraph induced by A[i]
    std::vector<int> getReachable(int start, int i) {
        std::vector<int> reachable;
        std::queue<int> q;
        std::unordered_set<int> visited;

        if (A[i].count(start)) {
            q.push(start);
            visited.insert(start);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            reachable.push_back(u);

            for (int w : adj_[u]) {
                if (A[i].count(w) && !visited.count(w)) {
                    visited.insert(w);
                    q.push(w);
                }
            }
        }

        return reachable;
    }

    // Check if vertex `w` is already in tau
    bool inTau(int w) {
        return posInA[w] == -1;
    }

    // Output the current AMO ordering
    void outputAMO(const std::vector<int> &ordering) {
        std::cout << "AMO ordering: ";
        for (int v : ordering) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
};