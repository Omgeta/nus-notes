#include <stdio.h>
#include <stdlib.h>

int *p;

void dfun1(int x, int y) {
    int *z;

    z = (int *)malloc(sizeof(int));

    p = z;
    *z = x + y;

    printf("Inside dfun1: &x = %p, &y = %p, &z = %p, &p = %p\n", &x, &y, &z,
           &p);
    printf("Address stored in z: %p\n", z);
}

void dfun2() {
    printf("Inside dfun2: The value p is pointing to is: %d\n", *p);
}

int main() {
    printf("Address stored in p before calling dfun1: %p\n", p);
    dfun1(10, 20);
    printf("Address stored in p after calling dfun1: %p\n", p);
    printf("Value pointed to by p: %d\n", *p);
    dfun2();
    free(p);
}
