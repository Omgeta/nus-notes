#include "stack.h"
#include <stdio.h>

int main() {
    int v;
    for (int i = 0; i <= MAX_STACK_SIZE; i++) {
        printf("Adding %d\n", i);
        push(i);
    }

    for (int i = 0; i <= MAX_STACK_SIZE; i++) {
        v = pop();
        printf("Element %d is %d\n", i, v);
    }
}
