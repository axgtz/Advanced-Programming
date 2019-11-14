//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-11-11.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 6
#define MAX_COUNT 1000000

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * increment(void * arg);

int main(){
    pthread_t tid[NUM_THREADS];

    // Create threads
    for(int i  = 0; i < NUM_THREADS; i++){
        pthread_create(&tid[i], NULL, increment, NULL);
    }

    // Wait for the threads to finish
    for(int i  = 0; i < NUM_THREADS; i++){
        pthread_join(tid[i], NULL);
    }

    printf("The counter is %d ÷ %d\n", counter, NUM_THREADS * MAX_COUNT);


}

void * increment(void * arg){
    printf("Thread started \n");
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < MAX_COUNT; ++i) {
        // Critical section
        counter++;
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}