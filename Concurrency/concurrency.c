#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10 // Maximum number of toys in the box

int count = 0; // Number of toys currently in the box
sem_t m;       // Mutex semaphore for mutual exclusion
sem_t fullBox; // Semaphore to signal that the box has toys
sem_t emptyBox;// Semaphore to signal that the box is empty

void* child(void* arg) {
    while (1) {
        sem_wait(&fullBox); // Wait until the box has at least one toy
        sem_wait(&m);       // Enter critical section
        count--;            // Take a toy from the box
        printf("Child %ld took a toy. Toys left: %d\n", (long)arg, count);
        if (count == 0) {
            sem_post(&emptyBox); // If the box is now empty, signal the mother
        }
        sem_post(&m); // Exit critical section
        // Simulate playing with the toy
        sleep(rand() % 3);
    }
    return NULL;
}

void* mother(void* arg) {
    while (1) {
        sem_wait(&emptyBox); // Wait until the box is empty
        sem_wait(&m);        // Enter critical section
        printf("Mother refilling the toy box.\n");
        // Refill the box with N toys
        count = N;
        for (int i = 0; i < N; i++) {
            sem_post(&fullBox); // Signal that toys are available
        }
        sem_post(&m); // Exit critical section
        // Simulate time taken to refill the box
        sleep(1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    // Initialize semaphores
    sem_init(&m, 0, 1);
    sem_init(&fullBox, 0, 0);
    sem_init(&emptyBox, 0, 1);

    pthread_t mother_thread;
    pthread_t children_threads[5]; // Let's assume there are 5 children

    // Create the mother thread
    pthread_create(&mother_thread, NULL, mother, NULL);

    // Create the child threads
    for (long i = 0; i < 5; i++) {
        pthread_create(&children_threads[i], NULL, child, (void*)i);
    }

    // Join the threads (in this case, they run indefinitely)
    pthread_join(mother_thread, NULL);
    for (int i = 0; i < 5; i++) {
        pthread_join(children_threads[i], NULL);
    }

    // Destroy the semaphores
    sem_destroy(&m);
    sem_destroy(&fullBox);
    sem_destroy(&emptyBox);

    return 0;
}
