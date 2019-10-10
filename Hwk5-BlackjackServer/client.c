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
#include <stdbool.h>
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
    char * stringTempBuff; // Manage buffer divided by :
    int chars_read;
    int startMoney = 0; // Starting Money
    bool cont = true; // used to break from the second loop

    //Welcome message and starting money
    printf("\t-----Welcome to Guti Casino's Inc!-----\n\n");
    printf("\t\tBlackjack game\n");
    printf("I will be your dealer Mr. Guti itself\n\n");
    printf("Enter the money you have to start: ");
    scanf("%d", &startMoney);

    //-1 - Handshake, validate correct client with server
    // Send a request
    sprintf(buffer, "STARTBLACKJACK:%d", startMoney); // Write in buffer
    send(connection_fd, buffer, strlen(buffer)+1, 0);

    // 0 - Receive reply
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    if(strncmp(buffer, "CONFIRM", 8) != 0){ // strcmp = string compare
        printf("Invalid server. Exiting!\n");
        return;
    }

    printf("Game READY!\n");

    while(cont) { // Game starts
        int currentBet = 0;
        int myHand = 0; // Current amount of clients hand
        int lastCardClient = 0;

        // 1- Send a request, with the bet
        printf("\nPlace a bet: ");
        scanf("%d", &currentBet);
        sprintf(buffer, "CURRENTBET:%d", currentBet); // put guess in buffer
        send(connection_fd, buffer, strlen(buffer) + 1, 0);

        //3-  Receive answer if bet is okay
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);

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

        // 4 -Ask if client ready to start
        printf("\nAre you sure?  yes or no\n");
        scanf("%s", buffer);
        if(strncmp(buffer, "yes",4) != 0){
            sprintf(buffer, "NOK"); // put OK in buffer to avoid lock
            send(connection_fd, buffer, strlen(buffer) + 1, 0);
            continue;
        }
        sprintf(buffer, "OK"); // put OK in buffer to avoid lock
        send(connection_fd, buffer, strlen(buffer) + 1, 0);

        // Get first cards
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        stringTempBuff = strtok(buffer,":"); // Get client hand, first card
        lastCardClient = atoi(stringTempBuff);
        myHand = lastCardClient;
        stringTempBuff = strtok(NULL,":"); // Get dealer hand, first card
        printf("Your first card is a %d and the dealer got a %s\n", lastCardClient,stringTempBuff);

        // Get second hand client, total dealer and status
        stringTempBuff = strtok(NULL,":"); // Get current Card client
        lastCardClient = atoi(stringTempBuff);
        myHand+= lastCardClient; // add new card to client total
        printf("Your second card is a %d for a total of %d\n", lastCardClient,myHand);

        stringTempBuff = strtok(NULL, ":"); // Get status
        if(strncmp(stringTempBuff,"WIN", 4) == 0 ){
            printf("Guti says \"You have won congrats!! Got a natural 21! Winner winner, chicken dinner! You get 1.5 times your bet\"\n");
            break;
        }else if(strncmp(stringTempBuff,"LOST", 5) == 0 ){
            printf("Guti says \"You have lost sorry, not sorry. The dealer got 21\"\n");
            break;
        }else if(strncmp(stringTempBuff,"DRAW", 5) == 0 ){
            printf("Guti says \"A draw happened! You both got 21\"\n");
            break;
        }else if(strncmp(stringTempBuff,"CONTINUE", 9) == 0){
            printf("Guti says \" we continue\"\n");
        }else{
            printf("SERVER ERROR");
            break;
        }
        while(1){
            // 5 - Send hit or stand
            printf("Guti says \" do you want to hit or stand?\"\n");
            scanf("%s", buffer);
            if(strncmp(buffer, "hit",4) == 0 || strncmp(buffer, "stand",5) == 0){
                //sprintf(buffer, stringTempBuff); // put OK in buffer to avoid lock
                send(connection_fd, buffer, strlen(buffer) + 1, 0);
            }else{ // Restart loop
                continue;
            }

            // 6 - receive 1 card and current result
            chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
            stringTempBuff = strtok(buffer, ":"); // Get status
            lastCardClient = atoi(buffer);
            myHand += lastCardClient;
            printf("Your next card is a %d for a total of %d\n", lastCardClient,myHand);

            stringTempBuff = strtok(NULL, ":"); // Get status
            printf("---- %s ----\n", stringTempBuff);
            if(strncmp(stringTempBuff,"WIN", 4) == 0 ){
                printf("Guti says \"You have won congrats!! Got a 21! Winner winner, chicken dinner! You get 1.5 times your bet\"\n");
                break;
            }else if(strncmp(stringTempBuff,"LOST", 5) == 0 ){
                printf("Guti says \"You have lost sorry, not sorry. The dealer got 21\"\n");
                break;
            }else if(strncmp(stringTempBuff,"DRAW", 5) == 0 ){
                printf("Guti says \"A draw happened! You both got 21\"\n");
                break;
            }else if(strncmp(stringTempBuff,"CONTINUE", 9) == 0){
                printf("Guti says \" we continue\"\n");
            }else{
                printf("SERVER ERROR");
                break;
            }


            if(0){ // exit
                cont = false;
            }
        }

        break;
    } // WHILE ENDS_-------------
    //TODO check end
    // Receive answer
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    stringTempBuff= strtok(buffer,":");
    if(strncmp(stringTempBuff,"END",4)!=0){
        printf("Invalid client, Exiting!\n");
        return;
    }
    // Get second part and third part with start money and current
    stringTempBuff = strtok(NULL, ":"); // Get start money
    startMoney = atoi(stringTempBuff);
    stringTempBuff = strtok(NULL, ":"); // Get current money
    printf("Guti says \"You started with %d and ended with %s\"\n", startMoney,stringTempBuff);


    // Close the connection
    send(connection_fd, "END", 4, 0);
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);

}
