/*
    Checking process IDs and doing fork

    Gilberto Echeverria
    */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // UNIX functions

#define STR_SIZE 100

void preparePipes(int * in_pipe, int * out_pipe);

int main(){
    pid_t new_pid;
    int status;                             // pipe status
    int option = 0;
    char fileName[STR_SIZE];                // Name of the file to read
    char encryptionKey[STR_SIZE];          // Encryption key

    // Pipe arrays
    int parent_to_child[2];
    int child_to_parent[2];

    // Open the pipes
    status = pipe(parent_to_child);
    if (status < 0) {perror("PIPE"); exit(EXIT_FAILURE); }
    status = pipe(child_to_parent);
    if (status < 0) {perror("PIPE"); exit(EXIT_FAILURE); }

    // Create a new process
    new_pid = fork();

    // Identify the processes
    if (new_pid > 0) {   // Parent process
        preparePipes(child_to_parent, parent_to_child);

        printf("VIGNERE CYPHER\n");

        printf("Please write the name of the file.\n");
        fgets(fileName, STR_SIZE, stdin);          // Get the name of the file
        fileName[strlen(fileName)-1] = '\0';      // Remove the newline character
        write(parent_to_child[1], fileName, strlen(fileName)); // do a single write function

    }else if (new_pid == 0) {// Child process
        preparePipes(parent_to_child, child_to_parent);
        // Read the name of the file
        read(parent_to_child[0], fileName, STR_SIZE);
        printf("My parent sent: %s", fileName);

        // User choose to decode or encode a message
        printf("Do you want to encode or decode a message? Write 1 for encode or 2 for decode\n");
        scanf("%d", &option);
        fgetc(stdin);   // Remove the enter left from scanf
        if (option != 1 || option != 2){
            printf("Please write a valid option next time\n");
            exit(1);
        }
        /*
        printf("Please write the encryption key MAX LENGTH = 99\n");
        fgets(encryptionKey, STR_SIZE, stdin);
        encryptionKey[strlen(encryptionKey)-1] = '\0';      // Remove the newline character
        printf("%s", encryptionKey);
v
        */
    /*
        FILE *file_ptr_read = NULL;    // Pointer to file being read
        FILE *file_ptr_write = NULL;   // Pointer to file being writen
        char lineFileBuffer[STR_SIZE];        // Line from file
        char *resultFileName = NULL;           // Used for the file name of the encoded file

        strncpy(encryptionKey, "prograavanzada", STR_SIZE);

        // Open file for reading
        file_ptr_read = fopen(fileName, "r");

        if (!file_ptr_read) {// Check that file is valid
            perror("Error! reading the file");
            // Program exits if the file pointer returns NULL.
            exit(EXIT_FAILURE);
        }

        // Create new name for the result file

        resultFileName = malloc(strlen(fileName) + 12 + 1); // 12 of the encoded and text, 1 for the end char
        strcpy(resultFileName, "encoded_");         // Add encoded at the beginning
        strcat(resultFileName, fileName);              // Add file name

        // Open file for writing
        file_ptr_write = fopen(resultFileName, "w");

        if (!file_ptr_write) {// Check that file is valid
            perror("Error! creating the file");
            // Program exits if the file pointer returns NULL.
            exit(EXIT_FAILURE);
        }

        size_t sizeEncKey = strlen(encryptionKey); // Size of the encryption key
        // while it keeps reading
        while (fgets(lineFileBuffer, STR_SIZE, file_ptr_read)) {
            // Start reading
            size_t sizeFile = strlen(lineFileBuffer); // Size of the stored data inside the buffer

            // Encryption
            for (int i = 0; i < sizeFile; i++) {
                if (lineFileBuffer[i] > 96 && lineFileBuffer[i] < 123) { // if its within the alphabet
                    int tot = ((lineFileBuffer[i] - 97) + (encryptionKey[i % sizeEncKey]) - 97) % 26;
                    lineFileBuffer[i] = 97 + tot; // 97 is the ASCII number for 'a'
                }
            }

            fprintf(file_ptr_write, "%s", lineFileBuffer); // Write all the lineFileBuffer into the file
        }

        fclose(file_ptr_read);
        fclose(file_ptr_write);

        free(resultFileName); // Free memory of string
*/
    }else{
        printf("ERROR when doing the fork\n");
    }

    return 0;
}

void preparePipes(int * in_pipe, int * out_pipe){
    // Close the pipe ends that will not be used
    close(in_pipe[1]);  // Close the writing file descriptor
    close(out_pipe[0]); // Close the reading file descriptor
}
