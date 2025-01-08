#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <queue>
#include <algorithm> // for std::remove
#include "Graph.h"

class AMO {
public:
    AMO(int k) : k_(k) {
        if (k_ * (k_ - 1) / 2 > 64) {
            throw std::runtime_error("Too many vertices for uint64_t.");
        }
        adj.resize(k);
    }


    // // Start enumeration using the Enumerator class
    // void enumerate() {
    //     Enumerator enumerator(k_, adj);
    //     enumerator.enumerate();
    // }

private:
    int k_;
    std::vector<std::vector<int>> adj; // Adjacency list
};

int main() {
    Graph graph(7);

    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(5, 6);

    graph.print();
    // graph.enumerate();

    return 0;
}
