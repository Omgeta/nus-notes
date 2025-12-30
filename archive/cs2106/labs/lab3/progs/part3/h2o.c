/**
 * CS2106 AY 25/26 Semester 1 - Lab 3
 *
 * Your implementation should be in this file.
 */
#include <semaphore.h>
#include <stdio.h>

#include "h2o.h"
#include "barrier.h"

static sem_t mutex, h_wait, o_wait, newline;
static int count;

// Your initialization code goes here
void h2o_init() {
  sem_init(&mutex, 0, 1);
  sem_init(&h_wait, 0, 0);
  sem_init(&o_wait, 0, 0);
  sem_init(&newline, 0, 1);
  count = 0;
  init_barrier(3);
}

// Synchronization code for hydrogen thread
void hydrogen_thread(void (*releaseHydrogen)(void)) {
  sem_wait(&mutex);
  count++;
  if (count % 2 == 0) sem_post(&o_wait); // lets O progress for every H pair
  sem_post(&mutex);

  sem_wait(&h_wait); // wait in pairs until O signals

  reach_barrier(); // first barrier aligns prints
  releaseHydrogen(); // each H prints once
  reach_barrier(); // second barrier aligns exit so `\n' can print just after
}

// Synchronization code for oxygen thread
void oxygen_thread(void (*releaseOxygen)(void)) {
  sem_wait(&o_wait); // wait for signal to progress from two H
  
  sem_wait(&newline);

  sem_post(&h_wait); sem_post(&h_wait); // release the two H

  reach_barrier(); // first barrier aligns prints
  releaseOxygen(); // prints O
  reach_barrier(); // second barrier aligns exit so `\n' can print just after

  printf("\n"); // O prints newline since theres only one O thread but 2 H
  sem_post(&newline);
}

// Your cleanup code goes here
void h2o_destroy() {
  sem_destroy(&mutex);
  sem_destroy(&h_wait);
  sem_destroy(&o_wait);
  destroy_barrier();
  sem_destroy(&newline);
}
