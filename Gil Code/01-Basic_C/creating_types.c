/*
 * Examples of using struct / union / enum / typedef
 *
 * Gilberto Echeverria
 * 19/08/2019
 */

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

int main()
{
    person_t Gil;

    //byte data1;

    strncpy(Gil.name, "Gilberto Echeverria", NAME_LENGTH);
    //sprintf(Gil.name, "Gilberto Echeverria %d %f", Gil.age, Gil.height);
    Gil.age = 43;
    Gil.height = 1.76;
    Gil.weight = 73.1;

    printPerson(Gil);

    return 0;
}

// Function definitions
void printPerson(person_t anyone)
{
    printf("Size of the structure: %ld\n", sizeof(anyone));
    printf("Name: %s\n", anyone.name);
    printf("\tAge: %d\n", anyone.age);    
    printf("\tHeight: %.2f\n", anyone.height);
    printf("\tWeight: %.2f\n", anyone.weight);
}
