#include "stack.h"
#include <stdio.h>

static int _stack[MAX_STACK_SIZE];
static int _top = 0;

void push(int data) {
    if (_top == MAX_STACK_SIZE) {
        printf("Error: Stack is full. Item value %d is not added.\n", data);
    } else {
        _stack[_top++] = data;
    }
}

int pop() {
    int val = -1;
    if (_top == 0) {
        printf("Error: Stack is empty. Nothing to return\n");
    } else {
        val = _stack[--_top];
    }

    return val;
}

/* This section is for the function pointers exercise */

static int _res;

int xor(int x, int y) { return x ^ y; }

int or(int x, int y) { return x | y; }

void clear_xor(int *acc) { *acc = 0; }

void clear_or(int *acc) { *acc = 0; }

int reduce() {
    int ndx = _top;

    clear_xor(&_res);
    while (ndx != 0) {
        _res = xor(_stack[ndx - 1], _res);
        ndx--;
    }
    return _res;
}

/* Your implementation of flex_reduce goes here */
int flex_reduce(void (*clear)(int *), int (*op)(int, int)) {
    int ndx = _top;

    clear(&_res);
    while (ndx != 0) {
        _res = op(_stack[ndx - 1], _res);
        ndx--;
    }
    return _res;
}
