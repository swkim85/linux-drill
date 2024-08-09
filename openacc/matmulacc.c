// matmulacc.c
#include <stdio.h>
#include <omp.h>
#ifndef N
#define N 100
#endif
int A[N][N];
int B[N][N];
int C[N][N];
void matmul(int a[N][N], int b[N][N], int result[N][N]) {
  #pragma acc parallel loop
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      result[i][j] = 0;
      for (int k = 0; k < N; k++) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}
void printMatrix(int matrix[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}
void initialize(int mat[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      mat[i][j] = 1;
    }
  }
}
int main() {
  double start_time, end_time;
  initialize(A);  initialize(B);
  start_time = omp_get_wtime(); // 시간측정
  matmul(A, B, C);
  end_time = omp_get_wtime(); // 시간측정
  fprintf(stderr, "Elapsed time: %f seconds\n", end_time - start_time);
  printf("Result matrix is:\n");
  printMatrix(C);
  return 0;
}
