#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5

int main() {
  // create
  int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);

  // attach 
  int *shm = (int *) shmat(shmid, NULL, 0);
  shm[0] = 0;

  int i, j, pid;
  
  for(i=0; i<NUM_PROCESSES; i++)
  {
      if((pid = fork()) == 0) {
          break;
      }
  }

  if(pid == 0) {
      while (shm[0] != i);

      printf("I am child %d\n", i);

      for(j = i*10; j<i*10 + 10; j++){
          printf("%d ", j);
          fflush(stdout);
          usleep(250000);
      }

      printf("\n\n");

      shm[0] = (shm[0] + 1) % NUM_PROCESSES;
      shmdt((void*) shm);
  }
  else {
      for(i=0; i<NUM_PROCESSES; i++) 
          wait(NULL);

      shmdt((void*) shm);
      shmctl(shmid, IPC_RMID, 0);
  }

}

