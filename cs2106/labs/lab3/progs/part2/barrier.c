#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>

struct reusable_barrier {
  int nproc;
  int count;
  sem_t mutex;
  sem_t waitQ;
  sem_t resetQ;
};

static int shmid = -1;
static struct reusable_barrier *barrier = NULL;

void init_barrier(int numproc) {
  shmid = shmget(IPC_PRIVATE, sizeof(*barrier), IPC_CREAT | 0600);
  barrier = (struct reusable_barrier*) shmat(shmid, 0, 0);
  
  barrier->nproc = numproc;
  barrier->count = 0;
  sem_init(&barrier->mutex, 1, 1);
  sem_init(&barrier->waitQ, 1, 0);
  sem_init(&barrier->resetQ, 1, 1);
}

void reach_barrier() {
  sem_wait(&barrier->mutex);
  barrier->count++;
  if (barrier->count == barrier->nproc) {
    sem_wait(&barrier->resetQ);
    sem_post(&barrier->waitQ);
  }
  sem_post(&barrier->mutex);

  sem_wait(&barrier->waitQ);
  sem_post(&barrier->waitQ);

  sem_wait(&barrier->mutex);
  barrier->count--;
  if (barrier->count == 0) {
    sem_wait(&barrier->waitQ);       
    sem_post(&barrier->resetQ);
  }
  sem_post(&barrier->mutex);

  sem_wait(&barrier->resetQ);          
  sem_post(&barrier->resetQ);
}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(&barrier->mutex);
        sem_destroy(&barrier->waitQ);
        sem_destroy(&barrier->resetQ);
        shmdt(barrier);
        shmctl(shmid, IPC_RMID, 0);
        barrier = 0;
        shmid = -1;
    }
}


