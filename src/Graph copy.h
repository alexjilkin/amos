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

using AdjMatrix = vector<vector<int>>;

struct Graph {
  int n;
  AdjMatrix adj;

  Graph(int n) : n(n), adj(n) {}

  // Add an undirected edge (u,v)
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  // Remove an undirected edge (u,v) if it exists
  void remove_edge(int u, int v) {
    auto remove_from = [&](int x, int y) {
      auto &nbrs = adj[x];
      nbrs.erase(std::remove(nbrs.begin(), nbrs.end(), y), nbrs.end());
    };
    remove_from(u, v);
    remove_from(v, u);
  }

  // Get neighbors of a vertex
  set<int> get_neighbors(int u) const {
    return set<int>(adj[u].begin(), adj[u].end());
}
  // Check if the graph is connected via BFS/DFS
  bool is_connected() const {
    // If there are no vertices or 1 vertex, it's trivially connected
    if (n <= 1)
      return true;

    // Find a start vertex that has at least one edge or just start at 0
    int start = 0;
    // BFS/DFS from 'start'
    vector<bool> visited(n, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);

    int countVisit = 1;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int w : adj[u]) {
        if (!visited[w]) {
          visited[w] = true;
          countVisit++;
          q.push(w);
        }
      }
    }
    // If we visited all vertices => connected
    return (countVisit == n);
  }

  // Check if the graph is chordal using Maximum Cardinality Search (MCS).
  // If MCS yields a valid perfect elimination ordering, the graph is chordal.
  bool is_chordal() const {
    // MCS uses a "weight" array to pick the next vertex with highest weight
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
        // handle all)
        return false;
      }

      // place v in position 'step' of the ordering
      peo[step] = v;
      chosen[v] = true;

      // increment weight of neighbors of v that are not chosen
      for (int w : adj[v]) {
        if (!chosen[w]) {
          weight[w]++;
        }
      }
    }

    // 'peo' is the reverse MCS order (the last picked is at peo[n-1])
    // But MCS doesn't guarantee the order is PEO automatically. We must verify.

    // Check if each vertex has a clique among its higher-index neighbors in
    // 'peo'. A standard check: for each vertex v in peo, look at the neighbors
    // that come after v in the peo. They must form a clique. We'll store
    // positions of each vertex in the ordering to do that quickly.
    vector<int> posInOrder(n);
    for (int i = 0; i < n; i++) {
      posInOrder[peo[i]] = i;
    }

    // for each v in peo (in order: peo[0], peo[1], ... peo[n-1])
    for (int i = 0; i < n; i++) {
      int v = peo[i];
      // collect neighbors w of v that appear after v in peo
      // i.e. posInOrder[w] > posInOrder[v] (= i)
      vector<int> higherNeighbors;
      for (int w : adj[v]) {
        if (posInOrder[w] > i) {
          higherNeighbors.push_back(w);
        }
      }
      // they must form a clique
      // a quick way: each pair in higherNeighbors must be connected
      // This can be O(d^2) in the worst case, but n is small enough for
      // demonstration.
      for (int a = 0; a < (int)higherNeighbors.size(); a++) {
        for (int b = a + 1; b < (int)higherNeighbors.size(); b++) {
          // check adjacency
          if (!are_connected(higherNeighbors[a], higherNeighbors[b])) {
            return false;
          }
        }
      }
    }

    return true;
  }
  void print_edges() const {
    set<pair<int, int>> edges; 
    for (int u = 0; u < n; ++u) {
      for (int v : adj[u]) {
        if (u < v) { 
          edges.insert({u, v});
        }
      }
    }

    for (const auto &edge : edges) {
      cout << "(" << edge.first << ", " << edge.second << ")" << endl;
    }
  }

private:
  // Check if two vertices are directly connected (for chordality check).
  // We'll do a quick membership test. Because adj[u] might not be sorted,
  // we can do a linear check. For small n, that's fine.
  bool are_connected(int u, int v) const {
    for (int w : adj[u]) {
      if (w == v)
        return true;
    }
    return false;
  }
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