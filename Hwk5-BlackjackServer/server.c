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
#include <stdbool.h>

#define SERVICE_PORT 9999
#define BUFFER_SIZE 1024

void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);
int generateCard();
void checkStatus(int * clientHandTotal, int * dealerHandTotal, char * result, int *  numAcesClient, int * numAcesDealer);

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
    bool cont = true; // used to break from the second loop

    int chars_read;
    int startMoney;
    int currentMoney;
    int currentBet;

    // -1 - Handshake
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

    // 0 - Send a reply
    sprintf(buffer, "CONFIRM"); // put ready inside the buffer, prep message
    send(connection_fd, buffer, strlen(buffer)+1, 0);

    while(cont){ // Game loop
        int currentCardClient = 0;
        int currentCardDealer = 0;
        int numAcesClient = 0;
        int numAcesDealer = 0;
        int dealerHandTotal = 0 ;
        int clientHandTotal = 0;
        char result[10];

        // 1- Get client answer, if empty exit
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if (chars_read <= 0)
            break;

        // Get first part- HOW MUCH TO BET
        string = strtok(buffer,":");
        if(strncmp(string,"CURRENTBET",10)!=0){ // check that its a client to this game
            printf("Invalid client, Exiting!\n");
            return;
        }

        // Get second part, with the current bet
        string = strtok(NULL, ":"); // pass null as strtok remembers where it left
        currentBet = atoi(string);

        // 3- Send answer if bet is okay
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

        // 4 - Get client answer of OK
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if(strncmp(buffer,"OK",3)!=0){ // check that its a client to this game
            continue;
        }

        // Initialize the random seed outside so it changes
        srand(time(NULL));

        // Generate first dealer and client card
        currentCardClient  = generateCard();
        if(currentCardClient == 11){numAcesClient++;}
        currentCardDealer = generateCard();
        if(currentCardDealer == 11){numAcesDealer++;}
        clientHandTotal = currentCardClient;
        dealerHandTotal = currentCardDealer;

        // Generate second
        currentCardClient  = generateCard();
        if(currentCardClient == 11){numAcesClient++;}
        currentCardDealer = generateCard();         // Generate dealer second hand which is secret
        if(currentCardDealer == 11){numAcesDealer++;}
        clientHandTotal += currentCardClient;
        dealerHandTotal += currentCardDealer;

        // Get status
        checkStatus(&clientHandTotal, &dealerHandTotal, result, &numAcesClient, &numAcesDealer);

        sprintf(buffer, "%d:%d:%d:%s" ,currentCardClient, (dealerHandTotal - currentCardDealer),currentCardClient , result);
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        if(strncmp(result,"WIN", 4) == 0 ){
            currentMoney+= (int) (currentBet * 2.5);
            break;
        }else if(strncmp(result,"LOST",5) == 0 ){
            currentMoney -= currentBet;
            break;
        }else if(strncmp(result,"DRAW",5) == 0 ){
            break;
        } // Else would be continue

        while (1){
            // 5 - Hit or stand, if stand dealer gets cards out until win or over 21
            chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);

            if(strncmp(buffer,"hit",4)==0){ // 6 - return 1 card
                // Generate card and add it to current hand
                currentCardClient  = generateCard();
                if(currentCardClient == 11){numAcesClient++;}
                clientHandTotal += currentCardClient;

                // 6 - return 1 card and current result

                checkStatus(&clientHandTotal,&dealerHandTotal, result, &numAcesClient, &numAcesDealer);
                printf(" SERVER HIT %s\n", result);
                if(strncmp(result, "WIN",4) == 0){ // Dealer should now get cards
                    if(dealerHandTotal < 18){ // If dealer hand less than 18, he can ask for more cards
                        while(dealerHandTotal < 18){
                            currentCardDealer = generateCard();         // Generate dealer second hand which is secret
                            if(currentCardDealer == 11){numAcesDealer++;}
                            dealerHandTotal += currentCardDealer;
                            if(dealerHandTotal > 21){
                                if(numAcesDealer>0){
                                    numAcesDealer--;
                                    dealerHandTotal -= 10;
                                }else{
                                    break;
                                }
                            }
                        }
                        checkStatus(&clientHandTotal, &dealerHandTotal, result, &numAcesClient, &numAcesDealer);
                        sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                        send(connection_fd, buffer, strlen(buffer)+1, 0);
                        break;
                    }else{ // dealer lost, as he cannot bet when more than 17, keep result
                        if(clientHandTotal == 21){
                            currentMoney+= (int) (currentBet * 2.5);
                        }else{
                            currentMoney+= (int) (currentBet * 1.5);
                        }
                    }
                } else if(strncmp(result, "LOST",5) == 0){ // Client over 21, he lost, so dealer can avoid everything
                    currentMoney -= currentBet;
                    sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                    send(connection_fd, buffer, strlen(buffer)+1, 0);
                    break;
                } // Continue
                if(strncmp(result,"WIN",4) == 0 ){
                    if(clientHandTotal == 21){
                        currentMoney+= (int) (currentBet * 2.5);
                    }else{
                        currentMoney+= (int) (currentBet * 1.5);
                    }
                    break;
                }else if(strncmp(result,"LOST",5) == 0 ){
                    currentMoney -= currentBet;
                    break;
                }else if(strncmp(result,"DRAW",5) == 0 ){
                    break;
                } // Else would be continue, so asks for new card

                sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                send(connection_fd, buffer, strlen(buffer)+1, 0);

            }else if(strncmp(buffer,"stand",6)==0){ // 6.5 - dealer needs to go over 17 or go bust
                checkStatus(&clientHandTotal, &dealerHandTotal, result, &numAcesClient, &numAcesDealer);
                if(clientHandTotal > dealerHandTotal && dealerHandTotal > 17){
                    strncpy(result, "WIN", 4);
                    if(clientHandTotal == 21){
                        currentMoney+= (int) (currentBet * 2.5);
                    }else{
                        currentMoney+= (int) (currentBet * 1.5);
                    }
                    sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                    send(connection_fd, buffer, strlen(buffer)+1, 0);

                    break;
                }else{
                    if(strncmp(result,"LOST", 5) != 0 ){
                        if(dealerHandTotal < 18){ // If dealer hand less than 18, he can ask for more cards
                            while(dealerHandTotal < 18){
                                currentCardDealer = generateCard();         // Generate dealer second hand which is secret
                                if(currentCardDealer == 11){numAcesDealer++;}
                                dealerHandTotal += currentCardDealer;
                                if(dealerHandTotal > 21){
                                    if(numAcesDealer > 0){
                                        numAcesDealer--;
                                        dealerHandTotal -= 10;
                                    }else{
                                        break;
                                    }
                                }
                            }
                            if(clientHandTotal > dealerHandTotal){
                                strncpy(result, "WIN", 4);
                                currentMoney+= (int) (currentBet * 1.5);
                            }else{
                                strncpy(result, "LOST", 5);
                                currentMoney -= currentBet;
                            }
                            sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                            send(connection_fd, buffer, strlen(buffer)+1, 0);
                            break;
                        }else{ // dealer lost, as he cannot bet when more than 17, keep result
                            strncpy(result, "WIN", 4);
                            if(clientHandTotal == 21){
                                currentMoney+= (int) (currentBet * 2.5);
                            }else{
                                currentMoney+= (int) (currentBet * 1.5);
                            }
                        }
                        checkStatus(&clientHandTotal, &dealerHandTotal, result, &numAcesClient, &numAcesDealer);
                        sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                        send(connection_fd, buffer, strlen(buffer)+1, 0);
                        break;
                    }else if(strncmp(result, "LOST",5) == 0){ // Client over 21, he lost, so dealer can avoid everything
                        currentMoney -= currentBet;
                        sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                        send(connection_fd, buffer, strlen(buffer)+1, 0);
                        break;
                    }
                    if(dealerHandTotal < 18){ // If dealer hand less than 18, he can ask for more cards
                        while(dealerHandTotal < 18){
                            currentCardDealer = generateCard();         // Generate dealer second hand which is secret
                            if(currentCardDealer == 11){numAcesDealer++;}
                            dealerHandTotal += currentCardDealer;
                            if(dealerHandTotal > 21){
                                if(numAcesDealer>0){
                                    numAcesDealer--;
                                    dealerHandTotal -= 10;
                                }else{
                                    break;
                                }
                            }
                        }
                        checkStatus(&clientHandTotal, &dealerHandTotal, result, &numAcesClient, &numAcesDealer);

                        if(clientHandTotal > dealerHandTotal){
                            strncpy(result, "WIN", 4);
                            currentMoney+= (int) (currentBet * 1.5);
                        }else{
                            strncpy(result, "LOST", 5);
                            currentMoney -= currentBet;
                        }
                        sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                        send(connection_fd, buffer, strlen(buffer)+1, 0);
                        break;
                    }
                    checkStatus(&clientHandTotal, &dealerHandTotal, result, &numAcesClient, &numAcesDealer);

                    if(clientHandTotal > dealerHandTotal){
                        strncpy(result, "WIN", 4);
                        currentMoney+= (int) (currentBet * 1.5);
                    }else{
                        strncpy(result, "LOST", 5);
                        currentMoney -= currentBet;
                    }
                    sprintf(buffer, "%d:%d:%s" ,currentCardClient, clientHandTotal, result);
                    send(connection_fd, buffer, strlen(buffer)+1, 0);
                    break;
                }
            }else{ // Wrong , exit from 2 loops
                cont = false;
                break;
            }
        }
        currentBet = 0;

        sprintf(buffer, "END:%d:%d",startMoney, currentMoney);
        send(connection_fd, buffer, strlen(buffer) + 1, 0);
    }     // WHILE ENDS-------------

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
int generateCard(){
    int card;

    // Generate random number
    card = rand() % 13 + 1;

    if(card > 10){ // Joker, Queen, King become 10
        card = 10;
    }else if(card == 1){ // Ace will be always send as 11 then the server program can decide if it needs to be converted to 1
        card = 11;
    }
    //printf("The card number is: %d\n",card);

    return card;
}

// Program to see if someone won
void checkStatus(int * clientHandTotal, int * dealerHandTotal, char * result, int *  numAcesClient, int * numAcesDealer){
    // Check 21 and send result
    if(*dealerHandTotal == 21) {
        if (*clientHandTotal == 21) {    // C1: Draw
            strncpy(result, "DRAW", 5);
        } else {                        // C2: Dealer Wins
            strncpy(result, "LOST", 5);
        }
    }else if(*clientHandTotal == 21) {   // C3: Client Wins
        strncpy(result, "WIN", 4);
    }else if(*clientHandTotal > 21){// C4 : Client over 21 lost
        if(*numAcesClient > 0){ // has aces
            (*numAcesClient)--;
            (*clientHandTotal) -= 10;
            checkStatus(clientHandTotal, dealerHandTotal, result,numAcesClient,numAcesDealer);
        }else{ // no more aces
            strncpy(result, "LOST", 5);
        }
    }else if(*dealerHandTotal > 21){// C5 : Dealer over 21
        if(*numAcesDealer > 0){ // Saved by the ace
            (*numAcesDealer)--;
            (*dealerHandTotal) -= 10;
            checkStatus(clientHandTotal, dealerHandTotal, result,numAcesClient,numAcesDealer);
        }else{
            strncpy(result, "WIN", 5);
        }
    } else{// C6 : no one reached 21
        strncpy(result, "CONTINUE", 9);
    }
}
