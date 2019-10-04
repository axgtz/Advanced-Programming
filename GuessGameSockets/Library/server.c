/*
    Program for a simple chat server
    Can only connect with one client at a time

    Gilberto Echeverria
    gilecheverria@yahoo.com
    23/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>
// Include own socket library
#include "sockets.h"

#include <time.h>

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024

void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[]){
    int server_fd;

    printf("\n=== SERVER PROGRAM ===\n");

    if (argc != 2)
        usage(argv[0]);

    server_fd = startServer(argv[1]);
    waitForConnections(server_fd);

    printf("Closing the server socket\n");
    // Close the socket
    close(server_fd);

    return 0;
}

// Show the user how to run this program
void usage(char * program){
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Stand by for connections by the clients
void waitForConnections(int server_fd){
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    pid_t new_pid;

    while(1){
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        // Get the file descriptor to talk with the client
        connection_fd = accept(server_fd, (struct sockaddr *) &client_address,
                               &client_address_size);
        if (connection_fd == -1){
            perror("accept");
            close(connection_fd);
        }

        // Fork a child process to deal with the new client
        new_pid = fork();
        if (new_pid > 0)    // Parent
        {
            close(connection_fd);
        }else if (new_pid == 0) {     // Child

            close(server_fd);

            // Identify the client
            // Get the ip address from the structure filled by accept
            inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, INET_ADDRSTRLEN);
            printf("Received a connection from %s : %d\n", client_presentation, client_address.sin_port);

            // Establish the communication
            communicationLoop(connection_fd);
            close(connection_fd);
            // Finish the child process
            exit(EXIT_SUCCESS);
        }
    }
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    char * string;

    int chars_read;
    int limit;
    int target;
    int guess;

    // Initialize the random seed
    srand(time(NULL));

    // Handshake
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);

    // Get first part
    string= strtok(buffer,":");
    if(strncmp(string,"START",6)!=0){ // check that its a client to this game
        printf("Invalid client, Exiting!\n");
        return;;
    }
    // Get second part, with the limit number for guess game
    string = strtok(NULL, ":"); // pass null as strtok remembers where it left
    limit = atoi(string);

    // Generate random number
    target = rand() % limit + 1;
    printf("The target number is: %d\n",target);// DEBUG-----------------

    //Send a reply
    sprintf(buffer, "READY"); // put ready inside the buffer, prep message
    send(connection_fd, buffer, strlen(buffer)+1, 0);

    while(1){
        // Get client answert
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if (chars_read <= 0)
            break;

        // Get first part
        string= strtok(buffer,":");
        if(strncmp(string,"START",6)!=0){ // check that its a client to this game
            printf("Invalid client, Exiting!\n");
            return;;
        }
        // Get second part, with the limit number for guess game
        string = strtok(NULL, ":"); // pass null as strtok remembers where it left
        guess = atoi(string);


        if(guess > target ){   // Client needs higher number
            sprintf(buffer, "lower\n");
            send(connection_fd, buffer, strlen(buffer)+1, 0);   // the 0 means, option flag
            break;
        }else if(guess < target){ // Client needs lower num
            sprintf(buffer, "higher\n");
        }else if(gues == target){ // Client is right
            sprintf(buffer, "You are correct!!\n");
        }else{
            sprintf(buffer, "correct\n");
        }

        //Send reply
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        // Send a reply example
        /*
        sprintf(buffer, "Reply to message %d", ++message_counter);
        send(connection_fd, buffer, strlen(buffer)+1, 0);
         */
    }
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "BYE",4)!=0){ // The client is not using the same protocol
        printf("Invalid client. Exiting!\n");
        return;
    }
    send(connection_fd,"BYE", 4 ,0);
}
