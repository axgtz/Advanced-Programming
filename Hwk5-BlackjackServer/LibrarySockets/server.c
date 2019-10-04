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

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
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
void usage(char * program)
{
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    pid_t new_pid;

    while(1)
    {
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        // Get the file descriptor to talk with the client
        connection_fd = accept(server_fd, (struct sockaddr *) &client_address,
                               &client_address_size);
        if (connection_fd == -1)
        {
            perror("accept");
            close(connection_fd);
        }

        // Fork a child process to deal with the new client
        new_pid = fork();
        if (new_pid > 0)    // Parent
        {
            close(connection_fd);
        }
        else if (new_pid == 0)      // Child
        {
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
    int message_counter = 0;
    int chars_read;

    while(1)
    {
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if (chars_read <= 0)
            break;
        printf("The client sent me: '%s'\n", buffer);

        // Send a reply
        sprintf(buffer, "Reply to message %d", ++message_counter);
        send(connection_fd, buffer, strlen(buffer)+1, 0);
    }
}