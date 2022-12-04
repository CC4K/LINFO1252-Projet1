#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define CYCLES 100000

// Global variables
int nPhilo; // given by user
pthread_mutex_t* baguette;
__thread int cycle_counter = 0;

void error(int err, char* msg) {
    fprintf(stderr, "Error %d : %s\n", err, msg);
    exit(EXIT_FAILURE);
}

void mange(int id) {
//    printf("Philosophe [%d] mange\n", id);
}

void* philosophe(void* arg) {
    int* id = (int*) arg;
    int left = *id;
    int right = (left + 1) % nPhilo;
    if (nPhilo == 1) right = 1; // otherwise program will crash if given arg = 1

    while (cycle_counter < CYCLES) {
//        printf("Exécution n°%d\n", cycle_counter);
//        printf("Philosophe [%d] pense\n", *id);

        if (left < right) {
            pthread_mutex_lock(&baguette[left]);
//            printf("Philosophe [%d] possède baguette gauche [%d]\n", *id, left);
            pthread_mutex_lock(&baguette[right]);
//            printf("Philosophe [%d] possède baguette droite [%d]\n", *id, right);
        }
        else {
            pthread_mutex_lock(&baguette[right]);
//            printf("Philosophe [%d] a libéré baguette gauche [%d]\n", *id, left);
            pthread_mutex_lock(&baguette[left]);
//            printf("Philosophe [%d] a libéré baguette droite [%d]\n", *id, right);
        }
        mange(*id);
        cycle_counter++;
        // we can now unlock the baguettes
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}

int main(int argc, char* argv[]) {
    // Get cmd arg
    if (argc != 2) {
        error(-2, "Exactly 1 argument is required");
    }
    nPhilo = atoi(argv[1]);

    // Initialize vars
    int nBaguettes = nPhilo;
    if (nPhilo == 1) nBaguettes = 2; // otherwise program will crash if given arg = 1
    int err;

    // Allocate memory
    baguette = malloc(sizeof(pthread_mutex_t) * nBaguettes); // and not nPhilo or won't work for arg = 1
    if (baguette == NULL) return -1;
    pthread_t phil[nPhilo];
    int* id = malloc(sizeof(int) * nBaguettes);
    if (id == NULL) return -1;
    if (nPhilo == 1) id[1] = 1; // for arg = 1

    // Initialize
    for (size_t k = 0; k < nBaguettes; k++) {
        err = pthread_mutex_init(&baguette[k], NULL);
        if (err != 0) error(err, "pthread_mutex_init");
    }

    // Create
    for (int k = 0; k < nPhilo; k++) {
        id[k] = k;
        err = pthread_create(&phil[k], NULL, philosophe, &(id[k]));
        if (err != 0) error(err, "pthread_create");
    }

    // Join
    for (int k = 0; k < nPhilo; k++) {
        pthread_join(phil[k], NULL);
        if (err != 0) error(err, "pthread_join");
    }

    // Destroy
    for (int k = 0; k < nPhilo; k++) {
        pthread_mutex_destroy(&baguette[k]);
        if (err != 0) error(err, "pthread_mutex_destroy");
    }

    // Free mallocs
    free(baguette);
    free(id);

    return (EXIT_SUCCESS);
}
