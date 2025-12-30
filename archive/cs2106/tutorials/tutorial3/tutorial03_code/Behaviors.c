#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void DoWork(int iterations, int delay)
{
    int i, j;

    for (i = 0; i < iterations; i++){
        printf("[%d]: Step %d\n", getpid(), i);
        for (j = 0; j < delay; j++);    // introduce some fictional work
    }
}

int main(int argc, char* argv[])
{
    int childpid;
    int delay;

    if (argc < 2) {
        printf("Usage: %s Delay\n", argv[0]);
        printf("Delay is a positive integer number.\n");
        return -1;
    }

    delay = atoi(argv[1]);

    childpid = fork();
    if (childpid == 0){ //1st child
        DoWork(5, delay);
        printf("[%d] Child Done!\n", getpid());
        return 0;       //1st child end
    }

    DoWork(5, delay);
    wait(NULL);
    printf("[%d] Parent Done!\n", getpid());

    return 0;
}
