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

// Function to sum up the elements of an array
long long sumArray(const std::vector<int>& array) {
    long long sum = 0;
    for (size_t i = 0; i < array.size(); ++i) {
        sum += array[i];
    }
    return sum;
}

int main() {
    const size_t ARRAY_SIZE = 100000000; // 100 million elements

    // Seed the random number generator
    // std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Generate a large array
    std::cout << "Generating array with " << ARRAY_SIZE << " elements...\n";
    std::vector<int> largeArray = generateLargeArray(ARRAY_SIZE);

    // Sum up the array elements
    std::cout << "Summing array elements...\n";
    long long totalSum = sumArray(largeArray);

    // Output the result
    std::cout << "Total Sum: " << totalSum << "\n";

    return 0;
}
