// A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-15.
//
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 30

//TODO: Ask Gil to do +10 points in the assignments like Lulu
int main() {
    int arrayRandNum[ARRAY_SIZE];
    int i , userNum;
    FILE *file_ptr; //TODO: Ask Gil if we can ask for the number first then while calculating the random, do the minus in the same loop

    for(i=0; i < ARRAY_SIZE; i++) { // Populate array with random numbers
        arrayRandNum[i] = rand() % 100 + 1; // Generate random number between 1 - 100  then add to array
        printf("%d\n",arrayRandNum[i]);
    }

    printf("Write an integer number between 1 - 100\n");    // TODO: how do i do validation?
    scanf("%d", &userNum);

    for(i=0; i < ARRAY_SIZE; i++) { // Iterate array again to subtract the user number minus the number in the array
        arrayRandNum[i] = userNum - arrayRandNum[i];
        printf("%d\n",arrayRandNum[i]);
    }

    // Opening a file for writing
    file_ptr = fopen("differences.txt", "w");
    if(file_ptr){
        for(i=0; i < ARRAY_SIZE; i++){
            fprintf(file_ptr, "%d\n",arrayRandNum[i]);
        }
    }
    fclose(file_ptr);

    return 0;
}
