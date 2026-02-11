/*******************************************************************
 * ex789-prod-con-threads.cpp
 * Producer-consumer synchronisation problem in C++
 *******************************************************************/

#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <ctime>

constexpr int PRODUCERS = 2;
constexpr int CONSUMERS = 1;

constexpr int BUF_SIZE = 10;
int producer_buffer[BUF_SIZE];

int head = 0;
int tail = 0;
int count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *threadid)
{
	// Write producer code here
  while (true) {
    int item = rand();

    pthread_mutex_lock(&lock);
    while (count == BUF_SIZE)
      pthread_cond_wait(&not_full, &lock);

    producer_buffer[tail] = item;
    tail = (tail + 1) % BUF_SIZE;
    count++;

    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&lock);

    usleep(rand() % 40000);
    printf("Produced %d\n", item);
  }
  return nullptr;
}

void *consumer(void *threadid)
{
	// Write consumer code here
  while (true) {
    int consumed;

    pthread_mutex_lock(&lock);
    while (count == 0)
      pthread_cond_wait(&not_empty, &lock);

    consumed = producer_buffer[head];
    head = (head + 1) % BUF_SIZE;
    count--;

    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&lock);

    usleep(rand() % 40000);
    printf("Consumed %d\n", consumed);
  }
  return nullptr;
}

int main(int argc, char *argv[])
{
	pthread_t producer_threads[PRODUCERS];
	pthread_t consumer_threads[CONSUMERS];
	int producer_threadid[PRODUCERS];
	int consumer_threadid[CONSUMERS];

	int rc;
	int t1, t2;
	for (t1 = 0; t1 < PRODUCERS; t1++)
	{
		int tid = t1;
		producer_threadid[tid] = tid;
		printf("Main: creating producer %d\n", tid);
		rc = pthread_create(&producer_threads[tid], NULL, producer,
							(void *)&producer_threadid[tid]);
		if (rc)
		{
			printf("Error: Return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for (t2 = 0; t2 < CONSUMERS; t2++)
	{
		int tid = t2;
		consumer_threadid[tid] = tid;
		printf("Main: creating consumer %d\n", tid);
		rc = pthread_create(&consumer_threads[tid], NULL, consumer,
							(void *)&consumer_threadid[tid]);
		if (rc)
		{
			printf("Error: Return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL);

	/*
					some tips for this exercise:

					1. you may want to handle SIGINT (ctrl-C) so that your program
									can exit cleanly (by killing all threads, or just calling
		 exit)

					1a. only one thread should handle the signal (POSIX does not define
									*which* thread gets the signal), so it's wise to mask out the
		 signal on the worker threads (producer and consumer) and let the main
		 thread handle it
	*/
}
