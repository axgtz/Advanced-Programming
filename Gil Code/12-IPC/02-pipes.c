/*
    Checking process IDs and doing fork

    Gilberto Echeverria
    */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // UNIX functions

#define BUFFER_LENGTH 100

void preparePipes(int * in_pipe, int * out_pipe);

int main()
{
    pid_t new_pid;
    int status;
    char buffer[BUFFER_LENGTH];
    // Pipe arrays
    int parent_to_child[2];
    int child_to_parent[2];

    printf("My PID is %d, and my parent is: %d\n", getpid(), getppid());

    // Open the pipes
    status = pipe(parent_to_child);
    if (status < 0) {perror("PIPE"); exit(EXIT_FAILURE); }
    status = pipe(child_to_parent);
    if (status < 0) {perror("PIPE"); exit(EXIT_FAILURE); }

    printf("Parent to child: %d %d\n", parent_to_child[0], parent_to_child[1]);

    // Create a new process
    new_pid = fork();
    // Identify the processes
    if (new_pid > 0)    // Parent process
    {
        preparePipes(child_to_parent, parent_to_child);
        printf("I am the parent (%d) and my child is (%d)\n", getpid(), new_pid);
        sprintf(buffer, "Hello child\n");
        write(parent_to_child[1], buffer, strlen(buffer));
    }
    else if (new_pid == 0)
    {
        preparePipes(parent_to_child, child_to_parent);
        printf("I am the child (%d), with parent (%d)\n", getpid(), getppid());
        read(parent_to_child[0], buffer, BUFFER_LENGTH);
        printf("My parent sent: %s", buffer);
    }
    else
    {
        printf("ERROR when doing the fork\n");
    }
    //printf("After fork my PID is %d, parent is: %d\n", getpid(), getppid());

    return 0;
}

void preparePipes(int * in_pipe, int * out_pipe)
{
    // Close the pipe ends that will not be used
    close(in_pipe[1]);  // Close the writing file descriptor
    close(out_pipe[0]); // Close the reading file descriptor
}
