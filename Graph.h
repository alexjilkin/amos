#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <stdexcept>

class Graph {
public:
    // Constructor: Initializes an upper triangular adjacency matrix for `n` vertices
    Graph(int n) : n_(n) {
        if (n_ < 1) {
            throw std::runtime_error("Number of vertices must be at least 1.");
        }

        // Initialize the upper triangular adjacency matrix
        adj_matrix.resize(n_);
        for (int i = 0; i < n_; i++) {
            adj_matrix[i].resize(n_ - i - 1, false); // Row i has n-i-1 columns
        }
    }

    // Add an edge between vertices u and v
    void addEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= n_ || v >= n_ || u == v) {
            throw std::runtime_error("Invalid vertices for addEdge.");
        }

        if (u > v) std::swap(u, v); // Ensure u < v for upper triangular storage

        adj_matrix[u][v - u - 1] = true; // Set the appropriate bit
    }

    // Print all edges in the graph
    void print() const {
        std::cout << "Graph with " << n_ << " vertices:\n";
        for (int u = 0; u < n_; u++) {
            for (int v = u + 1; v < n_; v++) {
                if (adj_matrix[u][v - u - 1]) {
                    std::cout << "(" << u << "," << v << ") ";
                }
            }
        }
        std::cout << std::endl;
    }

private:
    int n_;                                // Number of vertices
    std::vector<std::vector<bool>> adj_matrix; // Upper triangular adjacency matrix
};

#endif // GRAPH_H
