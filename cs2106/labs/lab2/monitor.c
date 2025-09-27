#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_FORK 256

ssize_t fork_cnt = 0;

const char *sep =
    "============================================================\n";

void sigusr2_handler(int sig) {
    (void)(sig);

    fork_cnt++;

    if (fork_cnt >= MAX_FORK) {
        fputs(sep, stderr);
        fputs("YOU ARE HITTING THE FORK LIMIT\n", stderr);
        fputs(sep, stderr);

        kill(0, SIGKILL);
    }
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        fputs("Missing executable file\n", stderr);
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR2, sigusr2_handler);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    char monitor_pid[16];
    sprintf(monitor_pid, "%d", getpid());

    if (fork() == 0) {
        execl(argv[1], argv[1], monitor_pid, NULL);
        exit(EXIT_FAILURE);
    }

    while (waitpid(-1, NULL, 0) > 0)
        ;
}
