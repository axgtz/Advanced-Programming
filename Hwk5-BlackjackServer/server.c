/*
    code from Gilberto Echeverria for Sockets
    Roberto Alejandro Gutierrez Guillen
    A01019608
    Blackjack simple server
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
#include <time.h> // Rand

#define SERVICE_PORT 9999
#define BUFFER_SIZE 1024

void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);
int generateCard();

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
void communicationLoop(int connection_fd){
    char buffer[BUFFER_SIZE];
    char * string;

    int chars_read;
    int startMoney;
    int currentMoney;
    int currentBet;



    // Handshake
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);

    // Get first part
    string= strtok(buffer,":");
    if(strncmp(string,"STARTBLACKJACK",14)!=0){ // check that its a client to this game
        printf("Invalid client, Exiting!\n");
        return;;
    }
    // Get second part, with the limit number for guess game
    string = strtok(NULL, ":"); // pass null as strtok so it remembers where it left
    startMoney = atoi(string);
    currentMoney = startMoney; // Only modify the current money

    //Send a reply
    sprintf(buffer, "CONFIRM"); // put ready inside the buffer, prep message
    send(connection_fd, buffer, strlen(buffer)+1, 0);

    while(1){ // Game loop
        int currentCardClient = 0;
        int currentCardDealer = 0;
        int dealerHand = 0 ;
        int clientHand = 0;

        // Get client answer, if empty exit
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if (chars_read <= 0)
            break;

        // Get first part- HOW MUCH TO BET
        string = strtok(buffer,":");
        if(strncmp(string,"CURRENTBET",10)!=0){ // check that its a client to this game
            printf("Invalid client, Exiting!\n");
            return;;
        }

        // Get second part, with the current bet
        string = strtok(NULL, ":"); // pass null as strtok remembers where it left
        currentBet = atoi(string);

        // Send answer if bet is okay
        if(currentMoney <= 0){ // no money
            sprintf(buffer, "NOMON");
            send(connection_fd, buffer, strlen(buffer)+1, 0);
            break;
        }else if(currentBet > startMoney){ // Check if bet is not more than current money
            sprintf(buffer, "NOOK");
            send(connection_fd, buffer, strlen(buffer)+1, 0);
            continue;
        }
        sprintf(buffer, "OK");
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        // Initialize the random seed outside so it changes
        srand(time(NULL));

        // Generate first dealer and client card
        currentCardClient  = generateCard();
        currentCardDealer = generateCard();
        clientHand = currentCardClient;
        dealerHand = currentCardDealer;

        sprintf(buffer, "CARDS:%d:%d", clientHand, dealerHand);
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        // Generate dealer second hand which is secret
        currentCardClient  = generateCard();
        currentCardDealer = generateCard();
        clientHand += currentCardClient;
        dealerHand += currentCardDealer;
        sprintf(buffer, "CARDS2:%d:%d:%d", clientHand, dealerHand, currentCardClient);
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        printf("cards 2");
        // Check 21 and send result
        if(dealerHand == 21){
            if(clientHand < 21){// C1: Dealer reached 21 and client no
                currentMoney -= currentBet;
                sprintf(buffer, "LOST");
            }else {// C2: Dealer reached 21 and client reached too
                sprintf(buffer, "DRAW");
            }
            send(connection_fd, buffer, strlen(buffer)+1, 0);
        }else if(clientHand == 21){// C3: Client wins
            sprintf(buffer, "WIN");
            currentMoney+= (int) (currentBet * 2.5);
            send(connection_fd, buffer, strlen(buffer)+1, 0);
        }else{ // C4 : no one reached 21
            sprintf(buffer, "CONTINUE");
            send(connection_fd, buffer, strlen(buffer)+1, 0);
            /*while(dealerHand < 22 || clientHand < 22){

            }*/
        }
             // TODO think about Ace, ace is 1 until the hand is more than 22
    }     // WHILE ENDS_-------------


sprintf(buffer, "END:%d:%d",startMoney, currentMoney);
    send(connection_fd, buffer, strlen(buffer) + 1, 0);

    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "END",4)!=0){ // The client is not using the same protocol
        printf("Invalid client. Exiting!\n");
        return;
    }
    send(connection_fd,"END", 4 ,0);

}

// Program to draw cards
int generateCard(){// TODO ACE
    int card;

    // Generate random number
    card = rand() % 13 + 1;

    if(card > 11){ // Joker, Queen, King become 10
        card = 10;
    }
  //  printf("The card number is: %d\n",card);

    return card;
}
