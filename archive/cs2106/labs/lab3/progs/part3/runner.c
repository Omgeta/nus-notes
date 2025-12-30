#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "runner.h"
#include "h2o.h"

#define DEBUG

// Prints 'H'
// Modify the sleep duration to simulate variability in thread execution time
void releaseHydrogen(void) {
    putchar('H'); fflush(stdout); 
}

// Prints 'O'
// Modify the sleep duration to simulate variability in thread execution time
void releaseOxygen(void) { 
    putchar('O'); fflush(stdout); 
}


int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [molecules]\n", argv[0]);
        return 1;
    }
    int molecules = (argc == 2) ? atoi(argv[1]) : 8; // default 8 H2O

    int H = 2 * molecules, O = molecules;

    // Initialise necessary synchronization variables
    h2o_init();

    // start each task
    pthread_t hydrogen_threads[H];
    pthread_t oxygen_threads[O];

    int i;
    for (i = 0; i < H; i++) {
        pthread_create(&hydrogen_threads[i], NULL, run_hydrogen_thread, NULL);
    }
    for (i = 0; i < O; i++) {
        pthread_create(&oxygen_threads[i], NULL, run_oxygen_thread, NULL);
    }

#ifdef DEBUG
    printf("Created %d hydrogen and %d oxygen threads\n", H, O);
#endif


    // Join hydrogen and oxygen threads
    for (i = 0; i < H; i++) {
        pthread_join(hydrogen_threads[i], NULL);
    }
    for (i = 0; i < O; i++) {
        pthread_join(oxygen_threads[i], NULL);
    }
#ifdef DEBUG
    printf("All threads have finished their execution\n");
#endif

    // Cleanup
    h2o_destroy();

#ifdef DEBUG
    printf("Freed all necessary resources\n");
#endif

    return 0;
}

void *run_hydrogen_thread(void *)
{
    // Simulates thread reaching the factory at different times
    int sleep_time = (rand() % 500) * 200;
    usleep(sleep_time); 

    hydrogen_thread(releaseHydrogen);
    pthread_exit(0);
}

void *run_oxygen_thread(void *)
{
    // Simulates thread reaching the factory at different times
    usleep((rand() % 500) * 200); 

    oxygen_thread(releaseOxygen);
    pthread_exit(0);
}
