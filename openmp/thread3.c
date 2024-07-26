// thread3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 500*1024*1024  // 500MB
long unsigned totalalloc = 0;

void *MyThreadFunc(void* arg){
  void *ptr = malloc(SIZE);
  totalalloc += SIZE;
  printf("malloc size = %d, ptr=%p \n", SIZE, ptr);
  return NULL;
} 

int main() {
  pthread_t aThread;
  for (int i = 0; ; i++) { 
    printf("iter = %d, totalalloc %ld MB \n", i, totalalloc/1024/1024);
    pthread_create(&aThread, NULL, MyThreadFunc, NULL); 
    pthread_join(aThread, NULL); 
  }
  printf("main program exit.\n");
  return 0;
}

