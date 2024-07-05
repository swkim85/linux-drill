
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// 배열 크기 및 반복 횟수
#define N 10000000
#define REPEAT 1000

void workload(int *array, int n) {
    for (int i = 0; i < REPEAT; i++) {
        array[i % n] = array[i % n] * 2;
    }
}

int main() {
    int *array = (int*) malloc(N * sizeof(int));

    // 배열 초기화
    for (int i = 0; i < N; i++) {
        array[i] = rand() % 100;
    }

    double start, end;

    // 스케줄링 방식: static
    start = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        workload(array, N);
    }
    end = omp_get_wtime();
    printf("Static schedule time: %f seconds\n", end - start);

    // 스케줄링 방식: dynamic
    start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N; i++) {
        workload(array, N);
    }
    end = omp_get_wtime();
    printf("Dynamic schedule time: %f seconds\n", end - start);

    // 스케줄링 방식: guided
    start = omp_get_wtime();
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < N; i++) {
        workload(array, N);
    }
    end = omp_get_wtime();
    printf("Guided schedule time: %f seconds\n", end - start);

    // 스케줄링 방식: auto
    start = omp_get_wtime();
    #pragma omp parallel for schedule(auto)
    for (int i = 0; i < N; i++) {
        workload(array, N);
    }
    end = omp_get_wtime();
    printf("Auto schedule time: %f seconds\n", end - start);

    // 스케줄링 방식: runtime (환경 변수로 설정)
    start = omp_get_wtime();
    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < N; i++) {
        workload(array, N);
    }
    end = omp_get_wtime();
    printf("Runtime schedule time: %f seconds\n", end - start);

    free(array);

    return 0;
}

