#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("\nGood night!");
    for (int i = 0; i < 10; ++i) {
        sleep(2);
        printf("\nGoing to sleep");
    }
}
