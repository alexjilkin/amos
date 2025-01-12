#pragma once

#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

namespace amos {
class AMOs {
public:
  static void generate(const Graph &G, std::vector<std::set<int>> &A,
                       std::vector<u_int16_t> &amos,
                       std::vector<int> to = {},
                       std::vector<int> membership = {}) {
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
      amos.push_back(lehmer_code(to));
      return;
    }

    // Find the last non-empty subset in A
    int i = A.size() - 1;
    while (i >= 0 && A[i].empty()) {
      --i;
    }

    if (i < 0)
      return; // No valid subsets left

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
        if (std::find(to.begin(), to.end(), w) != to.end())
          continue;

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
        if (std::find(to.begin(), to.end(), w) != to.end())
          continue;

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

      if (R.empty())
        break;
      x = *R.begin();
      R.erase(R.begin());
    }
  }

private:
  static std::set<int> reachable_in_subset(const Graph &G, int v,
                                           const std::set<int> &subset) {
    const auto &adj = G.adj;
    std::set<int> visited;
    std::stack<int> stack;
    stack.push(v);

    while (!stack.empty()) {
      int current = stack.top();
      stack.pop();

      if (visited.count(current))
        continue;
      visited.insert(current);

      // Iterate over all potential neighbors
      for (int neighbor = 0; neighbor < adj[current].size(); ++neighbor) {
        if (adj[current][neighbor] && subset.count(neighbor) &&
            !visited.count(neighbor)) {
          stack.push(neighbor);
        }
      }
    }

    return visited;
  }

  int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
      result *= i;
    }
    return result;
  }

  // Compute the lexicographic index (Lehmer code) of a permutation
  static uint16_t lehmer_code(const std::vector<int> &perm) {
    int n = perm.size();
    uint16_t index = 0;

    // Compute factorials in reverse order
    std::vector<int> factorials(n);
    factorials[n - 1] = 1;
    for (int i = n - 2; i >= 0; --i) {
      factorials[i] = factorials[i + 1] * (n - 1 - i);
    }

    // Track used elements
    std::vector<bool> used(n, false);

    for (int i = 0; i < n; ++i) {
      int smaller = 0;

      // Count smaller unused elements
      for (int j = 0; j < perm[i]; ++j) {
        if (!used[j]) {
          ++smaller;
        }
      }

      index += smaller * factorials[i];
      used[perm[i]] = true;
    }

    return index;
  }
};
} // namespace amos