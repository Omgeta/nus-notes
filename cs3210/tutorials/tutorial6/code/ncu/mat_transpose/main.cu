#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include "mat_transpose.cuh"

int main(int argc, char** argv) {
    int n = 8192;

    if (argc != 1 && argc != 2) {
        std::cout << "Usage: ./mat_transpose [n]\n";
        std::cout << "  n: matrix dimension (must be divisible by 128)\n";
        return 1;
    }

    if (argc == 2) {
        n = std::atoi(argv[1]);
    }

    if (n % 128 != 0) {
        std::cerr << "n must be divisible by 128\n";
        return 1;
    }

    std::vector<float> h_A((size_t)n * n, 0.0f);
    std::vector<float> h_B((size_t)n * n, 0.0f);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            h_A[i * n + j] = static_cast<float>(i * n + j);
        }
    }

    transpose(h_A.data(), h_B.data(), n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (h_B[i * n + j] != static_cast<float>(j * n + i)) {
                std::printf("Incorrect! B[%d][%d] = %.2f, expected %.2f", i, j, h_B[i * n + j], static_cast<float>(j * n + i));
                return 1;
            }
        }
    }

    std::cout << "Correct!";

    return 0;
}
