/*
    Program to determine the number of cores available in the computer
    Obtained from an aswer at:
    https://stackoverflow.com/questions/4586405/how-to-get-the-number-of-cpus-in-linux-using-c
*/

#include <stdio.h>
#include <sys/sysinfo.h>

int main(int argc, char *argv[])
{
    printf("This system has %d processors configured and %d processors available.\n",
        get_nprocs_conf(), get_nprocs());
    return 0;
}
