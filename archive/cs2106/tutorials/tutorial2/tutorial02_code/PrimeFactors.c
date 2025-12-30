#include <stdio.h>
#include <stdlib.h>

// Give the total number of prime factors
// not unique prime factors :-)
// e.g. 9 = 3 x 3  (2 prime factors)

// No checks on argc, assume the user pass in the right number of
// arguments :-)

int main(int argc, char* argv[]) {
    int nFactor = 0, userInput, factor;

    // Convert string to number
    userInput = atoi( argv[1] );

    nFactor = 0;
    factor = 2;

    // Quick hack to get the number of prime factors
    // only for positive integer
    while (userInput > 1) {
        if (userInput % factor == 0) {
            userInput /= factor;
            nFactor++;
        } else {
            factor++;
        }
    }

    return nFactor;
}
