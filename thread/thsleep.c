// thsleep.c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  int tid;
  int s1;
  int s2;
} ThreadData;

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

void* thsleep(void* arg) {
  ThreadData *data = (ThreadData *)arg;
  sleep(data->s1);

  pthread_mutex_lock(&mutex);
  printf("thread %d lock mutex for %d seconds. \n", data->tid, data->s2);

  sleep(data->s2);

  pthread_mutex_unlock(&mutex);
  printf("thread %d unlock mutex \n", data->tid);
  return NULL;
}

int main() {
  pthread_t threads[3];
  ThreadData thread_data[3];

  thread_data[0].s1 = 15; thread_data[1].s1 = 10; thread_data[2].s1 = 5;
  thread_data[0].s2 = 10; thread_data[1].s2 = 10; thread_data[2].s2 = 10;

  for (int i = 0; i < 3; ++i) {
    thread_data[i].tid = i;
    pthread_create(&threads[i], NULL, thsleep, (void *)&thread_data[i]);
  }

  for (int i = 0; i < 3; ++i) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  return 0;
}

