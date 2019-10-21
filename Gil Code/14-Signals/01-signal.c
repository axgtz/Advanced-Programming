//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-10-10.
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char * argv[]) {
    if(argc != 2){
        printf("Please provide a valid PID as an argument \n");
        exit(EXIT_SUCCESS);
    }
    pid_t victim = atoi(argv[1]);

    if ( kill(victim, SIGKILL) == -1){
        printf("The program cannot be killed\n");
    }

    return 0;
}