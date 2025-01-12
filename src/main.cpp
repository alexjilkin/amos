#include "AMOCache.h"
#include "LehmerCode.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <vector>
using namespace amos;

int main() {
  int k = 6;

  auto start = std::chrono::high_resolution_clock::now();

  // Creates AMO Cache up to k vertices
  AmoCache cache(k);

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now() - start);

  // Print the time taken to create the cache
  std::cout << "Time to create AMO Cache: " << duration.count() << " ms\n";

  u_int16_t upper_triangle = 0b110010;

  AmoList amos = cache.get_amos(k, upper_triangle);

  std::vector<std::vector<int>> decoded_permutations;
  for (const auto &amo : amos) {
    decoded_permutations.push_back(LehmerCode::decode(amo, k));
  }

  std::cout << "\nAdjacency matrix: \n";
  cache.print_upper_triangle(upper_triangle, k);
  std::cout << "\nAMOs:\n";
  for (size_t i = 0; i < amos.size(); ++i) {
    std::cout << "Lehmer code: " << amos[i] << ", ";
    std::cout << "permutation: [";
    for (size_t j = 0; j < k; ++j) {
      std::cout << decoded_permutations[i][j];
      if (j < k - 1) {
        std::cout << ", ";
      }
    }
    std::cout << "]\n";
  }

  return 0;
}