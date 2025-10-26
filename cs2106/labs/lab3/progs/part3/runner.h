#ifndef RUNNER_H
#define RUNNER_H

void releaseHydrogen(void); // prints 'H'
void releaseOxygen(void); // prints 'O'


void *run_hydrogen_thread(void *);
void *run_oxygen_thread(void *);

#endif // RUNNER_H