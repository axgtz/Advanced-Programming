/*
    Test of using popen to get the output of a command

    Gilberto Echeverria
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main()
{
    FILE * in_data;
    char * command = "../11-Processes/test";
    char buffer[BUFFER_SIZE];

    printf("THE POPEN PROGRAM HAS PID: %d\n", getpid());

    // Open the command to read its output
    in_data = popen(command, "r");
    if (!in_data)
    {
        perror("Unable to open command");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, BUFFER_SIZE, in_data))
    {
        printf("I found this: %s", buffer);
    }
    
    // Close the file pointer
    pclose(in_data);

    return 0;
}
