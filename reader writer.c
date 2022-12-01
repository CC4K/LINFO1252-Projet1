#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

int n_writer;
int n_readers;




pthread_mutex_t mutex_readcount; // Protège readcount
pthread_mutex_t mutex_writercount; // Protège writecount

sem_t wsem; //Accès exclusif à la db
sem_t rsem; //Pour bloquer des readers

int readcount=0;
int writecount=0;

sem_init(&wsem,0,1);
sem_init(&rsem,0,1);


void write_data(){
    for (int i=0; i<10000; i++);
}
void read_database(){
    for (int i=0; i<10000; i++);
}

void writer(){
    while(true){


        pthread_mutex_lock(&mutex_writercount);

        writecount=writecount+1;
        if (writecount ==1){
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mutex_writercount);


        sem_wait(&wsem);
        write_data();
        sem_post(&wsem);

        pthread_mutex_lock(&mutex_writercount);
        writecount=writecount-1;
        if (writecount==0){
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mutex_writercount);

    }

}
void reader(){
    while(true){
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);
        readcount=readcount+1;
        if(readcount==1){
            sem_wait(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem);
        read_database();

        pthread_mutex_lock(&mutex_readcount);
       readcount=readcount-1;
        if(readcount==0){
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
    }

}
int main(int argc, char* argv[]) {

    n_writer = atoi(argv[1]);
    n_readers = atoi(argv[2]);
}
