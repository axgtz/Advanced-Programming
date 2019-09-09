//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-05.
//

#include <stdio.h>
#include <unistd.h> // pid

int main (int argc, char ** argv){
    int counter = 0;

    printf("Program begin\n");


    pid_t newPid = fork();

    if (newPid == 0){
        // Child process
        int i = 0;
        printf("I am the parent (%d) and my child is (%d)\n",getpid(),newPid);
        for(; i< 2; i++){
            printf("Child process: counter=%d\n", counter++);
        }

    } else if (newPid > 0){
        // Parent process
        int j = 0;

        for(; j< 2 ; j++){
            printf("Parent process: counter=%d\n", counter++);
        }

    } else {
        // fork failed
        printf("fork() failed");
        return 1;
    }
    return 0;
}
