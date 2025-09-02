#include <stdio.h>

int *p1, *p2, *p3, *p4;

void fun1(int x, int y) {
    static int w;
    int z;
    w = x * y;
    z = x + y;

    printf("Inside fun1\n");
    p1 = &w;
    p2 = &x;
    p3 = &y;
    p4 = &z;
    printf("Address of p1 = %p, address of p2 = %p, address of p3 = %p, "
           "address of p4 = %p\n",
           &p1, &p2, &p3, &p4);
    printf("Address of w = %p, address of x = %p, address of y = %p, address "
           "of z = %p\n",
           p1, p2, p3, p4);
    printf("w = %d, x = %d, y = %d, z = %d\n", w, x, y, z);
}

int fun2(int f, int g, int h) {
    printf("\nInside fun2\n");
    printf("w = %d, x = %d, y = %d, z = %d\n", *p1, *p2, *p3, *p4);
    return f + g + h;
}

int main() {
    int a = 5, b = 6, c;

    fun1(a, b);

    printf("\nOutside fun1\n");
    printf("w = %d, x = %d, y = %d, z = %d\n", *p1, *p2, *p3, *p4);

    fun2(1, 2, 3);
    printf("\nOutside fun2\n");
    printf("w = %d, x = %d, y = %d, z = %d\n", *p1, *p2, *p3, *p4);
}
