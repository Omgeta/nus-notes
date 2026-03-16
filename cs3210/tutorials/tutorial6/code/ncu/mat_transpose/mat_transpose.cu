#include <cuda_runtime.h>
#include <iostream>
#include "mat_transpose.cuh"

__global__ void transpose_kernel(const float* A, float* B, int n)
{
    int row_read = blockIdx.y * blockDim.y + threadIdx.y;
    int col_read = blockIdx.x * blockDim.x + threadIdx.x;

    int row_write = blockIdx.x * blockDim.x + threadIdx.y;
    int col_write = blockIdx.y * blockDim.y + threadIdx.x;

    __shared__ float smem[32][33];

    if (row_read < n && col_read < n) {
        float data = A[row_read * n + col_read];
        smem[threadIdx.x][threadIdx.y] = data;
    }
    __syncthreads();
    if (row_write < n && col_write < n) {
        float data = smem[threadIdx.y][threadIdx.x];
        B[row_write * n + col_write] = data;
    }

}

void transpose(float* h_A, float* h_B, int n) {
    size_t bytes = (size_t)n * (size_t)n * sizeof(float);
    float *d_A = nullptr, *d_B = nullptr;

    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);

    cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice);

    dim3 block(32, 32);
    dim3 grid((n + block.x - 1) / block.x, (n + block.y - 1) / block.y);

    transpose_kernel<<<grid, block>>>(d_A, d_B, n);
    cudaDeviceSynchronize();

    cudaMemcpy(h_B, d_B, bytes, cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
}
