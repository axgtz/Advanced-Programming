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
    int num_threads;
    int i;
    pthread_t tid;
    int * index = NULL;

    printf("Number of threads to use?\n");
    scanf("%d", &num_threads);

    // Dynamically allocate memory for threads indices array
    index = malloc(num_threads * sizeof(int));

    // Create a new thread with the function as a starting point
    for (i=0; i<num_threads; i++)
    {
        index[i] = i + 1;
        pthread_create(&tid, NULL, threadStart, &index[i]);
        //printf("Created a new thread with id: %ld\n", tid);
    }

    printf("This is the main thread\n");

    free(index);
    pthread_exit(NULL);
}

void * threadStart(void * arg)
{
    int * id = (int *) arg;
    int * result = *id * *id; // Get the square 
    printf("This is the new thread: %d\n", *id);

    pthread_exit(NULL);
}
