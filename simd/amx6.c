// amx6.c 
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>
#include "rdtsc.h"

// signal_handler
#include <signal.h>
#include <setjmp.h>

#ifndef MAX_ROWS
#define MAX_ROWS 4
#endif
#ifndef MAX_COLS
#define MAX_COLS 16
#endif
#define MAX      1024
#define STRIDE   MAX_COLS

#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18

jmp_buf jump_buffer;
void signal_handler(int signal) {
  if (signal == SIGSEGV) {
    printf("Caught segmentation fault (SIGSEGV)!\n");
    longjmp(jump_buffer, 1);
  }
}

typedef struct {
  uint16_t value;
} bfloat16;

// Define tile config data structure 
typedef struct __tile_config {
  uint8_t palette_id;
  uint8_t start_row;
  uint8_t reserved_0[14];
  uint16_t colsb[16]; 
  uint8_t rows[16]; 
} __tilecfg;

// Initialize tile config 
static void init_tile_config (__tilecfg *cfg) {
  cfg->palette_id = 1;
  cfg->start_row = 0;
  //cfg->rows[0] = MAX_ROWS; cfg->colsb[0] = MAX_ROWS;
  cfg->rows[1] = MAX_ROWS; cfg->colsb[1] = MAX_COLS;
  cfg->rows[2] = MAX_ROWS; cfg->colsb[2] = MAX_COLS;
  cfg->rows[3] = MAX_ROWS; cfg->colsb[3] = MAX_COLS;
}

// Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE 
static bool set_tiledata_use() {
  if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
    fprintf(stderr, "\n Fail to do XFEATURE_XTILEDATA \n\n"); return false;
  } else {
    //printf("Successfully set permission for XFEATURE_XTILEDATA.\n");
  }
  return true;
}

bfloat16 float_to_bfloat16(float f) {
  uint32_t int_value = *(uint32_t*)&f;
  uint16_t bfloat_value = int_value >> 16;
  //printf("%x ", bfloat_value);
  bfloat16 result;
  result.value = bfloat_value;
  return result;
}

float bfloat16_to_float(bfloat16 bf) {
  uint32_t int_value = bf.value << 16;
  //printf("%x ", int_value);
  float result = *(float*)&int_value;
  return result;
}

// Initialize bfloat16 buffer
static void init_buffer_bf16 (bfloat16 *buf) {
  int rows, colsb, i, j;
  bfloat16 v;
  rows  = MAX_ROWS;
  colsb = MAX_COLS;
  int colsb2 = colsb/2;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < colsb2; j++) {
      if (j % 4 == 0) v = float_to_bfloat16(1.0);
      else v = float_to_bfloat16(j);
      buf[i * colsb2 + j] = v;
    }
  }
}
// Initialize int32_t buffer
static void init_buffer32 (float *buf) {
  int rows, colsb, i, j;
  rows  = MAX_ROWS;
  colsb = MAX_COLS;
  int colsb2=colsb/4;
  for (i = 0; i < rows; i++)
    for (j = 0; j < (colsb2); j++)
      buf[i * colsb2 + j] = 0.0;
}

// Print bfloat16 buffer
static void print_buffer_bf16(bfloat16 *buf, int32_t rows, int32_t colsb) {
  printf("print_buffer_bf16 rows=%d colsb=%d\n", rows, colsb);
  bfloat16 v;
  float f;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < colsb; j++) {
       v = buf[i * colsb + j];
       f = bfloat16_to_float(v);
       printf("%3.1f ", f);
    }
    printf("\n");
  }
  printf("\n");
}
// Print int32_t buffer
static void print_buffer32(float *buf, int32_t rows, int32_t colsb) {
  float v;
  printf("print_buffer32 rows=%d colsb=%d\n", rows, colsb);
  for (int i = 0; i < rows; i++)  {
    for (int j = 0; j < colsb; j++) {
       v = buf[i * colsb + j];
       printf("%3.1f ", v);
    }
    printf("\n");
  }
  printf("\n");
}

int main() {

  if (!set_tiledata_use()) exit(-1);

  signal(SIGSEGV, signal_handler); // register a signal handler

  __tilecfg tile_data = {0};
  init_tile_config (&tile_data);

  if (setjmp(jump_buffer) == 0) {
    _tile_loadconfig (&tile_data);

  } else {
    fprintf(stderr, "some configuration error.\n");
    exit(1);
  }

  bfloat16 src1[MAX];
  bfloat16 src2[MAX];
  float res[MAX];
  int rows  = MAX_ROWS;
  int colsb = MAX_COLS;

  init_buffer_bf16(src1); print_buffer_bf16(src1, rows, colsb/2);
  init_buffer_bf16(src2); print_buffer_bf16(src2, rows, colsb/2);
  init_buffer32(res);     print_buffer32(res, rows, colsb/4);

  // Load tile rows from memory
  _tile_loadd (2, src1, STRIDE);
  _tile_loadd (3, src2, STRIDE);
  _tile_loadd (1, res,  STRIDE);

  uint64_t start, end;
  long long unsigned int elapsed;
  start = rdtsc();

  // Synopsis
  // void _tile_dpbf16ps (constexpr int dst, constexpr int a, constexpr int b)
  // #include <immintrin.h>
  // Instruction: tdpbf16ps tmm, tmm, tmm
  // CPUID Flags: AMX-BF16
  // Description
  // Compute dot-product of BF16 (16-bit) floating-point pairs in tiles a and b,
  // accumulating the intermediate single-precision (32-bit) floating-point elements with elements in dst,
  // and store the 32-bit result back to tile dst.
  _tile_dpbf16ps (1, 2, 3); // Compute dot-product

  end = rdtsc();
  elapsed = end - start;
  printf("_tile_dpbssd Elapsed Time (cycles): %llu\n", elapsed);

  _tile_stored (1, res, STRIDE);
  print_buffer32(res, rows, colsb/4);

  _tile_release ();
}

