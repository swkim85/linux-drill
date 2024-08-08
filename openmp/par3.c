#include <omp.h>
#include <stdio.h>

int main() {
    omp_set_dynamic(1); // Enable dynamic adjustment of threads

    #pragma omp parallel num_threads(4)
    {
        printf("Thread ID: %d, Total Threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
    }

    return 0;
}
