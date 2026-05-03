#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "transform.cuh"

__global__ void hello() {
    printf("Hello, CUDA!\n");    
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: ./transform <problems> <N> <iters>\n";
        return 1;
    }

    int P = std::atoi(argv[1]);
    int N = std::atoi(argv[2]);
    int iters = std::atoi(argv[3]);

    std::vector<std::vector<float>> inputs(P, std::vector<float>(N));
    for (int p = 0; p < P; ++p) {
        for (int i = 0; i < N; ++i) {
            inputs[p][i] = static_cast<float>(p * N + i);
        }
    }

    hello<<<1, 1>>>();

    auto t0 = std::chrono::high_resolution_clock::now();
    auto outputs = process_problems(inputs, N, iters);
    auto t1 = std::chrono::high_resolution_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::cout << "Time elapsed: " << ms << " ms\n";

    return 0;
}
