//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-28.
//

#ifndef ADVANCED_PROGRAMMING_MATRIXMULT_H
#define ADVANCED_PROGRAMMING_MATRIXMULT_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME 50 //max characters for the filename


typedef struct matrixS{
    int rowsNum;
    int columnsNum;
    float * arrayPointer;
} matrix;


typedef struct arguments{
    char nameMat1[MAX_NAME];
    char nameMat2[MAX_NAME];
    char nameRes[MAX_NAME];
} argm;


void  masterMultiply(struct arguments * );

matrix * readFile(char * matrixName);

// By sending the pointer as a const the data is not modifiable
matrix * multiplyM(const matrix * mx1, const matrix * mx2);

void writeFile(const matrix * mx, char* nameFile);

void printMatrix(const matrix * mx);

matrix * matrixCreator(int rowsNum, int columnsNum);

#endif //ADVANCED_PROGRAMMING_MATRIXMULT_H
