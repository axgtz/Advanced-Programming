/*
    Test of the "system" call

    Gilberto Echeverria
    */

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int status;
    char * command = "eog";

    status = system(command);
    printf("System returned %d\n", status);

    /*
    command = "pstree";
    status = system(command);
    printf("System returned %d\n", status);
    */

    return 0;
}
