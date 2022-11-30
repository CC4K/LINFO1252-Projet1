#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int nPhilo;
pthread_mutex_t *baguette;
pthread_t *phil;

void error(int err, char* msg) {
    fprintf(stderr, "%s a retourné %d message d'erreur : %s\n", msg, err, strerror(errno));
    exit(EXIT_FAILURE);
}

void mange(int id) {
    printf("Philosophe [%d] mange\n", id);
    for (int i = 0; i < rand(); i++) {
        // philosophe mange
    }
}

void* philosophe(void* arg) {
    int* id = (int*) arg;
    int left = *id;
    int right = (left + 1) % nPhilo;
    for (int i = 0; i < 100000; i++) {
        printf("Exécution n°%d\n", i);
        printf("Philosophe [%d] pense\n", *id);
        if (left < right) {
            pthread_mutex_lock(&baguette[left]);
            printf("Philosophe [%d] possède baguette gauche [%d]\n", *id, left);
            pthread_mutex_lock(&baguette[right]);
            printf("Philosophe [%d] possède baguette droite [%d]\n", *id, right);
        }
        else {
            pthread_mutex_lock(&baguette[right]);
            printf("Philosophe [%d] a libéré baguette gauche [%d]\n", *id, left);
            pthread_mutex_lock(&baguette[left]);
            printf("Philosophe [%d] a libéré baguette droite [%d]\n", *id, right);
        }
        mange(*id);
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}

int main(int argc, char* argv[]) {
    nPhilo = atoi(argv[1]);
    baguette = malloc(sizeof(pthread_mutex_t) * nPhilo);
    phil = malloc(sizeof(pthread_t) * nPhilo);
    int i;
    int id[nPhilo];
    int err;

    srand(getpid());

    for (i = 0; i < nPhilo; i++)
        id[i] = i;

    for (i = 0; i < nPhilo; i++) {
        err = pthread_mutex_init(&baguette[i], NULL);
        if (err != 0)
            error(err, "pthread_mutex_init");
    }

    for (i = 0; i < nPhilo; i++) {
        err = pthread_create(&phil[i], NULL, philosophe, (void*) &(id[i]));
        if (err != 0)
            error(err, "pthread_create");
    }

    for (i = 0; i < nPhilo; i++) {
        pthread_join(phil[i], NULL);
        if (err != 0)
            error(err, "pthread_join");
    }

    for (i = 0; i < nPhilo; i++) {
        pthread_mutex_destroy(&baguette[i]);
        if (err != 0)
            error(err, "pthread_mutex_destroy");
    }

    return (EXIT_SUCCESS);
}
