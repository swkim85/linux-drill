// amx3.c 
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>

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
  cfg->rows[0] = MAX_ROWS; cfg->colsb[0] = MAX_COLS;
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


// Initialize int8_t buffer
static void init_buffer8 (int8_t *buf) {
  int rows, colsb, i, j;
  int8_t v;
  rows  = MAX_ROWS;
  colsb = MAX_COLS;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < colsb; j++) {
      //v = (rand() % 10) + 10;
      //if (j < colsb/2) v = 0; else v = 3;
      //if (j % 3 == 0) v = 1; else v = 2;
      //if (j % 4 == 0) v = 0; else v = 1;
      if (j % 4 == 0) v = 0; else v = j;
      //if (j > 14) v = 2;
      buf[i * colsb + j] = v;
    }
  }
}
// Initialize int32_t buffer
static void init_buffer32 (int32_t *buf) {
  int rows, colsb, i, j;
  rows  = MAX_ROWS;
  colsb = MAX_COLS;
  int colsb2=colsb/4;
  for (i = 0; i < rows; i++)
    for (j = 0; j < (colsb2); j++)
        buf[i * colsb2 + j] = 0;
}

// Print int8_t buffer
static void print_buffer8(int8_t* buf, int32_t rows, int32_t colsb) {
  printf("print_buffer8 rows=%d colsb=%d\n", rows, colsb);
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

  int8_t src1[MAX];
  int8_t src2[MAX];
  int32_t res[MAX/4];
  int rows  = MAX_ROWS;
  int colsb = MAX_COLS;

  init_buffer8 (src1); print_buffer8(src1, rows, colsb);
  init_buffer8 (src2); print_buffer8(src2, rows, colsb);
  init_buffer32 (res); print_buffer32(res, rows, colsb/4);

  // Load tile rows from memory
  _tile_loadd (2, src1, STRIDE);
  _tile_loadd (3, src2, STRIDE);
  _tile_loadd (1, res, STRIDE);

  _tile_release ();
}

