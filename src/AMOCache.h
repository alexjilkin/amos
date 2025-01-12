#pragma once

#include "Graph.h"
#include "amo.h"
#include "uccg.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace amos {

class AmoCache {
public:
  AmoCache(int max_k) {
    for (int k = 2; k <= max_k; ++k) {
      generate_for_k(k);
    }
  }

  void print_all() const {
    for (const auto &level : L) {
      int k = level.first;
      std::cout << "k = " << k << ":\n";

      for (const auto &entry : level.second) {
        auto &G = entry.first;
        auto &amos = entry.second;

        for (int x = 0; x < 3; x++) {
          cout << "[";
          for (int y = 0; y < 3; y++) {
            cout << G[x][y] << ", ";
          }

          cout << "]" << "\n";
        }

        std::cout << "AMOs:\n";
        for (const auto &amo : amos) {
          std::cout << "{ ";
          for (int x : amo)
            std::cout << x << " ";
          std::cout << "}\n";
        }
      }
    }
  }

private:
  using AmoList = std::vector<std::vector<int>>;

  std::unordered_map<int, std::unordered_map<AdjMatrix, AmoList, AdjMatrixHash>>
      L; // L[k][G] stores AMOs for each graph G of size k

  void generate_for_k(int k) {
    auto graphs = UCCGBrute::generate(k); // Generate UCCGs for k
    std::unordered_map<AdjMatrix, AmoList, AdjMatrixHash> graph_to_amos;

    for (size_t idx = 0; idx < graphs.size(); ++idx) {
      Graph &G = graphs[idx];

      // Prepare data structures for AMO generation
      std::vector<std::set<int>> A;
      A.push_back(std::set<int>()); // A[0] initially contains all vertices
      for (int i = 0; i < G.n; ++i) {
        A[0].insert(i);
      }

      std::vector<std::vector<int>> amos;
      std::vector<int> to;

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