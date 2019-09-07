//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-28.
//

#include "MatrixMult.h"

void  masterMultiply(struct arguments * arg){
    matrix * mx1 = NULL;
    matrix * mx2 = NULL;
    matrix * resMax = NULL;

    mx1 = readFile(arg->nameMat1);

    mx2 = readFile(arg->nameMat2);

    //  Check if they can be multiplied
    if(mx1->columnsNum == mx2->rowsNum){
        printMatrix(mx1);
        printMatrix(mx2);
        resMax = multiplyM(mx1, mx2);
        free(mx1->arrayPointer);
        free(mx1);
        free(mx2->arrayPointer);
        free(mx2);
        writeFile(resMax, arg->nameRes);
    }else{
        printf("CANNOT MULTIPLY THESE MATRICES DUE TO DIMENSIONS\n");
    }
}

matrix* readFile(char* nameFile) {
    FILE* file_ptr = NULL; // Initialize pointer to file
    matrix * mx = NULL;            // Declare matrix struct
    int rowsNum;
    int columnsNum;

    // Opening a file for reading
    file_ptr = fopen(nameFile, "r");
    if(file_ptr == NULL){
        printf("Error when reading file");
    }

    // Saves the first numbers into the matrix, first the row then the column

    fscanf(file_ptr, "%i", rowsNum);
    fscanf(file_ptr, "%i",columnsNum);

    mx = matrixCreator(rowsNum, columnsNum);

    // Fill the matrix, cycle through the values in the file
    for(int i = 0; i < mx->rowsNum; i++){
        for(int j = 0; j < mx->columnsNum; j++){
            fscanf(file_ptr, "%f", &mx->arrayPointer[i*mx->columnsNum+j]);
        }
    }

    // Close file stream
    fclose(file_ptr);

    return mx;
}

matrix* multiplyM(const matrix * mx1, const matrix * mx2) {
    // Initialize the result matrix
    matrix * resultMatrix = matrixCreator(mx1->rowsNum, mx2->columnsNum);

    // Do multiplication
    for (int i = 0; i < mx1->rowsNum; ++i) {
        for (int j = 0; j < mx2->columnsNum; ++j) {
            for (int k = 0; k < mx1->columnsNum; ++k) {
                resultMatrix->arrayPointer[i * mx1->rowsNum + j] =mx1->arrayPointer[i * mx1->columnsNum + k] * mx2->arrayPointer[j * mx1->columnsNum + k];
                // printf(" %f" ,resultMatrix->arrayPointer[i * mx1->rowsNum + j]);
            }
        }
    }
    printMatrix(resultMatrix);

    return resultMatrix;
}

void writeFile(const matrix * mx,char* nameFile ){
    FILE * file_ptr = NULL; // Initialize pointer to file
    file_ptr = fopen(nameFile, "w");

    for(int i = 0; i < mx->rowsNum; i++){
        for (int j = 0; j < mx->columnsNum; j++){
            fprintf(file_ptr, "%.2f ", mx->arrayPointer[i*mx->columnsNum+j]); // Multiply by column then add the column
            //printf( "%f ", mx->arrayPointer[i][j]); Could use this if i had allocated the rows and columns separately
        }
        fprintf(file_ptr,"\n");
    }
    fprintf(file_ptr,"\n");

    // Liberate memory for matrix
    //free(mx);
    fclose(file_ptr); // Close file
}

void printMatrix(const matrix * mx){
    // Print the dimensions
    printf("Printing matrix with the following dimensions: \n\t Width:%.0i Height:%.0i\n",mx->columnsNum,mx->rowsNum);
    // Iterate through the matrix
    for(int i = 0; i < mx->rowsNum; i++){
        for (int j = 0; j < mx->columnsNum; j++){
            printf( "%.2f ", mx->arrayPointer[i*mx->columnsNum+j]); // Multiply by column then add the column
            //printf( "%f ", mx->arrayPointer[i][j]); Could use this if i had allocated the rows and columns separately
        }
        printf("\n");
    }
    printf("\n");
}

matrix * matrixCreator(int rowsNum, int columnsNum){
    // Initialize the result matrix
    matrix * mx = NULL;
    // Allocate memory
    mx = malloc(sizeof(matrix));

    // Allocate memory for the array, initialize array in 0. Cast to avoid errors
    mx->arrayPointer = (float*) calloc(rowsNum * columnsNum, sizeof(float));

    return mx;
}