// procon.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

typedef struct {
  int consid; char *name;
} threadData;

void *producer(void *arg) {
  int item;
  while (1) {
    item = rand() % 100;  // 생산할 아이템

    pthread_mutex_lock(&mutex);
    while (count == BUFFER_SIZE) pthread_cond_wait(&empty, &mutex);
    
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    count++;

    printf("produce item=%d, count=%d\n", item, count);

    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);

    sleep(1);  // 생산 간격
  }
  return NULL;
}

void *consumer(void *arg) {
  int item;
  threadData *data = (threadData *)arg;
  while (1) {
    pthread_mutex_lock(&mutex);
    while (count == 0) pthread_cond_wait(&full, &mutex);

    item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    count--;

    printf("%s consum item=%d count=%d\n", data->name, item, count);

    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);

    sleep(4);  // 소비 간격
  }
  return NULL;
}

int main() {
  pthread_t prod_thread, cons_thread1, cons_thread2;
  threadData thread_data[2];
  thread_data[0].name = "consumer 1";
  thread_data[1].name = "consumer 2";

  pthread_create(&prod_thread, NULL, producer, NULL);
  pthread_create(&cons_thread1, NULL, consumer, (void *)&thread_data[0]);
  pthread_create(&cons_thread2, NULL, consumer, (void *)&thread_data[1]);

  pthread_join(prod_thread, NULL);
  pthread_join(cons_thread1, NULL);
  pthread_join(cons_thread2, NULL);

  return 0;
}
