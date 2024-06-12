// bfloatsum.c 
#include <stdio.h>
#include <stdint.h>
#include <math.h>
typedef struct {
    uint16_t value;
} bfloat16;
bfloat16 float_to_bfloat16(float f) {
  uint32_t int_value = *(uint32_t*)&f;
  uint16_t bfloat_value = int_value >> 16;
  bfloat16 result;
  result.value = bfloat_value;
  return result;
}
float bfloat16_to_float(bfloat16 bf) {
  uint32_t int_value = bf.value << 16;
  float result = *(float*)&int_value;
  return result;
}
bfloat16 bfloat16_add(bfloat16 a, bfloat16 b) {
  float fa = bfloat16_to_float(a);
  float fb = bfloat16_to_float(b);
  float sum = fa + fb;
  return float_to_bfloat16(sum);
}
void print_bfloat16(bfloat16 bf) {
  float f;
  uint32_t int_value;
  printf("bfloat16 value: 0x%04x\n", bf.value);
  f = bfloat16_to_float(bf);
  printf("as float: %f\n", f);
  int_value = *(uint32_t*)&f; // type casting float to uint32_t
  printf("float value: 0x%04x\n", int_value);
}
int main() {
  float a = 3.14;     // bfloat16으로 변환하면서  3.125 가됨
  float b = 2.718;    // bfloat16으로 변환하면서  2.703125 가 됨
  bfloat16 bf_a = float_to_bfloat16(a);
  bfloat16 bf_b = float_to_bfloat16(b);
  bfloat16 bf_sum = bfloat16_add(bf_a, bf_b);
  printf("a ==> \n"); print_bfloat16(bf_a);
  printf("b ==> \n"); print_bfloat16(bf_b);
  printf("a + b ==> \n"); print_bfloat16(bf_sum);
  return 0;
}
