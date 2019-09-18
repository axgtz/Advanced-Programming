/*
 * Examples of using struct / union / enum / typedef
 *
 * Gilberto Echeverria
 * 19/08/2019
 */

#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>
#include <string.h>

#define NAME_LENGTH 60

// Define a new type called byte
typedef unsigned char byte;

// Define a new type person_t which is a structure
typedef struct person_struct {
    char name[NAME_LENGTH];
    int age;
    float height;
    float weight;
} person_t;

// Declaration of functions
void printPerson(person_t anyone);

#endif /* PERSON_H */
