//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-09.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // exec
// exec is used to switch the current process for another

int main (int argc, char * argv[]){
    /*char * command = "ls -l /";        // the / is for the root directory

    printf("BEFORE EXEC\n");
    system(command);
    printf("AFTER EXEC\n");
     */
    int status = 0;
    char * command = "/Users/axguti/Documents/GitHub/Advanced-Programming/Processes/forks";

    printf("BEFORE EXEC, pid %d\n", getpid());
    status = execl(command, "forks", (char *) NULL); // char * NULL indicates that the list of arguments is over
    if(status == -1){
        printf("Error on exec\n");
    }
    // The folloing code is never executed, as exec changes the code
    printf("AFTER EXEC\n");

    return 0;
}
