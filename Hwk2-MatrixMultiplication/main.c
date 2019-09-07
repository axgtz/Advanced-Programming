//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-01.
//

#include "MatrixMult.h"
#include <unistd.h> // getopt
#include <string.h>


int main (int argc, char *argv[]) {
    int option;
    printf("MAXIMUM NAME FOR FILES IS 50 CHARACTERS\n");

    // Initialize struct of arguments
    argm * a = NULL;
    a = malloc(sizeof(struct arguments));

    if (argc != 7){
        printf("Needs exactly 7 arguments!");
    }else{
// You need to call getopt several times for each flag
    while ((option = getopt(argc, argv, "1:2:r:")) != -1) {
// Followed by : you have to provide additional arguments, :: optional argument following the flag
// no double point means you dont have to provide following argument
        switch (option){
            case '1':
                strcpy(a->nameMat1, optarg);
                break;
            case '2':
                strcpy(a->nameMat2, optarg);
                break;
            case'r':
                strcpy(a->nameRes, optarg);
                break;
            case '?':
                printf("Unknown option %c\n", option);
                break;
        }
    }
    //strcpy(a->nameMat1, argv[2]);
    //strcpy(a->nameMat2, argv[4]);
    //strcpy(a->nameRes, argv[6]);

        masterMultiply(a);
    }

    return 0 ;
}
