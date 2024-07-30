// thread_example2.c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_THREADS 3

typedef struct {
  int thread_id;
  int some_argument;
} thread_data_t;


void* thread_function(void* arg) {
  thread_data_t *data = (thread_data_t*)arg;
  int i;

  printf("Thread %d is running. some_argument=%d\n", data->thread_id, data->some_argument);
  for (i = 0; i < data->some_argument; i++) {
    printf("Thread %d loop %d.\n", data->thread_id, i);
  } 
  printf("Thread %d finish.\n", data->thread_id);
  return NULL;
}

int main() {
  pthread_t threads[N_THREADS];
  thread_data_t thread_data[N_THREADS];

  int thread_ids[N_THREADS];

  for (int i = 0; i < N_THREADS; ++i) {
    thread_data[i].thread_id = i;
    thread_data[i].some_argument = i * 10;

    pthread_create(&threads[i], NULL, thread_function, (void*)&thread_data[i]);
  }
  for (int i = 0; i < N_THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}

