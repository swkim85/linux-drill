// amxtile.c 
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>

// signal_handler
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#include "rdtsc.h"

#ifndef MAX_ROWS
#define MAX_ROWS 16
#endif
#ifndef MAX_COLS
#define MAX_COLS 64
#endif
#ifndef MAX
#define MAX 1024 // MAX_ROWS x MAX_COLS
#endif
#ifndef STRIDE
#define STRIDE 64
#endif

#define ARCH_GET_XCOMP_PERM     0x1022
#define ARCH_REQ_XCOMP_PERM     0x1023
#define XFEATURE_XTILECFG       17
#define XFEATURE_XTILEDATA      18
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
  int i;
  cfg->palette_id = 1;
  cfg->start_row = 0;
  cfg->reserved_0[0] = 0;
  cfg->reserved_0[1] = 0;
  cfg->reserved_0[2] = 0;
  cfg->reserved_0[3] = 0;
  for (i = 0; i < 1; i++) {
    cfg->colsb[i] = MAX_ROWS;
    cfg->rows[i] =  MAX_ROWS;
  }
  for (i = 1; i < 4; i++) {
    cfg->colsb[i] = MAX_COLS;
    cfg->rows[i] =  MAX_ROWS;
  }
}

// Initialize int8_t buffer 
static void init_buffer (int8_t *buf, int8_t v1, int8_t v2) {
  int rows, colsb, i, j;
  int8_t v;
  rows  = MAX_ROWS;
  colsb = MAX_COLS;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < colsb; j++) {
      if (j < colsb/2) v = v1; else v = v2;
      buf[i * colsb + j] = v;
    }
  }
}
// Initialize int32_t buffer 
static void init_buffer32 (int32_t *buf, int32_t value) {
  int rows, colsb, i, j;
  rows  = MAX_ROWS;
  colsb = MAX_COLS;
  int colsb2=colsb/4;
  for (i = 0; i < rows; i++)
    for (j = 0; j < (colsb2); j++)
        buf[i * colsb2 + j] = value;
}
// Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE 
static bool set_tiledata_use() {
  if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
    printf("\n Fail to do XFEATURE_XTILEDATA \n\n");
    return false;
  }
  return true;
}
// Print int8_t buffer 
static void print_buffer(int8_t* buf, int32_t rows, int32_t colsb) {
  printf("print_buffer rows=%d colsb=%d\n", rows, colsb);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < (colsb); j++) {
       printf("%d ", buf[i * colsb + j]);
    }
    printf("\n");
  }
  printf("\n");
}
// Print int32_t buffer 
static void print_buffer32(int32_t* buf, int32_t rows, int32_t colsb) {
  printf("print_buffer32 rows=%d colsb=%d\n", rows, colsb);
  for (int i = 0; i < rows; i++)  {
    for (int j = 0; j < colsb; j++) {
       printf("%d ", buf[i * colsb + j]);
    }
    printf("\n");
  }
  printf("\n");
}


jmp_buf jump_buffer;
void signal_handler(int signal) {
  if (signal == SIGSEGV) {
    printf("Caught segmentation fault (SIGSEGV)!\n");
    longjmp(jump_buffer, 1);
  }
}

int main() {
  __tilecfg tile_data = {0};
  int8_t src1[MAX];
  int8_t src2[MAX];
  int32_t res[MAX/4];
  int rows  = MAX_ROWS;
  int colsb = MAX_COLS;
  uint64_t start, end;
  long long unsigned int elapsed;

  signal(SIGSEGV, signal_handler);  // Register the signal handler

  // Request permission to linux kernel to run AMX 
  if (!set_tiledata_use()) exit(-1);

  init_tile_config (&tile_data); // Load tile configuration 

  if (setjmp(jump_buffer) == 0) {

    _tile_loadconfig (&tile_data); // initialize AMX tiles
    printf("This line will not be executed.\n");

  } else {
    fprintf(stderr, "some configuration error.\n");
    exit(1);
  }

  __asm__ __volatile__("nop");

  init_buffer (src1, 0, 1); print_buffer(src1, rows, colsb);
  init_buffer (src2, 0, 2); print_buffer(src2, rows, colsb);
  init_buffer32 (res, 100); print_buffer32(res, rows, colsb/4);

  // Load tile rows from memory
  _tile_loadd (2, src1, STRIDE);
  _tile_loadd (3, src2, STRIDE);
  _tile_loadd (1, res, STRIDE);

  start = rdtsc();
  // Compute dot-product of bytes in tiles 
  _tile_dpbssd (1, 2, 3);
  end = rdtsc();
  elapsed = end - start;
  printf("tile_dpbssd Elapsed Time (cycles): %llu\n", elapsed);

  _tile_stored (1, res, STRIDE);
  print_buffer32(res, rows, colsb/4);
  _tile_release ();
}

