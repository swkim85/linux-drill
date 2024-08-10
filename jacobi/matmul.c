// matmul.c
#include <stdio.h>

#define N 4

void matmul(float a[N][N], float b[N][N], float result[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      result[i][j] = 0;
      for (int k = 0; k < N; k++) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

void triangle(float in[N][N], float out[N][N], int upper) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (upper) 
        if (j > i) out[i][j] = in[i][j]; else out[i][j] = 0;
      else
        if (j < i) out[i][j] = in[i][j]; else out[i][j] = 0;
    }
  }
}

void inv_diag(float in[N][N], float out[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (j == i) out[i][j] = 1/in[i][j]; else out[i][j] = 0;
    }
  }
}

void matrix_vector_multiply(float matrix[N][N], float vector[N], float result[N]) {
  for (int i = 0; i < N; i++) {
    result[i] = 0;
    for (int j = 0; j < N; j++) {
      result[i] += matrix[i][j] * vector[j];
    }
  }
}

void matrix_add(float a[N][N], float b[N][N], float result[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      result[i][j] = a[i][j] + b[i][j];
}

void matrix_matrix_multiply(float a[N][N], float b[N][N], float result[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      result[i][j] = 0; 
      for (int k = 0; k < N; k++) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

void vector_vector_add(float a[N], float b[N], float result[N]) {
  for (int i = 0; i < N; i++) {
    result[i] = a[i] + b[i];
  }
}

void jacobi_step(float result[N], float x[N], float b[N], float DinvLU[N][N], float Dinv[N][N]) {
  float tmp1[N];
  float tmp2[N];
  float tmp3[N];

  matrix_vector_multiply(DinvLU, x, tmp1);
  matrix_vector_multiply(Dinv, b, tmp2);
  vector_vector_add(tmp1, tmp2, result);
}


void printMatrix(float matrix[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }
}

void initialize(float mat[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      mat[i][j] = 1;
    }
  }
}

int main() {
  float A[N][N] = {{10., -1., 2., 0.},
              {-1., 11., -1., 3.},
              {2., -1., 10., -1.},
              {0.0, 3., -1., 8.}};
  float b[N] = {6., 25., -11., 15.};

  float B[N][N];
  float C[N][N];
  float L[N][N];
  float U[N][N];
  float LU[N][N];
  float Dinv[N][N];
  float DinvLU[N][N];

  triangle(A, U, 1);
  triangle(A, L, 0);

  inv_diag(A, Dinv);
  matrix_add(U, L, LU);
  matrix_matrix_multiply(Dinv, LU, DinvLU);

  printf("A : \n"); printMatrix( A );
  printf("U : \n"); printMatrix( U );
  printf("L : \n"); printMatrix( L );
  printf("LU : \n"); printMatrix( LU );
  printf("Dinv : \n"); printMatrix( Dinv );
  printf("DinvLU : \n"); printMatrix( DinvLU );

//jacobi_step(float result[N], float x[N], float b[N], float DinvLU[N][N], float Dinv[N][N]);

  return 0;
}
