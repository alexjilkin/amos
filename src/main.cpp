// #include "amo.h"
// #include "uccg.h"
#include "AMOCache.h"
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

  AmoCache cache(k);

  cache.print_all();

  std::cout << "Done!";
  return 0;
}
