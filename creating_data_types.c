//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-19.
//
#include <stdio.h>
#include <string.h>

#define NAME_LENGTH 60

// When defining a struct we need to be careful how we organize it, as it always evens to power of 2
typedef struct person_struct {     // typedef used to rename a data type
    char name[NAME_LENGTH];
    int age;
    float weight;
    float height;
} person_t;                 // _t used as a convention to indicate datatype

// Declaration should go first as good practice
void printPerson(person_t anyone);

int main () {
    struct person_struct Alex;

    Alex.age = 222;
    Alex.height = 900.2;
    Alex.weight = 900.8;

    strncpy(Alex.name,"Juan Alex Perez", NAME_LENGTH); // Copies from one string to another
    sprintf(Alex.name,"Juan Alex Perez");  // Prints text into a string, better for adding other pieces of data

    printPerson(Alex);

    return 0;
}

void printPerson(person_t anyone){
    printf("Size of struct: %ld\n", sizeof(anyone));             // size is 72 = 60 char + 4 char + 4 char + 4 int
    printf("Name: %s\n", anyone.name);
    printf("Weight: %f\n", anyone.weight);
    printf("Height: %f\n", anyone.height);
    printf("Age: %i\n", anyone.age);
}