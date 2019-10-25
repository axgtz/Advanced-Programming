/*
        Server program to compute the value of PI
        This program calls the library function 'get_pi'
        It will create child processes to attend requests
        It receives connections using sockets

        Gilberto Echeverria
        gilecheverria@yahoo.com
        21/10/2018
    Roberto Alejandro Gutierrez Guillen
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// Custom libraries
#include "get_pi.h"
#include "sockets.h"
#include "fatal_error.h"

//Signal library
#include <signal.h>
//poll library
#include <poll.h>

#define BUFFER_SIZE 1024
#define MAX_QUEUE 5
#define TIMEOUT 5

///// Global Variables
int interrupted = 0;

///// FUNCTION DECLARATIONS
void usage(char * program);
void waitForConnections(int server_fd);
void attendRequest(int client_fd);
void detectInterruption(int signal);
void setUpHandlers();
void setupMask();

///// MAIN FUNCTION
int main(int argc, char * argv[])
{
    int server_fd;
    setUpHandlers();
    setupMask();

    printf("\n=== SERVER FOR COMPUTING THE VALUE OF pi ===\n");

    // Check the correct arguments
    if (argc != 2)
    {
        usage(argv[0]);
    }

	// Show the IPs assigned to this computer
	printLocalIPs();

    // Start the server
    server_fd = initServer(argv[1], MAX_QUEUE);
	// Listen for connections from the clients
    waitForConnections(server_fd);
    // Close the socket
    close(server_fd);

    return 0;
}
///// FUNCTION DEFINITIONS
/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd){
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pid_t new_pid;

    // Poll variables
    struct pollfd serverPoll;
    int pollRes;

    // Prepare Poll
    serverPoll.fd = server_fd;
    serverPoll.events = POLL_IN; // Wait for data to read

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (!interrupted){

        while(1){ // While to keep polling
            pollRes = poll(&serverPoll,1, TIMEOUT*100);
            if(pollRes == 0){ // timeout ended
                printf(".");
                fflush(stdout);
                continue;
            }else if(pollRes > 0){  // check if
                if(serverPoll.revents & POLLIN){ // check bitmask returned events if it was CTRL - C
                    // ACCEPT
                    // Wait for a client connection
                    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
                    if (client_fd == -1){
                        fatalError("accept");
                    }
                    break;
                }
                //TODO ??
            }else{ // if less than 0, poll error
                perror("Poll failed");
                break;
            }
        }

        if(!interrupted){
            // FORK
            // Create a new child process to deal with the client
            new_pid = fork();
            // Parent
            if (new_pid > 0){
                // Close the new socket
                close(client_fd);
            }else if (new_pid == 0){
                // Close the main server socket to avoid interfering with the parent
                close(server_fd);

                // Get the data from the client
                inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
                printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

                printf("Child process %d dealing with client\n", getpid());
                //--------- Deal with the client----------
                attendRequest(client_fd);
                // Close the new socket
                close(client_fd);
                // Terminate the child process
                exit(EXIT_SUCCESS);
            }else{
                fatalError("fork");
            }
        }

    }
}

/*Hear the request from the client and send an answer*/
void attendRequest(int client_fd){
    char buffer[BUFFER_SIZE];

    unsigned long int iterations;
    unsigned long int counter = 0;

    // Calculate pi
    double result = 4;
    int sign = -1;
    unsigned long int divisor = 3;

    // Poll variables
    struct pollfd serverPoll;
    int pollRes;

    // Prepare Poll
    serverPoll.fd = client_fd;
    serverPoll.events = POLLIN; // Wait for data to read

    // RECV
    // Receive the request
    recvString(client_fd, buffer, BUFFER_SIZE);
    sscanf(buffer, "%lu", &iterations);

    printf(" > Got request from client with iterations=%lu\n", iterations);

    while(!interrupted || counter < iterations){
        pollRes = poll(&serverPoll,1, TIMEOUT);

        if(pollRes == 0){ // timeout ended

            // Compute the value of PI
            result += sign * (4.0/divisor);
            sign *= -1;
            divisor += 2;
            counter++;

            continue;
        }else if(pollRes > 0){  // check if
            if(serverPoll.revents & POLLIN){ // check bitmask returned events if it was CTRL - C
                // TODO recieve CTRC -C of client result
                recvString(client_fd, buffer, BUFFER_SIZE);

                break;
            }
        }else{ // if less than 0, poll error
            perror("You killed him");
            break;
        }


    }


    printf(" > Sending PI=%.20lf\n Iterations %lu\n", result,counter);

    // Prepare the response to the client
    sprintf(buffer, "%.20lf\n%lu", result, counter); // put result and iteration
    // SEND
    // Send the response
    sendString(client_fd, buffer);
}

void setUpHandlers(){
    struct sigaction new_action;

    // Prepare structure, block all signals
    new_action.sa_handler = detectInterruption;
    // Clear Flags
    new_action.sa_flags = 0;
    sigfillset(&new_action.sa_mask);

    // Catch signal CTRL-C
    sigaction(SIGINT, &new_action, NULL); // Do i put old action?
}

// Signal handler
void detectInterruption(int signal){
    printf("Detected CTRL-C\n");

    interrupted = 1; // Change global variable, only way to do it =(
}

// Modify the signal mask. Define to block SIGINT
void setupMask(){
    sigset_t mask;

    // Add all signals to block
    sigfillset(&mask);
    sigdelset(&mask, SIGINT); // Dont block SIGINT

    // Apply the set to the program. The program has a "default" set (think of it as a "third set"), and it is now replaced by the new_set
    sigprocmask(SIG_BLOCK,&mask,NULL);
}
