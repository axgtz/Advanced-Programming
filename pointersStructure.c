//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-29.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definition data type
typedef struct info_structure{
    int size;       // 4 bytes
    int * data;     // 8 bytes
} info_t;

// Function declarations
info_t * initInfo(int size);
void fillRandom(info_t * info);
void printInfo(info_t * info);

int main() {
    info_t * info = NULL;
    int size = 10;

    // Initialize random seed
    srand(time(NULL));

    info = initInfo(size);
    fillRandom(info);
    printInfo(info);

    free(info->data);
    free(info);

    return 0;
}

info_t * initInfo(int size){
    info_t * info = NULL;

    // Get memory for the structure
    info = malloc(sizeof(info_t));

    // Get memory for the internal array
    info->data = malloc(size * sizeof(int));

    // Store the size
    info->size = size;

    return info;
}

void fillRandom(info_t * info){
    for (int i = 0; i < info->size; ++i) {
        // Integer in the range of 1 - 100
        info->data[i] = rand() % 100 + 1;
    }
}

void printInfo(info_t * info){
    printf("Structure of %d ints\n", info->size);
    for (int i = 0; i < info->size; ++i) {
        printf("array[%d] = %d\n", i ,info->data[i]);
    }
}
