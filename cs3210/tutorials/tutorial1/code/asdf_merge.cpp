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

void merge(std::vector<int>& array, size_t left, size_t mid, size_t right) {
  int n1 = mid - left + 1;

  // copy left half into separate std::vector
  // right half stays in array[m+1..r]
  std::vector<int> left_half(n1);
  for (size_t i = 0; i < n1; i++) left_half[i] = array[left + i];

  size_t i = 0;       // index in left
  size_t j = mid + 1; // index in right half of array
  size_t k = left;    // write index in array
  
  while (i < n1 && j <= right) {
    if (left_half[i] <= array[j]) array[k++] = left_half[i++];
    else array[k++] = array[j++];
  }

  // copy the remaining elements in the left half
  // if there are remaining elements, it means they are all < any element in j..r
  while (i < n1) array[k++] = left_half[i++];
}

void merge_sort(std::vector<int>& array, size_t left, size_t right) {
  if (left >= right) return;

  size_t mid = left + (right - left) / 2;
  merge_sort(array, left, mid);
  merge_sort(array, mid+1, right);
  merge(array, left, mid, right);
}

void my_sort(std::vector<int>& array) {
  merge_sort(array, 0, array.size() - 1);
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
