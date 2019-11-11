/*
    First example of using threads in C

    Gilberto Echeverria
    04/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Function declaration
void * threadStart(void * arg);

int main()
{
    int num_threads = 5;
    int i;
    pthread_t tid;
    int index[num_threads];

    // Create a new thread with the function as a starting point
    for (i=0; i<num_threads; i++)
    {
        index[i] = i + 1;
        pthread_create(&tid, NULL, threadStart, &index[i]);
        //printf("Created a new thread with id: %ld\n", tid);
    }

    printf("This is the main thread\n");

    pthread_exit(NULL);
}

void * threadStart(void * arg)
{
    int * id = (int *) arg;
    printf("This is the new thread: %d\n", *id);

    pthread_exit(NULL);
}
