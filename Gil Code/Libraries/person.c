/*
 * Examples of using struct / union / enum / typedef
 *
 * Gilberto Echeverria
 * 19/08/2019
 */

#include "person.h"

// Function definitions
void printPerson(person_t anyone)
{
    printf("Size of the structure: %ld\n", sizeof(anyone));
    printf("Name: %s\n", anyone.name);
    printf("\tAge: %d\n", anyone.age);    
    printf("\tHeight: %.2f\n", anyone.height);
    printf("\tWeight: %.2f\n", anyone.weight);
}
