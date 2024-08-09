// vecadd2.c
#include <stdio.h>
#include <stdlib.h>
void vecaddgpu( float *restrict r, float *a, float *b, int n ){
  #pragma acc kernels loop present(r, a, b)
  for ( int i = 0; i < n; ++i ) r[i] = a[i] + b[i];
}
int main ( int argc, char* argv[] ) {
  int n; // vector length 
  float * a; // input vector 1 
  float * b; // input vector 2 
  float * r; // output vector 
  float * e; // expected output values 
  int i, errs;
  if ( argc > 1 ) n = atoi( argv[1] ); else n = 100000; // default vector length 
  if ( n <= 0 ) n = 100000;
  a = (float*)malloc( n*sizeof(float) );
  b = (float*)malloc( n*sizeof(float) );
  r = (float*)malloc( n*sizeof(float) );
  e = (float*)malloc( n*sizeof(float) );
  for (i = 0; i < n; ++i) {
    a[i] = (float)(i+1);
    b[i] = (float)(1000*i);
  }
  #pragma acc data copyin(a[0:n],b[0:n]) copyout(r[0:n])
  {
    vecaddgpu( r, a, b, n ); // GPU에서 계산
  }
  for( i = 0; i < n; ++i ) e[i] = a[i] + b[i]; // 호스트에서 계산
  errs = 0;
  for (i = 0; i < n; ++i) { // 두가지 결과를 비교
    if (r[i] != e[i]) ++errs;
  }
  printf( "%d errors found\n", errs );
  return errs;
}

