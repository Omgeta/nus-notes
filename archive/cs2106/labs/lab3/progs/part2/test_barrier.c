#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include "barrier.h"

#define NUM_PROCESSES   6
#define MAX_SLEEP       1000000  // Maximum sleep period in microseconds
#define NUM_TESTS       6        // Number of test

int main() {
    int i, pid;
    int sleep_time[NUM_PROCESSES];

    init_barrier(NUM_PROCESSES+1);

    srand(time(NULL));
    for(i=0; i<NUM_PROCESSES; i++) {

        // The children will all slip at different amounts of time
        sleep_time[i] = (int) (((float) rand() / RAND_MAX) * MAX_SLEEP);
        if((pid = fork()) == 0) {
            srand(time(NULL));
            break;
        }
    }

    for (int n_test = 1; n_test <= NUM_TESTS; n_test++) {
        if(pid == 0) {
            usleep(sleep_time[i]);

            // Reach the barrier.
            printf("\tChild %d slept for %3.2f seconds and has now reached the barrier\n", i, sleep_time[i]/1000000.0);
            reach_barrier();
        }
        else {
            // Parent will just wait at barrier for all children to return
            printf("**\tTest #%d      **\n", n_test);
            printf("**Parent waiting for children**\n\n");
            reach_barrier();
            printf("\n**All the children have returned**\n\n");
        }
    }

    // Only parent do the clean up
    if (pid != 0) {
        // Clean up the process table
        for(i=0; i<NUM_PROCESSES; i++)
            wait(NULL);

        destroy_barrier(pid);
    }

}

