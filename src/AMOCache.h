#pragma once

#include "Graph.h"
#include "amo.h"
#include "uccg.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

namespace amos {

class AmoCache {
public:
  AmoCache(int max_k) {
    for (int k = 3; k <= max_k; ++k) {
      generate_for_k(k);
    }
  }

  void print_all() const {
    for (const auto &level : L) {
      int k = level.first;
      cout << "k = " << k << ":\n";

      for (const auto &entry : level.second) {
        auto &G = entry.first;
        auto &amos = entry.second;

        for (int x = 0; x < k; x++) {
          cout << "[";
          for (int y = 0; y < k; y++) {
            cout << G[x][y] << ", ";
          }

          cout << "]" << "\n";
        }

        cout << "AMOs:\n";
        for (const auto &amo : amos) {
          cout << amo << "\n";
        }
      }
    }
  }

private:
  //AMO encoded to lehmer
  using AmoList = vector<u_int16_t>;

  unordered_map<int, unordered_map<AdjMatrix, AmoList, AdjMatrixHash>>
      L; // L[k][G] stores AMOs for each graph G of size k

  void generate_for_k(int k) {
    auto graphs = UCCGBrute::generate(k); // Generate UCCGs for k

    unordered_map<AdjMatrix, AmoList, AdjMatrixHash> graph_to_amos;

    for (size_t idx = 0; idx < graphs.size(); ++idx) {
      Graph &G = graphs[idx];

      // Prepare data structures for AMO generation
      vector<set<int>> A;
      A.push_back(set<int>()); // A[0] initially contains all vertices
      for (int i = 0; i < G.n; ++i) {
        A[0].insert(i);
      }

      vector<u_int16_t> amos;
      vector<int> to;

      AMOs::generate(G, A, amos, to);

      // Store the AMOs for the current graph
      auto adj_matrix = G.adj;
      graph_to_amos[adj_matrix] = std::move(amos);
    }

    // Store the results for this value of k
    L[k] = std::move(graph_to_amos);
  }
};
} // namespace amos