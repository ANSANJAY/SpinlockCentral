#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10
#define NUM_INCREMENTS 100000

// Shared counter
volatile int counter = 0;

// Spinlock
pthread_spinlock_t spinlock;

// Thread function
void *increment_counter(void *arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        pthread_spin_lock(&spinlock); // Acquire spinlock
        counter++; // Increment the shared counter
        pthread_spin_unlock(&spinlock); // Release spinlock
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize spinlock
    if (pthread_spin_init(&spinlock, 0)) {
        perror("pthread_spin_init");
        exit(EXIT_FAILURE);
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL)) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // Destroy spinlock
    pthread_spin_destroy(&spinlock);

    // Print final counter value
    printf("Final counter value: %d\n", counter); // Expected: NUM_THREADS * NUM_INCREMENTS

    return 0;
}
