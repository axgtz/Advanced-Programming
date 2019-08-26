/*
 * First test of C
 *
 * Gilberto Echeverria
 * 15/08/2019
 */

#include <stdio.h>
#include <string.h>

#define STR_SIZE 50

int main()
{
    char name[STR_SIZE];
    char full_name[STR_SIZE];
    int age;
    char gender;
    FILE * file_ptr = NULL;

    printf("What is your name? ");
    //scanf("%s", name);
    fgets(name, STR_SIZE, stdin);
    // Remove the newline character
    name[strlen(name)-1] = '\0';
    printf("Hello '%s'\n", name);

    strncpy(full_name, name, STR_SIZE);

    /*
    for (int i=0; i<strlen(name); i++)
    {
        printf("\t%c\n", name[i]);
    }
    */

    printf("Enter your age: ");
    scanf("%d", &age);
    printf("You are %d years old\n", age);

    printf("Enter your gender: ");
    scanf(" %c", &gender);
    printf("Your gender is: %c\n", gender);

    // Opening a file for writing
    file_ptr = fopen("nothing.txt", "w");
    if (file_ptr)
    {
        fprintf(file_ptr, "HELLO");
    }
    fclose(file_ptr);

    return 0;
}
