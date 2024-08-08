// dynamic.c
#include <omp.h>
#include <stdio.h>

int main() {
    // Enable dynamic adjustment of the number of threads
    omp_set_dynamic(1);

    // Set the number of threads to 4
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        // Each thread will print its ID
        printf("Thread ID: %d\n", omp_get_thread_num());
    }

    return 0;
}
