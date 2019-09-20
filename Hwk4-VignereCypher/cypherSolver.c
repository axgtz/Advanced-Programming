//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-09-15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 100

int main () {
    // User choose to decode or encode a message
    printf("VIGNERE CYPHER\n");
    printf("Do you want to encode or decode a message? Write 1 for encode or 2 for decode\n");
    scanf("%d", &option);
    fgetc(stdin);   // Remove the enter left from scanf

    if(option == 1){ // Encode
        printf("Please write the name of the file.\n");
        fgets(fileName, STR_SIZE, stdin);          // Get the name of the file
        fileName[strlen(fileName)-1] = '\0';      // Remove the newline character
        printf("%s", fileName);


    }else if(option == 2){ // Decode
        printf("Please write the encryption key MAX LENGTH = 99\n");
        fgets(encryptionKey, STR_SIZE, stdin);
        encryptionKey[strlen(encryptionKey)-1] = '\0';      // Remove the newline character
        printf("%s", encryptionKey);


    }else{
        printf("Please write a valid option next time\n");
    }

    FILE * file_ptr_read = NULL;    // Pointer to file being read
    FILE * file_ptr_write = NULL;   // Pointer to file being writen

    int option = 0;
    char fileName[STR_SIZE];                // Name of the file to read
    char encryptionKey[STR_SIZE];          // Encryption key
    char lineFileBuffer[STR_SIZE];        // Line from file
    char *resultFileName = NULL;           // Used for the file name of the encoded file

    strncpy(fileName, "aladdin.txt", STR_SIZE);
    strncpy(encryptionKey, "prograavanzada", STR_SIZE);

    // Open file for reading
    file_ptr_read = fopen(fileName , "r");

    if (!file_ptr_read){// Check that file is valid
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
                int tot = ((lineFileBuffer[i] - 97) + (encryptionKey[i%sizeEncKey]) - 97) % 26;
                lineFileBuffer[i] = 97 + tot; // 97 is the ASCII number for 'a'
            }
        }

        fprintf(file_ptr_write, "%s", lineFileBuffer); // Write all the lineFileBuffer into the file
    }

    fclose(file_ptr_read);
    fclose(file_ptr_write);

    free(resultFileName); // Free memory of string

    return  0;
}

/*
 A fork is used to create a child process that will take care of the task
 of encoding or decoding. When the child finishes it will inform its parent process,
 write a new file with the prefix "encoded_" or "decoded_" and send back the name of
 the new file to its parent.

You can use any of the Inter Process Communication (IPC) methods seen so far
 (pipe, popen, fifo, passing arguments), as long as you have two way communication
 between the parent and the child processes.
 */
/*for (int j = 0; j < tamFile; ++j) { // Print result encoded
    printf("%c", lineFileBuffer[j]);
}*/

/* Print the ASCII characters
 * for (int i = 0; i < 26 ;i++){
    int a = i + 97;
    char c = a;
    printf("%c", c);
}*/


