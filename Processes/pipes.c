//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-12.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main (){
    FILE * in_data;
    char * command = "/Excercise/test";
    char buffer[BUFFER_SIZE]; // To read

    printf("THE POPEN PROGRAM HAS PID: %d\n", getpid());

    // open the document for reading, so we can read the output of the program
    in_data = popen(command, "r"); // communicate processes
    if(!in_data){
        perror("Unable to open command");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, BUFFER_SIZE, in_data)){
        printf("I found this: %s", buffer);
    }


    pclose(in_data);
    return 0;
}
