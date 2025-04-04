#include <stdio.h>

char *str_as_pointer = "This is a test!";
char str_as_array[] = "This is a test!";

int main(int argc, char *argv[]) {
#if defined TEST0
    str_as_array[0] = 'R';
    printf("%s\n", str_as_array);
#elif defined TEST1
    str_as_pointer[0] = 'R';
    printf("%s\n", str_as_pointer);
#elif defined TEST2
    str_as_pointer++;
    printf("%s\n", str_as_pointer);
#elif defined TEST3
    str_as_array++;
    printf("%s\n", str_as_array);
#endif
}
