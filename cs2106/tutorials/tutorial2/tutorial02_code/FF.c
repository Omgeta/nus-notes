#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int factorial(int n) {
    if (n == 0) {
        fork();
        return 1;
    }

    return factorial(n-1) * n;
}

int main() {
    printf("fac(2) = %d\n", factorial(2));

    return 0;
}
