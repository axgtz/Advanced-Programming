/*
    Program to test the blocking of signals
    It can take a number of seconds to live as an argument

    Gilberto Echeverria
    gilecheverria@yahoo.com
    28/02/2017

    sigset_t is a data type for storing signals
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define LIFETIME 20

// Function declarations
sigset_t setupMask();
void unsetMask(sigset_t old_set);
void detectBlocked(int signal);
void waitLoop();

int main(int argc, char * argv[])
{
    int seconds;
    sigset_t old_set;

    // Use a timeout obtained as an argument
    if (argc >= 2)
        seconds = atoi(argv[1]);
        // Otherwise use a default time
    else
        seconds = LIFETIME;

    // Change the signal handlers
    old_set = setupMask();

    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    detectBlocked(SIGINT);
    detectBlocked(SIGUSR1);

    // Remove the masks
    unsetMask(old_set);

    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    return 0;
}

// Function definitions

// Modify the signal mask. Define to block SIGINT
sigset_t setupMask()
{
    sigset_t new_set;
    sigset_t old_set;
    //Clear the new set. Just to be sure it comes with no garbage
    sigemptyset(&new_set);
    // Add signal SIGINT to the new_set. These signals are the ones to be blocked
    sigaddset(&new_set,SIGINT); // SIGINT is now is new_set = ctrl+c
    sigaddset(&new_set,SIGTSTP); // ctrl+z

    if(sigismember(&new_set,SIGSTOP))
        printf("SIGSTOP is member of new_set\n");

    // Apply the set to the program. The program has a "default" set (think of it as a "third set"), and it is now replaced by the new_set
    sigprocmask(SIG_BLOCK,&new_set,&old_set); //old set is the set of signals that the program started with. It is empty
    // sigblock añade las señales que estan en new_set al programa
    return old_set;

}

// Reset the blocking mask to the previous one
void unsetMask(sigset_t old_set)
{
    sigprocmask(SIG_SETMASK,&old_set,NULL); // SIG_SETMASK = Replace what was before, with old_set. Now the default set is old_set
    // reemplaza el set de señales que se bloquean por old_set
}

void detectBlocked(int signal)
{
    sigset_t temp_set;
    // Get any signals that have been received while blocking
    sigpending(&temp_set);

    if(sigismember(&temp_set,signal))
    {
        printf("Received %d\n",signal);
    }
}

// Wait a few seconds until the program ends
void waitLoop(int seconds)
{
    int i;

    // Show the PID so that it can be sent signals form the terminal
    //  or other process
    printf("My PID is: %d\n", getpid());

    printf("Will finish on my own in %3d s", seconds);
    fflush(stdout);
    // Loop to wait for something to happen
    for (i=seconds-1; i>=0; i--)
    {
        sleep(1);
        printf("\b\b\b\b\b%3d s", i);
        fflush(stdout);
    }
    printf("\nTime out. Good bye\n");
}
