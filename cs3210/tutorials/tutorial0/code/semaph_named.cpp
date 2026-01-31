/*******************************************************************
 * ex3-semaphore.cpp
 * Demonstrates using semaphores to synchromize Linux processes created with fork()
 * Partially adapted from https://stackoverflow.com/questions/16400820/c-how-to-use-posix-semaphores-on-forked-processes
 *******************************************************************/

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>

int main(int argc, char **argv)
{
	// Seed the random number generator with the current time
	srand((unsigned) time(NULL));
	// Get a random number to use as the shared memory key
	// This ensures that the shared memory key will be different across users and runs
	int shmrand = rand();
	// Generate the final shared memory key
	key_t shmkey = ftok("/dev/null", shmrand); /* valid directory name and a number */
	printf("shmkey for p = %d\n", shmkey);

	int shmid = shmget(shmkey, sizeof(int), 0644 | IPC_CREAT);
	if (shmid < 0)
	{
		perror("shmget\n");
		exit(1);
	}

	// attach p to shared memory
	int *p = reinterpret_cast<int *>(shmat(shmid, NULL, 0));
	*p = 0;
	printf("p = %d is allocated in shared memory.\n\n", *p);

	printf("How many children do you want to fork?\n");
	printf("Fork count: ");

	unsigned int num_children = 0;
	std::cin >> num_children;

	printf("What do you want the semaphore's initial value to be?\n");
	printf("Semaphore value: ");
	unsigned int initial_value = 0;
	std::cin >> initial_value;

	// Compute the semaphore name for this run as a string containing the random number
	// i.e., pSem and shmrand appended together
	// This also ensures that the semaphore name will be different across users and runs
	auto semaphore_string = "pSem-" + std::to_string(shmrand);
	const char* SEMAPHORE_NAME = semaphore_string.c_str();

	// initialise a semaphore named "pSem"
	sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0644, initial_value);
	printf("semaphore initialised with name: %s\n", SEMAPHORE_NAME);

	for (int i = 0; i < num_children; i++)
	{
		if (auto child_pid = fork(); child_pid < 0)
		{
			// if the fork failed, we don't want to keep the semaphores around
			fprintf(stderr, "fork error: %s\n", strerror(errno));
			sem_unlink(SEMAPHORE_NAME);
			sem_close(sem);
		}
		else if (child_pid == 0)
		{
			sem_wait(sem);
			printf("  Child(%d) is in critical section.\n", i);
			sleep(1);
			*p += i % 3; /* increment *p by 0, 1 or 2 based on i */
			printf("  Child(%d) new value of *p=%d.\n", i, *p);
			sem_post(sem);
			exit(0);
		}
	}

	// only the parent process will reach this place
	while (waitpid(-1, NULL, 0))
	{
		if (errno == ECHILD)
			break;
	}

	printf("\nParent: All children have exited.\n");

	// shared memory detach
	shmdt(p);
	shmctl(shmid, IPC_RMID, 0);

	// cleanup semaphores
	sem_unlink(SEMAPHORE_NAME);
	sem_close(sem);
	exit(0);
}
