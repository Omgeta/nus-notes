#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    // We just want 5 threads for this program
    omp_set_num_threads(5);

    // Static schedule 
    #pragma omp parallel
    {
        /* Obtain thread id */
        int thread_id = omp_get_thread_num();
        // Print out the thread number and the for loop iteration with openmp
        #pragma omp for schedule (static, 2)
        for (int i = 0; i < 10; i++) {
            printf("Static schedule: hello World from thread = %d, i = %d\n", thread_id, i);
        }
    }

    // Only one thread will print this
    // The threads join after the parallel region
    printf("\n");

    // Dynamic schedule
    #pragma omp parallel 
    {
        /* Obtain thread id */
        int thread_id = omp_get_thread_num();
        # pragma omp for schedule (dynamic, 2)
        for (int i = 0; i < 10; i++) {
            printf("Dynamic schedule: hello World from thread = %d, i = %d\n", thread_id, i);
        }
    }
}
