#include <stdio.h>

int (*fptr)(int);

int func(int x) {
    return 2 * x;
}

int y = 10;

int *(*pfptr)();

int *func2() {
    return &y;
}

int main() {
    printf("Calling func with value 6: %d\n", func(6));
    printf("Now setting fptr to point to func.\n");
    fptr = func;
    printf("Caling fptr with value 6: %d\n", fptr(6));

    printf("\nNow caling func2 which returns the address of global variable y: "
           "%p\n",
           func2());
    printf("Pointing pfptr to func2.\n");
    pfptr = func2;
    printf("Now calling fpfptr: %p\n", pfptr());
}
