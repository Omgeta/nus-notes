#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_sigusr(int signum) {
    (void)signum;
    puts("Give me 5 more seconds");
    sleep(5);
    _Exit(0);
}

int main() {
    signal(SIGTERM, handle_sigusr);
    puts("Good morning...");
    for (int i = 0; i < 10; ++i) {
        sleep(100000);
    }
}
