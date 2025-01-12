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

  // Get neighbors of a vertex
  set<int> get_neighbors(int u) const {
    return set<int>(adj[u].begin(), adj[u].end());
  }

  void add_edge(int u, int v) {
    adj[u][v] = true;
    adj[v][u] = false;
  }

  // Remove an undirected edge (u, v)
  void remove_edge(int u, int v) {
    adj[u][v] = false;
    adj[v][u] = false;
  }

  // Check if two vertices are directly connected
  bool are_connected(int u, int v) const { return adj[u][v] == true; }

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

  bool is_chordal() const {
    // MCS uses a "weight" array to pick the next vertex with the highest weight
    // that is not yet chosen.
    // Then we produce an ordering and verify if it is indeed a PEO.

    vector<int> weight(n, 0);      // MCS weights
    vector<bool> chosen(n, false); // track if vertex is chosen
    vector<int> peo(n, -1); // will store the order (perfect elimination order)

    for (int step = n - 1; step >= 0; step--) {
      // pick an unchosen vertex of maximum weight
      int maxW = -1, v = -1;
      for (int i = 0; i < n; i++) {
        if (!chosen[i] && weight[i] > maxW) {
          maxW = weight[i];
          v = i;
        }
      }
      if (v == -1) {
        // no vertex found (shouldn't happen if the graph is connected or if we
        // handle all vertices)
        return false;
      }

      // place v in position 'step' of the ordering
      peo[step] = v;
      chosen[v] = true;

      // increment weight of neighbors of v that are not chosen
      for (int w = 0; w < n; w++) {
        if (adj[v][w] &&
            !chosen[w]) { // Check adjacency and whether the vertex is chosen
          weight[w]++;
        }
      }
    }

    // 'peo' is the reverse MCS order (the last picked is at peo[n-1])
    // But MCS doesn't guarantee the order is PEO automatically. We must verify.

    // Check if each vertex has a clique among its higher-index neighbors in
    // 'peo'.
    vector<int> posInOrder(n);
    for (int i = 0; i < n; i++) {
      posInOrder[peo[i]] = i;
    }

    // for each vertex in peo (in order: peo[0], peo[1], ... peo[n-1])
    for (int i = 0; i < n; i++) {
      int v = peo[i];
      // collect neighbors of v that appear after v in peo
      vector<int> higherNeighbors;
      for (int w = 0; w < n; w++) {
        if (adj[v][w] && posInOrder[w] > i) {
          higherNeighbors.push_back(w);
        }
      }
      // they must form a clique
      // Check if each pair of higherNeighbors are directly connected
      for (int a = 0; a < (int)higherNeighbors.size(); a++) {
        for (int b = a + 1; b < (int)higherNeighbors.size(); b++) {
          if (!adj[higherNeighbors[a]][higherNeighbors[b]]) {
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
  size_t operator()(const vector<vector<bool>> &vec) const {
    std::hash<bool> hasher;
    size_t seed = 0;

    // Iterate over each row in the 2D boolean vector
    for (const auto &row : vec) {
      for (bool value : row) {
        // Combine the hash of each element
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
    }

    return seed;
  }
};

} // namespace amos