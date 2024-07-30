// mutex_type.c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;


int main() {
  //pthread_mutex_init(&mutex, NULL);

  printf("size of pthread_mutex_t = %ld \n", sizeof(pthread_mutex_t));
  printf("size of pthread_cond_t  = %ld \n", sizeof(pthread_cond_t));

  //pthread_mutex_destroy(&mutex);
  return 0;
}
