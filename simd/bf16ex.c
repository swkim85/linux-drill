// bf16ex.c
#include <immintrin.h>
#include <stdio.h>
#include <math.h>
#include "rdtsc.h"

void hexDump(char *name, void* data, size_t size) {
  const unsigned char* byteData = (const unsigned char*)data;
  for (size_t i = 0; i < size; i++) {
    if (i % 32 == 0) printf("\n%s %04zx: ", name, i);
    if (i % 4 == 3) printf("%02x ", byteData[i]);
    else printf("%02x", byteData[i]);
  }
  printf("\n");
}

int main() {
  uint64_t start;
  float op1_f32[16], op2_f32[16], op3_f32[16]; // float=32bit * 16 = 512bits

  // register variables
  __m512   v1_f32, v2_f32, v3_f32;
  __m512bh v1_f16, v2_f16, v3_f16;

  start = rdtsc();
  for (int i = 0; i < 16; i++) {
    op1_f32[i]  = i+1;
    op2_f32[i]  = i+32;
    op3_f32[i]  = 1.0;
  }

  start = rdtsc();
  // 길이가 16인 float32 배열을 레지스터에 로딩 (총 512비트)
  v1_f32  = _mm512_loadu_ps(op1_f32); // ps : packed single-precision floating-point
  v2_f32  = _mm512_loadu_ps(op2_f32);
  v3_f32  = _mm512_loadu_ps(op3_f32);
  hexDump("v1_f32 ", &v1_f32, sizeof(v1_f32));
  hexDump("v2_f32 ", &v2_f32, sizeof(v1_f32));
  hexDump("v3_f32 ", &v3_f32, sizeof(v3_f32));

  start = rdtsc();
  // 길이가 16인 2개의 float32 벡터를, 길이가 32인 bf16 백터로 변환
  v1_f16 = _mm512_cvtne2ps_pbh (v1_f32, v2_f32);
  v2_f16 = _mm512_cvtne2ps_pbh (v1_f32, v2_f32);
  v3_f16 = _mm512_cvtne2ps_pbh (v3_f32, v3_f32);
  hexDump("v1_f16 ", &v1_f16, sizeof(v1_f16));
  hexDump("v2_f16 ", &v2_f16, sizeof(v2_f16));

  start = rdtsc();
  // bf16 벡터 2개를 곱하여 누적. FMA
  v3_f32 = _mm512_dpbf16_ps (v3_f32, v1_f16, v2_f16);
  hexDump("v3_f32 ", &v3_f32, sizeof(v3_f32));

  return 0;
}
