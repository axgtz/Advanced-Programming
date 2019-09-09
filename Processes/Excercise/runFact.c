//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    char * command = "./fact";
    char * arguments[] = {"./fact", "5", (char *) NULL}; // The first argument should be command
    pid_t new_pid;
    int status = 0;

    new_pid = fork();

    if(new_pid > 0){ // Parent
        printf("Parent (%d) is waiting for the child (%d)\n", getpid(), new_pid);
        wait(&status);
        printf("Now the parent can continue\n");
        printf("Child finished with status: %d\n", status);

    } else if (new_pid == 0){ // Child
        printf("Switching to commandls '%s'\n", command);
        status = execvp(command, arguments);            // Takes a vector for the list of arguments and the path
        if(status < 0){
            printf("ERROR on exec\n");
        }
    }else{
        printf("ERROR on fork");
    }

    return 0;
}
