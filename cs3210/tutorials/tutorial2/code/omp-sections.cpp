#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
Simple example of openmp sections and why we need them
*/
int main(int argc, char* argv[])
{
    
    #pragma omp parallel 
    {
        // Try adding a "nowait" after "sections" 
        #pragma omp sections
        {
            #pragma omp section
            {
                printf("Hello World from thread = %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Hello World from thread = %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Hello World from thread = %d\n", omp_get_thread_num());
            }
            #pragma omp section
            {
                printf("Hello World from thread = %d\n", omp_get_thread_num());
            }
        }

        #pragma omp single
        printf("Single thread printing - thread = %d\n", omp_get_thread_num());

        #pragma omp master
        printf("Master thread printing - thread = %d\n", omp_get_thread_num());
    }


}
