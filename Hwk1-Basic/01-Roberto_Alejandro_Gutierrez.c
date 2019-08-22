// A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-15.
//
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 30       // Assign constant of array size

int main() {
    int arrayRandNum[ARRAY_SIZE];
    int i , userNum;
    FILE *file_ptr;

    for(i=0; i < ARRAY_SIZE; i++) { // Populate array with random numbers
        arrayRandNum[i] = rand() % 100 + 1; // Generate random number between 1 - 100  then add to array
        //-printf("%d\n",arrayRandNum[i]);
    }

    printf("Write an integer number between 1 - 100\n");
    scanf("%d", &userNum);          // Read number from user

    // Opening a file for writing
    file_ptr = fopen("differences.txt", "w"); // open file stream, as w to write as new file
    if(file_ptr){                                              // Validate that file exists
        for(i=0; i < ARRAY_SIZE; i++){                          // Write on file a number per row
            arrayRandNum[i] = userNum - arrayRandNum[i];    // Substract from the user number, the number in the array
            fprintf(file_ptr, "%d\n",arrayRandNum[i]);
        }
    }
    fclose(file_ptr); // Close file stream

    return 0;
}
