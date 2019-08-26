//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-26.
//
#include <stdio.h>
#include <unistd.h> // getopt
#include <stdlib.h> // atoi

int main (int argc, char * argv[]){
    int option;
    int output;
    int input;

    printf("This program got %d arguments:\n", argc);
    for (int i = 0; i < argc; ++i) {
        // The first argument would be the name of the program being executed, in this case LIVE_Arguments
        printf("\t%d: %s\n",i,argv[i]);
    }

// You need to call getopt several times for each flag
    while ((option = getopt(argc, argv, "i:o:n:yc::")) != -1) {
// Followed by : you have to provide additional arguments, :: optional argument following the flag
// no double point means you dont have to provide following argument
        switch (option){
            case 'i':
                input = atoi(optarg);  // atoi converts string to int
                break;
            case 'o':
                output = atoi(optarg);
                break;
            case'n':
                // Do something else
                break;
            case '?':
                printf("Unknown option %c\n", option);
                break;
        }
    }

    printf("Remaining arguments: \n");
    for (int j = optind; j < argc; ++j) {   // optind stores the index from argv where is currently on
        printf("argv[j]= %s\n",argv[j]);
    }

    return 0;
}
