#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

// cmd arguments
int n_prod;
int n_conso;

// Final non-moving sizes
#define BUFFER_SIZE 8
#define DATA_SIZE 10000

// Semaphores
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// Threads
pthread_t* prod;
pthread_t* conso;

// Buffer, counters
int* buffer;
int in_index = 0;
int out_index = 0;
int produced = DATA_SIZE;
int consumed = DATA_SIZE;

// CPU working simulator
void CPU_go_brrrr() {
    int entier;
    for (int i = 0; i < 10000; i++) {
        entier++;
    }
}

// Producer
void* producer() {
    int item;
    while (1) {
        item = rand(); // produce random int
        // wait
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // break condition
        if (produced == 0) {
            sem_post(&empty);
            sem_post(&full);
            pthread_mutex_unlock(&mutex);
            break;
        }
        // debug
        printf("Producer inserted item at slot %d\n", in_index % BUFFER_SIZE);
        // insert item in buffer
        buffer[in_index % BUFFER_SIZE] = item;
        in_index++;
        // update producer counter
        produced--;
        // restart
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        // simulate CPU working
        CPU_go_brrrr();
    }
}

// Consumer
void* consumer() {
    while (1) {
        // wait
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // break condition
        if (consumed == 0) {
            sem_post(&full);
            sem_post(&empty);
            pthread_mutex_unlock(&mutex);
            break;
        }
        // debug
        printf("Consumer removed item from slot %d\n", out_index % BUFFER_SIZE);
        // update consumer counter
        consumed--;
        // remove item from buffer
        buffer[out_index % BUFFER_SIZE] = 0;
        out_index++;
        // restart
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        // simulate CPU working
        CPU_go_brrrr();
    }
}

int main(int argc, char* argv[]) {
    // Get cmd args
    n_prod = atoi(argv[1]);
    n_conso = atoi(argv[2]);

    // Allocate memory for threads and buffer
    buffer = malloc(sizeof(int) * BUFFER_SIZE);
    if (buffer == NULL) return -1;
    prod = malloc(sizeof(pthread_t) * n_prod);
    if (prod == NULL) return -1;
    conso = malloc(sizeof(pthread_t) * n_conso);
    if (conso == NULL) return -1;

    // Initialize semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Threads for producing
    for (int i = 0; i < n_prod; i++) {
        pthread_create(&prod[i], NULL, producer, NULL);
    }
    // Threads for consuming
    for (int i = 0; i < n_conso; i++) {
        pthread_create(&conso[i], NULL, consumer, NULL);
    }

    // Join threads
    for (int i = 0; i < n_prod; i++) {
        pthread_join(prod[i], NULL);
    }
    for (int i = 0; i < n_conso; i++) {
        pthread_join(conso[i], NULL);
    }

    // Destroy and free
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(prod);
    free(conso);

    return 0;
}
