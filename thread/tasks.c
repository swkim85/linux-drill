#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define NUM_TASKS 100

typedef struct {
  int task_id;
  int random_number;
} Task;

Task task_queue[NUM_TASKS]; // task queue
int task_index = 0;  // 다음 작업의 인덱스

pthread_mutex_t  mutex; // 뮤텍스 변수
pthread_cond_t   cond;   // 조건 변수

void* process_task(void* arg) {
  int thread_id = *((int*)arg);

  while (1) {
    pthread_mutex_lock(&mutex);

    while (task_index >= NUM_TASKS) {
      pthread_cond_wait(&cond, &mutex);
    }

    if (task_index >= NUM_TASKS) {
      pthread_mutex_unlock(&mutex);
      break;
    }

    Task task = task_queue[task_index];
    task_index++;

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    printf("Thread %d processing task %d\n", thread_id, task.task_id);
    // 작업이 있는 경우 sleep으로 대체
    int sleepT = task.random_number * 1000;
    usleep(sleepT);
  }

  return NULL;
}

int main() {
  pthread_t  threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  srand(time(NULL)); // 랜덤 넘버 생성을 위한 시드 설정

  for (int i = 0; i < NUM_TASKS; i++) {
    task_queue[i].task_id = i;
    task_queue[i].random_number = rand() % 91 + 10; // 10에서 100 사이의 랜덤 넘버
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, process_task, (void*)&thread_ids[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}

