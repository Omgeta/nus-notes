#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#define NUM_PROCESSES 5

int main() {
  int i, j, pid;

  // create
  int shmid = shmget(IPC_PRIVATE, sizeof(sem_t) * NUM_PROCESSES, IPC_CREAT | 0600);

  // attach 
  sem_t *shm = (sem_t *) shmat(shmid, NULL, 0);
  for (i = 0; i < NUM_PROCESSES; i++) {
    sem_init(&shm[i], 1, i == 0);
  }

  
  for(i=0; i<NUM_PROCESSES; i++)
  {
      if((pid = fork()) == 0) {
          break;
      }
  }

  if(pid == 0) {
      sem_wait(&shm[i]);

      printf("I am child %d\n", i);

      for(j = i*10; j<i*10 + 10; j++){
          printf("%d ", j);
          fflush(stdout);
          usleep(250000);
      }

      printf("\n\n");

      sem_post(&shm[(i+1) % NUM_PROCESSES]);
      shmdt((void*) shm);
  }
  else {
      for(i=0; i<NUM_PROCESSES; i++) 
          wait(NULL);

      shmdt((void*) shm);
      shmctl(shmid, IPC_RMID, 0);
  }

}

