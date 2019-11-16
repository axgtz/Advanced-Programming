/*
        Program for a simple bank server
        It uses sockets and threads
        The server will process simple transactions on a limited number of accounts

        Gilberto Echeverria
        gilecheverria@yahoo.com
        29/03/2018
        08/04/2018  Added initialization of account balances
        27/10/2018  Simplify the function 'processOperation' by validating the account before the switch
        14/03/2019  Added the new operation for transfer from one account to another

    Roberto Alejandro Gutierrez Guillen
    A01019608
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Signals library
#include <errno.h>
#include <signal.h>
// Sockets libraries
#include <netdb.h>
#include <sys/poll.h>
// Posix threads library
#include <pthread.h>

// Custom libraries
#include "bank_codes.h"
#include "sockets.h"
#include "fatal_error.h"

#define MAX_ACCOUNTS 5
#define BUFFER_SIZE 1024
#define MAX_QUEUE 5
#define TIMEOUT 10

///// Structure definitions

// Data for a single bank account
typedef struct account_struct {
    int id;
    int pin;
    float balance;
} account_t;

// Data for the bank operations
typedef struct bank_struct {
    // Store the total number of operations performed
    int total_transactions;
    // An array of the accounts
    account_t * account_array;
} bank_t;

// Structure for the mutexes to keep the data consistent
typedef struct locks_struct {
    // Mutex for the number of transactions variable
    pthread_mutex_t transactions_mutex;
    // Mutex array for the operations on the accounts
    pthread_mutex_t * account_mutex;
} locks_t;

// Data that will be sent to each structure
typedef struct data_struct {
    // The file descriptor for the socket
    int connection_fd;
    // A pointer to a bank data structure
    bank_t * bank_data;
    // A pointer to a locks structure
    locks_t * data_locks;
} thread_data_t;

// Global variables for signal handlers
int interrupted = 0;


///// FUNCTION DECLARATIONS
void usage(char * program);
void setupHandlers();
void initBank(bank_t * bank_data, locks_t * data_locks);
void readBankFile(bank_t * bank_data);
void waitForConnections(int server_fd, bank_t * bank_data, locks_t * data_locks);
void * attentionThread(void * arg);
void closeBank(bank_t * bank_data, locks_t * data_locks);
int checkValidAccount(int account);
/*
    TODO: Add your function declarations here
*/
void detectInterruption(int signal);
void setupMask();
void writeBankFile(bank_t * bank_data);
void updateNumTrans(thread_data_t* data);

float check(thread_data_t* data, int account_from);
void deposit(thread_data_t* data, int account_to, float amount);
int withdraw(thread_data_t* data, int account_from, float amount);
int transfer(thread_data_t* data, int account_from, int account_to, float amount);

///// MAIN FUNCTION
int main(int argc, char * argv[]){
    int server_fd;
    bank_t bank_data;
    locks_t data_locks;

    printf("\n=== SIMPLE BANK SERVER ===\n");

    // Check the correct arguments
    if (argc != 2){
        usage(argv[0]);
    }

    // Configure the handler to catch SIGINT
    setupHandlers();
    setupMask();

    // Initialize the data structures
    initBank(&bank_data, &data_locks);

	// Show the IPs assigned to this computer
	printLocalIPs();
    // Start the server
    server_fd = initServer(argv[1], MAX_QUEUE);
	// Listen for connections from the clients
    waitForConnections(server_fd, &bank_data, &data_locks);
    // Close the socket
    close(server_fd);

    // Clean the memory used
    closeBank(&bank_data, &data_locks);

    // Finish the main thread
    pthread_exit(NULL);

    return 0;
}

///// FUNCTION DEFINITIONS
/*Explanation to the user of the parameters required to run the program*/
void usage(char * program){
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*Modify the signal handlers for specific events*/
void setupHandlers(){
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


/*  Function to initialize all the information necessary
    This will allocate memory for the accounts, and for the mutexes */

void initBank(bank_t * bank_data, locks_t * data_locks){
    // Set the number of transactions
    bank_data->total_transactions = 0;

    // Allocate the arrays in the structures
    bank_data->account_array = malloc(MAX_ACCOUNTS * sizeof (account_t));
    // Allocate the arrays for the mutexes
    data_locks->account_mutex = malloc(MAX_ACCOUNTS * sizeof (pthread_mutex_t));

    // Initialize the mutexes, using a different method for dynamically created ones
    //data_locks->transactions_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&data_locks->transactions_mutex, NULL);
    for (int i=0; i<MAX_ACCOUNTS; i++){
        //data_locks->account_mutex[i] = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&data_locks->account_mutex[i], NULL);
        // Initialize the account balances too
        bank_data->account_array[i].balance = 0.0;
    }

    // Read the data from the file
    readBankFile(bank_data);
}


/*Get the data from the file to initialize the accounts*/
void readBankFile(bank_t * bank_data){
    FILE * file_ptr = NULL;
    char buffer[BUFFER_SIZE];
    int account = 0;
    char * filename = "accounts.txt";

    file_ptr = fopen(filename, "r");
    if (!file_ptr){
        fatalError("ERROR: fopen");
    }

    // Ignore the first line with the headers
    fgets(buffer, BUFFER_SIZE, file_ptr);
    // Read the rest of the account data
    while( fgets(buffer, BUFFER_SIZE, file_ptr) ){
        sscanf(buffer, "%d %d %f", &bank_data->account_array[account].id, &bank_data->account_array[account].pin, &bank_data->account_array[account].balance); 
        account++;
    }
    // add unkown accounts
    while(account < MAX_ACCOUNTS){
        bank_data->account_array[account].id = account;
        bank_data->account_array[account].pin = 0000;
        bank_data->account_array[account].balance = 0.00;
        account++;
    }
    
    fclose(file_ptr);
}

// At the end write the result
void writeBankFile(bank_t * bank_data) {
    FILE * file_ptr = NULL;
    char * filename = "accounts.txt";

    file_ptr = fopen(filename, "w");
    if (!file_ptr){
        fatalError("ERROR: fopen");
    }

    // Print header
    fprintf(file_ptr, "Account_number PIN Balance\n");


    // write the rest of the account data
    for (int account = 0; account < MAX_ACCOUNTS  ; ++account) {
        fprintf(file_ptr,"%d %d %.2f\n", bank_data->account_array[account].id, bank_data->account_array[account].pin, bank_data->account_array[account].balance);
    }

    fclose(file_ptr);
}


/*Main loop to wait for incomming connections*/
void waitForConnections(int server_fd, bank_t * bank_data, locks_t * data_locks){ //TODO
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    int poll_response;

    // Create a structure array to hold the file descriptors to poll
    struct pollfd test_fds[1];
    // Fill in the structure
    test_fds[0].fd = server_fd;
    test_fds[0].events = POLLIN;    // Check for incomming data

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (!interrupted){
        while(!interrupted){ // While to keep polling
            poll_response = poll(test_fds,1, TIMEOUT*100);
            if(poll_response == 0){ // timeout ended
                printf(".");
                fflush(stdout); // mandatory print
            }else if(poll_response > 0){  // check if
                if(test_fds[0].revents & POLLIN){ // check bitmask returned events if it was CTRL - C
                    // ACCEPT
                    // Wait for a client connection
                    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
                    if (client_fd == -1){
                        fatalError("ERROR: accept");
                    }
                    break;
                }
            }else{ // if less than 0, poll error
                perror("Poll failed server");
                break;
            }
        }
        if(!interrupted){
            // Get the data from the client
            inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
            printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

            // Prepare the structure to send to the thread, thread t
            connection_data = malloc(sizeof(thread_data_t));
            connection_data->bank_data = bank_data;
            connection_data->data_locks = data_locks;
            connection_data->connection_fd = client_fd;

            // CREATE A THREAD
            if (pthread_create(&new_tid, NULL, attentionThread, connection_data) == 0)
                printf("Thread created\n");
        }
    }

    // Show the number of total transactions
    printf("Transactions completed: %d\n", connection_data->bank_data->total_transactions); //
    // Store any changes in the file
    writeBankFile(bank_data);
}

/*Hear the request from the client and send an answer, every thread executes this*/
void * attentionThread(void * arg){
    // Receive the data for the bank, mutexes and socket file descriptor
    thread_data_t* data = arg;

    // Poll stuff
    int poll_response;

    // Create a structure array to hold the file descriptors to poll
    struct pollfd test_fds[1];
    // Fill in the structure
    test_fds[0].fd = data->connection_fd;
    test_fds[0].events = POLLIN;    // Check for incomming data

    // Variables for message
    int operation = -1;
    int account_from = -1;
    int account_to = -1;
    float amount = -1;
    int in = 0;
    int status = -1;
    char buffer[BUFFER_SIZE];

    // check if account exists, then do operations
    while (!interrupted) {
        // Loop to listen for messages from the client
        while(!interrupted){ // While to keep polling /
            poll_response = poll(test_fds,1, TIMEOUT*100);
            if(poll_response == 0){ // timeout ended
                fflush(stdout); // mandatory print
            }else if(poll_response > 0){  // check if
                if(test_fds[0].revents & POLLIN){ // check bitmask returned events if it was CTRL - C
                    in = 1;
                    break;
                }
            }else{ // if less than 0, poll error
                perror("Poll failed server, attention thread");
                break;
            }
        }

        if(!interrupted && in) { // TODO check in
            // Receive the request
            if (recvString(data->connection_fd, buffer, BUFFER_SIZE) == 0){
                printf("Client disconnected\n"); // TODO check which client?
                break;
            }
            //printf("String from client: %s\n", buffer);
            sscanf(buffer, "%d %d %d %f", &operation, &account_from, &account_to, &amount);
            // Process the request being careful of data consistency

            //  Massive Switch
            switch (operation) {
                case CHECK:
                    if(checkValidAccount(account_from)){ // Account exists
                        amount = check(data, account_from);
                        if(amount <= -1){ // error
                            sprintf(buffer, "%d 0",  NO_ACCOUNT);//finish the connection
                            sendString(data->connection_fd, buffer ,BUFFER_SIZE + 1);
                        }else{
                            sprintf(buffer, "%d %f", OK, amount);
                            sendString(data->connection_fd, buffer, BUFFER_SIZE + 2);
                        }
                    }else{ // Account not valid
                        sprintf(buffer, "%d 0",  NO_ACCOUNT);//finish the connection
                        sendString(data->connection_fd, buffer ,BUFFER_SIZE + 1);
                    }
                    break;
                case DEPOSIT:
                    if(checkValidAccount(account_to)){
                        deposit(data,account_to, amount);
                        amount = check(data, account_to);

                        sprintf(buffer, "%d %f", OK, amount);
                        sendString(data->connection_fd, buffer, BUFFER_SIZE + 1);
                    }else{
                        sprintf(buffer, "%d 0",  NO_ACCOUNT);//finish the connection
                        sendString(data->connection_fd, buffer ,BUFFER_SIZE + 1);
                    }
                    break;
                case WITHDRAW:
                    if(checkValidAccount(account_from)){ // check balance
                        status = withdraw(data,  account_from, amount);
                        if(1 == status){ // successful
                            amount = check(data, account_from);
                            sprintf(buffer, "%d %f", OK, amount);
                            sendString(data->connection_fd, buffer, BUFFER_SIZE +  1);
                        }else if(status == 0){
                            sprintf(buffer, "%d 0", INSUFFICIENT);
                            sendString(data->connection_fd, buffer, BUFFER_SIZE + 1);
                        }else{// status = -1
                            sprintf(buffer, "%d 0", ERROR);
                            sendString(data->connection_fd, buffer, BUFFER_SIZE +1);
                        }
                    }else{
                        sprintf(buffer, "%d 0",  NO_ACCOUNT);//finish the connection
                        sendString(data->connection_fd, buffer ,BUFFER_SIZE + 1);
                    }
                    break;
                case TRANSFER:
                    if(checkValidAccount(account_to) && checkValidAccount(account_from)){ // check balance
                        status = transfer(data,  account_from, account_to,amount);
                        if(1 == status){ // successful
                            amount = check(data, account_from);
                            sprintf(buffer, "%d %f", OK, amount);
                            sendString(data->connection_fd, buffer, BUFFER_SIZE + 1);
                        }else if(status == 0){
                            sprintf(buffer, "%d 0", INSUFFICIENT);
                            sendString(data->connection_fd, buffer, BUFFER_SIZE +1 );
                        }else{// status = -1
                            sprintf(buffer, "%d 0", ERROR);
                            sendString(data->connection_fd, buffer, BUFFER_SIZE +1 );
                        }

                    }else{ // one of the accounts doesnt exist
                        sprintf(buffer, "%d 0",  NO_ACCOUNT);//finish the connection
                        sendString(data->connection_fd, buffer ,BUFFER_SIZE + 1);
                    }
                    break;
                case EXIT: default:
                    printf("Client says goodbye, adios, ciao\n");
                    break;

            }// -- end switch --
        }
    }

    // TODO check who exited

    printf("Server Shutting down... Adios Ciao\n");
    sprintf(buffer, "%i 0",  BYE);//finish the connection
    sendString(data->connection_fd,buffer, BUFFER_SIZE + 1);

    free(data);
    pthread_exit(NULL);
}

/*Free all the memory used for the bank data*/
void closeBank(bank_t * bank_data, locks_t * data_locks){
    printf("DEBUG: Clearing the memory for the thread\n");
    free(bank_data->account_array);
    free(data_locks->account_mutex);
}


/*Return true if the account provided is within the valid range,
    return false otherwise*/
int checkValidAccount(int account){
    return (account >= 0 && account < MAX_ACCOUNTS);
}

void updateNumTrans(thread_data_t* data){
    pthread_mutex_lock(&data->data_locks->transactions_mutex);
    data->bank_data->total_transactions++;
    pthread_mutex_unlock(&data->data_locks->transactions_mutex);
}

float check(thread_data_t* data, int account_from){
    float amount = -1;
    pthread_mutex_lock(&data->data_locks->account_mutex[account_from]);
    amount = data->bank_data->account_array[account_from].balance;
    updateNumTrans(data);
    pthread_mutex_unlock(&data->data_locks->account_mutex[account_from]);
    return amount;
}

void deposit(thread_data_t* data, int account_to, float amount){
    pthread_mutex_lock(&data->data_locks->account_mutex[account_to]);
    data->bank_data->account_array[account_to].balance += amount;
    updateNumTrans(data);
    pthread_mutex_unlock(&data->data_locks->account_mutex[account_to]);
}

int withdraw(thread_data_t* data, int account_from, float amount){
    int res = -1; // 0 means insufficient , -1 error and 1 successful
    pthread_mutex_lock(&data->data_locks->account_mutex[account_from]);
    if(amount <= data->bank_data->account_array[account_from].balance){
        data->bank_data->account_array[account_from].balance -= amount;
        updateNumTrans(data);
        res = 1;
    }else{// insufficient
        res = 0;
    }
    pthread_mutex_unlock(&data->data_locks->account_mutex[account_from]);

    return res;
}

int transfer(thread_data_t* data, int account_from, int account_to, float amount){
    int res = -1; // 0 means insufficient , -1 error and 1 successful

    pthread_mutex_lock(&data->data_locks->account_mutex[account_from]);
    // Loop while the mutex is locked by another thread
    while (pthread_mutex_trylock(&data->data_locks->account_mutex[account_to])){
        pthread_mutex_unlock(&data->data_locks->account_mutex[account_from]);
        pthread_mutex_lock(&data->data_locks->account_mutex[account_from]);
    }
    // if can retrieve
    if(amount <= data->bank_data->account_array[account_from].balance){
        data->bank_data->account_array[account_from].balance -= amount;
        updateNumTrans(data);

        // Deposit
        data->bank_data->account_array[account_to].balance += amount;
        updateNumTrans(data);

        res = 1;
    }else{// insufficient
        res = 0;
    }
    pthread_mutex_unlock(&data->data_locks->account_mutex[account_from]);
    pthread_mutex_unlock(&data->data_locks->account_mutex[account_to]);


    return res;
}


