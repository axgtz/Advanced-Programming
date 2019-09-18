/*
 * Examples of using struct / union / enum / typedef
 *
 * Gilberto Echeverria
 * 19/08/2019
 */

#include <stdio.h>
#include <string.h>
#include "person.h"

int main()
{
    person_t Gil;

    strncpy(Gil.name, "Gilberto Echeverria", NAME_LENGTH);
    //sprintf(Gil.name, "Gilberto Echeverria %d %f", Gil.age, Gil.height);
    Gil.age = 43;
    Gil.height = 1.76;
    Gil.weight = 73.1;

    printPerson(Gil);

    return 0;
}
