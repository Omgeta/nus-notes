#include <iostream>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

// Function to generate a large array with random integers
std::vector<int> generateLargeArray(size_t size) {
    std::vector<int> array(size);
    for (size_t i = 0; i < size; ++i) {
        array[i] = rand() % 100; // Random number between 0 and 99
    }
    return array;
}

// Function to sum up the elements of an array with strided access
long long sumArrayWithStrides(const std::vector<int>& array, size_t stride) {
    long long sum = 0;
    for (size_t j = 0; j < stride; j++) {
	    for (size_t i = j; i < array.size(); i += stride) {
		sum += array[i];
	    }
    }
    return sum;
}

int main() {
    const size_t ARRAY_SIZE = 100000000; // 100 million elements
    const size_t STRIDE = 64;           // Stride size to increase cache misses

    // Seed the random number generator
    // std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Generate a large array
    std::cout << "Generating array with " << ARRAY_SIZE << " elements...\n";
    std::vector<int> largeArray = generateLargeArray(ARRAY_SIZE);

    // Sum up the array elements with strided access
    std::cout << "Summing array elements with stride of " << STRIDE << "...\n";
    long long totalSum = sumArrayWithStrides(largeArray, STRIDE);

    // Output the result
    std::cout << "Total Sum: " << totalSum << "\n";

    return 0;
}

