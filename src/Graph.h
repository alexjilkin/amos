#pragma once

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <vector>

using namespace std;

namespace amos {
/**
 * A simple undirected graph structure with:
 * - adjacency list
 * - is_connected()
 * - is_chordal() using Maximum Cardinality Search (MCS)
 */

using AdjMatrix = vector<vector<bool>>;

struct Graph {
  int n;
  AdjMatrix adj;

  Graph(int n) : n(n), adj(n, vector<bool>(n, 0)) {}

  set<int> get_neighbors(int u) const {
    set<int> neighbors;
    for (int v = 0; v < n; v++) {
      if (adj[u][v]) { 
        neighbors.insert(v);
      }
    }
    return neighbors;
  }

  void add_edge(int u, int v) {
    adj[u][v] = true;
    adj[v][u] = true;
  }

  // Remove an undirected edge (u, v)
  void remove_edge(int u, int v) {
    adj[u][v] = false;
    adj[v][u] = false;
  }

  bool edge_exist(int u, int v) const { return adj[u][v] == true; }

  // Check if the graph is connected via BFS/DFS
  bool is_connected() const {
    // If there are no vertices or only one vertex, it's trivially connected
    if (n <= 1)
      return true;

    // BFS/DFS to check connectivity
    vector<bool> visited(n, false);
    queue<int> q;

    // Start from the first vertex
    visited[0] = true;
    q.push(0);

    int countVisit = 1; // Count of visited vertices

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      // Iterate over all vertices to check adjacency
      for (int v = 0; v < n; v++) {
        if (adj[u][v] &&
            !visited[v]) { // Check if there's an edge and if it's unvisited
          visited[v] = true;
          countVisit++;
          q.push(v);
        }
      }
    }

    // The graph is connected if all vertices are visited
    return (countVisit == n);
  }

  // Check if the graph is chordal using Maximum Cardinality Search (MCS).
  // If MCS yields a valid perfect elimination ordering, the graph is chordal.
  bool is_chordal() const {
    vector<int> weight(n, 0);      // MCS weights
    vector<bool> chosen(n, false); // Track if vertex is chosen
    vector<int> peo(n, -1);        // Perfect elimination order

    // Maximum Cardinality Search (MCS)
    for (int step = n - 1; step >= 0; step--) {
      int maxW = -1, v = -1;
      for (int i = 0; i < n; i++) {
        if (!chosen[i] && weight[i] > maxW) {
          maxW = weight[i];
          v = i;
        }
      }
      if (v == -1)
        return false; // No vertex found

      peo[step] = v;
      chosen[v] = true;

      for (int w = 0; w < n; w++) {
        if (edge_exist(v, w) && !chosen[w]) {
          weight[w]++;
        }
      }
    }

    // Verify Perfect Elimination Order (PEO)
    vector<int> posInOrder(n);
    for (int i = 0; i < n; i++) {
      posInOrder[peo[i]] = i;
    }

    for (int i = 0; i < n; i++) {
      int v = peo[i];
      vector<int> higherNeighbors;
      for (int w = 0; w < n; w++) {
        if (edge_exist(v, w) && posInOrder[w] > i) {
          higherNeighbors.push_back(w);
        }
      }

      std::set<int> higherSet(higherNeighbors.begin(), higherNeighbors.end());
      for (int a : higherNeighbors) {
        for (int b : higherNeighbors) {
          if (a != b && higherSet.find(b) == higherSet.end()) {
            return false;
          }
        }
      }
    }

    return true;
  }

private:
};

struct AdjMatrixHash {
  size_t operator()(const AdjMatrix &vec) const {
    std::hash<int> hasher;
    size_t seed = 0;

    // Iterate over each row in the 2D vector
    for (const auto &row : vec) {
      for (int value : row) {
        // Combine the hash of each element
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
    }

    return seed;
  }
};

} // namespace amos