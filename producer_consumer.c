#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define NProd 3
#define NConso 3
// à mettre dans le cmd

#define N 8
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int in_index = 0;
int out_index = 0;
int buffer[N];

// Producteur
void* producer(void* pro) {
    int item;
    for (int i = 0; i < 10; i++) {
        item = rand(); // produce random int
        sem_wait(&empty); // wait for free slots in_index buffer
        pthread_mutex_lock(&mutex);
        // section critique
        buffer[in_index] = item;
        printf("Producteur %d: inserted item %d at slot %d\n", *(int*) pro, buffer[in_index], in_index);
        in_index = (in_index + 1) % N; // update index
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // post that a slot has been filled
    }
}

// Consommateur
void* consumer(void* con) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&full); // wait for empty slots
        pthread_mutex_lock(&mutex);
        // section critique
        int item = buffer[out_index];
        printf("Consumer %d: removed item %d from slot %d\n", *(int*) con, item, out_index);
        out_index = (out_index + 1) % N; // update index
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // post that a slot has been filled
    }
}

int main(int argc, char* argv[]) {
    printf("Hello there !\n");

    pthread_t pro[8], con[8];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    int a[8] = {1,2,3,4,5,6,7,8};

    for (int i = 0; i < NProd; i++) {
        pthread_create(&pro[i], NULL, producer, &a[i]);
    }
    for (int i = 0; i < NConso; i++) {
        pthread_create(&con[i], NULL, consumer, &a[i]);
    }

    for (int i = 0; i < NProd; i++) {
        pthread_join(pro[i], NULL);
    }
    for (int i = 0; i < NConso; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
