/*******************************************************************
 * ex1-processes.cpp
 * Demonstrates process creation in Linux.
 ******************************************************************/

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <cstdio>
#include <cerrno>
#include <cstring>

int value = 10;
int main(int argc, char *argv[])
{
	// create the child process
	pid_t fork_ret = fork();

	if (fork_ret < 0)
	{
		// fork_ret would be -1 if unsuccessful
		fprintf(stderr, "Fork failed!: %s\n", strerror(errno));
		return 1;
	}
	else if (fork_ret == 0)
	{
		// fork_ret would return 0 in child
		value += 10;
		printf("Child process: value = %d\n", value);
	}
	else
	{
		// fork_ret is the PID of the child (in the parent)
		// parent waits until child is completed
    printf("We just cloned a process..!\n");
		wait(nullptr);
		printf("Child completed ....\n");
		printf("Parent process: value = %d\n", value);
	}

	return 0;
}
