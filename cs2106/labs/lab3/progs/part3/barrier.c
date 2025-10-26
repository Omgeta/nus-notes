#include <semaphore.h>

static int nproc = 0;
static int count = 0;
static sem_t mutex;
static sem_t waitQ;
static sem_t resetQ;

void init_barrier(int numproc) {
  nproc = numproc;
  count = 0;
  sem_init(&mutex, 0, 1);
  sem_init(&waitQ, 0, 0);
  sem_init(&resetQ, 0, 1);
}

void reach_barrier() {
  sem_wait(&mutex);
  count++;
  if (count == nproc) {
    sem_wait(&resetQ);
    sem_post(&waitQ);
  }
  sem_post(&mutex);

  sem_wait(&waitQ);
  sem_post(&waitQ);

  sem_wait(&mutex);
  count--;
  if (count == 0) {
    sem_wait(&waitQ);       
    sem_post(&resetQ);
  }
  sem_post(&mutex);

  sem_wait(&resetQ);          
  sem_post(&resetQ);
}

void destroy_barrier() {
  sem_destroy(&mutex);
  sem_destroy(&waitQ);
  sem_destroy(&resetQ);
  nproc = 0;
  count = 0;
}


