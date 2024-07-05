// thread_example.c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg) {
  int* id = (int*)arg;
  printf("Thread %d is running\n", *id);
  sleep(10);
  printf("Thread %d finish.\n", *id);
  return NULL;
}

int main() {
  pthread_t threads[3];
  int thread_ids[3];
  for (int i = 0; i < 3; ++i) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
  }
  for (int i = 0; i < 3; ++i) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}

