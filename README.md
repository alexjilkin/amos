# AMOCache Library

`AMOCache` generates and caches Acyclic Moral Orientations (AMOs) for chordal graphs.

## Usage

### 1. Include the Header
Include the main header file in your project:
```cpp
#include "AMOCache.h"
```

### 2. Initialize the Cache
Create an `AMOCache` object by specifying the maximum graph size \( k \) you want to process, currently supporting up to 6. This will precompute and cache all AMOs for graphs of sizes 2 through \( k \).
```cpp
amos::AmoCache cache(max_k);
```

### 3. Query AMOs
Use the get_amos method to retrieve the cached AMOs for a specific graph, identified by its compressed upper triangular adjacency matrix (GraphKey/u_int16_t). The AMOs are encoded using the Lehmer code and stored as u_int16 values.
Use `LehmerCode::decode` to get the original permutation.
```cpp
u_int16_t upper_triangle = /* compressed matrix */;
auto amos = cache.get_amos(k, upper_triangle);
```

### 4. Print AMOs
To inspect the cached AMOs, you can use the `print` method:
```cpp
cache.print();
```

## Example

See main.cpp for example.

With the input upper triangle adjacency matrix encoded in u_int16, beginning from the k*(k-1)/2 significant bit.
```
0b110010
```
The program will output:
```
AMO generation completed for all k up to 4.

Adjacency matrix: 
0 1 1 0 
0 0 0 1 
0 0 0 0 
0 0 0 0 

AMOs:
Lehmer code: 0, permutation: [0, 1, 2, 3]
Lehmer code: 6, permutation: [1, 0, 2, 3]
Lehmer code: 12, permutation: [2, 0, 1, 3]
Lehmer code: 20, permutation: [3, 1, 0, 2]
...
```

## Notes
- Ensure the maximum graph size \( k \) does not exceed the supported range of the compressed representation.
- The library assumes graphs are represented using upper triangular adjacency matrices for compactness and efficiency.

