/*
 * Hello World in CUDA
 *
 * CS3210
 *
 * This program should print "HELLO WORLD" if successful.
 *
 */

#include <stdio.h>

#define N       32 

// #define      DISCRETE

__global__ void hello(char *a, int len)
{
	int blockId = blockIdx.x + blockIdx.y * gridDim.x
                      + gridDim.x * gridDim.y * blockIdx.z;
        int tid = blockId * (blockDim.x * blockDim.y)
                  + (threadIdx.y * blockDim.x) + threadIdx.x;
        if (tid >= len)
                return;
        a[tid] += 'A' - 'a';
}

int main()
{
        // original string
        char a[N] = "hello@world";
        // length
        int len = strlen(a);
        // pointer to the string on device
        char* ad;
        // pointer to the final string on host
        char* ah;
        // CUDA returned error code
        cudaError_t rc;

	// dimensions
	dim3 gridDimensions(2,2,2);
	dim3 blockDimensions(2,4);


        //allocate space for the string on device (GPU) memory
        cudaMalloc((void**)&ad, N);
        cudaMemcpy(ad, a, N, cudaMemcpyHostToDevice);

        // launch the kernel
        hello<<<gridDimensions, blockDimensions>>>(ad, len);
        cudaDeviceSynchronize();

	// for discrete GPUs, get the data from device memory to host memory
        cudaMemcpy(a, ad, N, cudaMemcpyDeviceToHost);
        ah = a;

        // was there any error?
        rc = cudaGetLastError();
        if (rc != cudaSuccess)
                printf("Last CUDA error %s\n", cudaGetErrorString(rc));

        // print final string
        printf("%s!\n", ah);

        // free memory
        cudaFree(ad);

        return 0;
}

