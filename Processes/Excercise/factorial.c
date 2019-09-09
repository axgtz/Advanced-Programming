//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-09.
//

#include <stdio.h>
#include <stdlib.h>

int factorial(int n);

int main (int argc, char * argv[]){
    int result = 0;
    result = factorial(atoi(argv[1]));
    printf("RESULT: %d\n",result);

    return result;
}

int factorial(int n)
{
    if(n > 1) {
        return n * factorial(n - 1);
    }else{
        return 1;
    }
}
