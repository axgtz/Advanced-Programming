/*
    Program for a simple chat client
    The server address and port are provided as arguments to the program

    Gilberto Echeverria
    gilecheverria@yahoo.com
    26/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024

void usage(char * program);
void connectToServer(char * address, char * port);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    printf("\n=== CLIENT PROGRAM ===\n");

    if (argc != 3)
        usage(argv[0]);

    connectToServer(argv[1], argv[2]);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Establish a connection with the server indicated by the parameters
void connectToServer(char * address, char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;
    int client_fd;

    // Prepare the information to determine the local address
    // Clear the structure
    bzero(&hints, sizeof hints);
    // Let the IP address be found automatically
    hints.ai_flags = AI_PASSIVE;
    // Use IPv4 address
    hints.ai_family = AF_INET;
    // Type of socket
    hints.ai_socktype = SOCK_STREAM;

    ///// GETADDRINFO
    // Get the actual address of this computer
    if (getaddrinfo(address, port, &hints, &server_info) == -1)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    client_fd = socket(server_info->ai_family, server_info->ai_socktype,
                        server_info->ai_protocol);
    if (client_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ///// CONNECT
    // Connect to the server
    if ( connect(client_fd, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        perror("accept");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(server_info);

    communicationLoop(client_fd);
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    int chars_read;

    while(1)
    {
        // Send a request
        printf("Message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        // Remove the enter from the message
        buffer[strlen(buffer)-1] = '\0';
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        // Clear the buffer
        bzero(buffer, BUFFER_SIZE);
        // Get an incomming message
        chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
        // Error when receiving data
        if (chars_read == -1)
        {
            perror("recv");
            close(connection_fd);
            return;
        }
        else if (chars_read == 0)
        {
            printf("Connection closed by server\n");
            close(connection_fd);
            return;
        }

        printf("The server sent me: '%s'\n", buffer);
    }
}
