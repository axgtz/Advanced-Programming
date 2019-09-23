/*
    Roberto Alejandro Gutierrez Guillen
    19 / 09 / 2019
    pipe code from  Gilberto Echeverria
    Hwk 4 - VignereCypher
    To use code just call startCypher from main and import this file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // UNIX functions

#define STR_SIZE 100

void startCypher() {
    pid_t new_pid;
    int status;                             // pipe status
    char fileName[STR_SIZE];                // Name of the file to read

    // Pipe arrays, 2 file descriptors, read and write
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
        close(parent_to_child[0]); // Close reading of parent

        printf("VIGNERE CYPHER\n");
        // Get and send name of file
        printf("Please write the name of the file.\n");
        fgets(fileName, STR_SIZE, stdin);          // Get the name of the file
        fileName[strlen(fileName)-1] = '\0';      // Remove the newline character
        write(parent_to_child[1], fileName, strlen(fileName)); // do a single write function
        close(parent_to_child[1]); // Close writing of parent

        wait(NULL); //For child answer

        close(child_to_parent[1]); // Close writing of child

        read(child_to_parent[0], fileName, STR_SIZE); // Read answer from child
        fileName[strlen(fileName)-3] = '\0'; // Something is adding 3 random characters for pipes
        printf("The name of the file with the resulting text is:  %s\n", fileName);
        close(child_to_parent[0]); // Close reading from child


    }else if (new_pid == 0) {// Child process
        close(parent_to_child[1]); // close writing from parent

        // Read the name of the file from pipe of parent
        read(parent_to_child[0], fileName, STR_SIZE);
        fileName[strlen(fileName)-3] = '\0'; // Something is adding 3 random characters

        // Close reading from both
        close(child_to_parent[0]);
        close(parent_to_child[0]);

        // Encryption key read
        char encryptionKey[STR_SIZE];          // Encryption key
        printf("Please write the encryption key MAX LENGTH = 99\n");
        fgets(encryptionKey, STR_SIZE, stdin);
        encryptionKey[strlen(encryptionKey)-1] = '\0';      // Remove the newline character

        // User choose to decode or encode a message
        int option = 0;
        printf("Do you want to encode or decode a message? Write 1 for encode or 2 for decode\n");
        scanf("%d", &option);
        fgetc(stdin);   // Remove the enter left from scanf
        if (option != 1 && option != 2){
            printf("Please write a valid option next time\n");
            exit(1);
        }

        // Initialize pointers and buffer when needed
        FILE *file_ptr_read = NULL;    // Pointer to file being read
        FILE *file_ptr_write = NULL;   // Pointer to file being writen
        char lineFileBuffer[STR_SIZE];        // Line from file


        // Open file for reading
        file_ptr_read = fopen(fileName, "r");

        // Check that file is valid
        if (!file_ptr_read) {
            perror("Error! reading the file");
            // Program exits if the file pointer returns NULL.
            exit(EXIT_FAILURE);
        }

        // Prepare name of the encoded or decoded file
        char *resultFileName = NULL;
        resultFileName = malloc(strlen(fileName) + 12 + 1); // 12 of the encoded and text, 1 for the end char
        if(option == 1){ // encode
            strcpy(resultFileName, "encoded_");         // Add encoded at the beginning
        }else if (option == 2){ // Decode
            strcpy(resultFileName, "decoded_");         // Add encoded at the beginning
        }else{
            printf("Error when creating name for new file");
            exit(1);
        }

        strcat(resultFileName, fileName);              // Add file name

        // Open file for writing
        file_ptr_write = fopen(resultFileName, "w");

        if (!file_ptr_write) {// Check that file is valid
            perror("Error! creating the file");
            // Program exits if the file pointer returns NULL.
            exit(EXIT_FAILURE);
        }

        size_t sizeEncKey = strlen(encryptionKey); // Size of the encryption key

        if(option == 1){ // encode
            // while it keeps reading
            while (fgets(lineFileBuffer, STR_SIZE, file_ptr_read)) {
                // Start reading
                size_t sizeFile = strlen(lineFileBuffer); // Size of the stored data inside the buffer

                // Encryption
                for (int i = 0; i < sizeFile; i++) {
                    if (lineFileBuffer[i] > 96 && lineFileBuffer[i] < 123) { // if its within the alphabet
                        int tot = ((lineFileBuffer[i] - 97) + (encryptionKey[i % sizeEncKey] - 97)) % 26; // - 97 to remove the A
                        lineFileBuffer[i] = 97 + tot; // 97 is the ASCII number for 'a'
                    }
                }

                fprintf(file_ptr_write, "%s", lineFileBuffer); // Write all the lineFileBuffer into the file
            }
        }else if(option == 2){ // Decode
            // while it keeps reading
            while (fgets(lineFileBuffer, STR_SIZE, file_ptr_read)) {
                // Start reading
                size_t sizeFile = strlen(lineFileBuffer); // Size of the stored data inside the buffer

                // Encryption
                for (int i = 0; i < sizeFile; i++) {
                    if (lineFileBuffer[i] > 96 && lineFileBuffer[i] < 123) { // if its within the alphabet, if not it just copies
                        int tot = ((lineFileBuffer[i] - 97) - (encryptionKey[i % sizeEncKey] - 97) + 26) % 26;
                        lineFileBuffer[i] = 97 + tot; // 97 is the ASCII number for 'a'
                    }
                }

                fprintf(file_ptr_write, "%s", lineFileBuffer); // Write all the lineFileBuffer into the file
            }
        }else{
            printf("Error when encoding or decoding ");
            exit(1);
        }

        //Close files
        fclose(file_ptr_read);
        fclose(file_ptr_write);

        // Send to parent the name of the resulting file
        write(child_to_parent[1], resultFileName, strlen(resultFileName));
        close(child_to_parent[1]); // Close child write

        free(resultFileName); // Free memory of string
    }else{
        printf("ERROR when doing the fork\n");
    }
}
