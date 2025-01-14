#pragma once

#include "AMOs.h"
#include "Graph.h"
#include "UCCG.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

using GraphKey = u_int16_t;

namespace amos {

using AmoList = vector<u_int16_t>;

class AmoCache {
public:
  AmoCache(int max_k) : L(max_k + 1), max_k(max_k) {
    for (int k = 2; k <= max_k; ++k) {
      std::cout << "Generating AMOs for k = " << k << "..." << std::endl;
      generate_for_k(k);
    }
    std::cout << "AMO generation completed for all k up to " << max_k << "."
              << std::endl;
  }

  static void print_upper_triangle(GraphKey compressed, int n) {
    int total_bits = (n * (n - 1)) / 2;
    int bit_position = total_bits - 1;

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (j > i) {
          std::cout << ((compressed & (1 << bit_position)) ? "1 " : "0 ");
          --bit_position; // Move to the next bit (from left-to-right
                          // perspective)
        } else {
          std::cout << "0 ";
        }
      }
      std::cout << std::endl;
    }
  }

  void print() const {
    for (int k = 0; k < max_k; ++k) {
      cout << "k = " << k << ":\n";

      for (GraphKey G = 0; G < L[k].size(); ++G) {
        auto &amos = L[k][G]; // Access the AmoList at index G

        if (amos.empty())
          continue; // Skip if the AmoList is empty

        print_upper_triangle(
            G, k); // Use G as the GraphKey and k as the size parameter

        std::cout << "AMOs:\n";
        for (const auto &amo : amos) {
          std::cout << amo << "\n";
        }
      }
    }
  }

  AmoList get_amos(int k, GraphKey key) { return L[k][key]; }

private:
  // AMO encoded to lehmer
  vector<vector<AmoList>> L;

  int max_k;

  void generate_for_k(int k) {    
    auto graphs = UCCG::generate(k); // Generate UCCGs for k

    vector<AmoList> amos_list;
    amos_list.resize(1 << (k * (k - 1)) / 2);

    for (size_t idx = 0; idx < graphs.size(); ++idx) {
      Graph &G = graphs[idx];

      vector<u_int16_t> A;
      A.push_back(0); // A[0] initially contains all vertices
      for (int i = 0; i < G.n; ++i) {
        A[0] |= (1 << i);
      }

      vector<u_int16_t> amos;
      vector<int> to;

      AMOs::generate(G, A, amos, to);

      // Store the AMOs for the current graph
      u_int16_t upper_triangle = G.get_upper_triangle();
      amos_list[upper_triangle] = std::move(amos);
    }

    // Store the results for this value of k
    L[k] = std::move(amos_list);
  }
};
} // namespace amos