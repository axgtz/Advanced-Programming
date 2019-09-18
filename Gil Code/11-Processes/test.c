#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    printf("THIS IS THE PROGRAM %s\n", argv[0]);
    printf("My process ID is: %d\n", getpid());
    printf("My parent PID is: %d\n", getppid());
    fflush(stdout);
    sleep(10);
    printf("Now the test is going to finish\n");
}
