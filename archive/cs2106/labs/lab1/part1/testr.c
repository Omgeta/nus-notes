#include "stack.h"
#include <stdio.h>

int main() {

    int i;
    for (i = 1; i <= 9; i++) {
        printf("Pushing %d\n", i);
        push(i);
    }

    printf("\nCalling reduce result is %d\n", reduce());

    /* Uncomment the following two statements to test flex_reduce */

    printf("Calling flex reduce with xor. Result is %d\n", flex_reduce(clear_xor, xor)); 
    printf("Calling flex reduce with or. Result is %d\n", flex_reduce(clear_or, or));
}
