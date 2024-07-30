// signalwait.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
int shared_data = 0;

void* waiter(void* arg) {
  pthread_mutex_lock(&mutex);  // 뮤텍스 잠금
  while (shared_data == 0) {
    printf("waiting...\n");
    // 조건 변수 대기, 뮤텍스 잠금 해제 후 대기, 신호를 받으면 다시 잠금
    pthread_cond_wait(&cond, &mutex);
  }
  printf("OK: shared_data = %d\n", shared_data);
  pthread_mutex_unlock(&mutex);  // 뮤텍스 잠금 해제
  return NULL;
}

void* signaler(void* arg) {
  sleep(1);
  pthread_mutex_lock(&mutex);  // 뮤텍스 잠금
  shared_data = 1;  // 조건 충족
  pthread_cond_signal(&cond);  // 조건 변수 신호
  pthread_mutex_unlock(&mutex);  // 뮤텍스 잠금 해제
  return NULL;
}

int main() {
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, waiter, NULL);
  pthread_create(&thread2, NULL, signaler, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  return 0;
}
