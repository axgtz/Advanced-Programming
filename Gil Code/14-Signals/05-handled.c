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

#define LIFETIME 5

void waitLoop();
void onInterrupt(int signalNum);

int main(int argc, char * argv[]){
    int seconds;

    struct sigaction new_action;
    struct sigaction old_action;

    // Use a timeout obtained as an argument
    if (argc >= 2)
        seconds = atoi(argv[1]);
        // Otherwise use a default time
    else
        seconds = LIFETIME;

    // Ignore the Ctrl-C interruption
    // signal(SIGINT,SIG_IGN); // handler blocks ctrl c, this is using default handler
    // signal(SIGINT,onInterrupt); // this is using our custom signal handler
    new_action.sa_handler = onInterrupt;
    sigaction(SIGINT, &new_action, &old_action);

    /*     // Only discard once the signal, if u call twice, the second call works
     *     new_action.sa_handler = onInterrupt;
     *     new_action.sa_flags = SA_RESETHAND; // Only discards once
     *     sigaction(SIGINT, &new_action, &old_action);
     */


    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    // Restore Ctrl-C interruption
    sigaction(SIGINT, &old_action, NULL);


    signal(SIGINT,SIG_DFL);

    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    return 0;
}

// Wait a few seconds until the program ends
void waitLoop(int seconds){
    int i;

    // Show the PID so that it can be sent signals form the terminal
    //  or other process
    printf("My PID is: %d\n", getpid());

    printf("Will finish on my own in %3d s", seconds);
    fflush(stdout);
    // Loop to wait for something to happen
    for (i=seconds-1; i>=0; i--){
        sleep(1);
        printf("\b\b\b\b\b%3d s", i);
        fflush(stdout);
    }
    printf("\nTime out. Good bye\n");
}

// Signal handler, not safe

void onInterrupt(int signalNum){
    printf("\nThe signal %d has been recieved \n", signalNum);
    printf("Hasta la vista signal!\n");
}
