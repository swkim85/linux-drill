
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5  // Number of threads to create

// Function that each thread will execute
void *thread_function(void *arg) {
    int thread_id = *((int *)arg);  // Get thread ID from argument
    printf("Hello from thread %d\n", thread_id);
    return NULL;  // Return NULL when done
}

int main() {
    pthread_t threads[NUM_THREADS];  // Array to hold thread identifiers
    int thread_ids[NUM_THREADS];      // Array to hold thread IDs

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;  // Assign ID to each thread
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed.\n");
    return 0;
}
