#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime> 

#ifndef ARRAY_SIZE
#define ARRAY_SIZE 10000
#endif

#ifndef MAX_VALUE
#define MAX_VALUE 100000
#endif

std::vector<int> generateLargeArray(size_t size, size_t max_value) {
    std::vector<int> array(size);
    for (size_t i = 0; i < size; ++i) {
        array[i] = rand() % max_value; 
    }
    return array;
}

void my_sort(std::vector<int>& array) {
    size_t n = array.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int main() {
    std::vector<int> largeArray = generateLargeArray(ARRAY_SIZE, MAX_VALUE);

    my_sort(largeArray);

    // Print the last 5 elements of the sorted array
    std::cout << "Last 5 elements of the sorted array: ";
    for (size_t i = ARRAY_SIZE - 5; i < ARRAY_SIZE; ++i) {
        std::cout << largeArray[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
