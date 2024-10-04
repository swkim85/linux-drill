// sort.cc
#include <iostream>
#include <random>
//#include <vector>
#include "sort.h"

const int ARRAY_SIZE = 10;

void print_vec(std::string title, int *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void initialize_vector(int *data, int n) {
  std::random_device  rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 10000);
  for (int i = 0; i < n; i++) {
    data[i] = dis(gen);
  }
}

bool is_sorted(int *arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      return false;
    }
  }
  return true;
}

void swap(int *a, int *b) {
  int tmp;
  tmp = *a; *a = *b; *b = tmp;
}
void heapify(int *arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int *arr, int n) {
  // 최대 힙 구성
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);

  // 힙에서 요소를 하나씩 추출
  for (int i = n - 1; i > 0; i--) {
    swap(&arr[0], &arr[i]);
    heapify(arr, i, 0);
    print_vec("** ", arr, n);
  }
}

/*
int main() {
  constexpr unsigned int n = ARRAY_SIZE;
  int data[n];
  initialize_vector(data, n); // random number initialize

  print_vec("before ", data, n);
  heapSort(data, n); // 힙 소트
  print_vec("after ", data, n);

  if (is_sorted(data, n)) std::cout << "Sorting success" << std::endl;
  else std::cout << "Sorting FAIL!!" << std::endl;
  return 0;
}
*/


