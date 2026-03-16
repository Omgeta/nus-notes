#include <cuda_runtime.h>
#include <vector>
#include <iostream>
#include "transform.cuh"

__global__ void transform(float *in, int N, int iters)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < N) {
        float x = in[i];

        for (int k = 0; k < iters; k++) {
            x = __sinf(x) + __cosf(x) + sqrtf(fabsf(x) + 1.0f);
        }

        in[i] = x;
    }
}

std::vector<std::vector<float>> process_problems(const std::vector<std::vector<float>>& inputs, int N, int iters) {
    size_t bytes = (size_t)N * sizeof(float);
    size_t problems_count = inputs.size();

    float *d_data = nullptr;
    cudaMalloc(&d_data, bytes * problems_count);

    int threads = 256;
    int blocks = (N + threads - 1) / threads;

    std::vector<std::vector<float>> outputs;
    outputs.reserve(inputs.size());

    for (size_t p = 0; p < inputs.size(); ++p) {
        outputs.emplace_back(N);

        cudaMemcpy(d_data + p * N, inputs[p].data(), bytes, cudaMemcpyHostToDevice);

        transform<<<blocks, threads>>>(d_data + p * N, N, iters);

        cudaMemcpy(outputs[p].data(), d_data + p * N, bytes, cudaMemcpyDeviceToHost);
    }

    cudaFree(d_data);
    
    return outputs;
}
