#include <stdio.h>

#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH 1000
#endif

void addArrays(float *result, float *array1, float *array2, int length) {
    for (int i = 0; i < length; ++i) {
        result[i] = array1[i] + array2[i];
    }
}

int main() {
  float va[ARRAY_LENGTH];
  float vb[ARRAY_LENGTH];
  float result[ARRAY_LENGTH];

  // Initialize arrays va and vb with some values (example)
  for (int i = 0; i < ARRAY_LENGTH; ++i) {
      va[i] = i * 1.5;
      vb[i] = i * 2.0;
  }

  // Add arrays va and vb
  addArrays(result, va, vb, ARRAY_LENGTH);

  // Print the first few elements of the result array as an example
  printf("First 10 elements of the result array:\n");
  for (int i = 0; i < 10; ++i) {
      printf("%.2f ", result[i]);
  }
  printf("\n");

  return 0;
}

