// amx2.c 
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

#define MAX_ROWS 16
#define MAX_COLS 64

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
  cfg->reserved_0[0] = 1;
  //cfg->reserved_0[1] = 0;
  //cfg->reserved_0[2] = 0;
  //cfg->reserved_0[3] = 0;
  cfg->rows[0] = MAX_ROWS; cfg->colsb[0] = MAX_ROWS;
  cfg->rows[1] = MAX_ROWS; cfg->colsb[1] = MAX_COLS;
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

  _tile_release ();
}

