__global__ void transpose_kernel(const float* A, float* B, int n)
{
    int row_read = blockIdx.y * blockDim.y + threadIdx.y;
    int col_read = blockIdx.x * blockDim.x + threadIdx.x;

    int row_write = blockIdx.x * blockDim.x + threadIdx.y;
    int col_write = blockIdx.y * blockDim.y + threadIdx.x;

    __shared__ float smem[16][16];

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