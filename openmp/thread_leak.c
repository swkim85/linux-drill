// thread3.c
// 오버커밋 방지
// $ echo 2 | sudo tee /proc/sys/vm/overcommit_memory

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 128*1024*1024  // 128 MB

long unsigned totalalloc = 0;
int flag = 0;

void *MyThreadFunc(void* arg){
  void *ptr = malloc(SIZE);
  if (ptr) {
    totalalloc += SIZE;
    printf("malloc size = %d, ptr=%p \n", SIZE, ptr);
  } else {
    printf("malloc failed \n");
    flag = 1;
  }
  sleep(2);
  printf("thread exit.\n");
  return NULL;
} 

int main() {
  pthread_t aThread;
  for (int i = 0; ; i++) { 
    pthread_create(&aThread, NULL, MyThreadFunc, NULL); 
    pthread_join(aThread, NULL); 
    if (flag) break;
    printf("iter = %d, totalalloc %ld MB \n", i, totalalloc/1024/1024);
    sleep(1);
  }
  printf("main program exit.\n");
  return 0;
}
