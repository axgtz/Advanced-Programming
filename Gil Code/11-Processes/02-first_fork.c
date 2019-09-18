/*
    Checking process IDs and doing fork

    Gilberto Echeverria
    */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // UNIX functions

int main()
{
    pid_t my_pid;
    pid_t parent_pid;
    pid_t new_pid;

    my_pid = getpid();
    parent_pid = getppid();

    printf("My PID is %d, and my parent is: %d\n", my_pid, parent_pid);

    // Create a new process
    new_pid = fork();
    // Identify the processes
    if (new_pid > 0)    // Parent process
    {
        printf("I am the parent (%d) and my child is (%d)\n", getpid(), new_pid);
        // Make the parent wait a bit
        sleep(1);
        printf("Parent finishing\n");
    }
    else if (new_pid == 0)
    {
        printf("I am the child (%d), with parent (%d)\n", getpid(), getppid());
    }
    else
    {
        printf("ERROR when doing the fork\n");
    }
    //printf("After fork my PID is %d, parent is: %d\n", getpid(), getppid());

    return 0;
}
