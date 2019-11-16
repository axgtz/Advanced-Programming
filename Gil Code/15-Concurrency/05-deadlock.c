/*
    Multiple threads working on two variables
    Using mutexes to avoid conflict
    Each variable has a mutex

    Gilberto Echeverria
    11/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
#define MAX_COUNT 1000000

pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;

int counter_1 = 0;
int counter_2 = MAX_COUNT;

// Declaration of thread function
void * increment(void * arg);
void * decrement(void * arg);

int main()
{
    pthread_t tid[NUM_THREADS];

    // Create the threads
    pthread_create(&tid[0], NULL, increment, NULL);
    pthread_create(&tid[1], NULL, decrement, NULL);

    // Wait for the threads to finish
    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    printf("The counter is %d / %d\n", counter_1, counter_2);
}

void * increment(void * arg)
{
    printf("Increment Thread started\n");
    // Critical section
    for (int i=0; i<MAX_COUNT; i++)
    {
        pthread_mutex_lock(&mutex_1);
        pthread_mutex_lock(&mutex_2);
            counter_1++;
            counter_2--;
        pthread_mutex_unlock(&mutex_1);
        pthread_mutex_unlock(&mutex_2);
    }
    pthread_exit(NULL);
}

void * decrement(void * arg)
{
    printf("Decrement Thread started\n");
    // Critical section
    for (int i=0; i<MAX_COUNT; i++)
    {
        pthread_mutex_lock(&mutex_2);
        pthread_mutex_lock(&mutex_1);
            counter_2--;
            counter_1++;
        pthread_mutex_unlock(&mutex_2);
        pthread_mutex_unlock(&mutex_1);
    }
    pthread_exit(NULL);
}
