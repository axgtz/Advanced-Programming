/*
    Multiple threads working on the same variable
    Using mutexes to avoid conflict

    Gilberto Echeverria
    11/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define MAX_COUNT 1000000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int counter = 0;

// Declaration of thread function
void * increment(void * arg);

int main()
{
    pthread_t tid[NUM_THREADS];

    // Create the threads
    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_create(&tid[i], NULL, increment, NULL);
    }

    // Wait for the threads to finish
    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    printf("The counter is %d / %d\n", counter, NUM_THREADS * MAX_COUNT);
}

void * increment(void * arg)
{
    printf("Thread started\n");
    // Critical section
    for (int i=0; i<MAX_COUNT; i++)
    {
        pthread_mutex_lock(&mutex);
            counter++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
