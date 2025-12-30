
// Maximum number of elements in a stack
//
#define MAX_STACK_SIZE 10

// declare prototypes below
void push(int);
int pop(void);

int xor(int x, int y);
int or(int x, int y);

void clear_xor(int *acc);
void clear_or(int *acc);

int reduce(void);
int flex_reduce(void (*)(int *), int (*)(int, int));

