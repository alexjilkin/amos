#pragma once
#include "Graph.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>
#include <chrono>

using namespace std;

namespace amos {
/**
 *  Get all UCCGs of size k in a brute force method
 */
class UCCG {
public:
  static vector<Graph> generate(int k) {
    // auto start = chrono::high_resolution_clock::now();

    vector<Graph> result;

    // 1) List all possible edges among k vertices
    //    We'll store them as pairs (u,v) with u < v
    vector<pair<int, int>> allEdges;
    for (int u = 0; u < k; u++) {
      for (int v = u + 1; v < k; v++) {
        allEdges.push_back({u, v});
      }
    }


    // Connected graph on k nodes must have at least (k-1) edges
    int totalEdges = (int)allEdges.size();

    for (int r = k - 1; r <= totalEdges; r++) {
      // Generate all combinations of "allEdges" of size r
      vector<int> combination;
      generate_combinations(allEdges, 0, r, combination, result, k);
    }

    // auto duration = chrono::duration_cast<chrono::milliseconds>(
    //   chrono::high_resolution_clock::now() - start);

    // std::cout << "For k=" << k << " There are " << result.size() << " UCCGs and It took " << duration.count() << "ms\n";
    return result;
  }

private:
  // Helper to generate combinations of size 'r' from allEdges, starting at
  // index 'start'.
  static void generate_combinations(const vector<pair<int, int>> &allEdges,
                                    int start, int r, vector<int> &combo,
                                    vector<Graph> &result, int k) {
    // If we've chosen r edges, build and check the graph
    if (r == 0) {
      // Build graph with edges in 'combo'
      Graph g(k);
      for (int idx : combo) {
        auto &e = allEdges[idx];
        g.add_edge(e.first, e.second);
      }
      // Check connected & chordal
      if (g.is_connected() && g.is_chordal()) {
        result.push_back(g);
      }
      return;
    }

    // If not enough elements left to pick
    if (start + r > (int)allEdges.size()) {
      return;
    }

    // Option 1: pick edge at 'start'
    combo.push_back(start);
    generate_combinations(allEdges, start + 1, r - 1, combo, result, k);
    combo.pop_back();

    // Option 2: skip edge at 'start'
    generate_combinations(allEdges, start + 1, r, combo, result, k);
  }
};

} // namespace amos