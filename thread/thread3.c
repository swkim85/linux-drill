// thread3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define SLEEP 100000
void *MyThreadFunc(void* arg){
  usleep(SLEEP);
  printf("thread exit.\n");
  return NULL;
} 

int main() {
  pthread_t aThread;
  int ret;
  for (int i = 0; ; i++) { 
    ret = pthread_create(&aThread, NULL, MyThreadFunc, NULL); 
    if (ret) { printf(" pthread_create failed \n"); break; }
#ifdef DETACH
    printf("detach thread.\n");
    pthread_detach (aThread);
#endif
#ifdef JOIN
    ret = pthread_join(aThread, NULL); 
    if (ret) { printf(" pthread_join failed \n"); }
#endif
    printf("iter = %d \n", i);
    usleep(SLEEP);
  }
  printf("main program exit.\n");
  return 0;
}
