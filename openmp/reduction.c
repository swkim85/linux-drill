// reduction.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
int main() {
  const int size = 1000;
  int *arr = (int *)malloc(size * sizeof(int));
  int sum = 0;
  int min_value = INT_MAX;
  int max_value = INT_MIN;

  for (int i = 0; i < size; i++) {
    arr[i] = i % 99; 
  }

  #pragma omp parallel for reduction(+:sum) reduction(min:min_value) reduction(max:max_value)
  for (int i = 0; i < size; ++i) {
    sum += arr[i];
    if (arr[i] < min_value) min_value = arr[i];
    if (arr[i] > max_value) max_value = arr[i];
  }

  printf("sum=%d\n", sum);
  printf("min=%d\n", min_value);
  printf("max=%d\n", max_value);
  free(arr);
  return 0;
}
