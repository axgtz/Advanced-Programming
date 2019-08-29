//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-29.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definition data type
typedef struct info_structure{
    int size;
    int * data;
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


    return 0;
}

info_t * initInfo(int size){

}

void fillRandom(info_t * info){

}

void printInfo(info_t * info){

}

