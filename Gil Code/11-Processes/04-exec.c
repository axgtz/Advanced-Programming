#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char * command = "/home/gilberto/ITESM/Github/TC2025/11-Processes/test";
    int status;

    printf("BEFORE EXEC, pid: %d\n", getpid());
    status = execl(command, "test", (char *) NULL); 
    if (status == -1)
    {
        printf("Error on exec\n");
    }
    printf("AFTER EXEC\n");
    return 0;
}
