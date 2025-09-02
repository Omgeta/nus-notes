#include <stdio.h>

int accumulate(int x) {
    // Modify this function so that it accumulates values passed to it to
    // a value called "acc".

    static int acc = 0;

    acc = acc + x;
    printf("acc is now %d\n", acc);
}

int main() {

    // Call fun2 to accumulate from 1 to 10
    for (int i = 1; i <= 10; i++)
        accumulate(i);
}
