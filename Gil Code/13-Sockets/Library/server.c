/*
    Program for a simple chat server
    Can only connect with one client at a time
//./server 8989
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

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

int startServer(char * port);
int recieveMessage();

// Initialize the server to be ready for connections
int startServer(char * port){
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;
    int server_fd;
    
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
    if (getaddrinfo(NULL, port, &hints, &server_info) == -1){
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    server_fd = socket(server_info->ai_family, server_info->ai_socktype,
                        server_info->ai_protocol);
    if (server_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Allow using the same port immediately, even if the previous server did not close correctly
    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    ///// BIND
    // Connect with the desired port
    if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);
    
    ///// LISTEN
    // Prepare for connections
    if (listen(server_fd, MAX_QUEUE) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server ready and waiting!\n");
    // Stand by to receive connections from the clients
    return server_fd;
}

// Establish a connection with the server indicated by the parameters
int connectToServer(char * address, char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;
    int connection_fd;
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

    return client_fd;
}