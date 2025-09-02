#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p = (int *)malloc(sizeof(int));
    *p = 5;
    printf("p is %p and *p is %d\n", p, *p);
    free(p);
}
