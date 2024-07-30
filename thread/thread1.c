// thread1.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *MyThreadFunc(void* arg){
  printf("Hello message from a thread\n");
  return NULL;
} 

int main() {
  pthread_t aThread;
  pthread_create(&aThread, NULL, MyThreadFunc, NULL); 
  pthread_join(aThread, NULL); 
  printf("main program exit.\n");
  return 0;
}

