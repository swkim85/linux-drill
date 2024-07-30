// thread4.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 8*1024*1024  // 8MB
long unsigned totalalloc = 0;
int flag = 0;

void *MyThreadFunc(void* arg){
#ifdef DETECH
  pthread_detach (pthread_self ());
#endif
  void *ptr = malloc(SIZE);
  if (ptr) {
    totalalloc += SIZE;
    printf("malloc size = %d, ptr=%p \n", SIZE, ptr);
  } else flag = 1;
  return NULL;
} 

int main() {
  pthread_t aThread;
  for (int i = 0; ; i++) { 
    printf("iter = %d, totalalloc %ld MB \n", i, totalalloc/1024/1024);
    pthread_create(&aThread, NULL, MyThreadFunc, NULL); 
    pthread_join(aThread, NULL); 
    if (flag) break;
  }
  printf("main program exit.\n");
  return 0;
}

