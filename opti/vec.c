
//#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline") //Optimization flags
//#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
//#pragma GCC target("avx")  //Enable AVX
//#include <x86intrin.h>
#include <stdio.h>

//using namespace std;
//using namespace std::chrono;

int main()
{
  const int N = 20000; //Array Size
  const int noTests = 1; //Number of tests
  float a[N], b[N], c[N], result[N];

  // data initialization  
  for (int i = 0; i < N; ++i) {
    a[i] = ((float)i)+ 0.1f; 
    b[i] = 1.50f*((float)i)+ 0.23f; 
    c[i] = 0.33f*((float)i)+ 0.42f; 
  }    

  for (int i = 0; i < noTests; ++i) {
    for (int j = 0; j < N; ++j) {
      result[j] = a[j] + b[j] * c[j];
    }
  }

  //"CG> message -channel \"exercise results\" Time used: "<< end_time << "s, N * noTests="<<(N*noTests)<< endl;
  return 0;
}

