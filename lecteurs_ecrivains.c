#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

// Global variables
int n_writer; // given by user
int n_reader; // given by user
pthread_mutex_t mutex_writecount;
pthread_mutex_t mutex_readcount;
sem_t wsem;
sem_t rsem;
pthread_t* th_writer;
pthread_t* th_reader;
// counters
int readcount = 0;
int writecount = 0;
int lu = 0;
int ecrit = 0;

void error(int err, char* msg) {
    fprintf(stderr, "Error %d : %s\n", err, msg);
    exit(EXIT_FAILURE);
}

// CPU working simulator
void CPU_go_brrrr() {
    for (int i = 0; i < 10000; i++);
}

void* writer() {
    while (1) {
        pthread_mutex_lock(&mutex_writecount);
        // check counter
//        printf("writer is writing / ecrit = %d\n", ecrit);
        ecrit++;
        // break condition
        if (ecrit > 640) {
            pthread_mutex_unlock(&mutex_writecount);
            break;
        }
        writecount++;
        if (writecount == 1) {
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);

        sem_wait(&wsem);
        CPU_go_brrrr();
        sem_post(&wsem);

        pthread_mutex_lock(&mutex_writecount);
        writecount--;
        if (writecount == 0) {
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
    }

}

void* reader() {
    while (1) {
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);
        // check counter
//        printf("reader is reading / lu = %d\n", lu);
        lu++;
        // break condition
        if (lu > 2560) {
            sem_post(&rsem);
            pthread_mutex_unlock(&mutex_readcount);
            break;
        }
        readcount++;
        if (readcount == 1) {
            sem_wait(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);

        sem_post(&rsem);
        CPU_go_brrrr();

        pthread_mutex_lock(&mutex_readcount);
        readcount--;
        if (readcount == 0) {
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
    }

}

int main(int argc, char* argv[]) {
    // Get cmd args
    if (argc != 3) {
        error(-2, "Exactly 2 arguments are required");
    }
    n_writer = atoi(argv[1]);
    n_reader = atoi(argv[2]);

    // Allocate memory for threads
    th_writer = malloc(sizeof(pthread_t) * n_writer);
    if (th_writer == NULL) return -1;
    th_reader = malloc(sizeof(pthread_t) * n_reader);
    if (th_reader == NULL) return -1;


    // Initialize semaphores and mutex
    pthread_mutex_init(&mutex_writecount, NULL);
    pthread_mutex_init(&mutex_readcount, NULL);
    sem_init(&wsem, 0, 1);
    sem_init(&rsem, 0, 1);

    // Create th_writer thread
    for (int i = 0; i < n_writer; i++) {
        pthread_create(&th_writer[i], NULL, writer, NULL);
    }
    // Create th_reader thread
    for (int i = 0; i < n_reader; i++) {
        pthread_create(&th_reader[i], NULL, reader, NULL);
    }

    // Join threads
    for (int i = 0; i < n_writer; i++) {
        pthread_join(th_writer[i], NULL);
    }
    for (int i = 0; i < n_reader; i++) {
        pthread_join(th_reader[i], NULL);
    }

    // Destroy and free
    pthread_mutex_destroy(&mutex_writecount);
    pthread_mutex_destroy(&mutex_readcount);
    sem_destroy(&wsem);
    sem_destroy(&rsem);
    free(th_writer);
    free(th_reader);

    return 0;
}
