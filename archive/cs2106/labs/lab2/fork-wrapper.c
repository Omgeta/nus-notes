#include <dlfcn.h>
#include <signal.h>
#include <stdio.h>

typedef pid_t (*fork_t)();
static fork_t __fork = NULL;

extern pid_t __monitor_pid;

pid_t fork() {

    pid_t pid;
    if (__fork == NULL) {
        __fork = dlsym(RTLD_NEXT, "fork");
    }

    if ((pid = __fork()) == 0 && __monitor_pid) {
        kill(__monitor_pid, SIGUSR2);
    }

    return pid;
}
