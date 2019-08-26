//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-26.
//
#include <stdio.h>

int main(){
    int variable;
    int * pointer = NULL;
    double another;
    double * fptr = &another;  // point to the address of another

    variable = 123;
    pointer = &variable;

    printf("");// Pointer to nothing, as you dont know what type will be

    return 0;
}