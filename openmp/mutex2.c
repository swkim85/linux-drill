// mutex2.c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void *threadCount(void *arg) {
  int i;
  char* name = (char*)arg;

  pthread_mutex_lock(&mutex);

  counter = 0;
  for (i = 0; i < 3; i++) {
    printf("%s counter: %d\n", name, counter);
    usleep(50);
    counter++;
  }

  pthread_mutex_unlock(&mutex);
}


int main() {
  pthread_t thread1;
  pthread_t thread2;
  pthread_mutex_init(&mutex, NULL);

  pthread_create(&thread1, NULL, threadCount, (void *)"thread1");
  pthread_create(&thread2, NULL, threadCount, (void *)"thread2");

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_mutex_destroy(&mutex);
  return 0;
}
