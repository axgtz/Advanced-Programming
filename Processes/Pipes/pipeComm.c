//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-12.
// PIPE COMMUNICATION

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // UNIX functions
#include <string.h>

#define BUFFER_LENGTH 100

void preparePipes(int * in_pipe, int * out_pipe);

int main(){
    char buffer[BUFFER_LENGTH]; // To read
    pid_t new_pid;
    int status = -1;

    // Pipe Arrays
    int parent_to_child[2]; // file descriptor
    int child_to_parent[2];

    printf("My PID is (%d) and my parent PID is (%d)",getpid(),getppid());

    // open pipes
    status = pipe(parent_to_child);
    if (status < 0) { perror("PIPE");exit(EXIT_FAILURE);}
    status = pipe(child_to_parent);
    if (status < 0) { perror("PIPE");exit(EXIT_FAILURE);}

    printf("Parent to child: [%d][%d]\n", parent_to_child[0], parent_to_child[1]);
    printf("Child to parent: [%d][%d]\n", child_to_parent[0], child_to_parent[1]);

    // Create a new process
    new_pid = fork();

    //Identify Process
    if(new_pid > 0){ // Parent
        preparePipes(child_to_parent, parent_to_child);

        printf("I am the parent (%d) and my child is (%d)\n", getpid(), new_pid);

        sprintf(buffer, "Hello Child\n");
        write(parent_to_child[1], buffer, strlen(buffer));

    } else if (new_pid == 0){ // Child
        preparePipes(child_to_parent, parent_to_child);
        printf("I am the child with PID (%d) and my parent (%d)\n",getpid(), getppid());
        read(parent_to_child[0], buffer, BUFFER_LENGTH);
        printf("%s", buffer); // Print the recieved buffer
    }else{
        printf("ERROR on fork");
    }

    return 0;
}

// Close the pipe ends that will not be used
void preparePipes(int * in_pipe, int * out_pipe){
    close(in_pipe[1]);  // Close the writing file descriptor
    close(in_pipe[0]);  // Close the reading file descriptor
}
