// thread2.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *MyThreadFunc(void* arg){
  //sleep(10);
  printf("Hello message from a thread\n");
  return NULL;
} 

int main() {
  pthread_t aThread;
  pthread_create(&aThread, NULL, MyThreadFunc, NULL); 
  //pthread_join(aThread, NULL); 
  printf("main program exit.\n");
  return 0;
}

