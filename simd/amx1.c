// amx1.c 
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_ROWS 16
#define MAX_COLS 64

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
  cfg->palette_id = 0;
  cfg->start_row = 0;
  //cfg->reserved_0[0] = 0;
  //cfg->reserved_0[1] = 0;
  //cfg->reserved_0[2] = 0;
  //cfg->reserved_0[3] = 0;
  cfg->rows[0]  = MAX_ROWS; cfg->colsb[0] = MAX_ROWS;
  cfg->rows[1]  = MAX_ROWS; cfg->colsb[1] = MAX_COLS;
  cfg->rows[2]  = MAX_ROWS; cfg->colsb[2] = MAX_COLS;
  cfg->rows[3]  = MAX_ROWS; cfg->colsb[3] = MAX_COLS;
/*
  for (i = 0; i < 1; i++) {
    cfg->colsb[i] = MAX_ROWS;
    cfg->rows[i] =  MAX_ROWS;
  }
  for (i = 1; i < 4; i++) {
    cfg->colsb[i] = MAX_COLS;
    cfg->rows[i] =  MAX_ROWS;
  }
*/
}

// Set_tiledata_use() - Invoke syscall to set ARCH_SET_STATE_USE 
static bool set_tiledata_use() {
  if (syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM, XFEATURE_XTILEDATA)) {
    fprintf(stderr, "\n Fail to do XFEATURE_XTILEDATA \n\n"); return false;
  } else {
    printf("Successfully set permission for XFEATURE_XTILEDATA.\n");
  }
  return true;
}

int main() {
  __tilecfg tile_data = {0};

  // Request permission to linux kernel to run AMX 
  if (!set_tiledata_use()) exit(-1);

  init_tile_config (&tile_data); // Load tile configuration 
  _tile_loadconfig (&tile_data); // initialize AMX tiles
  //__asm__ __volatile__("nop");

  _tile_release ();
}

