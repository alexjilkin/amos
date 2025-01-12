#include "AMOCache.h"
#include "LehmerCode.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <vector>
using namespace amos;

int main() {
  int k = 3;

  // Creates AMO Cache up to k vertices
  AmoCache cache(k);

  u_int16_t upper_triangle = 0b11;

  AmoList amos = cache.get_amos(k, upper_triangle);

  vector<vector<int>> decoded_permutations;
  for (const auto &amo : amos) {
    decoded_permutations.push_back(LehmerCode::decode(amo, k));
  }

  cout << "\nAdjacency matrix: \n";
  cache.print_upper_triangle(upper_triangle, k);
  cout << "\nAMOs:\n";
  for (size_t i = 0; i < amos.size(); ++i) {
    cout << "Lehmer code: " << amos[i] << ", ";
    cout << "permutation: [";
    for (size_t j = 0; j < k; ++j) {
      cout << decoded_permutations[i][j];
      if (j < k - 1) {
        cout << ", ";
      }
    }
    cout << "]\n";
  }
  return 0;
}
