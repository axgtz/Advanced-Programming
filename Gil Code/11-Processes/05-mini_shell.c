/*
    Simplified shell program to run commands as children

    Gilberto Echeverria
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    //char * command = "ls";
    //char * arguments[] = {"ls", "-a", (char *) NULL};
    char * command = "./fact";
    char * arguments[] = {"./fact", "6", (char *) NULL};
    pid_t new_pid;
    int status;

    new_pid = fork();
    if (new_pid > 0)        // PARENT
    {
        printf("The parent (%d) is waiting for the child (%d)\n", getpid(), new_pid);
        wait(&status);
        printf("Now the parent can continue\n");
        if (WIFEXITED(status))
        {
            printf("Child finished with status: %d\n", WEXITSTATUS(status));
        }
    }
    else if (new_pid == 0)  // CHILD
    {
        printf("Switching to command '%s'\n", command);
        status = execvp(command, arguments);
        if (status < 0)
        {
            printf("ERROR on exec\n");
        }
    }
    else
    {
        printf("ERROR on fork");
    }

    return 0;
}
