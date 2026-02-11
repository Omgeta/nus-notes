#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    uint64_t sum = 0;

    // Declaring explicitly that sum is shared between all the threads
    #pragma omp parallel shared(sum)
    {
        // Add to the sum over a large for loop (with a delay to make it take longer) using openmp
        #pragma omp for schedule (static, 2)
        for (int i = 0; i < 10000000; i++) {
            #pragma omp atomic
            sum += i;
        }
    }

    printf("Sum = %lu\n", sum);

}
