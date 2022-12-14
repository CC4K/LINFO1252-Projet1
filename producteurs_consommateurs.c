#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

// Global variables
int n_prod; // given by user
int n_conso; // given by user
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
pthread_t* prod;
pthread_t* conso;
// buffer, counters
int* buffer;
int in_index = 0;
int out_index = 0;
int produced = 8192;
int consumed = 8192;

void error(int err, char* msg) {
    fprintf(stderr, "Error %d : %s\n", err, msg);
    exit(EXIT_FAILURE);
}

// CPU working simulator
void CPU_go_brrrr() {
    for (int i = 0; i < 10000; i++);
}

// Producer
void* th_producer() {
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
//        printf("Producer inserted item at slot %d\n", in_index % BUFFER_SIZE);
        // insert item in buffer
        buffer[in_index % 8] = item;
        in_index++;
        // update th_producer counter
        produced--;
        // restart
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        // simulate CPU working
        CPU_go_brrrr();
    }
}

// Consumer
void* th_consumer() {
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
//        printf("Consumer removed item from slot %d\n", out_index % BUFFER_SIZE);
        // update th_consumer counter
        consumed--;
        // remove item from buffer
        buffer[out_index % 8] = 0;
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
    if (argc != 3) {
        error(-2, "Exactly 2 arguments are required");
    }
    n_prod = atoi(argv[1]);
    n_conso = atoi(argv[2]);

    // Allocate memory for threads and buffer
    buffer = malloc(sizeof(int) * 8);
    if (buffer == NULL) return -1;
    prod = malloc(sizeof(pthread_t) * n_prod);
    if (prod == NULL) return -1;
    conso = malloc(sizeof(pthread_t) * n_conso);
    if (conso == NULL) return -1;

    // Initialize semaphores and mutex
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 8);
    sem_init(&full, 0, 0);

    // Create th_producer thread
    for (int i = 0; i < n_prod; i++) {
        pthread_create(&prod[i], NULL, th_producer, NULL);
    }
    // Create th_consumer thread
    for (int i = 0; i < n_conso; i++) {
        pthread_create(&conso[i], NULL, th_consumer, NULL);
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
    free(buffer);

    return 0;
}
