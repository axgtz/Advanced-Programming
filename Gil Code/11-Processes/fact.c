#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    unsigned long result = 1;
    unsigned int num = atoi(argv[1]);

    while (num > 1)
    {
        result *= num;
        num -=1;
    }

    printf("RESULT: %ld\n", result);
    return result;
    exit(result);
}
