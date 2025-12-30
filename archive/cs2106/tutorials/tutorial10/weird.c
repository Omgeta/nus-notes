#include <stdio.h>

int main()
{
    int *ip = NULL; //just a convenient way to crash a program :-)
    int i, trigger;


    printf("Trigger at (positive int): ");
    scanf("%d", &trigger);

    for(i = 0; ; i++)  { // essentially an infinite loop
        printf("%d\n", i%10);
        if (i == trigger)
            *ip = 123; //boom!
    }

    return 0;
}
