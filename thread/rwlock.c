// rwlock.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_READERS 3
pthread_rwlock_t rwlock;
int shared_data = 0;

void *reader_thread(void *arg) {
  int thread_id = *((int*)arg);
  while (1) {
    pthread_rwlock_rdlock(&rwlock);
    printf("Reader %d read data %d\n", thread_id, shared_data);
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }
  return NULL;
}

void *writer_thread(void *arg) {
  while (1) {
    pthread_rwlock_wrlock(&rwlock);
    shared_data++;
    printf("Writer thread: %d\n", shared_data);
    pthread_rwlock_unlock(&rwlock);
    sleep(2);
  }
  return NULL;
}

int main() {
  pthread_t readers[NUM_READERS], writer;
  int thread_ids[NUM_READERS];

  pthread_rwlock_init(&rwlock, NULL);

  for (int i = 0; i < NUM_READERS; i++) {
    thread_ids[i] = i;
    pthread_create(&readers[i], NULL, reader_thread, &thread_ids[i]);
  }
  pthread_create(&writer, NULL, writer_thread, NULL);

  for (int i = 0; i < NUM_READERS; i++) {
    pthread_join(readers[i], NULL);
  }
  pthread_join(writer, NULL);

  pthread_rwlock_destroy(&rwlock);
  return 0;
}

