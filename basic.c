/*
* A01019608
* Roberto Alejandro Gutierrez Guillen
* 15/08/2019
*/
#include <stdio.h>
#include <string.h>

#define STR_SIZE 50

int main() {
    char name[STR_SIZE];
    int age;
    char gender;
    int nothing;
    FILE *file_ptr;

    age = nothing + 1;

    printf("What is your name?\n");
    //scanf("%s",name);               // "%s" Read string, doesnt take into account enter. Scanf can provoke stackoverflow
    fgets(name, STR_SIZE, stdin);         // Equivalent a get line
    name[strlen(name)-1] = '\0';                 // \0 = end of the string
    // Instead of printf("Hello ", name , "\n"); cause C needs you to specify the type of variable
    printf("Hello %s\n", name);

    printf("What is your age?\n");
    // &age needs the address of the variable, the previous scanf doesnt need a & cause arrays are already pointers
    scanf("%d", &age);
    // %d is for numbers with base 10 and %i auto detects the base
    printf("You are %d years old \n",age);

    printf("What is your gender?\n");
    scanf(" %c", &gender);                       // Add a space before %c to ignore the previous \n
    printf("Your gender is: %c\n", gender);

    // Opening a file for writing
    file_ptr = fopen("nothing.txt", "w");
    if(file_ptr){
        fprintf(file_ptr, "HELLO");
    }
    fclose(file_ptr);

    return 0;
}