/*******************************************************************
 * ex6-cond-example.cpp
 * Demonstrates the use of condition variables for synchronising threads.
 * Adapted from example code from LLNL Pthreads tutorial
 ******************************************************************/

#include <cstdio>
#include <cstdlib>
#include <pthread.h> // include the pthread library
#include <unistd.h>

constexpr size_t NUM_THREADS = 3;
constexpr size_t NUM_INCREMENTS = 10;
constexpr size_t COUNT_LIMIT = 12;

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *incrementer(void *t)
{
	size_t thread_num = *(size_t *)t;
	for (size_t i = 0; i < NUM_INCREMENTS; i++)
	{
		pthread_mutex_lock(&count_mutex);
		count += 1;

		/*
		 * Check the value of count and signal the waiting thread when condition
		 * is reached. Note that this occurs while the mutex is locked.
		 */
		if (count == COUNT_LIMIT)
		{
			printf("incrementer(): thread %zu, count = %d Threshold reached. ", thread_num, count);
			pthread_cond_signal(&count_threshold_cv);
			printf("Just sent signal.\n");
		}

		// manually unlock before sleeping (don't sleep while holding locks!)
		printf("inc_count(): thread %zu, count = %d, unlocking mutex\n", thread_num, count);
		pthread_mutex_unlock(&count_mutex);
		// Do some work so threads can alternate on mutex lock
		sleep(1);
	}
	pthread_exit(NULL);
}

void *watcher(void *t)
{
	size_t thread_num = *(size_t *)t;
	printf("Starting watch_count(): thread %zu\n", thread_num);

	/*
		Lock the mutex and wait for someone to signal (notify) us. Note that
		condition_variable::wait will automatically and atomically unlock the
		mutex (using the unique_lock we pass to it) while it waits.

		Also, if COUNT_LIMIT is reached before we start to wait, then we
		don't even enter the loop to prevent waiting forever.
	*/
	pthread_mutex_lock(&count_mutex);
	while (count < COUNT_LIMIT)
	{
		printf("watch_count(): thread %zu Count= %d. Going into wait...\n", thread_num, count);
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		printf("watch_count(): thread %zu Condition signal received. Count= %d\n", thread_num, count);
	}

	printf("watch_count(): thread %zu Updating the value of count...\n", thread_num);
	count += 125;
	printf("watch_count(): thread %zu count now = %d.\n", thread_num, count);
	printf("watch_count(): thread %zu Unlocking mutex.\n", thread_num);

	pthread_mutex_unlock(&count_mutex);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i, rc;
	long t1 = 1, t2 = 2, t3 = 3;
	pthread_t threads[3];
	pthread_attr_t attr;

	/* Initialize mutex and condition variable objects */
	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init(&count_threshold_cv, NULL);

	pthread_create(&threads[0], NULL, watcher, (void *)&t1);
	pthread_create(&threads[1], NULL, incrementer, (void *)&t2);
	pthread_create(&threads[2], NULL, incrementer, (void *)&t3);

	/* Wait for all threads to complete */
	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
	printf("Main(): Waited and joined with %zu threads. Final value of count = %d. Done.\n", NUM_THREADS, count);

	/* Clean up and exit */
	pthread_mutex_destroy(&count_mutex);
	pthread_cond_destroy(&count_threshold_cv);
	pthread_exit(NULL);
}
