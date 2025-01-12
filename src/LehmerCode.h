#pragma once

#include <vector>
#include <iostream>
#include <cstdint>
#include <stdexcept>

class LehmerCoder {
public:
    // Encode: Compute the lexicographic index (Lehmer code) of a permutation
    static uint16_t encode(const std::vector<int>& perm) {
        int n = perm.size();
        uint16_t index = 0;

        // Compute factorials in reverse order
        std::vector<int> factorials = compute_factorials(n);

        // Track used elements
        std::vector<bool> used(n, false);

        for (int i = 0; i < n; ++i) {
            int smaller = 0;

            // Count smaller unused elements
            for (int j = 0; j < perm[i]; ++j) {
                if (!used[j]) {
                    ++smaller;
                }
            }

            index += smaller * factorials[i];
            used[perm[i]] = true;
        }

        return index;
    }

    // Decode: Reconstruct a permutation from its Lehmer code
    static std::vector<int> decode(uint16_t code, int n) {
        if (n < 1 || factorial(n) > 65535) {
            throw std::invalid_argument("Permutation size exceeds uint16_t range.");
        }

        // Compute factorials in reverse order
        std::vector<int> factorials = compute_factorials(n);

        // Initialize used elements
        std::vector<bool> used(n, false);
        std::vector<int> perm(n);

        for (int i = 0; i < n; ++i) {
            int quotient = code / factorials[i];
            code %= factorials[i];

            // Find the (quotient + 1)-th unused element
            int count = 0;
            for (int j = 0; j < n; ++j) {
                if (!used[j]) {
                    if (count == quotient) {
                        perm[i] = j;
                        used[j] = true;
                        break;
                    }
                    ++count;
                }
            }
        }

        return perm;
    }

private:
    // Helper: Compute factorial of n
    static int factorial(int n) {
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    // Helper: Compute factorials in reverse order for a given size
    static std::vector<int> compute_factorials(int n) {
        std::vector<int> factorials(n);
        factorials[n - 1] = 1;
        for (int i = n - 2; i >= 0; --i) {
            factorials[i] = factorials[i + 1] * (n - 1 - i);
        }
        return factorials;
    }
};