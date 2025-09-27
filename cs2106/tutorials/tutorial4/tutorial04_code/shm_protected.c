#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
    int i, shmid, n = 100, childPid, nChild = 1, pOrder = 0;
    int *shm;

    if (argc > 1)
        n = atoi(argv[1]);

    if (argc > 2)
        nChild = atoi(argv[2]);

    // create Share Memory Region
    // read, write, excute
    // 1      1      0        = 6
    // 1      0      0        = 4
    
    // TODO: create a shared memory region that contains two values
    shmid = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0600);
    if (shmid == -1) {
        printf("Cannot create shared memory!\n");
        exit(1);
    }
    else {
        printf("Shared Memory Id = %d\n", shmid);
    }

    // TODO: attach the shared memory region to this process
    shm = (int*)shmat(shmid, NULL, 0);
    if (shm == (int*) -1) {
        printf("Cannot attach shared memory!\n");
        exit(1);
    }

    // TODO: initialize the shared memory
    shm[0] = 0;    // initialize shared memory to 0
    shm[1] = 0;    // initialize pOrder in shm

    for (i = 0; i < nChild; i++) {
        childPid = fork();
        if (childPid == 0) {
            pOrder = i + 1; // each process gets its pOrder
            break;
        }
    }

    // TODO: only increment the shared value if it's the process' turn
    // don't forget to let the other process know its turn has come
    i = 0;
    while (i < n) {
      if (shm[1] == pOrder) {
        shm[0]++;
        i++;
        shm[1] = (shm[1] + 1) % (nChild + 1);
      }  
    }
        

    if (childPid != 0) {
        for (i = 0; i < nChild; i++)
            wait(NULL);
        // TODO: print the result value
        printf("The value in the shared memory is: %d\n", shm[0]);
        // TODO: detach and destroy
        shmdt((char*)shm);
        shmctl( shmid, IPC_RMID, 0);
    }

    return 0;
}
