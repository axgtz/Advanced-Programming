/*
    Send signals to other programs

    Gilberto Echeverria
    10/10/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        printf("Please provide a PID as argument\n");
        exit(EXIT_SUCCESS);
    }

    // Get the PID of the other process
    pid_t victim = atoi(argv[1]);
    if (kill(victim, SIGCONT) == -1)
    {
        printf("Could not kill the requested process\n");
    }

    return 0;
}
