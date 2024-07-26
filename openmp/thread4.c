// thread3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *MyThreadFunc(void* arg){

  printf("child thread start. \n");
  printf("pthread_detach. \n");
  pthread_detach (pthread_self ());
  sleep( 2 );
  printf("child thread exit.\n");
  return NULL;
} 

int main() {
  pthread_t aThread;

  pthread_create(&aThread, NULL, MyThreadFunc, NULL); 
  pthread_join(aThread, NULL); 

  printf("main program exit.\n");
  return 0;
}

