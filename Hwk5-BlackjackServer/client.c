/*
    code from Gilberto Echeverria for Sockets
    Roberto Alejandro Gutierrez Guillen
    A01019608
    Blackjack simple client
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
// Include own sockets library
#include "sockets.h"

#define SERVICE_PORT 9999
#define BUFFER_SIZE 1024

void usage(char * program);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[]){
    int client_fd;

    printf("\n=== CLIENT PROGRAM ===\n");

    if (argc != 3)
        usage(argv[0]);

    client_fd = connectToServer(argv[1], argv[2]);
    communicationLoop(client_fd);

    // Closing the socket
    printf("Closing the connection socket\n");
    close(client_fd);

    return 0;
}

// Show the user how to run this program
void usage(char * program){
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd){
    char buffer[BUFFER_SIZE];
    char * string; // Manage buffer divided by :
    int chars_read;
    int startMoney = 0; // Starting Money

    //Welcome message and starting money
    printf("\t-----Welcome to Guti Casino's Inc!-----\n\n");
    printf("\t\tBlackjack game\n");
    printf("I will be your dealer Mr. Guti itself\n\n");
    printf("Enter the money you have to start: ");
    scanf("%d", &startMoney);

    // Handshake, validate correct client with server
    // Send a request
    sprintf(buffer, "STARTBLACKJACK:%d", startMoney); // Write in buffer
    send(connection_fd, buffer, strlen(buffer)+1, 0);

    // Receive reply
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "CONFIRM", 8) != 0){ // strcmp = string compare
        printf("Invalid server. Exiting!\n");
        return;
    }

    printf("Game READY!\n");

    while(1) { // Game starts
        int currentBet = 0;
        int myHand; // Current amount of clients hand
        int lastCard;

        // Send a request, with the bet
        printf("Place a bet: ");
        scanf("%d", &currentBet);
        sprintf(buffer, "CURRENTBET:%d", currentBet); // put guess in buffer
        send(connection_fd, buffer, strlen(buffer) + 1, 0);

        // Receive answer //TODO ASK GIL about checking info
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if (chars_read <= 0) { // Check if contains info
            printf("SERVER ERROR");
            break;
        }

        if(strncmp(buffer, "NOOK",5) == 0){ // Restart loop, to ask for diff money
            printf("Guti says \"Your bet is higher than what you currently have, please bet less\"\n");
            continue;
        }else if(strncmp(buffer, "NOMON",6) == 0){
            printf("Guti says \"You have no money sorry, not sorry\"\n");
            break;
        }else if(strncmp(buffer, "OK",3) != 0){ // If not ok end
            printf("SERVER ERROR");
            break;
        }

        // Get first cards
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        string = strtok(buffer,":");
        if(strncmp(buffer, "CARDS",6) != 0) {
            printf("SERVER ERROR");
            break;
        }

        string = strtok(NULL, ":"); // Get client hand money
        lastCard = atoi(string);
        myHand = lastCard;
        string = strtok(NULL, ":"); // Get current money
        printf("Your first card is a %d and the dealer got a %s\n", myHand,string);

        // Get second card, only the client
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        string = strtok(buffer,":");
        if(strncmp(buffer, "CARDS2",7) != 0) {
            printf("SERVER ERROR");
            break;
        }

        string = strtok(NULL, ":"); // Get client hand money
        lastCard = atoi(string);
        myHand += lastCard;
        string = strtok(NULL, ":"); // Get current money
        printf("Your second card is a %d for a total of %d\n", lastCard,myHand);


        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if(strncmp(buffer, "CONTINUE",9) != 0){
            printf("Guti says \"You have no money sorry, not sorry\"\n");

        }else if(strncmp(buffer, "LOST",5) != 0){
            printf("House got BLACKJACK! You loose %d\n", currentBet);
        }else if(strncmp(buffer, "DRAW",5) != 0){
            printf("House got BLACKJACK! and you got BLACKJACK! You get your current bet back %d\n", currentBet);
        }else if(strncmp(buffer, "WIN",4) != 0){
            printf("You got BLACKJACK! You win %d\n", (int)( currentBet * 2.5));
        }else{ // WRONG
            printf("SERVER ERROR");
            break;
        }


    } // WHILE ENDS_-------------

    // Receive answer
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    string= strtok(buffer,":");
    if(strncmp(string,"END",4)!=0){
        printf("Invalid client, Exiting!\n");
        return;
    }
    // Get second part and third part with start money and current
    string = strtok(NULL, ":"); // Get start money
    startMoney = atoi(string);
    string = strtok(NULL, ":"); // Get current money
    printf("Guti says \"You started with %d and ended with %s\"\n", startMoney,string);


    // Close the connection
    send(connection_fd, "END", 4, 0);
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);

}
