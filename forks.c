//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-05.
//

#include <stdio.h>
#include <unistd.h> // pid

int main (int argc, char ** argv){
    printf("Beginning of Program\n");

    int counter = 0;

    pid_t  pid = fork();

    if (pid == 0){
        // Child process
        int i = 0;

        for(; i< 50 ; i++){
            printf("Child process: counter=%d\n", counter++);
        }

    } else if (pid > 0){
        // Parent process
        int j = 0;

        for(; j< 50 ; j++){
            printf("Parent process: counter=%d\n", counter++);
        }

    } else {
        // fork failed
        printf("fork() failed");
        return 1;
    }

    return 0;
}
