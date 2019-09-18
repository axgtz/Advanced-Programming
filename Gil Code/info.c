/*
    Examples of pointers to structures

    Gilberto Echeverria
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definition of data types
typedef struct info_structure {
    int size;       // 4 bytes
    int * data;     // 8 bytes
} info_t;

// Function declarations
info_t * initInfo(int size);
void fillRandom(info_t * info);
void printInfo(info_t * info);
void freeInfo(info_t * info);

int main()
{
    info_t * info = NULL;
    int size;

    // Initialize random seed
    srand(time(NULL));

    size = rand() % 30 + 5;

    info = initInfo(size);
    fillRandom(info);
    printInfo(info);
    freeInfo(info);

    return 0;
}

// Allocate the memory for an info structure
info_t * initInfo(int size)
{
    info_t * info = NULL;

    // Get memory for the structure
    info = malloc(sizeof (info_t));
    // Get memory for the internal array
    info->data = malloc(size * sizeof (int));
    // Store the size
    info->size = size;

    return info;
}

// Fill the array with numbers
void fillRandom(info_t * info)
{
    for (int i=0; i<info->size; i++)
    {
        // An integer in the range [1, 100]
        info->data[i] = rand() % 100 + 1; 
    }
}

// Print the contents of the structure
void printInfo(info_t * info)
{
    printf("Structure of %d ints\n", info->size);
    for (int i=0; i<info->size; i++)
    {
        printf("array[%d] = %d\n", i, info->data[i]);
    }
}

// Release the memory
void freeInfo(info_t * info)
{
    free(info->data);
    free(info);
}
