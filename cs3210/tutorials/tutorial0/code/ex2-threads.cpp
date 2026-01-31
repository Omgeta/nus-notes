/*******************************************************************
 * ex2-threads.cpp
 * Demonstrates thread creation and termination.
 ******************************************************************/

#include <cstdio>
#include <cstdlib>
#include <pthread.h> // include the pthread library
#include <vector>

int counter = 0;

void *work(void *threadid) // function to run in parallel
{
	size_t tid = (size_t)threadid;
	counter++;
	printf("thread #%ld incrementing counter. counter = %d\n", tid, counter);
	pthread_exit(NULL); // terminate thread
}

int main(int argc, char *argv[])
{
	constexpr size_t NUM_THREADS = 8;
	pthread_t threads[NUM_THREADS]; // reference to threads

	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		printf("main thread: creating thread %zu\n", i);

		// pthread_create spawns a new thread and return 0 on success
		int rc = pthread_create(&threads[i], NULL, work, (void *)i);
		if (rc)
		{
			printf("Return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	pthread_exit(NULL);
}
