// deadlock.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *thread1_function(void *arg) {
  pthread_mutex_lock(&mutex1);
  printf("thread1 locked mutex1 ...\n");
  sleep(1);
  pthread_mutex_lock(&mutex2);
  printf("thread1 locked mutex2 ...\n");
  printf("thread1 working...\n"); // 임계 영역 코드
  pthread_mutex_unlock(&mutex2);
  pthread_mutex_unlock(&mutex1);
  return NULL;
}

void *thread2_function(void *arg) {
  pthread_mutex_lock(&mutex2);
  printf("thread2 locked mutex2 ...\n");
  sleep(1);
  pthread_mutex_lock(&mutex1);
  printf("thread2 locked mutex1 ...\n");
  printf("thread2 working...\n"); // 임계 영역 코드
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex2);
  return NULL;
}

int main() {
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, thread1_function, NULL);
  pthread_create(&thread2, NULL, thread2_function, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  return 0;
}
